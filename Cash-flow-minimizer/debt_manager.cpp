#include "debt_manager.h"
#include <iostream>
#include <cstring>
using namespace std;

// ======== String Implementation ========
String::String() {
    str = new char[1];
    str[0] = '\0';
}

String::String(const char* s) {
    int len = strlen(s);
    str = new char[len + 1];
    strcpy_s(str, len + 1, s);
}

String::String(const String& other) {
    int len = strlen(other.str);
    str = new char[len + 1];
    strcpy_s(str, len + 1, other.str);
}

String::~String() {
    delete[] str;
}

const char* String::c_str() const {
    return str;
}

int String::length() const {
    return strlen(str);
}

bool String::operator==(const String& other) const {
    return strcmp(str, other.str) == 0;
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] str;
        int len = strlen(other.str);
        str = new char[len + 1];
        strcpy_s(str, len + 1, other.str);
    }
    return *this;
}

void String::append(const char* s) {
    int len = strlen(str) + strlen(s);
    char* new_str = new char[len + 1];
    strcpy_s(new_str, len + 1, str);
    strcat_s(new_str, len + 1, s);
    delete[] str;
    str = new_str;
}

char String::operator[](int index) const {
    return str[index];
}

ostream& operator<<(ostream& os, const String& s) {
    os << s.c_str();
    return os;
}

// ======== Vector Implementation ========
template <typename T>
Vector<T>::Vector() : capacity(10), size(0) {
    data = new T[capacity];
}

template <typename T>
Vector<T>::~Vector() {
    delete[] data;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size == capacity) {
        capacity *= 2;
        T* new_data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }
    data[size++] = value;
}

template <typename T>
T& Vector<T>::operator[](int index) {
    return data[index];
}

template <typename T>
int Vector<T>::getSize() const {
    return size;
}

// Explicit template instantiation for required types
template class Vector<Participant*>;
template class Vector<pair<pair<String, String>, int>>;

// ======== UnorderedMap Implementation ========
UnorderedMap::Node::Node(const String& k, int v) : key(k), value(v), next(nullptr) {}

int UnorderedMap::hash(const String& key) const {
    int hashValue = 0;
    for (int i = 0; i < key.length(); ++i) {
        hashValue = (hashValue * 31 + key[i]) % TABLE_SIZE;
    }
    return hashValue;
}

UnorderedMap::UnorderedMap() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

UnorderedMap::~UnorderedMap() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* current = table[i];
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
}

void UnorderedMap::insert(const String& key, int value) {
    int index = hash(key);
    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    table[index] = newNode;
}

bool UnorderedMap::find(const String& key) const {
    int index = hash(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int& UnorderedMap::operator[](const String& key) {
    int index = hash(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    insert(key, 0);
    return table[index]->value;
}

int UnorderedMap::getSize() const {
    int size = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* current = table[i];
        while (current) {
            ++size;
            current = current->next;
        }
    }
    return size;
}

// ======== Iterator Implementation ========
UnorderedMap::Iterator::Iterator(const UnorderedMap& m) : map(m), index(0), current(m.table[0]) {
    while (index < TABLE_SIZE && current == nullptr) {
        ++index;
        if (index < TABLE_SIZE) {
            current = map.table[index];
        }
    }
}

bool UnorderedMap::Iterator::hasNext() const {
    return index < TABLE_SIZE && current != nullptr;
}

void UnorderedMap::Iterator::next() {
    if (current) {
        current = current->next;
        while (index < TABLE_SIZE && current == nullptr) {
            ++index;
            if (index < TABLE_SIZE) {
                current = map.table[index];
            }
        }
    }
}

String UnorderedMap::Iterator::getKey() const {
    return current->key;
}

int UnorderedMap::Iterator::getValue() const {
    return current->value;
}

UnorderedMap::Iterator UnorderedMap::begin() const {
    return Iterator(*this);
}

// ======== Participant Implementation ========
Participant::Participant(const String& n) : name(n), balance(0) {}

// ======== DebtGraph Implementation ========
void DebtGraph::addParticipant(const String& name) {
    if (!participants.find(name)) {
        participants.insert(name, 0);
    }
}

bool DebtGraph::addDebt(const String& from, const String& to, int amount) {
    if (!participants.find(from) || !participants.find(to)) {
        cout << "Unrecognized participant(s) in debt record: " << from.c_str() << " owes " << to.c_str() << "\n";
        return false;
    }

    participants[from] -= amount;
    participants[to] += amount;
    return true;
}

void DebtGraph::minimizeTransactions() {
    Vector<Participant*> debtors;
    Vector<Participant*> creditors;

    for (auto it = participants.begin(); it.hasNext(); it.next()) {
        String participantName = it.getKey();
        int balance = it.getValue();

        Participant* p = new Participant(participantName);
        p->balance = balance;

        if (balance < 0) {
            debtors.push_back(p);
        }
        else if (balance > 0) {
            creditors.push_back(p);
        }
    }

    Vector<pair<pair<String, String>, int>> transactions;
    int i = 0, j = 0;

    while (i < debtors.getSize() && j < creditors.getSize()) {
        Participant* debtor = debtors[i];
        Participant* creditor = creditors[j];

        int amount = (debtor->balance < creditor->balance) ? -debtor->balance : creditor->balance;

        transactions.push_back({ {debtor->name, creditor->name}, amount });

        debtor->balance += amount;
        creditor->balance -= amount;

        if (debtor->balance == 0) i++;
        if (creditor->balance == 0) j++;
    }

    cout << "Minimum number of transactions required: " << transactions.getSize() << "\n";
    cout << "Optimized Transactions to Settle Debts:\n";
    for (int i = 0; i < transactions.getSize(); ++i) {
        auto& transaction = transactions[i];
        cout << transaction.first.first.c_str() << " pays " << transaction.first.second.c_str()
            << " $" << transaction.second << "\n";
    }
}

void DebtGraph::displayParticipants() {
    cout << "Participants in the system:\n";
    for (auto it = participants.begin(); it.hasNext(); it.next()) {
        cout << it.getKey().c_str() << "\n";
    }
}
