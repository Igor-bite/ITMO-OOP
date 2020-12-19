#include <iostream>
#include "Bank.cpp"

int main() {
    ClientBuilder clientBuilder = ClientBuilder();
    ClientMaker clientMaker = ClientMaker(&clientBuilder);

    Client *client1 = clientMaker.getBuilder()->setName("Client1NAME")->
            setSurname("Client1SURNAME")
            ->Build(); // подозрительный клиент

    Client *client2 = clientMaker.getBuilder()->setName("Client2NAME")->
            setSurname("Client2SURNAME")->
            setAddress("Miami beach 32/2")->
            setPassport(426182)->
            Build();

    double weirdAccountLimit = 30000;
    vector<pair<double, double>> depositPercents = {{50000, 3}, {100000, 3.5}, {100000, 4}};
    // Если открываем депозит до 50 000 р. - 3%, если от 50 000 р. до 100 000 р. - 3.5%, больше 100 000 р. - 4%.
    double debitRemainderPercent = 7;
    double commissionPercent = 2;

    double _weirdAccountLimit = 40000;
    vector<pair<double, double>> _depositPercents = {{30000, 3}, {40000, 4}, {60000, 6}, {1000000, 8}, {1000000, 10}};
    // Если открываем депозит до 30 000 р. - 3%, если от 30 000 р. до 40 000 р. - 4%, если от 40 000 р. до 60 000 р. - 6%, если от 60 000 р. до 1 000 000 р. - 8%, больше 1 000 000 р. - 10%.
    double _debitRemainderPercent = 5;
    double _commissionPercent = 1;

    Bank bank1 = Bank(weirdAccountLimit, depositPercents, debitRemainderPercent, commissionPercent);
    Bank bank2 = Bank(_weirdAccountLimit, _depositPercents, _debitRemainderPercent, _commissionPercent);

    bank1.addClient(client1);
    bank1.addClient(client2);

    bank2.addClient(client1);
    bank2.addClient(client2);

    Debit *deb = bank1.makeDebitAccount(client1, 30000);
    Credit *cred = bank1.makeCreditAccount(client1, 100000);
    Deposit *dep = bank1.makeDepositAccount(client2, 70000, 10);

    Date date1 = Date(12, 11, 2020);
    Date date2 = Date(18, 11, 2020);
    Date date3 = Date(8, 12, 2020);
    Date date4 = Date(3, 4, 2021);
    Date date5 = Date(4, 5, 2021);
    Date date6 = Date(4, 6, 2021);
    Date date7 = Date(12, 10, 2021);
    Date date8 = Date(5, 5, 2022);

    try {
        bank1.showEverything();

        bank1.putCash(deb, 1000, date1);
        bank1.showEverything();

        bank1.takeCashFrom(deb, 2000, date2);
        bank1.showEverything();

        string id1 = bank1.transferMoney(deb, dep, 10000, date3);
        bank1.showEverything();

        string id2 = bank1.takeCashFrom(deb, 5000, date4);
        bank1.showEverything();

        bank1.undoTransaction(id2);
        bank1.showEverything();

        deb->calcPercent(date5);
        bank1.showEverything();

        bank1.putCash(deb, 700000, date6);
        bank1.showEverything();

        bank1.takeCashFrom(cred, 40000, date7);
        bank1.showEverything();

        bank1.undoTransaction(id1);
        bank1.showEverything();

        bank1.changeClientAddress(client1, "Nevskiy prospekt 6");
        bank1.changeClientPassport(client1, 654793);
        bank1.showEverything();

        bank1.takeCashFrom(cred, 40000, date8);
        bank1.showEverything();

    } catch (exception& e) {
        cerr << e.what() << endl;
    }

//    bank2.showEverything();

    return 0;
}
