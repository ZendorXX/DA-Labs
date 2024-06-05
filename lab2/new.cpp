#include <iostream>
#include <fstream>
#include <cstdint>
#include <string.h>

const int MAX_KEY_LENGTH =  256 + 1;
using ull = unsigned long long;

int getBit(char *string, int bit) {
    return (string[bit / 8] >> (7 - (bit % 8))) & 1;
}

int firstDiffBit(char *string_1, char *string_2) {
    if (string_1 == 0 || string_2 == 0) {
        return 0;
    }

    int matchingByte = 0;
    for (; string_1[matchingByte] == string_2[matchingByte]; ++matchingByte);
    
    int matchingBit = matchingByte * 8;
    for (; getBit(string_1, matchingBit) == getBit(string_2, matchingBit); ++matchingBit);

    return matchingBit;
}

bool isEqual(char *string_1, char *string_2) {
    if (string_1 == 0 || string_2 == 0) {
        return 0;
    }
    return (strcmp(string_1, string_2) == 0);
}

void toLower(char *string) {
    for (size_t i = 0; i < strlen(string); ++i) {
        if ('A' <= string[i] && string[i] <= 'Z') {
            string[i] = (char)(string[i] + 32);
        }
    }
}


struct Node {
    int bitNumber;

    char *key;
    ull value;

    Node *left;
    Node *right;

    void set(int _bitNumber, char *_key, ull _value, Node *_left, Node *_right) {
        bitNumber = _bitNumber;
        if (_key) {
            key = new char[strlen(_key) + 1];
            strcpy(key, _key);
        }
        else {
            key = _key;
        }
        
        value = _value;
        left = _left;
        right = _right;
    }

    Node() {
        set(-1, 0, 0, this, this);
    }

    Node(int _bitNumber, char *_key, ull _value) {
        set(_bitNumber, _key, _value, this, this);
    }

    ~Node() {
        delete [] key;
    }
};

struct Patricia {
    Node *root;

    Patricia() {
        root = new Node();
    }

    Patricia(int _bitNumber, char *_key, ull _value) {
        root = new Node(_bitNumber, _key, _value);
    }

    void destruct(Node *node) {
        if (node->left->bitNumber > node->bitNumber) {
            destruct(node->left);
        }
        if (node->right->bitNumber > node->bitNumber) {
            destruct(node->right);
        }
        delete node;
    }

    ~Patricia() {
        destruct(root);
    }

    Node* Search(char *key) {
        Node *parent = root;
        Node *current = root->left;

        while (parent->bitNumber < current->bitNumber) {
            parent = current;
            current = (getBit(key, current->bitNumber) ? current->right : current->left);
        }

        if (!isEqual(key, current->key)) {
            return nullptr;
        }

        return current;
    }

    Node* Insert(char *key, ull value) {
        if (root == nullptr) {
            root = new Node(0, key, value);
            return root;
        }

        Node *parent = root;
        Node *current = root->left;

        while (parent->bitNumber < current->bitNumber) {
            parent = current;
            current = (getBit(key, current->bitNumber) ? current->right : current->left);
        }

        if (isEqual(key, current->key)) {
            return nullptr;
        }

        int lBitPos = firstDiffBit(key, current->key);
        Node *reachedKey = current;

        parent = root;
        Node *x = root->left;

        while (parent->bitNumber < x->bitNumber && x->bitNumber < lBitPos) {
            parent = x;
            x = (getBit(key, x->bitNumber) ? x->right : x->left);
        }

        current = new Node();

        Node *left, *right;
        if (getBit(key, lBitPos)) {
            left = x;
            right = current;
        } else {
            left = current;
            right = x;
        }

        current->set(lBitPos, key, value, left, right);

        if (getBit(key, parent->bitNumber)) {
            parent->right = current;
        }
        else {
            parent->left = current;
        }

        return current;
    }

    bool Remove(char *key) {
        Node *greatParent, *parent, *current;

        parent = root;
        current = root->left;

        while (parent->bitNumber < current->bitNumber) {
            greatParent = parent;
            parent = current;
            current = (getBit(key, current->bitNumber) ? current->right : current->left);
        }

        if (!isEqual(key, current->key)) {
            return false;
        }

        Node *x, *r;
        char *key_tmp;
        if (parent != current) {
            if (strlen(current->key) < strlen(parent->key)) {
                delete [] current->key;
                current->key = new char[strlen(parent->key) + 1];
            }

            strcpy(current->key, parent->key);
            current->value = parent->value;

            key_tmp = parent->key;
            r = parent;
            x = (getBit(key_tmp, parent->bitNumber) ? parent->right : parent->left);

            while (r->bitNumber < x->bitNumber) {
                r = x;
                x = (getBit(key_tmp, x->bitNumber) ? x->right : x->left);
            }

            if (getBit(key_tmp, r->bitNumber)) {
                r->right = current;
            }
            else {
                r->left = current;
            }
        }

        Node *ch = (getBit(key, parent->bitNumber) ? parent->left : parent->right);
        if (getBit(key, greatParent->bitNumber)) {
            greatParent->right = ch;
        } 
        else {
            greatParent->left = ch;
        }

        delete parent;

        return true;
    }

#define safeKey(node) (node->key ? node->key : "root")

    void PrintFile(Node *node, std::ofstream &out)
    {
        out << safeKey(node) << ' ' << node->value << "\n";
        if (node->left->bitNumber > node->bitNumber)
            PrintFile(node->left, out);
        if (node->right->bitNumber > node->bitNumber)
            PrintFile(node->right, out);
    }

    void Save(std::ofstream &out) {
        PrintFile(root, out);
    }

    void Load(std::ifstream &in) {
        char key[MAX_KEY_LENGTH];
        ull value;

        while (in >> key >> value) {
            Insert(key, value);
        }
    }

    void Print(Node *node)
    {
        std::cout << safeKey(node) << " " << node->value << "\n";
        if (node->left->bitNumber > node->bitNumber)
            Print(node->left);
        if (node->right->bitNumber > node->bitNumber)
            Print(node->right);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    Patricia *p = new Patricia();

    char input[MAX_KEY_LENGTH];
    ull value;

    while (std::cin >> input) {
        if (input[0] == '+') {
            std::cin >> input >> value;
            toLower(input);

            if (p->Insert(input, value) != nullptr) {
                std::cout << "OK\n";
            }
            else {
                std::cout << "Exist\n";
            }
        }
        else if (input[0] == '-') {
            std::cin >> input;
            toLower(input);

            if (p->Remove(input)) {
                std::cout << "OK\n";
            }
            else {
                std::cout << "NoSuchWord\n";
            }
        }
        else if (isalpha(input[0])) {
            toLower(input);
            Node *result = p->Search(input);
            if (result != nullptr) {
                std::cout << "OK: " << result->value << '\n';
            }
            else {
                std::cout << "NoSuchWord\n";
            }
        }
        else if (input[0] == '!') {
            std::cin >> input;
            if (isEqual(input, "Save")) {
                std::cin >> input;

                std::ofstream save(input, std::ios::out);

                p->Save(save);
                std::cout << "OK\n";
            }
            else if (isEqual(input, "Load")) {  
                std::cin >> input;

                std::ifstream load(input, std::ios::in);

                delete p;
                p = new Patricia();
                p->Load(load);

                std::cout << "OK\n";
            }
        }
        else if (input[0] == '?') {
            std::cout << "Print...\n";
            p->Print(p->root);
        }
    }

    delete p;

    return 0;
}