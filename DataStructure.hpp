#ifndef DATASTRUCTURE_HPP_
#define DATASTRUCTURE_HPP_

class DataStructure {
public:
    DataStructure() {
        countAdd=0;
        countDelete=0;
        countFind=0;
    }
    virtual ~DataStructure() {}

    void resetCountAdd() {
        countAdd=0;
    }
    void resetCountFind() {
        countFind=0;
    }
    void resetCountDelete() {
        countDelete=0;
    }

    unsigned int getCountAdd() {
        return countAdd;
    }
    unsigned int getCountDelete() {
        return countDelete;
    }
    unsigned int getCountFind() {
        return countFind;
    }

protected:
    unsigned int countAdd;
    unsigned int countDelete;
    unsigned int countFind;
};

#endif /*DATASTRUCTURE_HPP_*/
