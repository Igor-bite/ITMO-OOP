//
// Created by Игорь Клюжев on 14.11.2020.
//
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include "Nodes.cpp"

class AlreadyBackuped : public exception {
public:
    const char * what() const throw()
    {
        return "This backup has already been done";
    }
};

class Time {
public:
    Time(int hour, int min, int sec) : hour(hour), min(min), sec(sec) {
        time_t now = time(0);
        secFrom70 = now;
    }

    Time() = default;

    string getStr() {
        string h = to_string(hour);
        string m = to_string(min);
        string s = to_string(sec);
        return (h + ":" + m + ":" + s);
    }

    int getSecFrom70() {
        return secFrom70;
    }

private:
    int hour;
    int min;
    int sec;
    long secFrom70;
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

    Time getTime() {
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

class IBackup {
protected:
    vector<INode*> backupNodes;
    Date date;
    vector<INode*> nodesToBackup;
    bool isBackuped = false;
    virtual void CreateBackup(INode* node) {};

public:
    IBackup() = default;

    virtual void CreateBackup() {};

    virtual void addToBackup(INode *node) {};

    Date getDate() {
        return date;
    }

    vector<INode*> getBackupNodes() {
        return backupNodes;
    }

    int getOverallSize() {
        int result = 0;
        for (INode* node : backupNodes) {
            int size = node->getSize();
            result += size;
        }

        return result;
    }

    void printBackupedFiles() {
        for (int i = 0; i < backupNodes.size(); ++i) {
            cout << backupNodes[i]->getName() << " " << getDate().getTime().getStr() << endl;
        }
    }
};

class FullBackup : public IBackup {
private:
    void CreateBackup(INode* node) override {
//        node.copyTo(backupDir);
        backupNodes.push_back(node);
    }

public:
    FullBackup() = default;

    void CreateBackup() override {
        if (isBackuped) {
            throw AlreadyBackuped();
        }
        isBackuped = true;
        date = Date();
        for (INode* node : nodesToBackup) {
            CreateBackup(node);
        }
    }

    void addToBackup(INode *node) override {
        if (isBackuped) {
            throw AlreadyBackuped();
        }
        for (int i = 0; i < nodesToBackup.size(); ++i) {
            if (nodesToBackup[i]->getName() == node->getName()) {
                throw AlreadyExists();
            }
        }
        nodesToBackup.push_back(node);
    }

    void printBackupedFiles() {
        if (isBackuped) {
            for (int i = 0; i < backupNodes.size(); ++i) {
                cout << backupNodes[i]->getName() << " " << getDate().getTime().getStr() << endl;
            }
        }
    }
};


class Chain {
protected:
    vector<IBackup*> backups;
    Directory backupDir;
public:
    Chain(Directory backupDir) : backupDir(backupDir) {
        backups = vector<IBackup*>();
    }

    void deleteLast() {
        IBackup* last = backups[0];
        delete last;
        backups.erase(backups.begin());
    }

    void deleteLast(int q) {
        for (int i = 0; i < q; ++i) {
            deleteLast();
        }
    }

    void filterByQuantity(int maxQuantity) {
        if (maxQuantity < backups.size()) {
            deleteLast(backups.size() - maxQuantity);
        }
    }

    void filterByDate(int maxDays) {
        Date now = Date();
        Date lastBackupDate = backups[0]->getDate();
        while(now.getDaysFrom70() - lastBackupDate.getDaysFrom70() > maxDays) {
            deleteLast();
            lastBackupDate = backups[0]->getDate();
        }

        // just for testing (seconds instead of days)
//        while(now.getTime().getSecFrom70() - lastBackupDate.getTime().getSecFrom70() > maxDays) {
//            deleteLast();
//            lastBackupDate = backups[0]->getDate();
//        }
        // just for testing
    }

    void filterBySize(int maxSize) {
        for (int i = 0; i < backups.size(); ++i) {
            int size = backups[i]->getOverallSize();
            if (size > maxSize) {
                delete backups[i];
                backups.erase(backups.begin() + i);
            }
        }
    }

    void printAll() {
        for (int i = 0; i < backups.size(); ++i) {
            if (backups[i]->getOverallSize() != 0) {
                cout << "-------------------" << endl;
                backups[i]->printBackupedFiles();
            }
        }
    }

    vector<IBackup*> getBackups() {
        return backups;
    }
};

class IncrementalBackup : public IBackup {
private:
    void CreateBackup(INode *node) override {
    //        node.copyTo(backupDir);
        backupNodes.push_back(node);
    }

    Chain *chain;
public:
    IncrementalBackup(Chain *_chain) {
        chain = _chain;
    }

    void CreateBackup() override {
        if (isBackuped) {
            throw AlreadyBackuped();
        }
        isBackuped = true;
        date = Date();

        for (INode* nodePtr : nodesToBackup) {
            CreateBackup(nodePtr);
        }
    }

    void addToBackup(INode *node) override {
        if (isBackuped) {
            throw AlreadyBackuped();
        }
        for (int i = 0; i < nodesToBackup.size(); ++i) {
            if (nodesToBackup[i]->getName() == node->getName()) {
                throw AlreadyExists();
            }
        }

        vector<IBackup*> backups = chain->getBackups();
        reverse(backups.begin(), backups.end());
        for (IBackup *backup : backups) {
            FullBackup *fb = dynamic_cast<FullBackup*>(backup);
            if (fb) {
                for (INode* _node : backup->getBackupNodes()) {
                    if (_node->getName() == node->getName()) {
                        if (_node->getSize() != node->getSize()) {
                            nodesToBackup.push_back(node);
                        } else {
                            return;
                        }
                    }
                }
                return;
            }
        }
    }
};

class FullBackupChain : public Chain {
public:
    FullBackupChain(Directory backupDir) : Chain(backupDir) {}

    FullBackup* makeNewBackup() {
        FullBackup* newBackup = new FullBackup();
        backups.push_back(newBackup);
        return newBackup;
    }
};

class IncrementalBackupChain : public Chain {
public:
    IncrementalBackupChain(Directory backupDir, IBackup *fb) : Chain(backupDir) {
        backups.push_back(fb);
    }

    void checkForUselessIncrementals() {
        vector<IBackup*> backups = getBackups();
        IncrementalBackup *fb = dynamic_cast<IncrementalBackup*>(backups[backups.size()]);
        while (fb) {
            Chain::deleteLast();
            vector<IBackup*> backups = getBackups();
            fb = dynamic_cast<IncrementalBackup*>(backups[backups.size()]);
        }
    }

    IncrementalBackup* makeNewBackup() {
        IncrementalBackup* newBackup = new IncrementalBackup(this);
        backups.push_back(newBackup);
        return newBackup;
    }

    FullBackup* makeNewFullBackup() {
        FullBackup* newBackup = new FullBackup();
        backups.push_back(newBackup);
        return newBackup;
    }

    void addNewFullBackup(FullBackup *fullBackup) {
        backups.push_back(fullBackup);
    }

    void filterByQuantity(int maxQuantity) {
        Chain::filterByQuantity(maxQuantity);
        checkForUselessIncrementals();
    }

    void filterByDate(int maxDays) {
        Chain::filterByDate(maxDays);
        checkForUselessIncrementals();
    }

    void filterBySize(int maxSize) {
        Chain::filterBySize(maxSize);
        checkForUselessIncrementals();
    }
};
