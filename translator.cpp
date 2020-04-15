/*
 * translator.cpp
 *
 *  Created on: 12-Jan-2017
 *      Author: arpith
 */

#include <fstream>
#include <iostream>
#include <array>
#include <memory>
#include <string.h>
#include <iconv.h>

#include "translator.h"
#include "config.h"

using namespace std;

translator::translator() {
	multi_line = false;
	translated = L"";
}

/*
 * Prints usage in case the user dose not pass any arguments
 */

void translator::usage() {
	string line;
	ifstream myfile("usage");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			cout << line << endl;
		}
		myfile.close();
	}

	else
		cout << "Unable to open file";
}

/*
 * Translate the file
 * Parameters: File name to translate
 * Return value: true if the file is successfully translated
 */
bool translator::translate_file(char *file) {
	cout << " Translating " << file << endl;

	wstring line;
	wifstream infile(file);
	wofstream outfile((string(file) + string(".temp")));
	if (infile.is_open()) {
		while (getline(infile, line)) {
			line = process(line);
			outfile << line << endl;
		}
		infile.close();
		outfile.close();

		if (save_backup)
			rename(file, (string(file) + suffix).c_str());
		rename((string(file) + string(".temp")).c_str(), file);
		remove((string(file) + string(".temp")).c_str());
	}

	else
		cout << "Unable to open file";

	cout << "Translating " << file << " was successful." << endl;
	return true;
}

/*
 * For the given wchar_t string with its byte length, returns the equivalent UTF-8 string
 *
 * https://stackoverflow.com/questions/9117599/call-popen-on-a-command-with-chinese-characters-on-mac
 */
char* utf8string(const wchar_t* wchar, size_t utf32_bytes)
{
	char result_buffer[2048];

	iconv_t converter = iconv_open("UTF-8", "wchar_t");

	char* result = result_buffer;
	char* input = (char*)wchar;
	size_t output_available_size = sizeof result_buffer;
	size_t input_available_size = utf32_bytes;
	size_t result_code = iconv(converter, &input, &input_available_size, &result, &output_available_size);
	if (result_code == -1) {
		perror("iconv");
		return NULL;
	}
	iconv_close(converter);

	return strdup(result_buffer);
}

/*
 * Executes the shell command (cmd) and returns the shell output as a string
 */
wstring exec(const wchar_t* cmd) {
	char* utf8cmd = utf8string(cmd, (wcslen(cmd) + 1) * sizeof(wchar_t));
	array<wchar_t, 128> buffer;
	wstring result;
	shared_ptr<FILE> pipe(popen(utf8cmd, "r"), pclose);
	if (!pipe) {
		free(utf8cmd);
		throw runtime_error("popen() failed!");
	}
	while (!feof(pipe.get())) {
		if (fgetws(buffer.data(), 128, pipe.get()) != NULL)
			result += buffer.data();
	}
	free(utf8cmd);
	return result;
}

/*
 * Translates the comments (if any) in the line which is given as an input parameter
 * parameters: Line to translate
 * Returns: A string with all comments translated
 */
wstring translator::process(wstring line) {
	wstring command;
	if (multi_line == true) {

		wcout << L"Multi line: " << line << endl;

		if (line.find(multi_line_comment_end) != wstring::npos) {
			multi_line = false;
			translated = line.substr(0, line.find(multi_line_comment_end));
		} else
			translated = line;

		command = path + command_part1 + translated + command_part2;

		translated = exec(command.c_str());
		wcout << L"\t translated: " << translated << endl;

		if (line.find(multi_line_comment_end) != wstring::npos) {
			line = translated + line.substr(line.find(multi_line_comment_end));
		} else {
			line = translated;
		}

		return line;
	}
	if (line.find(single_line_comment) != wstring::npos && translate_single_line) {

		wcout << L" Single line: " << line << endl;
		translated = line.substr(line.find(single_line_comment) + 2);

		command = path + command_part1 + translated + command_part2;

		translated = exec(command.c_str());
		wcout << L"\t Translated: " << translated << endl;

		line = line.substr(0, line.find(single_line_comment) + 2) + translated;

		return line;
	}
	if (line.find(multi_line_comment_start) != wstring::npos && translate_multi_line) {

		wcout << L"Multi line: " << line << endl;
		multi_line = true;

		if (line.find(multi_line_comment_end) != wstring::npos) {
			multi_line = false;
			translated = line.substr(line.find(multi_line_comment_start) + 2,
					(line.find(multi_line_comment_end)) - (line.find(multi_line_comment_start) + 2));
		} else
			translated = line.substr(line.find(multi_line_comment_start) + 2);

		command = path + command_part1 + translated + command_part2;

		translated = exec(command.c_str());
		wcout << L"\t translated: " << translated << endl;

		if (line.find(multi_line_comment_end) != wstring::npos) {
			wstring line1 = line.substr(0, line.find(multi_line_comment_start) + 2);
			wstring line2 = line.substr(line.find(multi_line_comment_end));
			line = line1 + translated + line2;
		} else {
			line = line.substr(0, line.find(multi_line_comment_start) + 2) + translated;
		}

		return line;
	}
	wcout << L"No comment: " << line << endl;
	return line;
}
