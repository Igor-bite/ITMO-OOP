#pragma once
#include "Client.cpp"
#include "Transactions.cpp"
#include <map>
#include <utility>

class Bank {
private:
    map<Account*, Client*> accounts = map<Account*, Client*>();
    map<Client*, Account*> accountsReversed = map<Client*, Account*>();
    double weirdAccountLimit;
    vector<Client*> clients = vector<Client*>();
    map<string, ITransaction*> history = map<string, ITransaction*>(); // map<uid, transaction>

    vector<pair<double, double>> depositPercents; // pair<moneyEdge, percent> ; moneyEdge - наибольшая сумма денег, при которой будет процент percent
    double debitRemainderPercent;
    double commissionPercent;

    void addAccountToClient(Account *account, Client *client) {
        accounts[account] = client;
        accountsReversed[client] = account;
    }

    void addToHistory(ITransaction* transaction, string uid) {
        history[uid] = transaction;
    }

public:
    Bank(double weirdAccountLimit, vector<pair<double, double>> depositPercents, double debitRemainderPercent, double commissionPercent) :
    weirdAccountLimit(weirdAccountLimit),
    depositPercents(depositPercents),
    debitRemainderPercent(debitRemainderPercent),
    commissionPercent(commissionPercent) {}

    void addClient(Client *client) {
        clients.push_back(client);
    }

    void changeClientAddress(Client *client, string address) {
        client->changeAddress(address);
        bool isWeird = client->isClientWeird();
        if (!isWeird) {
            auto it = accountsReversed.find(client);
            Account* acc = it->second;

            acc->setIfAccountWeird(isWeird);
        }
    }

    void changeClientPassport(Client *client, int passport) {
        client->changePassport(passport);
        bool isWeird = client->isClientWeird();
        if (!isWeird) {
            auto it = accountsReversed.find(client);
            Account* acc = it->second;

            acc->setIfAccountWeird(isWeird);
        }
    }

    Deposit* makeDepositAccount(Client *client, double incomeMoney, int durationYears) {
        DepositBuilder depositBuilder = DepositBuilder();

        depositBuilder.setIncomeMoney(incomeMoney);
        depositBuilder.setDuration(durationYears);
        depositBuilder.setIsAccountWeird(client->isClientWeird());
        for (int i = 0; i < depositPercents.size(); ++i) {
            if (incomeMoney < depositPercents[i].first) {
                if (i == 0) {
                    depositBuilder.setRemainderPercent(depositPercents[i].second);
                    break;
                } else {
                    depositBuilder.setRemainderPercent(depositPercents[i - 1].second);
                    break;
                }
            } else if (i == depositPercents.size()) {
                depositBuilder.setRemainderPercent(depositPercents[i].second);
                break;
            }
        }
        Deposit *depositAcc = depositBuilder.Build();
        addAccountToClient(depositAcc, client);
        return depositAcc;
    }

    Debit* makeDebitAccount(Client *client, double incomeMoney) {
        DebitBuilder debitBuilder = DebitBuilder();
        debitBuilder.setIncomeMoney(incomeMoney);
        debitBuilder.setRemainderPercent(debitRemainderPercent);
        debitBuilder.setIsAccountWeird(client->isClientWeird());

        Debit *debitAcc = debitBuilder.Build();
        addAccountToClient(debitAcc, client);
        return debitAcc;
    }

    Credit* makeCreditAccount(Client *client, double incomeMoney) {
        CreditBuilder creditBuilder = CreditBuilder();
        creditBuilder.setIncomeMoney(incomeMoney);
        creditBuilder.setCommissionPercent(commissionPercent);
        creditBuilder.setIsAccountWeird(client->isClientWeird());

        Credit *creditAcc = creditBuilder.Build();
        addAccountToClient(creditAcc, client);
        return creditAcc;
    }

    string takeCashFrom(Account* acc, double money) {
        if (!acc->isAccountWeird() || money <= weirdAccountLimit) {
            withdrawalTransaction *transaction = new withdrawalTransaction(acc, money);
            transaction->execute();

            string uid = uuid::generate_uuid_v4();
            addToHistory(transaction, uid);
            return uid;
        } else {
            throw notAuthenticated(accounts[acc]->printAll());
        }
    }

    string takeCashFrom(Account* acc, double money, Date date) {
        if (!acc->isAccountWeird() || money <= weirdAccountLimit) {
            withdrawalTransaction *transaction = new withdrawalTransaction(acc, money, date);
            transaction->execute();

            string uid = uuid::generate_uuid_v4();
            addToHistory(transaction, uid);
            return uid;
        } else {
            throw notAuthenticated(accounts[acc]->printAll());
        }
    }

    string putCash(Account* acc, double money) {
        replenishmentTransaction *transaction = new replenishmentTransaction(acc, money);
        transaction->execute();

        string uid = uuid::generate_uuid_v4();
        addToHistory(transaction, uid);
        return uid;
    }

    string putCash(Account* acc, double money, Date date) {
        replenishmentTransaction *transaction = new replenishmentTransaction(acc, money, date);
        transaction->execute();

        string uid = uuid::generate_uuid_v4();
        addToHistory(transaction, uid);
        return uid;
    }

    string transferMoney(Account* from, Account* to, double money) {
        if (!from->isAccountWeird() || money <= weirdAccountLimit) {
            transferTransaction *transaction = new transferTransaction(from, to, money);
            transaction->execute();

            string uid = uuid::generate_uuid_v4();
            addToHistory(transaction, uid);
            return uid;
        } else {
            throw notAuthenticated(accounts[from]->printAll());
        }
    }

    string transferMoney(Account* from, Account* to, double money, Date date) {
        if (!from->isAccountWeird() || money <= weirdAccountLimit) {
            transferTransaction *transaction = new transferTransaction(from, to, money, date);
            transaction->execute();

            string uid = uuid::generate_uuid_v4();
            addToHistory(transaction, uid);
            return uid;
        } else {
            throw notAuthenticated(accounts[from]->printAll());
        }
    }

    void undoTransaction(string uid) {
        map<string, ITransaction*>::iterator it = history.find(uid);
        ITransaction * transaction = it->second;
        transaction->undo();
        history.erase(it);
    }

    void showEverything() {
        cout << "ACCOUNTS" << endl;
        map <Account*, Client*> :: iterator accIter = accounts.begin();
        for (int i = 0; accIter != accounts.end(); accIter++, i++) {  // выводим их
            cout << i + 1 << ") " << accIter->first->printAll() << " " << accIter->second->printAll() << endl;
        }

        cout << "--------------------------------------------------------------" << endl;

        cout << "CLIENTS" << endl;
        for(int i = 0; i < clients.size(); ++i) {
            cout << i + 1 << ") " << clients[i]->printAll() << endl;
        }

        cout << "--------------------------------------------------------------" << endl;

        cout << "HISTORY" << endl;
        if (history.empty()) {
            cout << "empty" << endl;
        } else {
            map<string, ITransaction *>::iterator histIter = history.begin();
            for (int i = 0; histIter != history.end(); histIter++, i++) {  // выводим их
                cout << i + 1 << ") " << histIter->first << " - " << histIter->second->printAll() << endl;
            }
        }

        cout << "--------------------------------------------------------------" << endl;
    }
};