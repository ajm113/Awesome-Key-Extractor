/*
	Copyright (c) 2016 Andrew McRobb

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "AwesomeKeyExtractor.hpp"

const std::string AwesomeKeyExtractor::KEY_FOLDER           = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\";
const std::string AwesomeKeyExtractor::KEY_NAME             = "DigitalProductId";
const std::string AwesomeKeyExtractor::VALID_KEY_CHARACTERS = "BCDFGHJKMPQRTVWXY2346789";

std::string AwesomeKeyExtractor::fetchWindowsKey() {
	BYTE* binaryKey = getProductIdFromRegistery();

	if(!binaryKey)
		return "";
	return convertToReadableKey(binaryKey);
}

BYTE* AwesomeKeyExtractor::getProductIdFromRegistery() {
	/*
		Create our 'register handler'.
	*/

	HKEY hKey;
	LONG result =  RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_FOLDER.c_str(), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);

	switch(result)
	{
		case ERROR_FILE_NOT_FOUND:
			std::cerr << "Registry path "<< KEY_FOLDER.c_str() <<" not found! Please ensure the key path is correct, and try again!" << std::endl;
			return NULL;
		case ERROR_SUCCESS:
			break;
		default:
			std::cerr << "unable to load path "<< KEY_FOLDER.c_str() <<"! Please check the key path, and please try again! Error Code:" << result << std::endl;
			return NULL;
	}

	/*
		Ensure we can actually read this value.
	*/
	DWORD type;
	result = RegQueryValueEx(hKey, KEY_NAME.c_str(), NULL, &type, NULL, NULL);

	switch(result)
	{
		case ERROR_SUCCESS:
			break;
		default:
			RegCloseKey(hKey);
			std::cerr << "Failed to query "<< KEY_NAME.c_str() << "! Please try again! Error Code:" << result << std::endl;
			return NULL;
	}


	if(type != REG_BINARY) {
		RegCloseKey(hKey);
		std::cerr << "Incorrect "<< KEY_NAME.c_str() << " value type! Expecting a REG_BINARY!" << std::endl;
		return NULL;
	}

	/*
		If we gone this far lets actually load the data in!
	*/
	BYTE* keyData = new BYTE[1000];
	DWORD dwBufSize = 1000;
	result = RegQueryValueEx(hKey, KEY_NAME.c_str(), NULL, NULL, keyData, &dwBufSize);
	
	/*
		Just to be extra safe, lets do another check to make sure everything was good.
	*/
	if(result  != ERROR_SUCCESS ) {
		std::cerr << "Could not read "<< KEY_NAME.c_str() << " data! Error Code:" << result << std::endl;
		RegCloseKey(hKey);
		return NULL;
	}

	RegCloseKey(hKey);

	return keyData;
}

std::string AwesomeKeyExtractor::convertToReadableKey(BYTE* keyData) {
	if(!keyData)
		return "";

	/* Offset last value to 43H. */
	const int keyEndIndex = KEY_OFFSET + 15;

	/* Length of decoded product key. */
	const int decodeLength  =   29;
	/* Length of decoded key in byte-form (each byte = 2 chars). */
	const int decodeStringLength = 15;
	/* Array to contain decoded key. */
	char* pDecodedChars = new char[ decodeLength + 1 ];

	memset( pDecodedChars, 0, decodeLength + 1 );

	/* Extract byte 52 to 67 inclusive. */
	byte hexPid[ keyEndIndex - KEY_OFFSET + 1 ];

	for ( int i = KEY_OFFSET; i <= keyEndIndex; i++ )
	{
	    hexPid[ i - KEY_OFFSET ] = keyData[ i ];
	}

	for ( int i = decodeLength - 1; i >= 0; i-- )
	{
		/* Every 6th character is a separator. */
		if ( ( i + 1 ) % 6 == 0 )
		{
		    *( pDecodedChars + i ) = '-';
		}
		else
		{
			/* Do the actual decoding. */
			int digitMapIndex = 0;
			for ( int j = decodeStringLength - 1; j >= 0; j-- )
			{
				int byteValue = ( digitMapIndex << 8 ) | hexPid[ j ];
				hexPid[ j ] = ( byte )( byteValue / 24 );
				digitMapIndex = byteValue % 24;
				*( pDecodedChars + i ) = VALID_KEY_CHARACTERS[ digitMapIndex ];
			}
		}
	}
	/*
	 * Return the decoded product key.
	 */
	std::string outputKey = pDecodedChars;
	delete pDecodedChars;

	return outputKey;
}