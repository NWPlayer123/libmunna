#include "types.h"
#include "munna/munna_file.h"
#include "munna/filetypes/munna_yaz0.h"

int main(void) {
	/*munna::file f = munna::file("fuck.txt", "wb");
	char* data = "fuck\n";
	f.write(data, strlen(data));*/
	munna::file input = munna::file("../testfiles/course_model.szs", "rb");
	munna::file output = munna::file("../testfiles/course_model.sarc", "wb");
	munna::yaz0 arc = munna::yaz0(input, output);
}