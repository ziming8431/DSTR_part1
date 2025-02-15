#include "DoublyLinkedList.hpp"
#include "WordFrequencyList.hpp" // Include WordFrequencyList header
#include "TextAnalyzer.hpp"      // Include TextAnalyzer header
#include <iostream>
#include <iomanip>

int main() {
	DoublyLinkedList trueNewsList;
	DoublyLinkedList fakeNewsList;

	trueNewsList.loadFromCSV("true.csv");
	fakeNewsList.loadFromCSV("fake.csv");

	//trueNewsList.sortByDate();
	//fakeNewsList.sortByDate();

	//cout << "Total True News Articles: " << trueNewsList.countArticles() << endl;
	//cout << "Total Fake News Articles: " << fakeNewsList.countArticles() << endl;

	// Display articles
	//cout << "\nDisplaying True News Articles:\n";
	//trueNewsList.displayArticles();

	// Search for fake news articles
	//trueNewsList.searchFakeNews();

	WordFrequencyAnalyzer analyzer;
	analyzer.analyzeAndDisplay(fakeNewsList, trueNewsList, "Government News", 10);

	return 0;
}