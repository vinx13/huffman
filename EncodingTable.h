#ifndef _ENCODINGTABLE_H
#define _ENCODINGTABLE_H

#include "type.h"

class EncodingTable {
public:
    EncodingTable(std::vector<CodeInfo> &code_infos);

    const std::vector<Encoding> getEncodings() const { return encodings_; }

    std::string toString() const;


private:
    std::vector<Encoding> encodings_;

    // increase the number that the binary string represents by one
    void increase(Code &code) const ;
};

#endif
