#ifndef KEY_HPP_
#define KEY_HPP_

#include <string>
using namespace std;

const int keyLength = 128;

class Key : public string {
public:
    Key():string(keyLength, ' ') {}
    Key(const char in[]): string(in) {}
    Key(const string& in): string(in) {}

    virtual ~Key() {}
};

#endif /*KEY_HPP_*/
