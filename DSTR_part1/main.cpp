#include "DoublyLinkedList.hpp"
#include "WordFrequencyList.hpp"
#include <iostream>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

int main() {
    // Load the original lists from CSV files.
    auto startLoadDoubly = high_resolution_clock::now();
    DoublyLinkedList trueNewsList;
    DoublyLinkedList fakeNewsList;
    trueNewsList.loadFromCSV("true.csv");
    fakeNewsList.loadFromCSV("fake.csv");
    auto endLoadDoubly = high_resolution_clock::now();
    auto loadTimeDoubly = duration_cast<milliseconds>(endLoadDoubly - startLoadDoubly).count();
    size_t memoryDoubly = trueNewsList.calculateMemoryUsage() + fakeNewsList.calculateMemoryUsage();
    cout << "Doubly Linked List Load Time: " << loadTimeDoubly << " ms\n";
    cout << "Doubly Linked List Memory Usage: " << memoryDoubly / (1024.0 * 1024.0) << " MB\n";
    cout << "Doubly Linked List Article Count: " << trueNewsList.countArticles() + fakeNewsList.countArticles() << "\n";

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
                cout << "Invalid choice. Returning to main menu.\n";
                break;
            }

            DoublyLinkedList* sortedTrue = trueNewsList.clone();
            DoublyLinkedList* sortedFake = fakeNewsList.clone();
            size_t extraSpaceTrue = 0, extraSpaceFake = 0;

            auto start = high_resolution_clock::now();
            switch (sortChoice) {
            case 1:
                cout << "\nSorting using Merge Sort..." << endl;
                sortedTrue->MergeSort(extraSpaceTrue);
                sortedFake->MergeSort(extraSpaceFake);
                break;
            case 2:
                cout << "\nSorting using Bubble Sort..." << endl;
                sortedTrue->bubbleSort(extraSpaceTrue);
                sortedFake->bubbleSort(extraSpaceFake);
                break;
            case 3:
                cout << "\nSorting using Quick Sort..." << endl;
                sortedTrue->quickSort(extraSpaceTrue);
                sortedFake->quickSort(extraSpaceFake);
                break;
            }
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start).count();
            size_t totalExtraSpace = extraSpaceTrue + extraSpaceFake;
            size_t totalMemory = sortedTrue->calculateMemoryUsage() + sortedFake->calculateMemoryUsage() + totalExtraSpace;

            cout << "\nSorting completed in " << duration << " milliseconds.\n";
            cout << "Extra Space Used: " << totalExtraSpace / 1024.0 << " KB\n";
            cout << "Total Memory Usage (Base + Extra): " << totalMemory / (1024.0 * 1024.0) << " MB\n";

            string fileSuffix = (sortChoice == 1) ? "merge_sorted" : (sortChoice == 2) ? "bubble_sorted" : "quick_sorted";
            sortedTrue->loadToTxt(fileSuffix + "_true_news.txt");
            sortedFake->loadToTxt(fileSuffix + "_fake_news.txt");

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
            cout << "\nprinting monthly fake political news percentage in 2016:\n";
            auto start = high_resolution_clock::now();

            fakeNewsList.AnalyseFakeArticles();
            trueNewsList.AnalyseTrueArticles();

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "\nLinear search approach completed in " << duration.count() << " milliseconds." << endl;
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