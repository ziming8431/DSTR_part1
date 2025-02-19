#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>

using namespace std;

// Structure representing a news article.
struct Article {
    string title;
    string text;
    string subject;
    string date;   // Original date string, e.g., "November 5, 2017" or "Mar 13, 2017"
    int year;      // Parsed year from the date
    int month;     // Parsed month from the date
    Article* next;
    Article* prev;
};

class DoublyLinkedList {
private:
    Article* head;
    Article* tail;
    int size;

    // Merge sort helper functions (iterative, bottom-up).
    Article* mergeSortIterative(Article* head);
    Article* split(Article* start, int step);
    Article* mergeLists(Article* l1, Article* l2);
    
    // Comparison function: sorts primarily by year, then by month.
    bool compareArticles(Article* a, Article* b);

    // Helper function to parse the date string into year and month.
    void parseDate(const string& dateStr, int &year, int &month);

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    // Article creation and insertion.
    Article* createArticle(string title, string text, string subject, string date);
    void InsertArticle(Article* article);

    // File operations.
    void loadFromCSV(string filename);
    void loadToTxt(string filename);

    // Utility functions.
    bool isValidRow(const Article& article);
    string getFakeGovernmentNewsText();
    bool hasArticle(const Article* article);
    string toLowercase(const string& s);

    // Sorting and counting.
    void sortByDate();
    int countArticles();

    Article* getHead() const { return head; }
};

#endif
