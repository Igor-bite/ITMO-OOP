#pragma once
#include <exception>
#include "Date.cpp"
#include "Client.cpp"
using namespace std;

struct notEnoughToBuildClient : public std::exception {
    const char * what () const throw ()
    {
        return "There aren't enough data to construct client - name and surname are obligatory.";
    }
};

struct notConfigured : public std::exception {
    const char * what () const throw ()
    {
        return "There aren't enough data.";
    }
};

struct notAuthenticated : public std::exception {
private:
    string clientInfo;
public:
    notAuthenticated(string clientInfo) {
        this->clientInfo = "There aren't enough data about the client " + clientInfo + ", owner of this account, add more information.";
    }

    const char * what () const throw ()
    {
        return clientInfo.c_str();
    }
};

struct notEnoughMoneyOnAccount : public std::exception {
    const char * what () const throw ()
    {
        return "There isn't enough money on this account.";
    }
};

struct notYetAvailable : public std::exception {
private:
    string errorMessage;
public:
    notYetAvailable(Date date) {
        string dateStr = to_string(date.getDay());
        dateStr += "/";
        dateStr += to_string(date.getMonth());
        dateStr += "/";
        dateStr += to_string(date.getYear());

        errorMessage = "You can't commit this action yet. Wait for " + dateStr + "please.";
    }

    const char * what () const throw ()
    {
        return errorMessage.c_str();
    }
};