#ifndef SKIPLIST_HPP_
#define SKIPLIST_HPP_

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "Key.hpp"
#include "DataStructure.hpp"

using namespace std;

class SkipListNode;
class SkipListNode: public Key {
public:
    SkipListNode(const Key& key, int height):Key(key),m_links(height) {
        assert (height>0);
        int i;
        for (i=0; i<height; i++) {
            m_links[i] = NULL;
        }
    }
    virtual ~SkipListNode() {}


	/* Returns the height of this node */
    unsigned int height() {
        return m_links.size();
    };

	/* Returns a pointer to the next node at a given level */

    SkipListNode* nextAtLevel(unsigned int level) {
        assert (level >=0 && level < m_links.size());
        return m_links[level];
    };

	/* Setup pointer at the next node at a given level */

    void setNextAtLevel(unsigned int level, SkipListNode* next) {
        assert (level >=0 && level < m_links.size());
        m_links[level] = next;
    }

private:
    SkipListNode() {}
    vector<SkipListNode*> m_links;
};



class SkipList : public DataStructure {
public:
    SkipList() {}
    SkipList(int maxHeight) {
        init(maxHeight);
    }
    virtual ~SkipList();
    void init(int maxHeight);


    // ADD FUNCTIONS
    int add(const Key& key, bool verbose=false);
    unsigned int randHeight();
    int add(SkipListNode* target, SkipListNode* newNode, unsigned int level);

    // FIND FUNCTIONS
    int find(const Key &key, bool verbose=false);
    SkipListNode* find(SkipListNode* target, const Key& key, unsigned int level);


    //DEL FUNCTIONS
    int del(const Key& key, bool verbose= false);
    SkipListNode* del(SkipListNode* target, const Key& key, unsigned int level);

    //DUMP FUNCTION
    void dump(char sep = '\n');
	
    int elements;


private:
    SkipListNode* m_head;
    unsigned int m_maxHeight;
};

#endif /*SKIPLIST_HPP_*/
