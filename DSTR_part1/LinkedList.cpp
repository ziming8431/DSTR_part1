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

Article* LinkedList::createArticle(string title, string text, string subject, string date) {
	Article* newArticle = new Article;
	newArticle->title = title;
	newArticle->text = text;
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

void LinkedList::loadToTxt(string filename) {
	ofstream outFile(filename);

	if (!outFile) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	Article* current = head;
	while (current) {
		outFile << "Title: " << current->title << endl;
		outFile << "Text: " << current->text << endl;
		outFile << "Subject: " << current->subject << endl;
		outFile << "Date: " << current->date << endl;
		outFile << "---------------------------" << endl;
		current = current->nextaddress;
	}

	outFile.close();
	cout << "Data successfully saved to " << filename << endl;
}

void LinkedList::loadFromCSV(string filename) {
	cout << "Attempting to open file: " << filename << endl;
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Failed to open file: " << filename << endl;
		return;
	}
	string line;


	// Skip header
	getline(file, line);

	while (getline(file, line)) {
		string title, text, subject, date;
		string field;
		bool insideQuote = false;
		int fieldIndex = 0;

		for (size_t i = 0; i < line.size(); i++) {
			char c = line[i];

			if (c == '"') {
				insideQuote = !insideQuote;  // Toggle insideQuote flag
			}
			else if (c == ',' && !insideQuote) {
				// Assign field to corresponding variable
				if (fieldIndex == 0) title = field;
				else if (fieldIndex == 1) text = field;
				else if (fieldIndex == 2) subject = field;
				else if (fieldIndex == 3) date = field;

				field.clear();  // Reset field for next value
				fieldIndex++;  // Move to next column
			}
			else {
				field += c;
			}
		}

		// Add last field (date column)
		if (fieldIndex == 3) {
			date = field;
		}

		// Ensure all fields are present before adding to linked list
		if (!title.empty() && !text.empty() && !subject.empty() && !date.empty()) {
			cout << "Inserting article: " << title << endl;
			InsertArticle(createArticle(title, text, subject, date));
		}
		else {
			cout << "Skipping invalid row: missing data" << endl;
		}
	}

	loadToTxt("displaytext.txt");
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
			cout << "Text: " << current->text << endl;
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
				swap(current->text, current->nextaddress->text);
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

bool LinkedList::isValidRow(const Article& row) {
	return !(row.title.empty() || row.text.empty() || row.subject.empty() || row.date.empty());
}