/*
	Copyright (c) 2016 Andrew McRobb

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef AWESOME_KEY_EXTRACTOR_HPP
#define AWESOME_KEY_EXTRACTOR_HPP

#include <windows.h>
#include <string>
#include <iostream>

class AwesomeKeyExtractor {
public:

	std::string fetchWindowsKey();

protected:
	static const std::string  KEY_FOLDER;
	static const std::string  KEY_NAME;
	static const std::string  VALID_KEY_CHARACTERS;
	static const unsigned int KEY_OFFSET = 52;
	
	/**
	* getProductIdFromRegistery Actully loads in the 'raw' key from the registory!
	*/
	BYTE* getProductIdFromRegistery();

	/**
	* convertToReadableKey Takes the BYTE a.k.a unsigned char pointer of the key from the registory and makes it readable.
	*
	*@see http://www.codeproject.com/Questions/334471/how-to-get-productId-in-windows-bit#ctl00_ctl00_MC_AMC_Answers_ctl00_A_Header
	*
	*@author enhzflep
	*
	*@return std::string
	*/
	std::string convertToReadableKey(BYTE* keyData);
};


#endif