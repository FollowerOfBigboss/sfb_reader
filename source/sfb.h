#include <iostream>

#ifndef SFB_H
#define SFB_H

#define REV(n) ((n << 24) | (((n>>16)<<24)>>16) | (((n<<16)>>24)<<16) | (n>>24)) 

class SFB
{
public:

	bool open(const std::string& sfb_file);
	void create(const std::string& filename);
	void* create_as(const std::string& filename);

	bool read();
	bool write();
	bool close();

	void write_as(const void* fhandle);
	bool close_as(const void* fhandle);

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

#endif