/*
 * translator.h
 *
 *  Created on: 12-Jan-2017
 *      Author: arpith
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include <string>

class translator {
private:
	std::wstring translated;
	bool multi_line;
public:
	translator();
	void usage();
	bool translate_file(char *file_name);
	std::wstring process(std::wstring line);
};

#endif /* TRANSLATOR_H_ */
