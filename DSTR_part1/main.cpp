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
            bool continueTesting = true;
            while (continueTesting) {
                cout << "\nSelect sorting algorithm to test:" << endl;
                cout << "1. Merge Sort" << endl;
                cout << "2. Bubble Sort" << endl;
                cout << "3. Quick Sort" << endl;
                cout << "Enter your choice: ";
                int sortChoice;
                cin >> sortChoice;
                cin.ignore();
                if (sortChoice != 1 && sortChoice != 2 && sortChoice != 3) {
                    cout << "Invalid choice. Please try again." << endl;
                    continue;
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

                string fileSuffix;
                if (sortChoice == 1)
                    fileSuffix = "merge_sorted";
                else if (sortChoice == 2)
                    fileSuffix = "bubble_sorted";
                else
                    fileSuffix = "quick_sorted";

                sortedTrue->loadToTxt(fileSuffix + "_true_news.txt");
                sortedFake->loadToTxt(fileSuffix + "_fake_news.txt");

                delete sortedTrue;
                delete sortedFake;

                cout << "\nDo you want to test another sorting algorithm? (Y/N): ";
                char cont;
                cin >> cont;
                cin.ignore();
                if (tolower(cont) != 'y') {
                    continueTesting = false;
                }
            }
            break;
        }
        case 2: {
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
            cout << "\n=== Frequency Word Analysis ===" << endl;
            WordFrequencyAnalyzer analyzer;
            analyzer.analyzeAndDisplay(fakeNewsList, trueNewsList, "Government News");
            break;
        }
        case 4: {
            cout << "\nPrinting the monthly percentage of fake political news in 2016...\n";
            // Ensure both lists are sorted by date.
            trueNewsList.sortByDate();
            fakeNewsList.sortByDate();

            DoublyLinkedList::printMonthlyFakePoliticalNewsPercentage2016(trueNewsList, fakeNewsList);
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
