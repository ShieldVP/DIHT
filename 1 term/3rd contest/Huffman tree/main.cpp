#include "HuffmanTree.h"

int main() {
    HuffmanTree archiver;
    archiver.encodeFile(R"(C:\Clion_projects\huffman\target.txt)", R"(C:\Clion_projects\huffman\archive)");
    archiver.decodeFile(R"(C:\Clion_projects\huffman\archive)", R"(C:\Clion_projects\huffman\result.txt)");
    return 0;
}