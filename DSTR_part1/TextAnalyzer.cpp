//#include "TextAnalyzer.hpp"
//#include "WordFrequencyList.hpp"
//#include <sstream>
//#include <string>
//#include <cctype> // For isalpha and tolower
//
//using namespace std;
//
//TextAnalyzer::TextAnalyzer() {}
//TextAnalyzer::~TextAnalyzer() {}
//
//WordFrequencyList* TextAnalyzer::calculateWordFrequency(const string& text) {
//    WordFrequencyList* wordFrequencyList = new WordFrequencyList();
//    stringstream ss(text);
//    string word;
//    while (ss >> word) {
//        string processedWord = "";
//        for (char original_c : word) { // Iterate using char
//            unsigned char c = static_cast<unsigned char>(original_c); // Cast to unsigned char here!
//            if (isalpha(c)) {
//                processedWord += tolower(c);
//            }
//        }
//        if (!processedWord.empty()) {
//            wordFrequencyList->addWord(processedWord);
//        }
//    }
//    return wordFrequencyList;
//}