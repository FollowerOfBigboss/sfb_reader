#include "sfb.h"

bool SFB::open(const std::string& sfb_file)
{
	if (stream != nullptr)
	{
		close();
	}

	stream = fopen(sfb_file.c_str(), "r+");
	if (stream != nullptr)
	{
		return true;
	}
	return false;
}

bool SFB::read()
{
	fread(magic, sizeof(char), 4, stream);
	fread(&version, sizeof(uint32_t), 1, stream);
	
	fseek(stream, 24, SEEK_CUR); // Pass unknown_zeroes0
	
	fread(hybrid_flag, sizeof(char), sizeof(hybrid_flag), stream);
	fread(&disc_content_data_offset, sizeof(uint32_t), 1, stream);
	fread(&disc_content_data_lenght, sizeof(uint32_t), 1, stream);
	
	fseek(stream, 8, SEEK_CUR); // Pass another unknown_zeroes1
	
	fread(disc_title_name, sizeof(char), sizeof(disc_title_name), stream);
	
	fseek(stream, 8, SEEK_CUR); // Pass another unknown_zeroes2
	
	fread(&disc_title_data_offset, sizeof(uint32_t), 1, stream);
	fread(&disc_title_data_lenght, sizeof(uint32_t), 1, stream);
	
	fseek(stream, 424, SEEK_CUR); // Pass another unknown_zeroes3
	
	fread(disc_content, sizeof(char), sizeof(disc_content), stream);
	fread(disc_title, sizeof(char), sizeof(disc_title), stream);
	
	return true;
}

bool SFB::write()
{
	char unknwon_zeroes[976];
	memset(unknwon_zeroes, 0, sizeof(unknwon_zeroes));
	fseek(stream, 0, SEEK_SET);

	fwrite(magic, sizeof(char), 4, stream);
	fwrite(&version, sizeof(uint32_t), 1, stream);
	fwrite(unknwon_zeroes, sizeof(char), 24, stream);
	fwrite(hybrid_flag, sizeof(char), 16, stream);
	fwrite(&disc_content_data_offset, sizeof(uint32_t), 1, stream);
	fwrite(&disc_content_data_lenght, sizeof(uint32_t), 1, stream);
	fwrite(unknwon_zeroes, sizeof(char), 8, stream);
	fwrite(disc_title_name, sizeof(char), 8, stream);
	fwrite(unknwon_zeroes, sizeof(char), 8, stream);
	fwrite(&disc_title_data_offset, sizeof(uint32_t), 1, stream);
	fwrite(&disc_title_data_lenght, sizeof(uint32_t), 1, stream);
	fwrite(unknwon_zeroes, sizeof(char), 424, stream);
	fwrite(disc_content, sizeof(char), 32, stream);
	fwrite(disc_title, sizeof(char), 16, stream);
	fwrite(unknwon_zeroes, sizeof(char), 976, stream);
	
	return true;
}

bool SFB::close()
{
	if (stream != nullptr)
	{
		if (fclose(stream) == NULL)
		{
			stream = nullptr;
			return true;
		}
	}
	return false;
}

void SFB::create(const std::string& filename)
{
	if (stream != nullptr)
	{
		close();
	}
	stream = fopen(filename.c_str(), "w");
}

void* SFB::create_as(const std::string& filename)
{
	FILE* create = fopen(filename.c_str(), "w");
	return create;
}

void SFB::write_as(const void* fhandle)
{
	char unknwon_zeroes[976];
	memset(unknwon_zeroes, 0, sizeof(unknwon_zeroes));
	fseek((FILE*)fhandle, 0, SEEK_SET);

	fwrite(magic, sizeof(char), 4, (FILE*)fhandle);
	fwrite(&version, sizeof(uint32_t), 1, (FILE*)fhandle);
	fwrite(unknwon_zeroes, sizeof(char), 24, (FILE*)fhandle);
	fwrite(hybrid_flag, sizeof(char), 16, (FILE*)fhandle);
	fwrite(&disc_content_data_offset, sizeof(uint32_t), 1, (FILE*)fhandle);
	fwrite(&disc_content_data_lenght, sizeof(uint32_t), 1, (FILE*)fhandle);
	fwrite(unknwon_zeroes, sizeof(char), 8, (FILE*)fhandle);
	fwrite(disc_title_name, sizeof(char), 8, (FILE*)fhandle);
	fwrite(unknwon_zeroes, sizeof(char), 8, (FILE*)fhandle);
	fwrite(&disc_title_data_offset, sizeof(uint32_t), 1, (FILE*)fhandle);
	fwrite(&disc_title_data_lenght, sizeof(uint32_t), 1, (FILE*)fhandle);
	fwrite(unknwon_zeroes, sizeof(char), 424, (FILE*)fhandle);
	fwrite(disc_content, sizeof(char), 32, (FILE*)fhandle);
	fwrite(disc_title, sizeof(char), 16, (FILE*)fhandle);
	fwrite(unknwon_zeroes, sizeof(char), 976, (FILE*)fhandle);

}

void SFB::close_as(const void* fhandle)
{
	fclose((FILE*)fhandle);
}

void SFB::set_defaults()
{
	strcpy(magic, "\x2E\x53\x46\x42");
	version = 0x00000100;
	strcpy(hybrid_flag, "HYBRID_FLAG");	
	disc_content_data_offset = 0x00020000;
	disc_content_data_lenght = 0x20000000;
	strcpy(disc_title_name, "TITLE_ID");
	disc_title_data_offset = 0x20020000;;
	disc_title_data_lenght = 0x10000000;
	strcpy(disc_content, "g");
	strcpy(disc_title, "UNKNOWN");
}
