#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Structure representing a news article.
struct Article {
    string title;
    string text;
    string subject;
    string date;   // "November 5, 2017"
    int year;      // Parsed year
    int month;     // Parsed month
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
    void parseDate(const string& dateStr, int& year, int& month);

    // Helper function to swap the data of two articles (used in bubble & quick sort).
    void swapArticleData(Article* a, Article* b);

    // Quick sort helper functions.
    Article* partition(Article* low, Article* high);
    void quickSortRec(Article* low, Article* high);

    // ======= Search Helper Functions (no built-in containers) =======
    void searchByYear(int year);
    void searchByMonth(int month);
    void searchBySubject(const string& subject);
    void searchByKeywordInTitle(const string& keyword);
    void searchByKeywordInText(const string& keyword);
    void displaySearchResults(Article** results, int resultCount, const string& searchType, const string& searchTerm);
    static bool containsIgnoreCase(const string& source, const string& search);

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

    // Make this static so we can call from static methods (or consider a free function).
    static string toLowercase(const string& s);

    bool hasArticle(const Article* target);
    string getFakeGovernmentNewsText();

    // Sorting and counting.
    void MergeSort();        // Merge Sort
    int countArticles() const; // <--- now const

    // Additional sorts.

	void bubbleSort();
    void quickSort();
    void swapNodes(Article* a, Article* b);


    // Search function (menu-based).
    void searchArticles();

    // Clone the list (deep copy).
    DoublyLinkedList* clone() const;


    // ======= NEW: Linear-Search-based analysis =======
    void AnalyseTrueArticles();
    void TrueMonthMatching(const string& month_str);
    void AnalyseFakeArticles();
    void FakeMonthMatching(const string& month_str);
    void DisplayPercentage();

    Article* getHead() const { return head; }
};

#endif