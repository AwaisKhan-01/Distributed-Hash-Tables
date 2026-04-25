#pragma once
#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <queue>
#include "BigInt.h"
using namespace std;

class BTree {
public:
    struct File {
        BigInt FileHash;
        string path;
        File* next;  // Linked list for handling collisions

        File() : FileHash(0), path(""), next(nullptr) {}
        File(const BigInt& hash, const string& p) : FileHash(hash), path(p), next(nullptr) {}
    };


    struct btnode {
        int count;
        File* fileData;
        struct btnode** child;
    };

    struct btnode* root;
    int maxOrder;

    BTree(int order) : root(NULL), maxOrder(order) {}
    BTree() : root(NULL), maxOrder(0) {}

    void printLevelOrder();

    struct btnode* insert(const BigInt& hash, const string& path, struct btnode* root) {
        File newFile(hash, path);
        File i;
        struct btnode* c, * n;
        int flag = setval(newFile, root, &i, &c);
        if (root != NULL)
        {
            if (i.FileHash == newFile.FileHash) {
                // Collision detected; insert into a linked list at the same position
                File* currentFile = &root->fileData[flag];
                while (currentFile->next != nullptr) {
                    currentFile = currentFile->next;
                }
                currentFile->next = new File(hash, path);
                return root;
            }
        }
        if (flag) {

            //else {
            n = new struct btnode;
            n->count = 1;
            n->fileData = new File[maxOrder + 1];
            n->child = new struct btnode* [maxOrder + 1];
            n->fileData[1] = i;
            n->child[0] = root;
            n->child[1] = c;
            return n;
            //}
        }

        return root;
    }



    int setval(const File& file, struct btnode* n, File* p, struct btnode** c) {
        int k;

        if (n == NULL) {
            *p = file;
            *c = NULL;
            return 1;
        }
        else {
            if (searchnode(file.FileHash, n, &k)) {
                cout << "Key FileHash already exists.\n";
                return 0;
            }

            if (setval(file, n->child[k], p, c)) {
                if (n->count < maxOrder) {
                    fillnode(*p, *c, n, k);
                    return 0;
                }
                else {
                    split(*p, *c, n, k, p, c);
                    return 1;
                }
            }

            return 0;
        }
    }

    struct btnode* search(const BigInt& hash, struct btnode* root, int* pos) {
        if (root == NULL) {
            return NULL;
        }
        else {
            if (searchnode(hash, root, pos)) {
                return root;
            }
            else {
                return search(hash, root->child[*pos], pos);
            }
        }
    }

    int searchnode(const BigInt& hash, struct btnode* n, int* pos) {
        if (hash < n->fileData[1].FileHash) {
            *pos = 0;
            return 0;
        }
        else {
            *pos = n->count;

            while ((hash < n->fileData[*pos].FileHash) && *pos > 1) {
                (*pos)--;
            }

            if (hash == n->fileData[*pos].FileHash) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }

    void fillnode(const File& file, struct btnode* c, struct btnode* n, int k) {
        int i;

        for (i = n->count; i > k; i--) {
            n->fileData[i + 1] = n->fileData[i];
            n->child[i + 1] = n->child[i];
        }

        n->fileData[k + 1] = file;
        n->child[k + 1] = c;
        n->count++;
    }

    void split(const File& file, struct btnode* c, struct btnode* n, int k, File* p, struct btnode** newnode) {
        int mid = maxOrder / 2;
        *newnode = new struct btnode;

        (*newnode)->fileData = new File[maxOrder];
        (*newnode)->child = new struct btnode* [maxOrder + 1];

        for (int i = mid + 1; i < maxOrder; i++) {
            (*newnode)->fileData[i - mid] = n->fileData[i];
            (*newnode)->child[i - mid] = n->child[i];
        }

        (*newnode)->child[0] = n->child[mid];
        (*newnode)->count = maxOrder - mid - 1;
        n->count = mid;

        if (k <= mid) {
            fillnode(file, c, n, k);
        }
        else {
            fillnode(file, c, *newnode, k - mid - 1);
        }

        *p = n->fileData[n->count];
        n->count--;
    }

    // ... (rest of the code remains unchanged)

    void display(struct btnode* root) {
        int i;
        if (root != NULL) {
            for (i = 0; i < root->count; i++) {
                display(root->child[i]);
                cout << root->fileData[i + 1].FileHash << " - " << root->fileData[i + 1].path << " ";
            }
            display(root->child[i]);
        }
    }
    int delhelp(BigInt val, struct btnode* root);
    void clear(struct btnode* node, int i);
    void copysucc(struct btnode* node, int i);
    void restore(struct btnode* node, int i);
    int balance(struct btnode* node, int i);
    void rightshift(struct btnode* node, int k);
    void leftshift(struct btnode* node, int k);
    void merge(struct btnode* node, int k);
    struct btnode* del(BigInt val, struct btnode* root);

    //~BTree() {
    //    clearTree(root);
    //}

    //void clearTree(struct btnode* node) {
    //    if (node != NULL) {
    //        for (int i = 0; i <= node->count; i++) {
    //            clearTree(node->child[i]);
    //        }
    //        delete[] node->fileData;
    //        delete[] node->child;
    //        delete node;
    //    }
    //}

};

struct BTree::btnode* BTree::del(BigInt val, struct btnode* root) {
    struct btnode* temp;
    if (!delhelp(val, root)) {
        cout << "\nFileHash " << val << " not present in tree.\n";
        return root;
    }
    if (root->count == 0) {
        temp = root;
        root = root->child[0];
        delete temp;
    }
    return root;
}

int BTree::delhelp(BigInt val, struct btnode* root) {
    int pos, i;
    struct btnode* temp, * pret, * par;
    if (root == NULL) {
        return 0;
    }
    if (searchnode(val, root, &pos)) {
        par = root;
        if (root->child[pos - 1] != NULL) {
            temp = root->child[pos - 1];
            while (temp->child[par->count] != NULL) {
                temp = temp->child[par->count];
            }
            root->fileData[pos].FileHash = temp->fileData[temp->count].FileHash;
            temp->fileData[temp->count].FileHash = val;
            return delhelp(val, temp);
        }
        else if (root->child[pos] != NULL) {
            temp = root->child[pos];
            while (temp->child[0] != NULL) {
                temp = temp->child[0];
            }
            root->fileData[pos].FileHash = temp->fileData[1].FileHash;
            temp->fileData[1].FileHash = val;
            return delhelp(val, temp);
        }
        else {
            for (i = pos + 1; i <= par->count; i++) {
                par->fileData[i - 1] = par->fileData[i];
                par->child[i - 1] = par->child[i];
            }
            par->count--;
            return 1;
        }
    }
    else {
        if (delhelp(val, root->child[pos])) {
            return balance(root, pos);
        }
        else {
            return 0;
        }
    }
}

void BTree::clear(struct btnode* node, int i) {
    for (int j = i + 1; j <= node->count; ++j) {
        node->fileData[j - 1] = node->fileData[j];
        node->child[j - 1] = node->child[j];
    }
    --node->count;
}

void BTree::copysucc(struct btnode* node, int i) {
    struct btnode* temp = node->child[i];
    while (temp->child[0] != NULL) {
        temp = temp->child[0];
    }
    node->fileData[i] = temp->fileData[1];
}

void BTree::restore(struct btnode* node, int i) {
    if (i == 0) {
        if (node->child[1]->count > (maxOrder / 2)) {
            rightshift(node, 1);
        }
        else {
            merge(node, 1);
        }
    }
    else {
        if (i == node->count) {
            if (node->child[i - 1]->count > (maxOrder / 2)) {
                leftshift(node, i);
            }
            else {
                merge(node, i);
            }
        }
        else {
            if ((node->child[i - 1]->count > (maxOrder / 2)) || (node->child[i + 1]->count > (maxOrder / 2))) {
                if (node->child[i - 1]->count > (maxOrder / 2)) {
                    leftshift(node, i);
                }
                else {
                    rightshift(node, i + 1);
                }
            }
            else {
                merge(node, i);
            }
        }
    }
}

int BTree::balance(struct btnode* node, int i) {
    if (i == 0) {
        if (node->child[1]->count > (maxOrder / 2)) {
            leftshift(node, 1);
        }
        else {
            merge(node, 1);
            return 1;
        }
    }
    else {
        if (i == node->count) {
            if (node->child[i - 1]->count > (maxOrder / 2)) {
                rightshift(node, i);
            }
            else {
                merge(node, i);
                return 1;
            }
        }
        else {
            if ((node->child[i - 1]->count > (maxOrder / 2)) || (node->child[i + 1]->count > (maxOrder / 2))) {
                if (node->child[i - 1]->count > (maxOrder / 2)) {
                    rightshift(node, i);
                }
                else {
                    leftshift(node, i + 1);
                }
            }
            else {
                merge(node, i);
                return 1;
            }
        }
    }
    return 0;
}

void BTree::rightshift(struct btnode* node, int k) {
    struct btnode* temp = node->child[k];
    for (int i = temp->count; i > 0; --i) {
        temp->fileData[i + 1] = temp->fileData[i];
        temp->child[i + 1] = temp->child[i];
    }
    temp->child[1] = temp->child[0];
    temp->count++;
    temp->fileData[1] = node->fileData[k];
    temp = node->child[k - 1];
    node->fileData[k] = temp->fileData[temp->count];
    node->child[k]->child[0] = temp->child[temp->count];
    temp->count--;
    return;
}

void BTree::leftshift(struct btnode* node, int k) {
    struct btnode* temp = node->child[k - 1];
    temp->count++;
    temp->fileData[temp->count] = node->fileData[k];
    temp->child[temp->count] = node->child[k]->child[0];
    temp = node->child[k];
    node->fileData[k] = temp->fileData[1];
    temp->child[0] = temp->child[1];
    temp->count--;
    for (int i = 1; i <= temp->count; ++i) {
        temp->fileData[i] = temp->fileData[i + 1];
        temp->child[i] = temp->child[i + 1];
    }
    return;
}

void BTree::merge(struct btnode* node, int k) {
    struct btnode* left = node->child[k - 1];
    struct btnode* right = node->child[k];
    struct btnode* temp;
    left->count++;
    left->fileData[left->count] = node->fileData[k];
    left->child[left->count] = right->child[0];

    for (int i = 1; i <= right->count; ++i) {
        left->count++;
        left->fileData[left->count] = right->fileData[i];
        left->child[left->count] = right->child[i];
    }

    clear(node, k);
    delete right;

    if (node->count == 0) {
        temp = left;
        delete temp;
    }

    return;
}


void BTree::printLevelOrder() {
    if (root == NULL) {
        cout << "No Files In Btree: \n";
        return;
    }

    queue<pair<btnode*, int>> q;
    q.push({ root, 0 });

    while (!q.empty()) {
        pair<btnode*, int> temp = q.front();
        q.pop();

        cout << "Level " << temp.second << ": ";
        for (int i = 0; i < temp.first->count; i++) {
            cout << temp.first->fileData[i + 1].FileHash << " - " << temp.first->fileData[i + 1].path << " ";

            // Print linked list values
            File* currentFile = &temp.first->fileData[i + 1];
            while (currentFile->next != nullptr) {
                currentFile = currentFile->next;
                cout << " -> " << currentFile->FileHash << " - " << currentFile->path << " ";
            }
        }

        for (int i = 0; i <= temp.first->count; i++) {
            if (temp.first->child[i] != NULL) {
                q.push({ temp.first->child[i], temp.second + 1 });
            }
        }
    }
}



#endif
