#pragma once
#include <exception>
using namespace std;

struct NoFileException : public exception {
    const char * what () const throw ()
    {
        return "There isn't this file or directory";
    }
};

struct NoPersonException : public exception {
    const char * what () const throw ()
    {
        return "There isn't this person";
    }
};

struct TaskStateException : public exception {
    const char * what () const throw ()
    {
        return "There is some kind of an error with task state";
    }
};

struct NoReportException : public exception {
    const char * what () const throw ()
    {
        return "There is no this report yet";
    }
};