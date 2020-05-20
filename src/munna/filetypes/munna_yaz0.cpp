#include "munna/filetypes/munna_yaz0.h"

namespace munna {
	yaz0::yaz0(munna::file& input, munna::file& output) {
		assert(input.read32() == 0x59617A30); //magic == "Yaz0"
		uncompressed_size = input.read32(); //yaz0+4 = uncompressed size
		input.seek(0x10);
		//for (u32 i = 0; i < uncompressed_size; ) {
		//for (int i = 0; i < 0x20; i++) {
		while (bytes_written <= uncompressed_size) {
			if (bits_left == 0) { //get new flag byte
				bits_left = 8;
				flags = input.readbyte();
				//printf("new byte: %02X\n", flags);
			}
			if (flags & (1 << (bits_left - 1))) {
				//printf("1\n");
				memmove((void*)&this->buffer[0], (void*)&this->buffer[1], 0xFFE); //move buffer back one byte
				this->buffer[0xFFE] = input.readbyte(); //set latest byte in buffer
				output.write(this->buffer[0xFFE]); //copy one byte to output
				bytes_written += 1;
			} else {
				//printf("0\n");
				rle_data = input.read16();
				if (rle_data >> 12)
					read_size = (rle_data >> 12) + 2;
				else
					read_size = input.readbyte() + 0x12;
				distance = (rle_data & 0xFFF) + 1;
				//printf("rle_data: %04X\n", rle_data);
				//printf("read_size: %d\n", read_size);
				//printf("distance: %03X\n", distance);
				if (read_size <= distance) {
					for (int j = 0; j < read_size; j++) {
						output.write(0x00);
					}
					/*printf("%016X\n", output.tell());
					output.seek(output.tell() - distance); //seek backwards
					printf("%016X\n", output.tell());
					input.read(&this->buffer[0xFFF - read_size], read_size);
					printf("%08X", 0xFFF - read_size);
					for (int j = 0; j < read_size; j++) {
						printf("%02X", this->buffer[(0xFFE - read_size) + j]);
					}
					printf("\n");
					output.seek(0, 2);
					//memset(this->buffer, 0, 0xFFF);
					output.write(&this->buffer[0xFFE - read_size], read_size);
					/*output.write(&this->buffer[0xFFF - distance], read_size);
					memmove((void*)&this->buffer[0], (void*)&this->buffer[read_size], read_size);*/
				} else {
					for (int j = 0; j < read_size; j++) {
						output.write(0x00);
					}
					/*for (int j = 0; j < read_size; j++) {
						output.write(0x00);
						memmove((void*)&this->buffer[0], (void*)&this->buffer[1], 0xFFE);
						this->buffer[0xFFE] = 0x00;
						//output.write(this->buffer[0xFFF - distance]);
						//memmove((void*)&this->buffer[0], (void*)&this->buffer[1], 0xFFE);
					}*/
				}
				/*if (read_size > distance) { //edge case, byte repeats itself
					output.write(&this->buffer[0xFFF - distance], distance);
					memmove((void*)&this->buffer[0], (void*)&this->buffer[distance], distance);
					for (int i = 0; i < read_size - distance; i++) {
						output.write(this->buffer[0xFFE]);
					}
				} else {
					output.write(&this->buffer[0xFFF - distance], read_size);
					memmove((void*)&this->buffer[0], (void*)&this->buffer[read_size], read_size);
				}*/
				/*for (int j = 0; j < read_size; j++)
					output.write(0x00);*/
				bytes_written += read_size;
			}
			bits_left--;
		}
		printf("bytes_written: %08X\n", bytes_written);
	}
}