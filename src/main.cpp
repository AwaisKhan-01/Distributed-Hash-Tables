#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include "Hash.h"
#include "DHT_Ring.h"
#include "BigInt.h"
#include "BigIntToDec.h"

using namespace std;

int main() {
    int bitsOfIS, machines;
    int action;
    bool exitFlag = false;
    string id, Mname, Mpath;
    string name, type, path, filePath;
    string fileHash;
    BigInt dec("");
    cout << "Welcome to DHT-IPFS Simulator" << endl;

    cout << "Enter the number of bits for the identifier space: " << endl;
    cin >> bitsOfIS;

    DHT_Ring dht;
    dht.setBitspace(bitsOfIS);

    cout << "Enter the number of machines you want to add: " << endl;
    cin >> machines;
    int ch = 0;
    int BTreeOrder = 0;
    int cha = 0;
    bool additioncheck = false;

    cout << "1. Manually assign each ID\n2. Auto-assign IDs (hash from machine name)" << endl;
    cin >> ch;
    switch (ch) {
    case 1:
        cout << "Enter the order of BTree: " << endl;
        cin >> BTreeOrder;
        for (int i = 0; i < machines; i++) {
            do {
                cout << "\n\nEnter the ID of machine: " << endl;
                cin >> id;
                cout << "\nEnter the name of machine: " << endl;
                cin >> Mname;
                cout << "\nEnter the path of machine: " << endl;
                cin.ignore();
                getline(cin, Mpath);

                BigInt ids(id);
                additioncheck = dht.add_machine(ids, Mname, Mpath, BTreeOrder);

            } while (additioncheck == false);
            dht.update_RTable();
        }
        break;
    case 2:
        cout << "Enter the order of BTree: " << endl;
        cin >> BTreeOrder;
        for (int i = 0; i < machines; i++) {
            do {
                cout << "\n\nEnter the name of machine: " << endl;
                cin >> Mname;
                cout << "\nEnter the path of machine: " << endl;
                cin.ignore();
                getline(cin, Mpath);
                string stringID = calculateStringHash(Mname);
                cout << "\nMachine name hash is " << stringID << endl;

                string stringID1 = calculateStringHash(Mname);
                Conversion con;
                con.fromHex(stringID1);
                string decimalNumber1 = con.toString();
                BigInt stringids(decimalNumber1);

                additioncheck = dht.add_machine(stringids, Mname, Mpath, BTreeOrder);
            } while (additioncheck == false);
            dht.update_RTable();
        }
        dht.update_RTable();
        break;
    default:
        std::cout << "Invalid choice\n";
        break;
    }

    BigInt hs("");
    Conversion convert2;
    string decimalNumber1;
    BigInt FileHashdeci(2);
    BigInt hashingWith = BigInt(2);
    string nametxt;
    while (!exitFlag)
    {
        cout << "\nWhat would you like to do?" << endl;
        cout << " 1. Print the DHT\n"
             << " 2. Add a file to the DHT\n"
             << " 3. Search a file\n"
             << " 4. Delete Machine\n"
             << " 5. Add Machine\n"
             << " 6. Print Routing Table of all machines\n"
             << " 7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> action;
        switch (action)
        {
        case 1:
            cout << "DHT is:\n";
            dht.print();
            break;

        case 2:
            cout << "\nEnter name of file: " << endl;
            cin >> name;
            cout << "\nEnter type of file (txt/mp3): " << endl;
            cin >> type;
            cout << "\nEnter the path of the file: ";
            cin >> path;
            nametxt = name + "." + type;
            filePath = path + "/" + name + "." + type;

            cout << "File path: " << filePath << endl;
            if (type == "txt" || type == "TXT")
                fileHash = calculateTextHash(filePath);
            else if (type == "mp3" || type == "MP3" || type == "wav" || type == "WAV")
                fileHash = calculateAudioHash(filePath);
            else {
                cout << "Unsupported file type" << endl;
                break;
            }

            convert2.fromHex(fileHash);
            decimalNumber1 = convert2.toString();
            cout << "File hash (decimal): " << decimalNumber1;
            FileHashdeci = decimalNumber1;
            hashingWith = pow(hashingWith, BigInt(to_string(bitsOfIS)));
            cout << "hashingWith: " << hashingWith << endl;
            cout << "File hash (hex): " << fileHash << endl;
            cout << "Bitspace: " << bitsOfIS;
            FileHashdeci = FileHashdeci % hashingWith;
            cout << "Mapped hash: " << FileHashdeci << endl;
            dht.search(FileHashdeci, filePath, nametxt);
            break;

        case 3:
            cout << "Enter the hash of file you want to search: " << endl;
            cin >> fileHash;
            hs = fileHash;
            dht.search(hs, "", "");
            break;

        case 4:
            cout << "Enter the hash of machine you want to delete: " << endl;
            cin >> fileHash;
            hs = fileHash;
            dht.delete_machine(hs);
            dht.update_RTable();
            break;

        case 5:
            cout << "1. Manually assign ID\n2. Auto-assign ID" << endl;
            cin >> cha;

            if (cha == 1)
            {
                cout << "\n\nEnter the ID of machine: " << endl;
                cin >> id;
                cout << "\nEnter the name of machine: " << endl;
                cin >> Mname;
                cout << "\nEnter the path of machine: " << endl;
                cin.ignore();
                getline(cin, Mpath);

                BigInt ids(id);
                dht.add_machine(ids, Mname, Mpath, BTreeOrder);
                dht.update_RTable();
            }
            else if (cha == 2)
            {
                cout << "Enter the name of machine: " << endl;
                cin >> Mname;
                cout << "Enter the path of machine: " << endl;
                cin.ignore();
                getline(cin, Mpath);
                string stringID1 = calculateStringHash(Mname);
                Conversion con;
                con.fromHex(stringID1);
                string decimalNumber11 = con.toString();
                BigInt stringids1(decimalNumber11);

                dht.add_machine(stringids1, Mname, Mpath, BTreeOrder);
                dht.update_RTable();
            }
            break;

        case 6:
            cout << "Routing table of all machines: " << endl;
            dht.print_RTable();
            break;

        case 7:
            exitFlag = true;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    cout << "Thank you for using DHT-IPFS Simulator!" << endl;
    return 0;
}
