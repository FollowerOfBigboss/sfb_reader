/*
   Another useless simple SFB reader
   Information taken from psdevwiki
   for more information please visit: https://www.psdevwiki.com/ps3/PS3_DISC.SFB
*/

#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include "Gui.h"
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

	FILE* sfb_handle = SFB_open_a(argv[1]);
	SFB sfb;
	SFB_read(&sfb, sfb_handle);
	SFB_print(&sfb);
	SFB_close(sfb_handle);
	return 0;
}
