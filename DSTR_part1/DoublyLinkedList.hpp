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
    string date;   // e.g., "November 5, 2017"
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
    void displaySearchResults(Article** results, int resultCount,
        const string& searchType, const string& searchTerm);
    static bool containsIgnoreCase(const string& source, const string& search);

    // ======= Helpers for arrays and binary search =======
    Article** toArray() const;
    static int lowerBound(Article** arr, int n, int targetYear);
    static int upperBound(Article** arr, int n, int targetYear);

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
    void sortByDate();        // Merge Sort
    int countArticles() const; // <--- now const

    // Additional sorts.
    void bubbleSort();
    void quickSort();

    // Search function (menu-based).
    void searchArticles();

    // Clone the list (deep copy).
    DoublyLinkedList* clone() const;

    // Functions for political news analysis
    static double calculatePoliticalFakePercentage2016(const DoublyLinkedList& trueList,
        const DoublyLinkedList& fakeList);
    static void printMonthlyFakePoliticalNewsPercentage2016(const DoublyLinkedList& trueList,
        const DoublyLinkedList& fakeList);



    // ======= NEW: Linear-Search-based analysis =======
    // Resets counters to 0
    static void resetMonthCounters();

    // Count how many "politicsNews" articles from 2016 are in each month
    void AnalyseTrueArticles();
    // Count how many "politics" articles from 2016 are in each month
    void AnalyseFakeArticles();

    // Helpers for counting months
    void TrueMonthMatching(const string& month_str);
    void FakeMonthMatching(const string& month_str);

    // Print the monthly bar chart (using linear counters)
    void DisplayPercentage();

    // Accessor
    Article* getHead() const { return head; }
};

#endif
