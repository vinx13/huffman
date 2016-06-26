#include <queue>
#include <sstream>

#include "HuffmanTree.h"

struct PNodeComp {
    bool operator()(const Node *node1, const Node *node2) const {
        return !(*node1 < *node2);
    }
};


HuffmanTree::HuffmanTree(const std::map<unsigned char, int> &freq) {
    build(freq);
}


HuffmanTree::HuffmanTree(std::istream &is, int code_count) {
    std::vector<CodeInfo> code_infos;
    while (code_count-- > 0) {
        Byte byte;
        CodeLength length;
        is.read(reinterpret_cast<char *>(&byte), sizeof(byte));
        is.read(reinterpret_cast<char *>(&length), sizeof(length));
        code_infos.emplace_back(byte, length);
    }
    rebuild(code_infos);
}


void HuffmanTree::build(const std::map<unsigned char, int> freq) {
    if (freq.size() == 1) {
        root_->l = new Node(freq.begin()->second, freq.begin()->first);
        return;
    }
    std::priority_queue<Node *, std::vector<Node *>, PNodeComp> q;

    for (const auto &pair: freq) {
        q.push(new Node(pair.second, pair.first));
    }

    if (q.empty()) return;

    while (q.size() > 1) {
        Node *l = q.top();
        q.pop();
        Node *r = q.top();
        q.pop();

        q.push(new Node(l->weight + r->weight, l, r));
    }
    root_ = q.top();

}


void HuffmanTree::rebuild(const std::vector<CodeInfo> code_infos) {
    root_ = new Node();
    for (const CodeInfo &code_info:code_infos) {
        tryInsert(root_, code_info.second, code_info.first);
    }
}


bool HuffmanTree::tryInsert(Node *node, int len, Byte value) {
    if (node->leaf)
        return false;

    if (len == 1) {
        if (!node->l) {
            node->l = new Node(0, value);
            return true;
        }
        else if (!node->r) {
            node->r = new Node(0, value);
            return true;
        }
        return false;
    }

    if (!node->l)
        node->l = new Node();
    if (tryInsert(node->l, len - 1, value))
        return true;

    if (!node->r)
        node->r = new Node();
    return tryInsert(node->r, len - 1, value);

}


void HuffmanTree::generateCodeInfo(Node *node, CodeLength length, std::vector<CodeInfo> &code_infos) const {
    //calculate the length of code of each character
    if (node->leaf) {
        code_infos.emplace_back(node->value, length);
    } else {
        generateCodeInfo(node->l, length + 1, code_infos);
        generateCodeInfo(node->r, length + 1, code_infos);
    }
}


EncodingTable HuffmanTree::getEncodingTable() const {
    std::vector<CodeInfo> code_infos;
    generateCodeInfo(root_, 0, code_infos);
    return EncodingTable(code_infos);
}


void HuffmanTree::deleteTree(Node *node) {
    if (node->l)
        deleteTree(node->l);
    if (node->r)
        deleteTree(node->r);
    delete node;
}

HuffmanTree::~HuffmanTree() {
    deleteTree(root_);
}
