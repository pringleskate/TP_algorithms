#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <algorithm>


//#include "Huffman.h"

void printBuffer(const std::vector<unsigned char> &buffer)
{
    for (auto &v: buffer)
    {
        std::cout << std::bitset<8>(v) << "|";
    }
    std::cout << std::endl;
}

class BitWriter
{
public:
    BitWriter() : bitCounter(0) {}

    void WriteBit(unsigned char bit)
    {
        if (bitCounter % 8 == 0)
            buffer.push_back(0);
        if (bit == '1')
            buffer[bitCounter/8] |= (unsigned)1 << (7 - bitCounter % 8);
        bitCounter++;
    }

    void WriteByte(unsigned char byte)
    {
        if (bitCounter % 8 == 0)
        {
            buffer.push_back(byte);
        }
        else
        {
            int offset = bitCounter % 8;
            buffer[bitCounter/8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }

        bitCounter += 8;
    }

    void SetLastByteOffset(size_t offset) { buffer.push_back(offset); }

    const std::vector<unsigned char> &GetBuffer() const
    {
        return buffer;
    }

    size_t GetBitCounter() const
    {
        return bitCounter;
    }


private:
    std::vector<unsigned char> buffer;
    size_t bitCounter;
};

class BitReader
{
public:
    BitReader() : bitCounter(0) {}

    unsigned char ReadBit()
    {
        size_t tmp_counter = bitCounter;
        bitCounter++;
        unsigned char bit = buffer[tmp_counter / 8] & ((unsigned)1 << (7 - tmp_counter % 8));
        return bit >> (7 - tmp_counter % 8);
    }

    unsigned char ReadByte()
    {
        size_t tmp_counter = bitCounter;
        bitCounter += 8;

        if (tmp_counter % 8 == 0)
            return buffer[tmp_counter / 8];
        else
        {
            unsigned char byte = buffer[tmp_counter / 8] & ((unsigned)255 >> tmp_counter % 8);
            unsigned char byte_2 = buffer[tmp_counter / 8 + 1] & ((unsigned)255 << (8 - tmp_counter % 8));

            byte = byte << tmp_counter % 8;
            byte_2 = byte_2 >> (8 - tmp_counter % 8);

            byte |= byte_2;

            return byte;
        }
    }

    const std::vector<unsigned char> &GetBuffer() const
    {
        return buffer;
    }

    size_t GetBitCounter() const
    {
        return bitCounter;
    }

    void SetBuffer(std::vector<unsigned char> bits_buffer) { buffer = std::move(bits_buffer); }

    size_t GetBufferSize() { return buffer.size() - 1; }

    void SetBitCounter(int count) { bitCounter = count; }

    size_t GetLastByteOffset() { return buffer[buffer.size() - 1]; }

    size_t GetCurrentByte() { return (bitCounter/8); }
private:
    std::vector<unsigned char> buffer;
    size_t bitCounter;
};


struct Node {
    unsigned char _data;
    size_t _frequency;
    Node * _left;
    Node * _right;

    Node(unsigned char data, size_t frequency) : _data(data), _frequency(frequency), _left(nullptr), _right(nullptr) {}
};

struct NodeComparator
{
public:
    bool operator()(const Node *node_1, const Node *node_2)
    {
        return node_1->_frequency > node_2->_frequency;
    }
};

class Encoder{
private:
    std::vector<unsigned char> symbol_vector;

    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> node_heap;

    std::unordered_map<unsigned char, size_t> frequencyMap;

    std::unordered_map<unsigned char, std::string> codes_table;

    std::vector<unsigned char> encoded_data;

    Node *root;

    BitWriter writer;
public:
   // Encoder(IInputStream& original)
    Encoder()
    {
       /* unsigned char value;
        while (original.Read(value))
        {
           symbol_vector.push_back(value);
        }
        */

        unsigned char sym;
        for (int i = 0; i < 99; i++) {
            std::cin >> sym;
            symbol_vector.push_back(sym);
        }
        root = nullptr;
    }

    void buildFrequencyHeap ()
    {

        for (auto sym : symbol_vector)
        {
            ++frequencyMap[sym];
        }

        for (auto elem : frequencyMap)
        {
            node_heap.push(new Node(elem.first, elem.second));
        }
    }

    void buildHuffmanTree()
    {
        Node *left, *right, *combo;
        while (node_heap.size() > 1)
        {
            left = node_heap.top();
            node_heap.pop();

            right = node_heap.top();
            node_heap.pop();

            combo = new Node(0, left->_frequency + right->_frequency);
            combo->_left = left;
            combo->_right = right;
            node_heap.push(combo);
        }

       root = node_heap.top();
       getCodesTable(root, "");
       encodeTree(root);
       encodeData();
    }

    void getCodesTable(Node *node, const std::string& trace)
    {
        if (node ->_left == nullptr) {
            codes_table[node->_data] = trace;
            return;
        }

        getCodesTable(node->_left, trace + "0");
        getCodesTable(node->_right, trace + "1");
    }

    void encodeTree(Node *node)
    {
        if (node == root)
            writer.WriteByte(codes_table.size());

        if (node->_left == nullptr)
        {
            writer.WriteBit('1');
            writer.WriteByte(node->_data);
        }
        else
        {
            encodeTree(node->_left);
            encodeTree(node->_right);
            writer.WriteBit('0');
        }
    }

    void encodeData()
    {
        for (auto elem : symbol_vector)
        {
            for (auto str: codes_table[elem])
            {
                writer.WriteBit(str);
            }
        }

        writer.SetLastByteOffset(writer.GetBitCounter() % 8);
    }

    //std::vector<unsigned char> getEncodedData(IOutputStream& compressed)
    std::vector<unsigned char> getEncodedData()
    {
       // for (auto v : vec)
         //   compressed.Write(v);
       return writer.GetBuffer();
    }
};


void Encode(std::vector<unsigned char> &compressed)
{
    Encoder encoder;
    encoder.buildFrequencyHeap();
    encoder.buildHuffmanTree();
    compressed = encoder.getEncodedData();
}
/*

void Encode(IInputStream& original, IOutputStream& compressed)
{
    Encoder encoder(original);
    encoder.buildFrequencyHeap();
    encoder.buildHuffmanTree();
   // compressed = encoder.getEncodedData();
    encoder.getEncodedData(compressed);
}
*/

class Decoder
{
private:
    BitReader reader;

    size_t alphabet_size;

    size_t last_offset;

    std::vector<unsigned char> original;

    Node *root;
public:
    //Decoder(IInputStream& compressed)
    Decoder(std::vector<unsigned char> vec)
    {
       /* unsigned char value;
        while (compressed.Read(value))
        {
            vec.push_back(value);
        }*/
        reader.SetBuffer(vec);
    }

    void decodeTree()
    {
        last_offset = reader.GetLastByteOffset();
        alphabet_size = reader.ReadByte();

        unsigned char bit;
        std::stack<Node *> tree_stack;

        size_t i = 0;
        while (true)
        {
            bit = reader.ReadBit();
            Node *node;
            if (bit == 0)
            {
                node = new Node(0, 1);
                node->_right = tree_stack.top();
                tree_stack.pop();

                node->_left = tree_stack.top();
                tree_stack.pop();

                tree_stack.push(node);
            }
            else if (bit == 1)
            {
                node = new Node(reader.ReadByte(), 1);
                tree_stack.push(node);
                i++;
            }

            if (i >= alphabet_size && tree_stack.size() == 1)
                break;
        }

        root = tree_stack.top();
    }

    //void readCompressedData(IOutputStream& output)
    void readCompressedData()
    {
        unsigned char bit;
        int i = 0;
        while (true) {
            if (reader.GetCurrentByte() == reader.GetBufferSize() - 1 && reader.GetBitCounter() % 8 == last_offset)
                break;

            Node *node = root;
            while (node->_left != nullptr)
            {
                bit = reader.ReadBit();
                if (bit == 0)
                    node = node->_left;
                else if (bit == 1)
                    node = node->_right;
                i++;
            }
            original.push_back(node->_data);
        }
    }
};

/*
void Decode(IInputStream& compressed, IOutputStream& original)
{
    Decoder decoder(compressed);
    decoder.decodeTree();
    decoder.readCompressedData(original);
}
*/

void Decode(std::vector<unsigned char> &compressed)
{
    Decoder decoder(compressed);
    decoder.decodeTree();
    decoder.readCompressedData();
}

int main(int argc, const char * argv[])
{
    std::vector<unsigned char> compressed;
    Encode(compressed);
    Decode(compressed);
    return 0;
}
