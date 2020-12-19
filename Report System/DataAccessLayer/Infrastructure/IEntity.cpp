#pragma once
using namespace std;
class IEntity {
protected:
    int ID;

public:
    string getFilename() const {
        return to_string(ID) + ".txt";
    }

    int getId() const {
        return ID;
    }

    void setId(int id) {
        ID = id;
    }
};