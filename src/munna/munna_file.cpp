#include "munna/munna_file.h"

namespace munna {
	file::file(const char* filename, const char* mode, int endian) {
		if (!strcmp(mode, "r") || !strcmp(mode, "rb"))
			this->handle = std::fstream(filename, std::ios::binary | std::ios::in);
		else if (!strcmp(mode, "w") || !strcmp(mode, "wb"))
			this->handle = std::fstream(filename, std::ios::binary | std::ios::out);
		else if (!strcmp(mode, "wb+")) //TODO: more?
			this->handle = std::fstream(filename, std::ios::binary | std::ios::in | std::ios::out);
		this->do_swap = endian != IS_BIG_ENDIAN;
		if (this->do_swap)
			this->parse = std::make_unique<parse_swap>();
		else
			this->parse = std::make_unique<parse_norm>();
		memset(this->buffer, 0, 128); //TODO: find good size?
	}

	file::~file() {
		this->handle.close();
	}

	/************* File Position Handling *************/
	u64 file::tell(void) {
		return this->handle.tellg();
	}

	void file::seek(u64 pos) {
		this->handle.seekg(pos);
	}

	void file::seek(s64 off, int pos) {
		switch (pos) {
			case 0:
				this->handle.seekg(off, std::ios::beg);
				break;
			case 1:
				this->handle.seekg(off, std::ios::cur);
				break;
			case 2:
				this->handle.seekg(off, std::ios::end);
				break;
		}
	}

	// have to do (end - beg) because neither are guaranteed to be 0
	u64 file::size(void) {
		std::streampos initialpos = this->tell();
		this->seek(0, std::ios::beg); //start of file
		this->filesize = this->tell(); // !!! NOT ALWAYS 0 !!!
		this->seek(0, std::ios::end); //end of file
		this->filesize = this->tell() - this->filesize;
		this->seek(initialpos);
		return this->filesize;
	}

	void file::read(char* handle, size_t num_bytes) { //accessor since handle is private
		this->handle.read(handle, num_bytes);
	}

	void file::write(char* data, size_t num_bytes) {
		this->handle.write(data, num_bytes);
	}

	void file::write(u8 byte) {
		this->buffer[0] = (char)byte;
		this->handle.write((char*)&this->buffer, 1);
	}

	/************* Parsing Data, Pipe to Endian *************/
	u8 file::readbyte(void) {
		this->handle.read((char*)&this->lastread, 1);
		return this->lastread;
	}

	u16 file::read16(void) {
		this->parse->read16(this->handle, this->lastread);
		return this->lastread;
	}

	u32 file::read32(void) {
		this->parse->read32(this->handle, this->lastread);
		return this->lastread;
	}

	u64 file::read64(void) {
		this->parse->read64(this->handle, this->lastread);
		return this->lastread;
	}

	/************* Parsing Data, Swap Endian *************/
	void parse_swap::read16(std::fstream& f, u64& value) {
		f.read((char*)&value, 2);
		value = __builtin_bswap16((u16)value);
	}

	void parse_swap::read32(std::fstream& f, u64& value) {
		f.read((char*)&value, 4);
		value = __builtin_bswap32((u32)value);
	}

	void parse_swap::read64(std::fstream& f, u64& value) {
		f.read((char*)&value, 8);
		value = __builtin_bswap64((u64)value);
	}

	/************* Parsing Data, Keep Endian *************/
	void parse_norm::read16(std::fstream& f, u64& value) {
		f.read((char*)&value, 2);
	}

	void parse_norm::read32(std::fstream& f, u64& value) {
		f.read((char*)&value, 4);
	}

	void parse_norm::read64(std::fstream& f, u64& value) {
		f.read((char*)&value, 8);
	}
}