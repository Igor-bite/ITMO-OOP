#include <ctime>

#pragma once

class Date {
public:
    Date() {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        _year = 1900 + ltm->tm_year;
        _month = 1 + ltm->tm_mon;
        _day = ltm->tm_mday;
    }

    Date(int years) {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        _year = 1900 + ltm->tm_year + years;
        _month = 1 + ltm->tm_mon;
        _day = ltm->tm_mday;
    }

    Date(int day, int month, int year) {
        _year = year;
        _month = month;
        _day = day;
    }

    int getDay() const {
        return _day;
    }

    int getMonth() const {
        return _month;
    }

    int getYear() const {
        return _year;
    }

    bool isLaterThenNow() {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        int year = 1900 + ltm->tm_year;
        int month = 1 + ltm->tm_mon;
        int day = ltm->tm_mday;

        if (year == _year) {
            if (month == _month) {
                if (day == _day) {
                    return false;
                } else if (day < _day) {
                    return true;
                } else {
                    return false;
                }
            } else if (month < _month) {
                return true;
            } else {
                return false;
            }
        } else if (year < _year) {
            return true;
        } else {
            return false;
        }
    }

    bool isLaterThenDate(Date date) {
        int year = date.getYear();
        int month = date.getMonth();
        int day = date.getDay();

        if (year == _year) {
            if (month == _month) {
                if (day == _day) {
                    return false;
                } else if (day < _day) {
                    return true;
                } else {
                    return false;
                }
            } else if (month < _month) {
                return true;
            } else {
                return false;
            }
        } else if (year < _year) {
            return true;
        } else {
            return false;
        }
    }

private:
    int _day;
    int _month;
    int _year;
};