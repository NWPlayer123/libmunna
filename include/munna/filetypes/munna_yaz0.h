#pragma once

#include "munna/munna_file.h"
#include "types.h"

namespace munna {
	class yaz0 {
		public:
			yaz0(munna::file& input, munna::file& output);
		private:
			u32 uncompressed_size; //from header
			u32 bytes_written; //for buffer loop
			u8 flags = 0;
			u8 bits_left = 0;
			u16 rle_data;
			u8 read_size;
			u16 distance;
			char buffer[0xFFF] = {0}; //rle_data & 0xFFF
	};
}