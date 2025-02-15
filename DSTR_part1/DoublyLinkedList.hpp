#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include <iostream>
#include <string>

using namespace std;

struct Article {
    string title;
    string text;
    string subject;
    string date;
    Article* nextaddress;
    Article* prevaddress;  // Pointer to the previous article
};

class DoublyLinkedList {
private:
    Article* head = nullptr;
    Article* tail = nullptr;
    int size = 0;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    // Function declarations
    Article* createArticle(string title, string text, string subject, string date);
    void InsertArticle(Article* article);  // Insert into doubly linked list
    void loadFromCSV(string filename);
    //void sortByDate();
    //int countArticles();
    //void searchFakeNews();
    bool isValidRow(const Article& row);
    void loadToTxt(string filename);
    string getFakeGovernmentNewsText();
    bool hasArticle(const Article* article) const;
    string toLowercase(const string& s);
    Article* getHead() const { return head; }
};

#endif
