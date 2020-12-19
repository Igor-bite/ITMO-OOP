#pragma once
#include "Accounts.cpp"

class ITransaction {
protected:
    Account* account;
    double money;
    Date date;

public:
    ITransaction(Account* account, double money) {
        this->account = account;
        this->money = money;
        date = Date();
    }

    ITransaction(Account* account, double money, Date date) {
        this->account = account;
        this->money = money;
        this->date = date;
    }

    virtual void execute() = 0;

    virtual void undo() = 0;

    virtual string printAll() = 0;
};

class withdrawalTransaction : public ITransaction { // снятие
public:
    withdrawalTransaction(Account* account, double money) : ITransaction(account, money) {}
    withdrawalTransaction(Account* account, double money, Date date) : ITransaction(account, money, date) {}

    void execute() override {
        try {
            account->calcPercent(date);
            account->getMoney(money);
        } catch (exception e) {
            cout << e.what() << endl;
        }
    }

    void undo() override {
        account->addMoney(money);
    }

    string printAll() override {
        return "Withdrawaling " + to_string(money) + "RUB from " + account->printAll() + ". Date " + to_string(date.getDay()) + "/" + to_string(date.getMonth()) + "/" + to_string(date.getYear());
    }
};

class replenishmentTransaction : public ITransaction { // пополнение
public:
    replenishmentTransaction(Account* account, double money) : ITransaction(account, money) {}
    replenishmentTransaction(Account* account, double money, Date date) : ITransaction(account, money, date) {}

    void execute() override {
        account->calcPercent(date);
        account->addMoney(money);
    }

    void undo() override {
        try {
            account->getMoney(money);
        } catch (exception e) {
            cout << e.what() << endl;
        }
    }

    string printAll() override {
        return "Putting " + to_string(money) + "RUB to " + account->printAll() + ". Date " + to_string(date.getDay()) + "/" + to_string(date.getMonth()) + "/" + to_string(date.getYear());
    }
};

class transferTransaction : public ITransaction { // перевод
private:
    Account* addressee;
public:
    transferTransaction(Account* account, Account* addressee, double money) : ITransaction(account, money) {
        this->addressee = addressee;
    }
    transferTransaction(Account* account, Account* addressee, double money, Date date) : ITransaction(account, money, date) {
        this->addressee = addressee;
    }

    void execute() override {
        try {
            account->calcPercent(date);
            account->getMoney(money);
            addressee->addMoney(money);
        } catch (exception e) {
            cout << e.what() << endl;
        }
    }

    void undo() override {
        try {
            addressee->getMoney(money);
            account->addMoney(money);
        } catch (exception e) {
            cout << e.what() << endl;
        }
    }

    string printAll() override {
        return "Transfered " + to_string(money) + "RUB from " + account->printAll() + " to " + addressee->printAll() + ". Date " + to_string(date.getDay()) + "/" + to_string(date.getMonth()) + "/" + to_string(date.getYear());
    }
};