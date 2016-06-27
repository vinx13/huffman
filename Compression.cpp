#include <iostream>
#include <fstream>
#include <sstream>
#include "HuffmanTree.h"
#include "Codec.h"


void compress(const std::string &src, const std::string &dest) {
    std::ifstream is(src);
    std::ofstream os(dest);

    unsigned char byte;
    std::map<Byte, int> freq;
    std::vector<Byte> inputs;
    while (is.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
        ++freq[byte];
        inputs.push_back(byte);
    }
    if (freq.empty()) return; // empty file

    HuffmanTree tree(freq);
    EncodingTable encoding_table = tree.getEncodingTable();
    Encoder encoder(encoding_table);

    uint8_t code_count = static_cast<unsigned char>(freq.size()-1); // decrease by one to prevent overflow
    os.write(reinterpret_cast<char *>(&code_count), sizeof(code_count));
    os << encoding_table.toString();

    int32_t char_count = inputs.size();
    os.write(reinterpret_cast<char *>(&char_count), sizeof(char_count));

    Byte buf = 0;
    int len = 0;

    for (const auto uc: inputs) {
        const auto &code = encoder.get(uc);
        for (bool bit: code) {
            buf |= bit;
            ++len;
            if (len == 8) {
                os.write(reinterpret_cast<char *>(&buf), sizeof(buf));
                buf = 0;
                len = 0;
            } else {
                buf <<= 1;
            }
        }
    }
    if (len) {
        buf <<= 7 - len;
        os.write(reinterpret_cast<char *>(&buf), sizeof(buf));
    }
};


void decompress(const std::string &src, const std::string &dest) {
    std::ifstream is(src);
    std::ofstream os(dest);

    uint8_t code_count;
    is.read(reinterpret_cast<char *>(&code_count), sizeof(code_count));
    if (!is) return; //empty file

    HuffmanTree tree(is, code_count + 1);
    Decoder decoder(tree.getRoot());
    int32_t char_count;
    is.read(reinterpret_cast<char *>(&char_count), sizeof(char_count));
    Byte buf;
    int len = 0;
    Byte mask = 1 << 7;
    while (true) {
        while (len-- > 0) {
            bool result = decoder.advance(mask & buf);
            buf <<= 1;
            if (result) {
                Byte value = decoder.getValue();
                os.write(reinterpret_cast<char *>(&value), sizeof(value));
                decoder.reset();
                --char_count;
                if (char_count == 0) return;
            }
        }
        is.read(reinterpret_cast<char *>(&buf), sizeof(buf));
        len = 8;
    }
}


void usage(const std::string &prog) {
    std::cerr << "Usege: " << std::endl
    << "    " << prog << "[-d] input_file output_file" << std::endl;
    std::exit(2);
}


int main(int argc, char *argv[]) {
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
