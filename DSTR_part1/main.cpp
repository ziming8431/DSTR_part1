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
	// Sort by date (year)
	trueNewsList.sortByDate();
	fakeNewsList.sortByDate();

	// Display total number of articles
	cout << "Total True News Articles: " << trueNewsList.countArticles() << endl;
	cout << "Total Fake News Articles: " << fakeNewsList.countArticles() << endl;

	trueNewsList.loadToTxt("sorted_true_news.txt");
	fakeNewsList.loadToTxt("sorted_fake_news.txt");

	//WordFrequencyAnalyzer analyzer;
	//analyzer.analyzeAndDisplay(fakeNewsList, trueNewsList, "Government News");

	return 0;
}