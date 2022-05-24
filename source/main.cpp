#include <iostream>
#include <cstring>
#include "sfb.h"

#ifdef USEGUI
    #include <QApplication>
    #include "sfbgui.h"
#endif

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
#	ifdef USEGUI
		QApplication app(argc, argv);
		SFBGui sfbgui;
		sfbgui.show();
		return app.exec();
#	else
		printf("Please provide arguments!\n");
		return 0;
#	endif
	}

	SFB sfb;
	if (strncmp(argv[1], "read", 4) == 0)
	{
		sfb.open(argv[2]);
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
	
	else if (strncmp(argv[1], "create", 6) == 0)
	{
		sfb.set_defaults();
		sfb.create(argv[2]);
		sfb.write();
		sfb.close();
	}
	
	return 0;
}
