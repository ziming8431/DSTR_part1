#ifndef WORDFREQUENCYLIST_HPP
#define WORDFREQUENCYLIST_HPP

#include "DoublyLinkedList.hpp"
#include <string>

using namespace std;

struct WordNode {
    string word;
    int count;
    WordNode* next;
	WordNode* prev;
    WordNode(string w, int c) {
		word = w;
		count = c;
		next = nullptr;
    }
};

class WordFrequencyAnalyzer {
private:

    WordNode* wordHead = nullptr;
	WordNode* wordTail = nullptr;
    static const string COMMON_WORDS[];

    // Helper functions
    static string cleanWord(const string& word);
    bool isCommonWord(const string& word) const;
    void insertOrIncrementWord(const string& word);
    void sortWordsByFrequency();
    void clearWordList();
	WordNode* findMostFrequentWord();

public:
    WordFrequencyAnalyzer();
    ~WordFrequencyAnalyzer();

    void analyzeAndDisplay(DoublyLinkedList& fakeList, DoublyLinkedList& trueList, const string& targetSubject);
};

#endif
