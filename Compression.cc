#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>


void compress(const std::string &src, const std::string &dest) {
};

void decompress(const std::string &src, const std::string &dest) {
}

void usage(const std::string &prog) {
    std::cerr << "Useage: " << std::endl
      << "    " << prog << "[-d] input_file output_file" << std::endl;
    std::exit(2);
}

int main(int argc, char* argv[]) {
    int i;
    std::string src, dest;
    bool isDecompress = false;
    for (i = 1; i < argc; i++) {
        if (argv[i] == std::string("-d")) isDecompress = true;
        else if (src == "") src = argv[i];
        else if (dest == "") dest = argv[i];
        else usage(argv[0]);
    }
    if (dest == "") usage(argv[0]);
    if (isDecompress) decompress(src, dest);
    else compress(src, dest);
    return 0;
}
