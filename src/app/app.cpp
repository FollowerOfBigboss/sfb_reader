#include "../sfb/sfb.h"
#include "config.h"

ENTRY
{
#ifdef USE_GUI
#ifdef _WIN32
	const int argc = __argc;
	const wchar_t* argv = GetCommandLineW();
#endif

if (argc < 2)
	{
		wxEntryStart(NULL, NULL);
		wxTheApp->CallOnInit();
		wxTheApp->OnRun();
		wxEntryCleanup();
		return 0;
	}
#else
	if (argc < 2)
	{
		printf("Please provide arguments!\n");
		return 0;
	}
#endif
	
	SFB sfb;
	if (strCmp(GetCmdArg(argv, 1), CPT("read"), 4) == 0)
	{
		sfb.open(GetCmdArg(argv, 2));
		sfb.read();

		printf("Version: 0x%.8X\n", REV(sfb.version));
		printf("HYBRID FLAG: %s\n", sfb.hybrid_flag);
		printf("Disc Content Data Offset: 0x%.8X\n", REV(sfb.disc_content_data_offset));
		printf("Disc Content Data Lenght: 0x%.8X\n", REV(sfb.disc_content_data_lenght));
		printf("Disc Title Name: %s\n", sfb.disc_title_name);
		printf("Disc Title Data Offset: 0x%.8X\n", REV(sfb.disc_title_data_offset));
		printf("Disc Title Data Lenght: 0x%.8X\n", REV(sfb.disc_title_data_lenght));
		printf("Disc Content: %s\n", sfb.disc_content);
		printf("Disc Title: %s\n", sfb.disc_title);
		
		sfb.close();
	}

	else if (strCmp(GetCmdArg(argv, 1), CPT("create"), 6) == 0)
	{
		sfb.set_defaults();
		sfb.create(GetCmdArg(argv, 2));
		sfb.write();
		sfb.close();
	}

	return 0;
}