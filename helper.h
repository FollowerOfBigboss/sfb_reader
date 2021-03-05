#include <stdbool.h>
#include <string.h>

bool startswith(const char* str, const char* str1)
{
	if (strncmp(str, str1, strlen(str1)) == 0)
	{
		return true;
	}
	return false;
}