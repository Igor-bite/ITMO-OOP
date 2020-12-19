#pragma once
#include <fstream>
#include <vector>
#include "../Exceptions.cpp"
using namespace std;

template<class T>
class IRepository {
protected:
    string path;
    int nextID;
    vector<int> availableIDs;
public:
    IRepository(string _path) : path(_path){
        nextID = 1;
    }

    virtual T Get(int id) = 0;

    virtual void Create(T& item) = 0;

    virtual void Update(const T& item) = 0;

    virtual void Delete(const T& item) {
        remove((path + item.getFilename()).c_str());
    }

    virtual bool Find(T& item) = 0;

    const vector<int> &getAvailableIDs() const {
        return availableIDs;
    }

};