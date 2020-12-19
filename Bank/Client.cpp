#pragma once
#include <vector>
#include "UUID.cpp"
#include "Exceptions.cpp"
using namespace std;

class Client {
    string name;    // *
    string surname; // *
    string uid;     // *

    string address;
    int passport;

    bool isWeird;

public:
    Client(string name, string surname, string address, int passport, string uid, bool isWeird) :
            name(name),
            surname(surname),
            address(address),
            passport(passport),
            uid(uid), isWeird(isWeird) {}

    bool isClientWeird() {
        return isWeird;
    }

    void changeAddress(string address) {
        this->address = address;
        if (passport != 0) {
            isWeird = false;
        }
    }

    void changePassport(int passport) {
        this->passport = passport;
        if (address != "") {
            isWeird = false;
        }
    }

    string printAll() {
        if (address == "" && passport == 0) {
            return name + " " + surname + " - " + uid + " WEIRD ";
        } else if (address == "") {
            return name + " " + surname + " " + to_string(passport) + " - " + uid + " WEIRD ";
        } else if (passport == 0) {
            return name + " " + surname + " " + address + " - " + uid + " WEIRD ";
        }

        return name + " " + surname + " " + address + " " + to_string(passport) + " - " + uid;
    }
};

class IClientBuilder {
protected:
    string name = "";    // *
    string surname = ""; // *
    string uid = uuid::generate_uuid_v4();     // *

    string address = "";
    int passport = 0;

    bool isWeird;

public:
    virtual IClientBuilder* setName(string name) = 0;

    virtual IClientBuilder* setSurname(string surname) = 0;

    virtual IClientBuilder* setAddress(string address) = 0;

    virtual IClientBuilder* setPassport(int passport) = 0;

    virtual Client *Build() = 0;

    void reset() {
        name = "";
        surname = "";
        uid = uuid::generate_uuid_v4();

        address = "";
        passport = 0;
    }
};

class ClientBuilder : public IClientBuilder {
public:
    IClientBuilder* setName(string name) override {
        this->name = name;
        return this;
    }

    IClientBuilder* setSurname(string surname) override {
        this->surname = surname;
        return this;
    }

    IClientBuilder* setAddress(string address) override {
        this->address = address;
        return this;
    }

    IClientBuilder* setPassport(int passport) override {
        this->passport = passport;
        return this;
    }

    Client *Build() override {
        if (name == "" || surname == "") {
            throw notEnoughToBuildClient();
        }

        if (address == "" || passport == 0) {
            isWeird = true;
        } else {
            isWeird = false;
        }
        Client *client = new Client(name, surname, address, passport, uid, isWeird);
        reset();
        return client;
    }
};

class ClientMaker { // должен быть преднастроенный строитель
    IClientBuilder* builder;

public:
    ClientMaker(IClientBuilder* builder) {
        this->builder = builder;
    }

    IClientBuilder* getBuilder() {
        if (builder == nullptr) {
            throw notConfigured();
        }
        return builder;
    }
};