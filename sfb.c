/*
   Another useless simple SFB reader
   Information taken from psdevwiki
   for more information please visit: https://www.psdevwiki.com/ps3/PS3_DISC.SFB
*/

#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include "wingui.h"
#endif

#include "sfb.h"

int main(int argc, char* argv[])
{
#ifdef _WIN32
	if (argc < 2)
	{
		CreateGuiApp();
		GuiAppRun();
		return 0;
	}
#else
	if (argc < 2)
	{
		printf("Please specifiy sfb file!\n");
		return 0;
	}
#endif

	SFB sfb;
	FILE* sfb_handle = SFB_open_a(argv[1]);

	if (sfb_handle == SFB_FAIL)
	{
		printf("Failed while opening file!\n");
		return 1;
	}

	
	if (SFB_read(&sfb, sfb_handle) == SFB_FAIL)
	{
		printf("Failed while reading file!\n");
		return 1;
	}

	SFB_print(&sfb);
	
	if (SFB_close(sfb_handle) == SFB_FAIL)
	{
		printf("Failed while closing file!\n");
		return 1;
	}

	return 0;
}
