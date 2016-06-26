#ifndef _CODEC_H
#define _CODEC_H

#include "HuffmanTree.h"
#include "EncodingTable.h"

class Decoder {
public:
    Decoder(Node *root) : root_(root), cur_(root_) { }

    void reset() { cur_ = root_; }

    //advance into next level of encoding tree and return if current node is leaf
    bool advance(bool dir) {
        cur_ = dir ? cur_->r : cur_->l;
        return cur_->leaf;
    }

    Byte getValue() { return cur_->value; }

private:
    Node *root_, *cur_;
};

class Encoder {
public:
    Encoder(const EncodingTable &encoding_table) {
        const auto &encodings = encoding_table.getEncodings();
        for (auto encoding: encodings) {
            encoding_map_[encoding.first] = encoding.second;
        }
    }

    const std::vector<bool> &get(Byte byte) const {
        return encoding_map_.at(byte);
    }

private:
    std::map<Byte, Code> encoding_map_;
};

#endif
