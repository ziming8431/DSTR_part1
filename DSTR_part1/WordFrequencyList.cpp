#include "WordFrequencyList.hpp"
#include "DoublyLinkedList.hpp"
#include <sstream>
#include <iostream>
#include <cctype>
#include <cmath> 

using namespace std;
WordFrequencyAnalyzer::WordFrequencyAnalyzer() {
    wordHead = nullptr;
    wordTail = nullptr;
}

const string WordFrequencyAnalyzer::COMMON_WORDS[] = {
    "the", "to", "of", "and", "a", "in", "that", "s", "is", "for", "it", "on",
    "with", "as", "was", "by", "at", "he", "she", "they", "his", "her", "their",
    "this", "that", "these", "those", "be", "are", "have", "has", "had", "will",
    "would", "should", "could", "can", "may", "might", "must", "an", "or", "not",
    "but", "if", "then", "else", "when", "where", "how", "why", "what", "which",
    "who", "whom", "there", "here", "from", "about", "into", "over", "under",
    "above", "below", "between", "through", "during", "before", "after", "out",
    "so", "such", "no", "yes", "up", "down", "just", "more", "most", "some", "any",
    "only", "very", "too", "also", "even", "than", "its", "my", "your", "our", "us",
    "we", "them", "being", "been", "do", "does", "did", "done", "say", "said", "says", "i", "you", "t", "were"
};

WordFrequencyAnalyzer::~WordFrequencyAnalyzer() {
    clearWordList();
}

string WordFrequencyAnalyzer::cleanWord(const string& word) {
    string cleaned;
    for (char c : word) {
        if (isalpha(c)) {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

bool WordFrequencyAnalyzer::isCommonWord(const string& word) const {
    for (const auto& common : COMMON_WORDS) {
        if (word == common) {
            return true;
        }
    }
    return false;
}
void WordFrequencyAnalyzer::insertOrIncrementWord(const string& word, size_t& memoryUsed) {
    WordNode* current = wordHead;
    WordNode* prev = nullptr;
    while (current) {
        if (current->word == word) {
            current->count++;
            return;
        }
        prev = current;
        current = current->next;
    }
    WordNode* newNode = new WordNode(word, 1);
    memoryUsed += sizeof(WordNode); // Track memory for new WordNode
    if (prev) prev->next = newNode;
    else wordHead = newNode;
}

void WordFrequencyAnalyzer::sortWordsByFrequency() {
    // If the list is empty or has only one element, there's nothing to sort.
    if (!wordHead || !wordHead->next)
        return;

    // Selection sort: for each node, find the node with the maximum count
    for (WordNode* current = wordHead; current != nullptr; current = current->next) {
        WordNode* maxNode = current;
        for (WordNode* runner = current->next; runner != nullptr; runner = runner->next) {
            if (runner->count > maxNode->count) {
                maxNode = runner;
            }
        }
        // Swap data if a node with a higher count was found.
        if (maxNode != current) {
            swap(current->word, maxNode->word);
            swap(current->count, maxNode->count);
        }
    }
}


void WordFrequencyAnalyzer::clearWordList() {
    while (wordHead) {
        WordNode* temp = wordHead;
        wordHead = wordHead->next;
        delete temp;
    }
}

WordNode* WordFrequencyAnalyzer::findMostFrequentWord() {
    if (!wordHead) {
        return nullptr;
    }

    WordNode* mostFrequentWord = wordHead;
    int maxFrequency = wordHead->count;

    WordNode* current = wordHead->next;

    while (current) {
        if (current->count > maxFrequency) {
            maxFrequency = current->count;
            mostFrequentWord = current;
        }
        current = current->next;
    }

    return mostFrequentWord;
}

void WordFrequencyAnalyzer::analyzeAndDisplay(DoublyLinkedList& fakeList, DoublyLinkedList& trueList, const string& targetSubject, size_t& memoryUsed) {
    cout << "\nStarting word frequency analysis for subject: " << targetSubject << endl;
    cout << "Processing articles..." << endl;
    Article* currentArticle = fakeList.getHead();
    int articleCount = 0;
    while (currentArticle) {
        if (fakeList.toLowercase(currentArticle->subject) == fakeList.toLowercase(targetSubject) &&
            !trueList.hasArticle(currentArticle)) {
            articleCount++;
            istringstream iss(currentArticle->text);
            string word;
            while (iss >> word) {
                string cleaned = cleanWord(word);
                if (!cleaned.empty() && !isCommonWord(cleaned)) {
                    insertOrIncrementWord(cleaned, memoryUsed);
                }
            }
        }
        currentArticle = currentArticle->next;
    }
    cout << "Processed " << articleCount << " matching articles." << endl;
    cout << "Sorting words by frequency..." << endl;
    sortWordsByFrequency();
    cout << "\n=== Top 20 Frequent Words ===" << endl;
    WordNode* currentWord = wordHead;
    int count = 0;
    while (currentWord && count < 20) {
        cout << currentWord->word << " (" << currentWord->count << ")" << endl;
        currentWord = currentWord->next;
        count++;
    }
    if (count == 0) cout << "No words found." << endl;
    cout << "\nWord frequency analysis complete." << endl;
    clearWordList();
}