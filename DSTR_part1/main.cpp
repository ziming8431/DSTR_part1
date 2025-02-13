#include "Linkedlist.hpp"

int main() {
	LinkedList trueNewsList;
	LinkedList fakeNewsList;

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

	return 0;
}