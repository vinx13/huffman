Compression: Compression.cpp EncodingTable.cpp HuffmanTree.cpp
	g++ -g -o $@ $^ -Wall -std=c++11
clean:
	rm *.o *.tmp Compression -f
