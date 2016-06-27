#include <sstream>
#include <algorithm>
#include "HuffmanTree.h"


EncodingTable::EncodingTable(std::vector<CodeInfo> &code_infos) {
    if (code_infos.empty()) { return; }

    std::sort(code_infos.begin(),
              code_infos.end(),
              [](
                  const CodeInfo &info1,
                  const CodeInfo &info2
              ) {
                  return info1.second < info2.second || info1.first < info1.
                      first;
              }
    ); //sort code_infos, first by code length and then by ascii value of characters

    auto it = code_infos.begin();

    encodings_.push_back(make_pair(it->first, Code(it->second, false)));
    while (++it != code_infos.end()) {
        Byte byte = it->first;
        CodeLength length = it->second;
        Code code = encodings_.rbegin()->second;

        increase(code);
        code.insert(code.end(), length - code.size(), false);

        encodings_.push_back(make_pair(byte, code));
    }
}


std::string EncodingTable::toString() const {
    std::ostringstream os;
    for (const Encoding &encoding: encodings_) {
        Byte byte = encoding.first;
        CodeLength length = static_cast<CodeLength>(encoding.second.size());
        os.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
        os.write(reinterpret_cast<const char *>(&length), sizeof(length));
    }
    return os.str();
}

void EncodingTable::increase(Code &code) const {
    bool carry = false;
    auto digit = code.rbegin();
    do {
        carry = *digit == true;
        *digit = !(*digit);
        ++digit;
        if (digit == code.rend())
            break;
    } while (carry);
    if (carry) {
        code.insert(code.begin(), true);
    }
}