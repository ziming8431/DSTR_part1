#include "DoublyLinkedList.hpp"
#include "WordFrequencyList.hpp"
#include <iostream>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

int main() {
    // Load the original lists from CSV files.
    DoublyLinkedList trueNewsList;
    DoublyLinkedList fakeNewsList;
    trueNewsList.loadFromCSV("true.csv");
    fakeNewsList.loadFromCSV("fake.csv");

    bool exitProgram = false;
    while (!exitProgram) {
        cout << "\n=== Main Menu ===" << endl;
        cout << "1. Test Sorting Algorithms" << endl;
        cout << "2. Search Articles" << endl;
        cout << "3. Find Frequency Word" << endl;
        cout << "4. Print Monthly Fake Political News Percentage" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        int mainChoice;
        cin >> mainChoice;
        cin.ignore(); // clear newline

        switch (mainChoice) {
        case 1: {
            // Test Sorting Algorithms
            cout << "\nSelect sorting algorithm to test:" << endl;
            cout << "1. Merge Sort" << endl;
            cout << "2. Bubble Sort" << endl;
            cout << "3. Quick Sort" << endl;
            cout << "Enter your choice: ";
            int sortChoice;
            cin >> sortChoice;
            cin.ignore();

            if (sortChoice != 1 && sortChoice != 2 && sortChoice != 3) {
                cout << "Invalid choice. Returning to main menu.\n";
                break;
            }

            // Clone the original lists so each sort runs on the same unsorted data.
            DoublyLinkedList* sortedTrue = trueNewsList.clone();
            DoublyLinkedList* sortedFake = fakeNewsList.clone();

            auto start = high_resolution_clock::now();
            switch (sortChoice) {
            case 1:
                cout << "\nSorting using Merge Sort..." << endl;
                sortedTrue->sortByDate();
                sortedFake->sortByDate();
                break;
            case 2:
                cout << "\nSorting using Bubble Sort..." << endl;
                sortedTrue->bubbleSort();
                sortedFake->bubbleSort();
                break;
            case 3:
                cout << "\nSorting using Quick Sort..." << endl;
                sortedTrue->quickSort();
                sortedFake->quickSort();
                break;
            }
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "\nSorting completed in " << duration.count() << " milliseconds." << endl;

            // Determine file names
            string fileSuffix;
            if (sortChoice == 1)
                fileSuffix = "merge_sorted";
            else if (sortChoice == 2)
                fileSuffix = "bubble_sorted";
            else
                fileSuffix = "quick_sorted";

            // Save results to .txt files
            sortedTrue->loadToTxt(fileSuffix + "_true_news.txt");
            sortedFake->loadToTxt(fileSuffix + "_fake_news.txt");

            // Clean up
            delete sortedTrue;
            delete sortedFake;
            break;
        }

        case 2: {
            // Search Articles
            cout << "\nSelect dataset to search:" << endl;
            cout << "1. True News" << endl;
            cout << "2. Fake News" << endl;
            cout << "Enter your choice: ";
            int datasetChoice;
            cin >> datasetChoice;
            cin.ignore();
            if (datasetChoice == 1) {
                trueNewsList.searchArticles();
            }
            else if (datasetChoice == 2) {
                fakeNewsList.searchArticles();
            }
            else {
                cout << "Invalid dataset choice." << endl;
            }
            break;
        }

        case 3: {
            // Find Frequency Word
            cout << "\n=== Frequency Word Analysis ===" << endl;
            auto start = high_resolution_clock::now();

            WordFrequencyAnalyzer analyzer;
            analyzer.analyzeAndDisplay(fakeNewsList, trueNewsList, "Government News");

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "\nFrequency word analysis completed in "
                << duration.count() << " milliseconds." << endl;
            break;
        }

        case 4: {
            // Print Monthly Fake Political News Percentage
            cout << "\nSelect an approach for printing monthly fake political news percentage:\n";
            cout << "1. Binary Search Approach (requires sorted lists)\n";
            cout << "2. Linear Search Approach\n";
            cout << "Enter your choice: ";
            int subChoice;
            cin >> subChoice;
            cin.ignore();

            if (subChoice == 1) {
                // === BINARY SEARCH APPROACH ===
                auto start = high_resolution_clock::now();

                // 1) Make sure both lists are sorted by date
                trueNewsList.sortByDate();
                fakeNewsList.sortByDate();

                // 2) Call the function that uses binary search
                DoublyLinkedList::printMonthlyFakePoliticalNewsPercentage2016(trueNewsList, fakeNewsList);

                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "\nBinary search approach completed in "
                    << duration.count() << " milliseconds." << endl;

            }
            else if (subChoice == 2) {
                // === LINEAR SEARCH APPROACH ===
                auto start = high_resolution_clock::now();

                // 1) Reset counters
                DoublyLinkedList::resetMonthCounters();

                // 2) Analyze the true news list for "politicsNews" in 2016
                trueNewsList.AnalyseTrueArticles();

                // 3) Analyze the fake news list for "politics" in 2016
                fakeNewsList.AnalyseFakeArticles();

                // 4) Display the monthly percentages (ASCII bar chart)
                fakeNewsList.DisplayPercentage();

                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "\nLinear search approach completed in "
                    << duration.count() << " milliseconds." << endl;
            }
            else {
                cout << "Invalid choice. Returning to main menu.\n";
            }
            break;
        }

        case 5:
            exitProgram = true;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
    return 0;
}
