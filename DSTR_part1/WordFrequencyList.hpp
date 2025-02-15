#ifndef WORDFREQUENCYLIST_HPP
#define WORDFREQUENCYLIST_HPP

#include "DoublyLinkedList.hpp"
#include <string>



class WordFrequencyAnalyzer {
private:
    struct WordNode {
        std::string word;
        int count;
        WordNode* next;
        WordNode(std::string w, int c) : word(w), count(c), next(nullptr) {}
    };

    WordNode* wordHead = nullptr;
    static const std::string COMMON_WORDS[];

    // Helper functions
    static std::string cleanWord(const std::string& word);
    bool isCommonWord(const std::string& word) const;
    void insertOrIncrementWord(const std::string& word);
    void sortWordsByFrequency();
    void clearWordList();

public:
    WordFrequencyAnalyzer() = default;
    ~WordFrequencyAnalyzer();

    void analyzeAndDisplay(DoublyLinkedList& fakeList,
        DoublyLinkedList& trueList,
        const std::string& targetSubject,
        int topN = 10);
};

#endif