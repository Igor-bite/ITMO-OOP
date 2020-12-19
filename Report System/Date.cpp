#pragma once
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

class Time {
public:
    Time(int hour, int min, int sec) : hour(hour), min(min), sec(sec) {}

    Time() = default;

    string getStr() {
        string h = to_string(hour);
        string m = to_string(min);
        string s = to_string(sec);
        return (h + ":" + m + ":" + s);
    }

    int getHour() const {
        return hour;
    }

    int getMin() const {
        return min;
    }

    int getSec() const {
        return sec;
    }

private:
    int hour;
    int min;
    int sec;
};

class Date {
public:
    Date() {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        daysFrom70 = now / 3600 / 24 ;
        _year = 1900 + ltm->tm_year;
        _month = 1 + ltm->tm_mon;
        _day = ltm->tm_mday;
        int hour = ltm->tm_hour;
        int min = ltm->tm_min;
        int sec = ltm->tm_sec;
        _time = Time(hour, min, sec);
    }

    Date(int day, int month, int year, int hour, int min, int sec) {
        _year = year;
        _month = month;
        _day = day;
        _time = Time(hour, min, sec);
    }

    Time getTime() const {
        return _time;
    }

    int getDay() {
        return _day;
    }

    int getMonth() {
        return _month;
    }

    int getYear() {
        return _year;
    }

    int getDaysFrom70() {
        return daysFrom70;
    }
private:
    int daysFrom70;
    int _day;
    int _month;
    int _year;
    Time _time;
};

bool operator==(Date t1, Date t2) {
    if (t1.getYear() == t2.getYear() && t1.getMonth() == t2.getMonth() && t1.getDay() == t2.getDay()) {
        return true;
    } else {
        return false;
    }
}