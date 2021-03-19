#include <iostream>
#include <fstream>

#define REV(n) ((n << 24) | (((n>>16)<<24)>>16) | (((n<<16)>>24)<<16) | (n>>24)) 

class SFB
{
public:
	bool open(const std::string& sfb_file);
	bool read();
	bool write();
	bool close();
	void create(const std::string& filename);
	
	void* create_as(const std::string& filename);
	void write_as(const void* fhandle);
	void close_as(const void* fhandle);

	void set_defaults();


	// Zeros only purpose is make a space, so we don't include and read them
	char magic[4];
	uint32_t version;
	// char unknown_zeroes0[24];
	char hybrid_flag[16];
	uint32_t disc_content_data_offset;
	uint32_t disc_content_data_lenght;
	// char unknown_zeroes1[8];
	char disc_title_name[8];
	// char unknown_zeroes2[8];
	uint32_t disc_title_data_offset;
	uint32_t disc_title_data_lenght;
	// char unknown_zeroes3[424];
	char disc_content[32];
	char disc_title[16];
	// char unknown_zeroes4[976];
private:
	FILE* stream = nullptr;
};



// typedef struct SFB
//{
//	char magic[4];
//	uint32_t version;
//	char unknown_zeroes[0x18];
//	char hybrid_flag[0x10];
//	uint32_t disc_content_data_offset;
//	uint32_t disc_content_data_lenght;
//	char unknown_zeroes1[0x8];
//	char disc_title_name[0x8];
//	char unknown_zeroes2[0x8];
//	uint32_t disc_title_data_offset;
//	uint32_t disc_title_data_lenght;
//	char unknown_zeroes3[0x1A8];
//	char disc_content[0x20];
//	char disc_title[0x10];
//	char unknown_zeroes4[0x3D0];
//}SFB;
// 
// #define SFB_FAIL		0
// #define SFB_SUCCESSFUL	1
// 
// #ifdef _WIN32
//FILE* SFB_open_w(const wchar_t*);
//FILE* SFB_create_w(const wchar_t*, SFB*);
// #endif
// 
// FILE* SFB_open_a(const char*);
// int SFB_read(SFB*, FILE*);
// int SFB_close(FILE*);
// void SFB_set_defaults(SFB*);
// FILE* SFB_create_a(const char*, SFB*);
// void SFB_print(SFB*);
// int SFB_write(SFB*, FILE*);
// 
// #ifdef _WIN32
// FILE* SFB_open_w(const wchar_t* SFB_file)
// {
// 	FILE* file = _wfopen(SFB_file, L"r+");
// 	if (file != NULL)
// 	{
// 		return file;
// 	}
// 	return SFB_FAIL;
// }
// 
// FILE* SFB_create_w(const wchar_t* filename, SFB* sfb)
// {
// 	FILE* fs = _wfopen(filename, L"w");
// 	return fs;
// }

// #endif
// 
// FILE* SFB_open_a(const char* SFB_file)
//{
//	FILE* file = fopen(SFB_file, "r");
//	if (file != NULL)
//	{
//		return file;
//	}
//	return SFB_FAIL;
//}
// 
// int SFB_read(SFB* sfb, FILE* fs)
//{
//	int s = fread(sfb, sizeof(SFB), 1, fs);
//	if (s != 1)
//	{
//			return SFB_FAIL;
//	}
//	return SFB_SUCCESSFUL;
//}
// 
// int SFB_close(FILE* fs)
//{
//	int fsr = fclose(fs);
//	if (fsr == EOF)
//	{
//		return SFB_FAIL;
//	}
//	return SFB_SUCCESSFUL;
//}
// 
// void SFB_set_defaults(SFB* sfb)
//{
//	strcpy(sfb->magic, "\x2E\x53\x46\x42" /* '.SFB' */);
//	sfb->version = 0x00000100;
//	memset(sfb->unknown_zeroes, 0, 0x18);
//	strcpy(sfb->hybrid_flag, "HYBRID_FLAG");
//	sfb->disc_content_data_offset = 0x00020000;
//	sfb->disc_content_data_lenght = 0x20000000;
//	memset(sfb->unknown_zeroes1, 0, 0x8);
//	strcpy(sfb->disc_title_name, "TITLE_ID");
//	memset(sfb->unknown_zeroes2, 0, 0x8);
//	sfb->disc_title_data_offset = 0x20020000;;
//	sfb->disc_title_data_lenght = 0x10000000;
//	memset(sfb->unknown_zeroes3, 0, 0x1A8);
//	strcpy(sfb->disc_content, "g");
//	strcpy(sfb->disc_title, "UNKNOWN");
//	memset(sfb->unknown_zeroes4, 0, 0x3D0);
//}
// 
// FILE* SFB_create_a(const char* filename, SFB* sfb)
//{
//	FILE* fs = fopen(filename, "w");
//	return fs;
//}
// 
// void SFB_print(SFB* sfb)
//{
//	printf("Version: 0x%.8X\n", REV(sfb->version));
//	printf("HYBRID FLAG: %s\n", sfb->hybrid_flag);
//	printf("Disc Content Data Offset: 0x%.8X\n", REV(sfb->disc_content_data_offset));
//	printf("Disc Content Data Lenght: 0x%.8X\n", REV(sfb->disc_content_data_lenght));
//	printf("Disc Title Name: %s\n", sfb->disc_title_name);
//	printf("Disc Title Data Offset: 0x%.8X\n", REV(sfb->disc_title_data_offset));
//	printf("Disc Title Data Lenght: 0x%.8X\n", REV(sfb->disc_title_data_lenght));
//	printf("Disc Content: %s\n", sfb->disc_content);
//	printf("Disc Title: %s\n", sfb->disc_title);
//}
// 
// int SFB_write(SFB* sfb, FILE* fs)
// {
// 	int fw = fwrite(sfb, sizeof(SFB), 1, fs);
// 	if (fw != 1)
// 	{
// 		return SFB_FAIL;
// 	}
// 	return SFB_SUCCESSFUL;
// }
