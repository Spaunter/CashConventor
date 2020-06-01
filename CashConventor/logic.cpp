#include"Header.h"

bool dirExists(const string& forderDir)
{
	DWORD ftyp = GetFileAttributesA(forderDir.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}