#ifndef _HUFFMANTREE_H
#define _HUFFMANTREE_H

#include <iostream>
#include <map>
#include <vector>

#include "type.h"
#include "EncodingTable.h"

struct Node {
    int weight;
    bool leaf;
    unsigned char value;
    Node *l, *r;

    Node(int weight, unsigned char value) :
        weight(weight), value(value), l(nullptr), r(nullptr), leaf(true) { }

    Node(int weight = 0, Node *l = nullptr, Node *r = nullptr) :
        weight(weight), value(0), l(l), r(r), leaf(false) { }

    bool operator<(const Node &node) const {
        return weight < node.weight;
    }
};

class HuffmanTree {
public:

    HuffmanTree(const std::map<unsigned char, int> &freq);

    HuffmanTree(std::istream &is, int code_count);

    ~HuffmanTree();

    bool tryInsert(Node *node, int len, Byte value);

    void generateCodeInfo(Node *node, CodeLength length, std::vector<CodeInfo> &code_infos) const;

    void rebuild(const std::vector<CodeInfo> code_infos);

    EncodingTable getEncodingTable() const;

    void build(const std::map<Byte, int> freq);

    Node *getRoot() { return root_; }

private:
    Node *root_;

    void deleteTree(Node *node);

};

#endif
