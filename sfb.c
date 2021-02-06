/*
   Another useless simple SFB reader
   Information taken from psdevwiki
   for more information please visit: https://www.psdevwiki.com/ps3/PS3_DISC.SFB
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define REV(n) ((n << 24) | (((n>>16)<<24)>>16) | (((n<<16)>>24)<<16) | (n>>24)) 
typedef struct SFB
{
	char magic[4];
	uint32_t version;
	char unknown_zeroes[0x18];
	char hybrid_flag[0x10];
	uint32_t disc_content_data_offset;
	uint32_t disc_content_data_lenght;
	char unknown_zeroes1[0x8];
	char disc_title_name[0x8];
	char unknown_zeroes2[0x8];
	uint32_t disc_title_data_offset;
	uint32_t disc_title_data_lenght;
	char unknown_zeroes3[0x1A8];
	char disc_content[0x20];
	char disc_title[0x10];
	char unknown_zeroes4[0x3D0];
}SFB;

void* SFB_open(const char* SFB_file);
void SFB_read(SFB* sfb,void* fs);
void SFB_close(void* fs);


void* SFB_open(const char* SFB_file)
{
	FILE* file = fopen(SFB_file, "r");
	if (file != NULL)
		return (void*)file;
	printf("[ERROR] SFB_open failed!\n");
	exit(EXIT_FAILURE);
}

void SFB_read(SFB* sfb,void* fs)
{
	int s = fread(sfb, sizeof(SFB), 1, fs);
	if (s != 1)
	{
		printf("[ERROR] SFB_open failed!\n");
		exit(EXIT_FAILURE);
	}
}
	
void SFB_close(void* fs)
{
	fclose(fs);
}


void main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Please specifiy sfb file!\n");
		return;
	}

	void* sfb_handle = SFB_open(argv[1]);
	SFB sfb;
	SFB_read(&sfb, sfb_handle);
	
	printf("Magic: %s\n", sfb.magic);
	printf("Version: 0x%.8X\n", REV(sfb.version));
	printf("HYBRID FLAG: %s\n", sfb.hybrid_flag);
	printf("Disc Content Data Offset: 0x%.8X\n", REV(sfb.disc_content_data_offset));
	printf("Disc Content Data Lenght: 0x%.8X\n", REV(sfb.disc_content_data_lenght));	
	printf("Disc Title Name: %s\n", sfb.disc_title_name);
	printf("Disc Title Data Offset: 0x%.8X\n", REV(sfb.disc_title_data_offset));
	printf("Disc Title Data Lenght: 0x%.8X\n", REV(sfb.disc_title_data_lenght));
	printf("Disc Content: %s\n", sfb.disc_content);
	printf("Disc Title: %s\n", sfb.disc_title);
	
	SFB_close(sfb_handle);
	return;
}
