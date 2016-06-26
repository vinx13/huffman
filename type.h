#ifndef _TYPE_H
#define _TYPE_H

#include <utility>
#include <cstdint>
#include <vector>

typedef std::vector<bool> Code;
typedef int16_t CodeLength;
typedef uint8_t Byte;
typedef std::pair<Byte, CodeLength> CodeInfo;
typedef std::pair<Byte, Code> Encoding;

#endif
