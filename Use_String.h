#ifndef __USE_STRING_H__
#define __USE_STRING_H__

#include <cstring>

class Use_String
{
public:
	Use_String();
	~Use_String();

	void setString(char * name);
	char* getString();
private:
	char string[64];
};

#endif