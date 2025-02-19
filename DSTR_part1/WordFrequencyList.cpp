#include "WordFrequencyList.hpp"

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
    "we", "them", "being", "been", "do", "does", "did", "done", "say", "said", "says", "i", "you", "t"
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
    // If the list is empty or has only one element, there's nothing to sort.
    if (!wordHead || !wordHead->next)
        return;

    // Selection sort: for each node, find the node with the maximum count
    // in the remainder of the list and swap data.
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
        return nullptr; // Return nullptr if the word list is empty
    }

    WordNode* mostFrequentWord = wordHead; // Assume the first word is the most frequent initially
    int maxFrequency = wordHead->count;     // Initialize max frequency with the first word's count

    WordNode* current = wordHead->next; // Start from the second word (if it exists)

    while (current) {
        if (current->count > maxFrequency) {
            maxFrequency = current->count;       // Update max frequency if we find a higher count
            mostFrequentWord = current;         // Update mostFrequentWord to the current word
        }
        current = current->next; // Move to the next word
    }

    return mostFrequentWord; // Return the WordNode with the highest frequency found
}

void WordFrequencyAnalyzer::analyzeAndDisplay(DoublyLinkedList& fakeList, DoublyLinkedList& trueList, const string& targetSubject) {
    cout << "\nStarting word frequency analysis for subject: " << targetSubject << endl;
    cout << "Processing articles..." << endl;

    // Process fake news articles
    Article* currentArticle = fakeList.getHead();
    int articleCount = 0;
    while (currentArticle) {
        // Check subject match and ensure it's not in true news
        if (fakeList.toLowercase(currentArticle->subject) ==
            fakeList.toLowercase(targetSubject) &&
            !trueList.hasArticle(currentArticle)) {

            articleCount++;
            istringstream iss(currentArticle->text);
            string word;
            while (iss >> word) {
                string cleaned = cleanWord(word);
                if (!cleaned.empty() && !isCommonWord(cleaned)) {
                    insertOrIncrementWord(cleaned);
                }
            }
        }
        currentArticle = currentArticle->next;
    }

    cout << "Processed " << articleCount << " matching articles." << endl;
    cout << "Sorting words by frequency..." << endl;

    // Sort words by frequency (highest count first)
    sortWordsByFrequency();

    cout << "\n=== Top 20 Frequent Words ===" << endl;
    WordNode* currentWord = wordHead;
    int count = 0;
    while (currentWord != nullptr && count < 20) {
        cout << currentWord->word << " (" << currentWord->count << ")" << endl;
        currentWord = currentWord->next;
        count++;
    }
    if (count == 0) {
        cout << "No words found." << endl;
    }

    // Display a final message so the user knows the analysis is complete.
    cout << "\nWord frequency analysis complete." << endl;

    // Clear the word list for the next analysis.
    clearWordList();
}

