#include "WordFrequencyList.hpp"

#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

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
    "we", "them", "being", "been", "do", "does", "did", "done", "say", "said", "says", "I"
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
void WordFrequencyAnalyzer::insertOrIncrementWord(const string& word) {
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

    // Word not found - insert new node
    WordNode* newNode = new WordNode(word, 1);
    if (prev) {
        prev->next = newNode;
    }
    else {
        wordHead = newNode;
    }
}

void WordFrequencyAnalyzer::sortWordsByFrequency() {
    if (!wordHead || !wordHead->next) return;

    bool swapped;
    do {
        swapped = false;
        WordNode** ptr = &wordHead;

        while (*ptr && (*ptr)->next) {
            WordNode* a = *ptr;
            WordNode* b = a->next;

            if (a->count < b->count) {
                a->next = b->next;
                b->next = a;
                *ptr = b;
                swapped = true;
            }
            ptr = &(*ptr)->next;
        }
    } while (swapped);
}

void WordFrequencyAnalyzer::clearWordList() {
    while (wordHead) {
        WordNode* temp = wordHead;
        wordHead = wordHead->next;
        delete temp;
    }
}

void WordFrequencyAnalyzer::analyzeAndDisplay(DoublyLinkedList& fakeList,
    DoublyLinkedList& trueList,
    const string& targetSubject,
    int topN) {
    // Process fake news articles
    Article* currentArticle = fakeList.getHead();
    while (currentArticle) {
        // Check subject match and ensure it's not in true news
        if (fakeList.toLowercase(currentArticle->subject) ==
            fakeList.toLowercase(targetSubject) &&
            !trueList.hasArticle(currentArticle)) {

            istringstream iss(currentArticle->text);
            string word;
            while (iss >> word) {
                string cleaned = cleanWord(word);
                if (!cleaned.empty() && !isCommonWord(cleaned)) {
                    insertOrIncrementWord(cleaned);
                }
            }
        }
        currentArticle = currentArticle->nextaddress;
    }

    // Sort words by frequency
    sortWordsByFrequency();

    // Display results
    cout << "\nTop " << topN << " Frequent Words in Fake "
        << targetSubject << " News:\n";
    WordNode* current = wordHead;
    for (int i = 0; i < topN && current; i++) {
        cout << current->word << ": " << current->count << endl;
        current = current->next;
    }

    // Clear data for next analysis
    clearWordList();
}