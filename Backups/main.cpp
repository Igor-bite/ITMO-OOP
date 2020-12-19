#include <iostream>
#include <unistd.h>
#include "Restore.cpp"
using namespace std;

int main() {

    Directory fs = Directory("/");

    Directory diskC = Directory("Local Drive C");
    fs.Add(&diskC);

    File txt = File("data.txt", 1);
    diskC.Add(&txt);

    File png = File("image.png", 4);
    diskC.Add(&png);

    File ini = File("data.ini", 8);
    diskC.Add(&ini);

    Directory backupDir("Backup");
    fs.Add(&backupDir);

    FullBackupChain fbChain = FullBackupChain(backupDir);
    FullBackup *fb = fbChain.makeNewBackup();

    fb->addToBackup(&png);
    fb->addToBackup(&txt);
    fb->CreateBackup();

//    fb->printBackupedFiles();

//    try {
//        fb->addToBackup(&diskC);
//    } catch (AlreadyBackuped ex) {
//        cout << ex.what() << endl;
//    }


    sleep(3); // ждать 3 секунды

    FullBackup *newfb = fbChain.makeNewBackup();
    newfb->addToBackup(&diskC);
    newfb->CreateBackup();

    fbChain.printAll();
    cout << "--------------------------------------------" << endl;
//    fbChain.filterByQuantity(1);
//    fbChain.filterBySize(10);
//    fbChain.filterByDate(2);
    fbChain.printAll();
    cout << "--------------------------------------------" << endl;



    IncrementalBackupChain ibChain = IncrementalBackupChain(backupDir, fb);
    ibChain.printAll();
    cout << "--------------------------------------------" << endl;

    IncrementalBackup *ib = ibChain.makeNewBackup();

    ib->addToBackup(&png);
    ib->addToBackup(&txt);
    ib->CreateBackup();

//    try {
//        ib->addToBackup(&diskC);
//    } catch (AlreadyBackuped ex) {
//        cout << ex.what() << endl;
//    }

    File png1 = File("image.png", 100);

    IncrementalBackup *newib = ibChain.makeNewBackup();
    try {
        newib->addToBackup(&png1);
    } catch (AlreadyBackuped ex) {
        cout << ex.what() << endl;
    }
    newib->CreateBackup();

    ibChain.printAll();

    return 0;
}
