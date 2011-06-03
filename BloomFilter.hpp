#ifndef BLOOMFILTER_HPP_
#define BLOOMFILTER_HPP_

#include <limits.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <math.h>
#include "Key.hpp"
#include "DataStructure.hpp"

class BloomFilter : public DataStructure {
public:
    BloomFilter() {}
    BloomFilter(unsigned long length) {
        init(length);
    }
    virtual ~BloomFilter() {}

    void init(unsigned long length);
    void dump();

    void add(const Key& key);
    void del(const Key& key);
    int testExist(const Key& key, bool verbose = false);

private:
    unsigned long hash1(const Key& key);
    unsigned long hash2(const Key& key);
    bool exist(const Key& key);

    vector<unsigned long> m_tickBook;
    unsigned long m_length; // number of bits in the bloom filter
    unsigned long m_pockets; //the number of pockets

    static const unsigned long m_pocketSize; //bits in each pocket
};

#endif /*BLOOMFILTER_HPP_*/

