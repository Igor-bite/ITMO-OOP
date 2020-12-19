#pragma once
#include "Date.cpp"

class Account {
protected:
    bool isWeird;

    double money;
    double remainderPercent;
    double remainderDayKoeff; // Например, 3.65% годовых. Значит в день: 3.65% / 365 дней = 0.01%
    double moneyToPay;

    Date lastCalcOfPercent;
    Date creationDate;
public:
    void addTodaysRemainder() { // вызывается в конце каждого дня - начисление процента на остаток
        moneyToPay += money * remainderDayKoeff;
    }

    void addRemainder() { // вызывается в конце каждого года
        money += moneyToPay;
        moneyToPay = 0;
    }

    virtual void calcPercent(Date date) {
        if (date.isLaterThenDate(lastCalcOfPercent)) {
            int yFromCreation = date.getYear() - creationDate.getYear();
            int alreadyAddedDays = abs(lastCalcOfPercent.getDay() - creationDate.getDay());
            int alreadyAddedMonths = abs(lastCalcOfPercent.getMonth() - creationDate.getMonth());
            alreadyAddedDays += (alreadyAddedMonths) * 30;

            for (int j = 0; j < yFromCreation; ++j) {
                for (int i = 0; i < 365 - alreadyAddedDays; ++i) {
                    addTodaysRemainder();
                }
                addRemainder();
            }

            int years = date.getYear() - creationDate.getYear();
            int months = abs(date.getMonth() - lastCalcOfPercent.getMonth());
            int days = abs(date.getDay() - lastCalcOfPercent.getDay());
            days += months * 30;

            for (int j = 0; j < years; ++j) {
                for (int i = 0; i < 365; ++i) {
                    addTodaysRemainder();
                }
                addRemainder();
            }

            for (int i = 0; i < days; ++i) {
                addTodaysRemainder();
            }
        }
        lastCalcOfPercent = date;
    }

    bool isAccountWeird() {
        return isWeird;
    }

    void setIfAccountWeird(bool isweird) {
        isWeird = isweird;
    }

    void addMoney(double money) {
        this->money += money;
    }

    virtual void getMoney(double money) = 0;

    string printAll() {
        return "Account with " + to_string(money) + "RUB  and percent " + to_string(remainderPercent) + " created on " + to_string(creationDate.getDay()) + "/" + to_string(creationDate.getMonth()) + "/" + to_string(creationDate.getYear());
    }
};

class IAccountBuilder {
protected:
    double money = 0;
    double remainderPercent = 0;
    bool isWeird = true;
public:
    virtual IAccountBuilder* setIncomeMoney(double money) = 0;

    IAccountBuilder* setIsAccountWeird(bool isWeird) {
        this->isWeird = isWeird;
        return this;
    }
};

/*
 * Депозит – счет, с которого нельзя снимать и переводить деньги до тех пор,
 * пока не закончится его срок (пополнять можно). Процент на остаток зависит
 * от изначальной суммы, например, если открываем депозит до 50 000 р. - 3%,
 * если от 50 000 р. до 100 000 р. - 3.5%, больше 100 000 р. - 4%. Комиссий нет.
 */
class Deposit : public Account {
private:
    Date endDate;
public:
    Deposit(double firstIncome, double percent, Date endDate, bool isWeird) {
        moneyToPay = 0;
        money = firstIncome;
        remainderPercent = percent;
        remainderDayKoeff = percent / 365 / 100;
        creationDate = Date();
        lastCalcOfPercent = creationDate;
        this->endDate = endDate;
        this->isWeird = isWeird;
    }

    void getMoney(double money) override {
        if (this->money < money) {
            throw notEnoughMoneyOnAccount();
        }
        if (this->endDate.isLaterThenNow()) {
            throw notYetAvailable(endDate);
        }
        this->money -= money;
    }
};

class DepositBuilder : public IAccountBuilder {
private:
    int durationYears = 0;
public:
    IAccountBuilder * setIncomeMoney(double money) override {
        this->money = money;
        return this;
    }

    IAccountBuilder * setRemainderPercent(double percent)  {
        this->remainderPercent = percent;
        return this;
    }

    IAccountBuilder * setDuration(int years) {
        durationYears = years;
        return this;
    }

    Deposit *Build() {
        if (durationYears == 0) {
            throw notConfigured();
        }
        Date endDate = Date(durationYears);
        Deposit *depositAccount = new Deposit(money, remainderPercent, endDate, isWeird);
        return depositAccount;
    }
};

/*
 * Дебетовый счет – обычный счет с фиксированным процентом на остаток.
 * Деньги можно снимать в любой момент, в минус уходить нельзя. Комиссий нет.
 */
class Debit : public Account {
public:
    Debit(double firstIncome, double percent, bool isWeird) {
        moneyToPay = 0;
        money = firstIncome;
        remainderPercent = percent;
        remainderDayKoeff = percent / 365 / 100;
        creationDate = Date();
        lastCalcOfPercent = creationDate;
        this->isWeird = isWeird;
    }

    void getMoney(double money) override {
        if (this->money < money) {
            throw notEnoughMoneyOnAccount();
        }
        this->money -= money;
    }
};

class DebitBuilder : public IAccountBuilder {
public:
    IAccountBuilder * setIncomeMoney(double money) override {
        this->money = money;
        return this;
    }

    IAccountBuilder * setRemainderPercent(double percent) {
        this->remainderPercent = percent;
        return this;
    }

    Debit* Build() {
        Debit* debitAccount = new Debit(money, remainderPercent, isWeird);
        return debitAccount;
    }
};

/*
 * Кредитный счет – имеет кредитный лимит, в рамках которого
 * можно уходить в минус (в плюс тоже можно). Процента на остаток нет.
 * Есть фиксированная комиссия за использование, если клиент в минусе.
 */
class Credit : public Account {
private:
    double commissionPercent;

    void takeCommission() { // вызывается в конце каждого дня
        if (this->money < 0) {
            this->money -= this->money * commissionPercent / 100;
        }
    }

public:
    Credit(double firstIncome, double remainderPercent, double commissionPercent, bool isWeird) {
        moneyToPay = 0;
        money = firstIncome;
        this->remainderPercent = remainderPercent;
        this->remainderDayKoeff = remainderPercent / 365 / 100;
        this->commissionPercent = commissionPercent;
        creationDate = Date();
        lastCalcOfPercent = creationDate;
        this->isWeird = isWeird;
    }

    void getMoney(double money) override {
        this->money -= money;
    }

    void calcPercent(Date date) override {
        if (date.isLaterThenDate(lastCalcOfPercent)) {
            int yFromCreation = date.getYear() - creationDate.getYear();
            int alreadyAddedDays = abs(lastCalcOfPercent.getDay() - creationDate.getDay());
            int alreadyAddedMonths = abs(lastCalcOfPercent.getMonth() - creationDate.getMonth());
            alreadyAddedDays += (alreadyAddedMonths) * 30;

            for (int j = 0; j < yFromCreation; ++j) {
                for (int i = 0; i < 365 - alreadyAddedDays; ++i) {
                    takeCommission();
                    addTodaysRemainder();
                }
                addRemainder();
            }

            int years = date.getYear() - creationDate.getYear();
            int months = abs(date.getMonth() - lastCalcOfPercent.getMonth());
            int days = abs(date.getDay() - lastCalcOfPercent.getDay());
            days += months * 30;

            for (int j = 0; j < years; ++j) {
                for (int i = 0; i < 365; ++i) {
                    takeCommission();
                    addTodaysRemainder();
                }
                addRemainder();
            }

            for (int i = 0; i < days; ++i) {
                takeCommission();
                addTodaysRemainder();
            }
        }
        lastCalcOfPercent = date;
    }
};

class CreditBuilder : public IAccountBuilder {
private:
    double commissionPercent = 0;
public:
    IAccountBuilder * setIncomeMoney(double money) override {
        this->money = money;
        return this;
    }

    IAccountBuilder * setCommissionPercent(double percent) {
        this->commissionPercent = percent;
        return this;
    }

    Credit *Build() {
        Credit *creditAccount = new Credit(money, remainderPercent, commissionPercent, isWeird);
        return creditAccount;
    }
};