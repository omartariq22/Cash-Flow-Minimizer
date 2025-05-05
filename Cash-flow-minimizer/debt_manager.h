#ifndef DEBT_MANAGER_H
#define DEBT_MANAGER_H

#include <iostream>
#include <cstring>

class String {
private:
    char* str;
public:
    String();
    String(const char* s);
    String(const String& other);
    ~String();
    const char* c_str() const;
    int length() const;
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    String& operator=(const String& other);
    void append(const char* s);
    char operator[](int index) const;
    friend std::ostream& operator<<(std::ostream& os, const String& s);
};

template <typename T>
class Vector {
private:
    T* data;
    int capacity;
    int size;
public:
    Vector();
    ~Vector();
    void push_back(const T& value);
    T& operator[](int index);
    int getSize() const;
};

class UnorderedMap {
private:
    struct Node {
        String key;
        int value;
        Node* next;
        Node(const String& k, int v);
    };

    static const int TABLE_SIZE = 10;
    Node* table[TABLE_SIZE];
    int hash(const String& key) const;

public:
    UnorderedMap();
    ~UnorderedMap();
    void insert(const String& key, int value);
    bool find(const String& key) const;
    int& operator[](const String& key);
    int getSize() const;

    class Iterator {
    private:
        const UnorderedMap& map;
        int index;
        Node* current;
    public:
        Iterator(const UnorderedMap& m);
        bool hasNext() const;
        void next();
        String getKey() const;
        int getValue() const;
    };

    Iterator begin() const;
};

class Participant {
public:
    String name;
    int balance;
    Participant(const String& n);
};

class DebtGraph {
private:
    UnorderedMap participants;
public:
    void addParticipant(const String& name);
    bool addDebt(const String& from, const String& to, int amount);
    void minimizeTransactions();
    void displayParticipants();
};

#endif
