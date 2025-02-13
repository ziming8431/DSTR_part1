#include "DoublyLinkedList.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// Constructor
DoublyLinkedList::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Destructor to free the memory
DoublyLinkedList::~DoublyLinkedList() {
    Article* current = head;
    while (current) {
        Article* temp = current;
        current = current->nextaddress;
        delete temp;
    }
}

// Create a new article (node)
Article* DoublyLinkedList::createArticle(string title, string text, string subject, string date) {
    Article* newArticle = new Article;
    newArticle->title = title;
    newArticle->text = text;
    newArticle->subject = subject;
    newArticle->date = date;
    newArticle->nextaddress = nullptr;
    newArticle->prevaddress = nullptr;  // No previous node initially
    return newArticle;
}

// Insert an article at the end of the doubly linked list
void DoublyLinkedList::InsertArticle(Article* article) {
    if (head == nullptr) {
        head = tail = article;
    }
    else {
        tail->nextaddress = article;
        article->prevaddress = tail;  // Link back to the previous node
        tail = article;
    }
    size++;
}

// Load articles from CSV file
void DoublyLinkedList::loadFromCSV(string filename) {
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
        int column = 0;

        for (size_t i = 0; i < line.size(); i++) {
            char c = line[i];

            if (c == '"') {
                insideQuote = !insideQuote;  // Toggle insideQuote flag
            }
            else if (c == ',' && !insideQuote) {
                // Assign field to corresponding variable
                if (column == 0) title = field;
                else if (column == 1) text = field;
                else if (column == 2) subject = field;
                else if (column == 3) date = field;

                field.clear();  // Reset field for next value
                column++;  // Move to next column
            }
            else {
                field += c;
            }
        }

        // Add last field (date column)
        if (column == 3) {
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
    file.close();
}

// Save the articles to a text file
void DoublyLinkedList::loadToTxt(string filename) {
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


//// Sort articles by date (ascending order)
//void DoublyLinkedList::sortByDate() {
//    if (head == nullptr || head->nextaddress == nullptr) {
//        return;
//    }
//
//    bool swapped;
//    do {
//        swapped = false;
//        Article* current = head;
//
//        while (current && current->nextaddress) {
//            // Compare the current article's date with the next article's date
//            if (current->date > current->nextaddress->date) {
//                // Swap the data between current article and next article
//                swap(current->title, current->nextaddress->title);
//                swap(current->text, current->nextaddress->text);
//                swap(current->subject, current->nextaddress->subject);
//                swap(current->date, current->nextaddress->date);
//                swapped = true;  // Set swapped to true since a swap happened
//            }
//            current = current->nextaddress;  // Move to the next article
//        }
//    } while (swapped);
//}

//// return the total number of articles in the list
//int doublylinkedlist::countarticles() {
//    return size;
//}

// Check if the row (article) is valid (non-empty fields)
bool DoublyLinkedList::isValidRow(const Article& row) {
    return !(row.title.empty() || row.text.empty() || row.subject.empty() || row.date.empty());
}
