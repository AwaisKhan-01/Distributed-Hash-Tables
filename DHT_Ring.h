#pragma once 

#ifndef DHT_Ring_H
#define DHT_Ring_H
#include <windows.h>
#include <string>
#include "Btree.h"
#include<iostream> 
#include<cmath> 
#include"Btree.h"
#include "BigInt.h"

using namespace std;
bool createDirectory(std::string dirname);
bool deleteDirectory(std::string dirname);

void copyFileContents(const std::string& sourcePath, const std::string& destinationPath) {
    // Open the source file for reading
    std::ifstream sourceFile(sourcePath, std::ios::binary);
    if (!sourceFile.is_open()) {
        std::cerr << "Error: Unable to open source file '" << sourcePath << "'.\n";
        return;
    }

    // Open the destination file for writing
    std::ofstream destinationFile(destinationPath, std::ios::binary);
    if (!destinationFile.is_open()) {
        std::cerr << "Error: Unable to open destination file '" << destinationPath << "'.\n";
        sourceFile.close(); // Close the source file
        return;
    }

    // Copy the contents from source to destination
    destinationFile << sourceFile.rdbuf();

    // Close the files
    sourceFile.close();
    destinationFile.close();


}
void copyAudioFileContents(const std::string& sourcePath, const std::string& destinationPath) {
	// Open the source file for reading
	std::ifstream sourceFile(sourcePath, std::ios::binary);
	if (!sourceFile.is_open()) {
		std::cerr << "Error: Unable to open source file '" << sourcePath << "'.\n";
		return;
	}

	// Open the destination file for writing
	std::ofstream destinationFile(destinationPath, std::ios::binary, ofstream::out);
	if (!destinationFile.is_open()) {
		std::cerr << "Error: Unable to open destination file '" << destinationPath << "'.\n";
		sourceFile.close(); // Close the source file
		return;
	}

	// Copy the contents from source to destination
	destinationFile << sourceFile.rdbuf();
	// Close the files
	sourceFile.close();
	destinationFile.close();


}

void removeFile(string removePath) {
	const char* filename = removePath.c_str();

	// remove the file
	int result = remove(filename);
}


class machine;
class RTable
{
public:
	machine* data;
	RTable* prev;
	RTable* next;
	RTable() { data = nullptr; prev = nullptr; next = nullptr; }
	RTable(machine* d, RTable* n, RTable* p) { data = d; prev = p; next = n; }
	machine* getData() { return data; }
	RTable* getPrev() { return prev; }
	RTable* getNext() { return next; }
	void setData(machine* i) { data = i; }
	void setNext(RTable* n) { next = n; }
	void setPrev(RTable* p) { prev = p; }


};
class  machine
{
public:
	BigInt machine_hash;
	machine* Mnext;
	BTree* Broot;
	string folderpath;
	string machine_name;
	RTable* table;

	//	machine():machine_hash(0){ folderpath = ""; Mnext = nullptr; 		Broot = new BTree(3);
	//}
	machine(BigInt mh, int BTreeOrder = 0, int bit_sp = 0, string mname = "", string folder = " ", machine* n = nullptr) :machine_hash(mh)
	{
		machine_name = mname;
		folderpath = folder;
		Mnext = n;
		table = nullptr;
		Broot = new BTree(3);
		for (int i = 0; i < bit_sp; i++)
		{
			if (!table)
			{
				RTable* newnode = new RTable;
				table = newnode;
			}
			else
			{
				RTable* newnode = new RTable;
				newnode->next = table;
				table->prev = newnode;
				table = newnode;
			}
		}


	}
	BigInt getMachine_hash() { return machine_hash; }
	machine* getNext() { return Mnext; }
	RTable* getRTable_head() { return table; }
	void insertAtTableIndex(machine* mp, int i)
	{
		int count = 0;
		RTable* curr = table;
		while (count != i)
		{
			count++;
			curr = curr->next;
		}
		curr->data = mp;
		//	 cout << "the machine returned is " << curr->getData()->getMachine_hash() << endl; 
	}
	void printTable()
	{
		int c = 0;
		RTable* curr = table;
		while (curr)
		{
			cout << "index " << c + 1 << " is machine: " << curr->getData()->getMachine_hash() << endl;
			curr = curr->next;
			c++;
		}

		cout << endl;
	}


};

class DHT_Ring {

	int  totalMachines;
	int  Existing_machines;
	int  bits_space;// identifier space 
	machine* head;
	machine* last;
public:
	DHT_Ring() { head = nullptr; Existing_machines = 0; bits_space = 4; }
	DHT_Ring(machine* h) { head = h; head->Mnext = head; Existing_machines = 0; }
	machine* getHead() { return head; }
	bool isEmpty() { return head == nullptr; }
	void setBitspace(int bi) { bits_space = bi; }

	void inorderTraversalAndMoveFiles(BTree::btnode* root, machine* newMachine) {
		if (root != nullptr) {
			for (int i = 0; i <= root->count; i++) {
				// Recursively traverse the left subtree.
				inorderTraversalAndMoveFiles(root->child[i], newMachine);

				if (i > 0) {
					// Move files with FileHash less than or equal to the new machine's hash.
					if (root->fileData[i].FileHash <= newMachine->getMachine_hash()) {
						// Add the file to the new machine.
						newMachine->Broot->root = newMachine->Broot->insert(
							root->fileData[i].FileHash, root->fileData[i].path, newMachine->Broot->root);

						// Delete the file from the next machine.
						root->fileData[i].FileHash = 0; // Or some invalid value to mark it for deletion.
						newMachine->Mnext->Broot->del(root->fileData[i].FileHash, newMachine->Mnext->Broot->root);

					}
				}
			}
		}
	}
	void moveFilesToNewMachine(machine* newMachine) {
		if (newMachine == nullptr || newMachine->Mnext == nullptr) {
			// If the new machine or its next machine is null, or the next machine's B-tree is null, return.
			return;
		}

		machine* nextMachine = newMachine->Mnext;
		BTree* nextMachineBTree = nextMachine->Broot;

		// Traverse the B-tree of the next machine and move files to the new machine.
		inorderTraversalAndMoveFiles(nextMachineBTree->root, newMachine);
	}


	bool add_machine(BigInt& newHash, string Mname, string filename, int BTreeOrder)// mname is Machine name 
	{
		cout << "newHash: " << newHash << endl;
		BigInt hashingWith = BigInt(2);
		hashingWith = pow(hashingWith, BigInt(bits_space));

		//cout << "hashingWith: " << hashingWith << endl;
		//cout << "newHash: " << newHash << endl;
		newHash = newHash % hashingWith;
		cout << "newHash IS " << newHash << endl;
		bool check = false;
		bool addittionsucc = true;

		if (isEmpty())
		{
			machine* newnode = new machine(newHash, BTreeOrder, bits_space, Mname, filename, nullptr);
			head = newnode;
			newnode->Mnext = head;
			last = head;
			Existing_machines++;
			createDirectory(Mname);
		}
		else
		{

			BigInt currHash(1);
			machine* newnode = new machine(newHash, BTreeOrder, bits_space, Mname, filename, nullptr);
			machine* curr = head->Mnext;
			machine* prev = head;
			machine* la = last;
			currHash = curr->getMachine_hash();
			int count = 0;
			if (newHash == head->getMachine_hash())
			{
				check = true;
				addittionsucc = false;
				cout << "MACHINE ALREADY EXISTS CANT ADD ON TOP" << endl;

			}
			else
			{
				while (curr != head && newHash > currHash)
				{
					if (newHash == currHash)
					{
						check = true;

						break;
					}
					prev = curr;
					curr = curr->Mnext;
					currHash = curr->getMachine_hash();

					count++;
				}
				if (newHash == currHash)
				{
					addittionsucc = false;
					cout << "MACHINE ALREADY EXISTS CANT ADD ON TOP" << endl;

				}
				else if (check == true)
				{
					addittionsucc = false;
					cout << "MACHINE ALREADY EXISTS CANT ADD ON TOP" << endl;
				}
				else if ((head->getMachine_hash() > newHash) && count == 0)
				{

					addittionsucc = true;
					cout << "last " << last->getMachine_hash() << endl;
					last->Mnext = newnode;
					newnode->Mnext = head;
					head = newnode;

					moveFilesToNewMachine(newnode);
					createDirectory(Mname);
					Existing_machines++;
				}
				else if (curr == head && newHash > currHash)
				{
					addittionsucc = true;
					newnode->Mnext = curr;
					prev->Mnext = newnode;
					last = newnode;
					moveFilesToNewMachine(newnode);
					createDirectory(Mname);
					Existing_machines++;
				}
				else if (curr == head && newHash < currHash)
				{
					addittionsucc = true;
					newnode->Mnext = curr;
					prev->Mnext = newnode;
					last = prev;
					head = newnode;
					moveFilesToNewMachine(newnode);
					createDirectory(Mname);
					Existing_machines++;
				}
				else if (newHash < currHash)
				{
					addittionsucc = true;
					newnode->Mnext = curr;
					prev->Mnext = newnode;
					moveFilesToNewMachine(newnode);
					createDirectory(Mname);
					Existing_machines++;
				}
			}
			//cout << "Existing_machines " << Existing_machines << endl; 
		}
		return addittionsucc;
	}
	void search(const BigInt& hash, const string& filepath,const string nametxt)
	{
		if (isEmpty())
		{
			cout << "no machines exist" << endl;
		}
		else
		{
			machine* curr = head;
			int count = 0;
			RTable* prevTable;
			RTable* tempTable;
			int c = 0;
			bool check = false;
			while (count <= Existing_machines)
			{

				prevTable = curr->getRTable_head();
				tempTable = curr->getRTable_head();
				c = 0; // counter for routing table
				while (tempTable)
				{
					if (curr->getMachine_hash() == hash)
					{
						check = true;
						break;
					}
					else if (tempTable->getData()->getMachine_hash() > hash && c == 0)
					{
						cout << "searching btree from middle somewhere of dht as higher value was found" << endl;
						check = true;
						curr = tempTable->getData();
						break;
					}
					else if (tempTable->getData()->getMachine_hash() > hash && c != 0)
					{
						cout << "repeating search of routing table of next machine\n " << endl;

						check = false;
						curr = prevTable->getData();
						break;
					}
					else if ((tempTable->getData()->getMachine_hash() < hash) && (tempTable->getData()->getMachine_hash() < prevTable->getData()->getMachine_hash()))
					{
						cout << "repeating search of routing table of next machine\n " << endl;

						check = false;
						curr = prevTable->getData();
						break;
					}
					else if ((tempTable->getData()->getMachine_hash() < hash) && (c == 0) && (tempTable->getData() == head) && (count != 0))
					{
						cout << "searching btree from dht head as higher value couldn't be found " << endl;
						check = true;
						curr = tempTable->getData();
						break;
					}
					else if ((tempTable->getData()->getMachine_hash() == hash))
					{
						cout << "searching btree from exact machine searched " << endl;
						check = true;
						curr = tempTable->getData();
						break;
					}
					c++;
					prevTable = tempTable;
					tempTable = tempTable->next;
					check = false;
				}
				if (check == true)
				{
					break;
				}
				count++;
			}
			cout << "machine at which we are searching the existance of file with hash " << hash << " is " << curr->getMachine_hash() << endl;
			int choice;
			std::string dirname;
			bool chk = false;
			do {
				std::cout << "\n1. print routing table values\n2. print btress\n3. add file\n4. delete file\n5. Exit\n";
				std::cout << "Enter your choice: ";
				std::cin >> choice;

				if (choice == 1) {

					chk = false;
					cout << "\nRTable values of machine with hash " << curr->getMachine_hash() << endl << endl;
					curr->printTable();

				}
				else if (choice == 2)
				{
					chk = false;
					curr->Broot->printLevelOrder();
				}
				else if (choice == 3)
				{
					chk = false;
					curr->Broot->root = curr->Broot->insert(hash, filepath, curr->Broot->root);
					string tempfldpath = curr->folderpath+"\\"+ curr->machine_name+"\\"+ nametxt;
					string tempfilepath = filepath;
					cout << "filepath is " << tempfilepath << endl;
					cout << "tempfldpath " << tempfldpath << endl;

					copyFileContents( tempfilepath, tempfldpath);
				}
				else if (choice == 4)
				{
					chk = false;
					curr->Broot->root = curr->Broot->del(hash, curr->Broot->root);
					string tempfilepath = filepath;
					removeFile(tempfilepath);

				}
				else if (choice == 5)
				{
					chk = true;

				}
				else {
					std::cout << "Invalid choice\n";
					chk == false;
				}

			} while (chk == false);
		}
	}
	void delete_machine(BigInt& hash)
	{
		if (isEmpty())
		{
			return;
		}
		else
		{
			machine* curr = head;
			machine* pre = last;


			while (curr)
			{
				if (curr->getMachine_hash() == hash)
				{
					//	cout << pre->getMachine_hash() << endl;
					if (curr == head)
					{
						head = curr->Mnext;
						pre->Mnext = curr->Mnext;

					}
					else
					{

						pre->Mnext = curr->Mnext;

					}

					deleteDirectory(curr->machine_name);
					delete curr;
					Existing_machines--;
					cout << "Existing_machines " << Existing_machines << endl;
					return;
				}
				pre = curr;
				curr = curr->Mnext;
			}
		}
	}
	void print() {

		if (isEmpty())
		{
			return;
		}
		else
		{
			machine* curr = head;
			int count = 0;
			while (curr != head || count == 0)
			{
				count++;
				cout << curr->getMachine_hash() << " -> ";
				curr = curr->Mnext;
			}
		}
	}
	void update_RTable()
	{
		machine* MP = head; // machine pointer 
		BigInt val(0);
		BigInt res;
		for (int j = 0; j < Existing_machines; j++)
		{

			//	cout << " machine " << MP->getMachine_hash() << " routing table values  " << endl;

			for (int i = 0; i < bits_space; i++)
			{

				val = MP->getMachine_hash();

				BigInt val1 = pow(BigInt(2), BigInt(i));
				val = val + val1;

				res = pow(BigInt(2), BigInt(bits_space));
				//	cout << "val is " << val << endl;
				res = val % res;
				//	cout << "res is  : " << res << endl;
				succ(res, i, MP);

			}

			MP = MP->Mnext;

			cout << endl;

		}

	}

	void succ(BigInt val, int i, machine* MP)
	{
		machine* curr = head;
		int count = 0;
		BigInt zero(0);
		if (val != zero)
		{
			while ((curr->getMachine_hash() < val && curr != head) || count == 0) {
				curr = curr->Mnext;
				count++;
			}
		}
		MP->insertAtTableIndex(curr, i);

		//cout << "the machine returned is " << curr->getMachine_hash() << endl;


	}
	void print_RTable()
	{
		machine* curr = head;
		for (int i = 0; i < Existing_machines; i++)
		{
			cout << " machine " << curr->getMachine_hash() << " routing table values  " << endl << endl;
			curr->printTable();
			curr = curr->Mnext;
		}
	}
	void print_RTable_ofMachineWithHash(int hs)
	{
		BigInt indexHash(hs);
		machine* curr = head;
		for (int i = 0; i < Existing_machines; i++)
		{
			if (curr->getMachine_hash() == indexHash)
			{
				cout << " machine " << curr->getMachine_hash() << " routing table values  " << endl << endl;
				curr->printTable();
			}
			curr = curr->Mnext;
		}
	}



	int getTotalMachines() const {
		return totalMachines;
	}
};

bool createDirectory(std::string dirname) {
	std::string fullPath = ".\\" + dirname;

	BOOL result = CreateDirectoryA(fullPath.c_str(), NULL);

	if (result) {
		std::cout << "Directory created successfully\n";
		return true;
	}
	else {
		DWORD error = GetLastError();
		if (error == ERROR_ALREADY_EXISTS) {
			std::cout << "Directory already exists\n";
			return false;
		}
		else {
			std::cout << "Error creating directory\n";
			return false;
		}
	}
}

bool deleteDirectory(std::string dirname) {
	std::string fullPath = ".\\" + dirname;

	BOOL result = RemoveDirectoryA(fullPath.c_str());

	if (result) {
		std::cout << "Directory deleted successfully\n";
		return true;
	}
	else {
		DWORD error = GetLastError();
		if (error == ERROR_DIR_NOT_EMPTY) {
			std::cout << "Directory is not empty\n";
			return false;
		}
		else {
			std::cout << "Error deleting directory\n";
			return false;
		}
	}
}
#endif // BIGINT_H