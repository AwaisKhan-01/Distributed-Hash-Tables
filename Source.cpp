

#include <iostream>
#include <string>
#include <fstream>
#include<sstream>
#include <bitset>
#include "Hash.h"
#include "DHT_Ring.h"
#include "BigInt.h"
#include "BigIntToDec.h"

using namespace std;

int main() {
	int bitsOfIS, machines;
	int action;
	bool exit = 0;
	string id, Mname, Mpath;
	string name, type, path, filePath;
	string fileHash;
	BigInt dec("");
	cout << "welcome to the IPFS" << endl;
	// specifying identifier sapce from user
	cout << "Enter the number of Bits of the identifier space" << endl;
	cin >> bitsOfIS;
	// creation of identifier space here
	DHT_Ring dht;
	dht.setBitspace(bitsOfIS);

	// specifying the number of machines the user wants to input
	cout << "Enter the number of machines you want to add" << endl;
	cin >> machines;
	int ch = 0;
	int BTreeOrder = 0;
	int cha = 0;
	bool additioncheck = false;

	cout << "1. manually assign each id \n2. autoassign ids" << endl;
	cin >> ch;
	switch (ch) {
	case 1:
		cout << "Enter the order of BTree" << endl;
		cin >> BTreeOrder;
		for (int i = 0; i < machines; i++) {
			do {

				cout << "\n\n\nEnter the ID of machine" << endl;
				cin >> id;
				cout << "\nEnter the name of machine" << endl;
				cin >> Mname;
				cout << "\nEnter the path of machine" << endl;
				cin.ignore();
				getline(cin, Mpath);
				


				BigInt ids(id);
				additioncheck = dht.add_machine(ids, Mname, Mpath, BTreeOrder);

			} while (additioncheck == false);
			dht.update_RTable();

		}
		break;
	case 2:
		cout << "Enter the order of BTree" << endl;
		cin >> BTreeOrder;
		for (int i = 0; i < machines; i++) {
			do {
				cout << "\n\n\nEnter the name of machine" << endl;
				cin >> Mname;
				cout << "\nEnter the path of machine" << endl;
				cin.ignore();
				getline(cin, Mpath);
				string stringID = calculateStringHash(Mname);
				cout << "\nmachine name hash is " << stringID << endl;

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

	// add machine into the identifier space


	BigInt hs("");
	Conversion convert2;
	string decimalNumber1;
	BigInt FileHashdeci(2);
	BigInt hashingWith = BigInt(2);
	string nametxt;
	while (!exit)
	{
		cout << "\nwhat do you want to do" << endl;
		cout << "\n 1. Print the DHT \n 2. Add a file to the DHT \n 3. Search a file \n 4. Delete Machine\n 5. Add Machine \n 6. Print Routing Table of all machines \n 7. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> action;
		switch (action)
		{
		case 1:
			cout << "DHT is:\n";
			dht.print();
			break;

		case 2:
			cout << "\nenter name of file: " << endl;
			
			cin >> name;
			cout << "\nenter type of file: " << endl;
			cin >> type;
			cout << "\nEnter the path of the file: ";
			cin >> path;
			nametxt = name + "." + type;
			filePath = path + "\\" + name + ".";
			filePath += type;
			
			cout << "file path " << filePath << endl;
			if (type == "txt" || type == "TXT")
				fileHash = calculateTextHash(filePath);

			else if (type == "mp3" || type == "MP3" || type == "wav" || type == "WAV")
				fileHash = calculateAudioHash(filePath);

			else {
				cout << "Invalid File Path" << endl;
			}

			///  only for testing fileHash is hardcoded
			convert2;
			convert2.fromHex(fileHash);
			decimalNumber1 = convert2.toString();
			cout << "HASHHH: " << decimalNumber1;
			FileHashdeci = decimalNumber1;
			hashingWith = pow(hashingWith, BigInt(to_string(bitsOfIS)));
			cout << "hashingWith: " << hashingWith << endl;
			cout << "file hash: " << fileHash << endl;
			cout << "BITSPACE:" << bitsOfIS;
			FileHashdeci = FileHashdeci % hashingWith;
			cout << "newHash IS " << FileHashdeci << endl;
			dht.search(FileHashdeci, filePath,nametxt);
			break;

		case 3:
			cout << "Enter the hash of file you want to search: " << endl;
			cin >> fileHash;
			hs = fileHash;
			dht.search(hs, "","");
			break;

		case 4:
			cout << "Enter the hash of machine you want to delete: " << endl;
			cin >> fileHash;
			hs = fileHash;
			dht.delete_machine(hs);
			dht.update_RTable();
			break;

		case 5:
			cout << "1. manually assign each id \n2. autoassign ids" << endl;
			cin >> cha;

			if (cha == 1)
			{
				cout << "\n\n\nEnter the ID of machine" << endl;
				cin >> id;
				cout << "\nEnter the name of machine" << endl;
				cin >> Mname;
				cout << "\nEnter the path of machine" << endl;
				cin.ignore();
				getline(cin, Mpath);


				BigInt ids(id);
				dht.add_machine(ids, Mname, Mpath, BTreeOrder);
				dht.update_RTable();
			}
			else if (cha == 2)
			{

				cout << "Enter the name of machine" << endl;
				cin >> Mname;
				cout << "Enter the path of machine" << endl;
				cin.ignore();
				getline(cin, Mpath);
				string stringID1 = calculateStringHash(Mname);
				Conversion con;
				con.fromHex(stringID1);
				string decimalNumber11 = con.toString();
				BigInt stringids1(decimalNumber1);
	
				dht.add_machine(stringids1, Mname, Mpath, BTreeOrder);
				
				dht.update_RTable();

			}
			break;

		case 6:
			cout << "Routing table of all machines: " << endl;
			dht.print_RTable();
			break;
		case 7:
			exit = 1;
			break;
		default:
			cout << "wrong input!!!" << endl;
			cout << "Please try again" << endl;
			cout << "To exit please press 'x'" << endl;
		}
	}
	cout << "Thank you for using IPFS" << endl;

}
//int main()
//{
//	string name, type, path,filePath;
//	cout << "\nenter name of file: " << endl;
//	
//	cin >> name;
//	cout << "\nenter type of file: " << endl;
//	cin >> type;
//	cout << "\nEnter the path of the file: ";
//	cin >> path;
//
//	filePath = path + "\\" + name + ".";
//	filePath += type;
//	cout << "file path " << filePath << endl;
////	DHT_Ring link;
////	link.setBitspace(5);
////	BigInt b1("1");
////	BigInt b4("4");
////	BigInt b9("9");
////	BigInt b11("11");
////	BigInt b14("14");
////	BigInt b18("18");
////	BigInt b20("20");
////	BigInt b21("21");
////	BigInt b28("28");
////	string m1 = "machine1";
////	string m2 = "machine2";
////	string m3 = "machine3";
////	string stringID1 = calculateStringHash(m1);
////	string stringID2 = calculateStringHash(m2);
////	string stringID3 = calculateStringHash(m3);
////	cout << "machine name hash is " << stringID1 << endl;
////	cout << "machine name hash is " << stringID2 << endl;
////	cout << "machine name hash is " << stringID3 << endl;
////
////	Conversion con;
////	con.fromHex(stringID1);
////	string decimalNumber1 = con.toString();
////
////	Conversion con2;
////	con2.fromHex(stringID2);
////	string decimalNumber2 = con2.toString();
////
////	Conversion con3;
////	con3.fromHex(stringID3);
////	string decimalNumber3 = con3.toString();
////
//////	cout << "stringID1 : " << decimalNumber1 << endl;
//////	cout << "stringID2 : " << decimalNumber2 << endl;
//////	cout << "stringID3 : " << decimalNumber3 << endl;
////	
////	BigInt c1("8");
////	BigInt c2("9");
////	BigInt c3("9");
////
////	link.add_machine(c1, "machine1", "filename", 3);
////	link.print();
////	cout << endl;
////
////	link.add_machine(c2, "machine2", "filename", 3);
////	link.print();
////	cout << endl;
////
////	link.add_machine(c3, "machine3", "filename", 3);
////	link.print();
////	cout << endl;
////
////	link.add_machine(b11, "machine11", "filename", 3);
////	//link.add_machine(b14, "machine14", "filename", 3);
////	//link.add_machine(b18, "machine18", "filename", 3);
////	//link.add_machine(b20, "machine20", "filename", 3);
////	//link.add_machine(b21, "machine21", "filename", 3);
////	//link.add_machine(b28, "machine28", "filename", 3);
////
////	link.print();
////	cout << endl;
//
//	///*link.delete_machine(b5);
//	//link.add_machine(b7, "machine7", "filename", 3);*/
//
//	///*link.print();
//	//cout << endl;*/
//	//link.update_RTable();
//	//link.print_RTable();
//	//cout << "\n\n\n\n";
//	//link.print();
//	//cout << endl;
//	//link.print_RTable_ofMachineWithHash(3);
//
//	//BigInt b9(9);
//	//BigInt b10(10);
//	//BigInt b11(11);
//	//BigInt b12(12);
//	//BigInt b13(13);
//	////BigInt b14(14);
//	//cout << "\n\n\n";
//	//link.search(b9,"/path/to/machine9");
//	//cout << "\n\n\n";
//	//link.search(b10,"/path/to/machine10");
//	//cout << "\n\n\n";
//	//link.search(b11, "/path/to/machine11");
//	//cout << "\n\n\n";
//	//link.search(b12, "/path/to/machine12");
//	//cout << "\n\n\n";
//	//link.search(b13, "/path/to/machine13");
//	//cout << "\n\n\n";
//	//link.search(b14, "/path/to/machine13");
//	//cout << "\n\n\n";
//}


