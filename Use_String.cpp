#include "Use_String.h"

Use_String::Use_String()
{
}

Use_String::~Use_String()
{
}

void Use_String::setString(char * name)
{
	strcpy(string, name);
}

char* Use_String::getString()
{
	return string;
}
