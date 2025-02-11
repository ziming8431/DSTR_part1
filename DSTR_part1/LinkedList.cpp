#include "Linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

//Constructor
LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
	size = 0;
}
// Destructor to free the memory
LinkedList::~LinkedList() {
	Article* current = head;
	while (current) {
		Article* temp = current;
		current = current->nextaddress;
		delete temp;
	}
}

Article* LinkedList::createArticle(string title, string content, string subject, string date) {
	Article* newArticle = new Article;
	newArticle->title = title;
	newArticle->content = content;
	newArticle->subject = subject;
	newArticle->date = date;
	newArticle->nextaddress = nullptr;
	return newArticle;
}

void LinkedList::InsertArticle(Article* article) {

	if (head == nullptr) 
	{
		head = tail = article;
	}
	else 
	{
		tail->nextaddress = article;
		tail = article;
	}
	size++;
}

void LinkedList::loadFromCSV(string filename) {
	cout << "Attempting to open file: " << filename << endl;
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Failed to open file: " << filename << endl;
		return;
	}
	string line;


	while (getline(file, line)) {
		stringstream ss(line);
		string title, content, subject, date;

		getline(ss, title, ',');
		getline(ss, content, ',');
		getline(ss, subject, ',');
		getline(ss, date, ',');

		cout << "Inserting article: " << title << endl;  // Check which articles are being processed
		InsertArticle(createArticle(title, content, subject, date));
	
	}
}

void LinkedList::displayArticles() {
	Article* current = head;

	if (head == nullptr) {
		cout << "No articles to display!" << endl;
		return;
	}

	else {
		while (current) {
			cout << "Title: " << current->title << endl;
			cout << "Content: " << current->content << endl;
			cout << "Subject: " << current->subject << endl;
			cout << "Date: " << current->date << endl;
			current = current->nextaddress;
		}
	}
}

void LinkedList::sortByDate() {
	if (head == nullptr || head->nextaddress == nullptr) {
		return;
	}

	bool swapped;
	do {
		swapped = false;
		Article* current = head;

		while (current && current->nextaddress) {
			// Compare the current article's date with the next article's date
			if (current->date > current->nextaddress->date) {
				// Swap the data between current article and next article
				swap(current->title, current->nextaddress->title);
				swap(current->content, current->nextaddress->content);
				swap(current->subject, current->nextaddress->subject);
				swap(current->date, current->nextaddress->date);
				swapped = true;  // Set swapped to true since a swap happened
			}
			current = current->nextaddress;  // Move to the next article
		}

	} while (swapped);
}

int LinkedList::countArticles() {
	return size;
}


