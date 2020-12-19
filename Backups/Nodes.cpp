//
// Created by Игорь Клюжев on 13.11.2020.
//
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class AlreadyExists : public exception {
public:
    const char * what() const throw()
    {
        return "This file/directory already exists";
    }
};

class MyException : public exception {
public:
    const char * what() const throw()
    {
        return "Something went wrong";
    }
};

class INode {
protected:
    string Name;
    double Size;
public:
    string getName() {
        return Name;
    }

    double getSize() {
        return Size;
    }

    INode() = default;

    virtual void Display() {};

    virtual ~INode() = default;
};

class File : public INode {
public:
    void Display() override {
        cout << "File: " << Name << endl;
    }

    File(string name, double size) {
        Name = name;
        Size = size;
    }

    void changeFileWithSize(int newSize) {
        Size = newSize;
    }
};

class Directory : public INode {
public:
    void Display() override {
        cout << "Directory: " << Name << endl;
        for (INode* child : children) {
            File *childFile = dynamic_cast<File *>(child);
            Directory *childDirectory = dynamic_cast<Directory *>(child);
            if (childFile) {
                childFile->Display();
            } else if (childDirectory) {
                childDirectory->Display();
            } else {
                 throw MyException();
            }
        }
    }

    void Add(INode *node) {
        children.push_back(node);
        Size += node->getSize();
    }

    void Remove(INode *node) {
        for (int i = 0; i < children.size(); ++i) {
            if (children[i]->getName() == node->getName()) {
                children.erase(children.begin() + i);
                Size -= node->getSize();
                break;
            }
        }
    }

    Directory(string name) {
        Name = name;
        Size = 0;
    }

private:
    vector<INode*> children;

};