/*
 * config.h
 *
 *  Created on: 12-Jan-2017
 *      Author: arpith
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>


bool translate_single_line = true;
bool translate_multi_line = true;

std::wstring single_line_comment = L"//";
std::wstring multi_line_comment_start = L"/*";
std::wstring multi_line_comment_end = L"*/";

//Path where the trans perl script is saved.
//For more information on 'trans', visit https://github.com/soimort/translate-shell
std::wstring path = L"./bin/";

//The final command to execute will be:
//command_part1 + "text to translate" + command_part2
//Eg "./trans -no-auto -brief \""+ 蝙蝠侠 +"\" | tr -d '\n'";
std::wstring command_part1 = L"trans -no-auto -brief \"";
std::wstring command_part2 = L"\" | tr -d '\n'";

//The original file (untranslated version) is saved with this suffix.
std::string suffix = ".bkp";
bool save_backup = false;

#endif /* CONFIG_H_ */
