#include "DoublyLinkedList.hpp"
#include "WordFrequencyList.hpp"
#include <iostream>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

int main() {
    DoublyLinkedList trueNewsList;
    DoublyLinkedList fakeNewsList;

    // Load data with memory tracking
    size_t memoryUsedLoad = 0;
    auto loadStart = high_resolution_clock::now();
    trueNewsList.loadFromCSV("true.csv", memoryUsedLoad);
    fakeNewsList.loadFromCSV("fake.csv", memoryUsedLoad);
    auto loadEnd = high_resolution_clock::now();
    auto loadDuration = duration_cast<milliseconds>(loadEnd - loadStart).count();
    cout << "Memory used for loading: " << memoryUsedLoad << " bytes" << endl;
    cout << "Time taken for loading CSVs: " << loadDuration << " milliseconds" << endl;

    bool exitProgram = false;
    while (!exitProgram) {
        cout << "\n=== Main Menu ===" << endl;
        cout << "1. Test Sorting Algorithms" << endl;
        cout << "2. Search Articles" << endl;
        cout << "3. Find Frequency Word in Government news" << endl;
        cout << "4. Print Monthly Fake Political News Percentage" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        int mainChoice;
        cin >> mainChoice;
        cin.ignore();

        switch (mainChoice) {
        case 1: {
            cout << "\nSelect sorting algorithm to test:" << endl;
            cout << "1. Merge Sort" << endl;
            cout << "2. Bubble Sort" << endl;
            cout << "3. Quick Sort" << endl;
            cout << "Enter your choice: ";
            int sortChoice;
            cin >> sortChoice;
            cin.ignore();

            if (sortChoice < 1 || sortChoice > 3) {
                cout << "Invalid choice." << endl;
                break;
            }


            DoublyLinkedList* sortedTrue = trueNewsList.clone();
            DoublyLinkedList* sortedFake = fakeNewsList.clone();
            int countBeforeTrue = sortedTrue->countArticles();
            int countBeforeFake = sortedFake->countArticles();

            size_t memoryUsedSort = 0;

            memoryUsedSort += sizeof(Article) * countBeforeTrue; // True articles
            memoryUsedSort += sizeof(Article) * countBeforeFake; // Fake articles

            auto start = high_resolution_clock::now();
            switch (sortChoice) {
            case 1:
                cout << "Initial memoryUsedSort: " << memoryUsedSort << " bytes" << endl;
                sortedTrue->MergeSort(memoryUsedSort);
                sortedFake->MergeSort(memoryUsedSort);
                cout << "Initial memoryUsedSort: " << memoryUsedSort << " bytes" << endl;
                break;
            case 2:
                sortedTrue->bubbleSort(memoryUsedSort);
                sortedFake->bubbleSort(memoryUsedSort);
                break;
            case 3:
                sortedTrue->quickSort(memoryUsedSort);
                sortedFake->quickSort(memoryUsedSort);
                break;
            }
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            int countAfterTrue = sortedTrue->countArticles();
            int countAfterFake = sortedFake->countArticles();

            cout << "\nSorting completed in " << duration.count() << " milliseconds." << endl;
            cout << "Memory used for sorting: " << memoryUsedSort << " bytes" << "\n" << endl;
            cout << "True Article Count: " << countAfterTrue << "\n";
            cout << "False Article Count: " << countAfterFake << "\n";

            // Ask user if they want to print sorted true articles
            char printTrueChoice;
            cout << "\nDo you want to print the sorted true news articles? (y/n): ";
            cin >> printTrueChoice;
            cin.ignore();
            if (tolower(printTrueChoice) == 'y') {
                cout << "\nSorted True News Articles:\n";
                sortedTrue->displayArticles();
            }

            // Ask user if they want to print sorted fake articles
            char printFakeChoice;
            cout << "\nDo you want to print the sorted fake news articles? (y/n): ";
            cin >> printFakeChoice;
            cin.ignore();
            if (tolower(printFakeChoice) == 'y') {
                cout << "\nSorted Fake News Articles:\n";
                sortedFake->displayArticles();
            }

            cout << "\nSorting completed in " << duration.count() << " milliseconds." << endl;
            cout << "True Article Count: " << countAfterTrue << "\n";
            cout << "False Article Count: " << countAfterFake << "\n";


            string fileSuffix;
            if (sortChoice == 1)
                fileSuffix = "merge_sorted";
            else if (sortChoice == 2)
                fileSuffix = "bubble_sorted";
            else
                fileSuffix = "quick_sorted";

            // Save results to .txt files based on the sorting algorithm
            sortedTrue->loadToTxt(fileSuffix + "_true_news.txt");
            sortedFake->loadToTxt(fileSuffix + "_fake_news.txt");

            delete sortedTrue;
            delete sortedFake;
            break;
        }
        case 2: {
            size_t searchMemoryUsed = 0;
            long long searchDuration = 0;
            cout << "\nSelect dataset to search:" << endl;
            cout << "1. True News" << endl;
            cout << "2. Fake News" << endl;
            cout << "Enter your choice: ";
            int datasetChoice;
            cin >> datasetChoice;
            cin.ignore();
            auto searchStart = high_resolution_clock::now();
            if (datasetChoice == 1) {
                trueNewsList.searchArticles();
                auto searchEnd = high_resolution_clock::now();
                auto searchDuration = duration_cast<milliseconds>(searchEnd - searchStart).count();
                cout << "Memory used for searching true news: " << searchMemoryUsed << " bytes" << endl;
                cout << "Time taken for searching true news: " << searchDuration << " milliseconds" << endl;
            }
            else if (datasetChoice == 2) {
                fakeNewsList.searchArticles();
                auto searchEnd = high_resolution_clock::now();
                auto searchDuration = duration_cast<milliseconds>(searchEnd - searchStart).count();
                cout << "Memory used for searching fake news: " << searchMemoryUsed << " bytes" << endl;
                cout << "Time taken for searching fake news: " << searchDuration << " milliseconds" << endl;
            }
            else cout << "Invalid dataset choice." << endl;
            break;
        }
        case 3: {
            size_t memoryUsedAnalyze = 0;
            auto start = high_resolution_clock::now();
            WordFrequencyAnalyzer analyzer;
            analyzer.analyzeAndDisplay(fakeNewsList, trueNewsList, "Government News", memoryUsedAnalyze);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "Memory used for frequency analysis: " << memoryUsedAnalyze << " bytes" << endl;
            cout << "Analysis completed in " << duration.count() << " milliseconds." << endl;
            break;
        }
        case 4: {
            auto start = high_resolution_clock::now();
            fakeNewsList.AnalyseFakeArticles();
            trueNewsList.AnalyseTrueArticles();
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "Linear search completed in " << duration.count() << " milliseconds." << endl;
            break;
        }
        case 5:
            exitProgram = true;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    }
    return 0;
}