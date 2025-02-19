#include "DoublyLinkedList.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

// ===================== Constructor and Destructor =====================
DoublyLinkedList::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

DoublyLinkedList::~DoublyLinkedList() {
    Article* current = head;
    while (current) {
        Article* temp = current;
        current = current->next;
        delete temp;
    }
}

// ===================== Date Parsing =====================
void DoublyLinkedList::parseDate(const string& dateStr, int& year, int& month) {
    istringstream iss(dateStr);
    string monthStr, dayWithComma, yearStr;
    if (!(iss >> monthStr >> dayWithComma >> yearStr)) {
        // If parsing fails, assign year and month to 0.
        year = 0;
        month = 0;
        return;
    }

    // Check if dayWithComma ends with a comma.
    if (dayWithComma.back() != ',') {
        year = 0;
        month = 0;
        return;
    }

    // Convert monthStr to lowercase for case-insensitive comparison.
    string lowerMonthStr = monthStr;
    transform(lowerMonthStr.begin(), lowerMonthStr.end(), lowerMonthStr.begin(), ::tolower);

    if (lowerMonthStr == "january" || lowerMonthStr == "jan")
        month = 1;
    else if (lowerMonthStr == "february" || lowerMonthStr == "feb")
        month = 2;
    else if (lowerMonthStr == "march" || lowerMonthStr == "mar")
        month = 3;
    else if (lowerMonthStr == "april" || lowerMonthStr == "apr")
        month = 4;
    else if (lowerMonthStr == "may")
        month = 5;
    else if (lowerMonthStr == "june" || lowerMonthStr == "jun")
        month = 6;
    else if (lowerMonthStr == "july" || lowerMonthStr == "jul")
        month = 7;
    else if (lowerMonthStr == "august" || lowerMonthStr == "aug")
        month = 8;
    else if (lowerMonthStr == "september" || lowerMonthStr == "sep" || lowerMonthStr == "sept")
        month = 9;
    else if (lowerMonthStr == "october" || lowerMonthStr == "oct")
        month = 10;
    else if (lowerMonthStr == "november" || lowerMonthStr == "nov")
        month = 11;
    else if (lowerMonthStr == "december" || lowerMonthStr == "dec")
        month = 12;
    else {
        month = 0;
        year = 0;
        return;
    }

    try {
        year = stoi(yearStr);
    }
    catch (...) {
        year = 0;
    }
}


// ===================== Article Creation and Insertion =====================
Article* DoublyLinkedList::createArticle(string title, string text, string subject, string date) {
    Article* newArticle = new Article;
    newArticle->title = title;
    newArticle->text = text;
    newArticle->subject = subject;
    newArticle->date = date;
    parseDate(date, newArticle->year, newArticle->month);
    newArticle->next = nullptr;
    newArticle->prev = nullptr;
    return newArticle;
}

void DoublyLinkedList::InsertArticle(Article* article) {
    if (head == nullptr) {
        head = tail = article;
    }
    else {
        tail->next = article;
        article->prev = tail;
        tail = article;
    }
    size++;
}

// ===================== File Operations =====================
void DoublyLinkedList::loadFromCSV(string filename) {
    cout << "Opening file: " << filename << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    // Skip header.
    getline(file, line);

    while (getline(file, line)) {
        string title, text, subject, date;
        string field;
        bool insideQuote = false;
        int column = 0;
        for (size_t i = 0; i < line.size(); i++) {
            char c = line[i];
            if (c == '"') {
                insideQuote = !insideQuote;
            }
            else if (c == ',' && !insideQuote) {
                if (column == 0) title = field;
                else if (column == 1) text = field;
                else if (column == 2) subject = field;
                else if (column == 3) date = field;
                field.clear();
                column++;
            }
            else {
                field += c;
            }
        }
        if (column == 3)
            date = field;
        if (!title.empty() && !text.empty() && !subject.empty() && !date.empty()) {
            cout << "Inserting article: " << title << endl;
            InsertArticle(createArticle(title, text, subject, date));
        }
        else {
            cout << "Skipping invalid row" << endl;
        }
    }
    loadToTxt("sorted_fake_news.txt");
    file.close();
}



void DoublyLinkedList::loadToTxt(string filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error opening file: " << filename << endl;
        return;
    }
    Article* current = head;
    while (current) {
        outFile << "Title: " << current->title << "\n";
        outFile << "Text: " << current->text << "\n";
        outFile << "Subject: " << current->subject << "\n";
        outFile << "Date: " << current->date << "\n";
        outFile << "Year: " << current->year << "\n";
        outFile << "---------------------------\n";
        current = current->next;
    }
    outFile.close();
    cout << "Data saved to " << filename << endl;
}

// ===================== Utility Functions =====================
bool DoublyLinkedList::isValidRow(const Article& article) {
    if (article.title.empty() || article.text.empty() || article.subject.empty() || article.date.empty())
        return false;
    // Regex to validate date format (e.g., "March 15, 2021" or "Dec. 5, 2022")
    regex datePattern(R"(^(January|February|March|April|May|June|July|August|September|October|November|December|Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Sept|Oct|Nov|Dec)(\.?)\s\d{1,2},\s\d{4}$)");
    return regex_match(article.date, datePattern);
}


string DoublyLinkedList::getFakeGovernmentNewsText() {
    string combinedText = "";
    Article* current = head;
    while (current) {
        if (current->subject == "government")
            combinedText += current->text + " ";
        current = current->next;
    }
    return combinedText;
}

bool DoublyLinkedList::hasArticle(const Article* target) {
    Article* current = head;
    while (current) {
        if (current->title == target->title &&
            current->text == target->text &&
            current->subject == target->subject &&
            current->date == target->date)
            return true;
        current = current->next;
    }
    return false;
}

string DoublyLinkedList::toLowercase(const string& s) {
    string result = s;
    for (char& c : result)
        c = tolower(c);
    return result;
}

// ===================== Merge Sort Functions =====================

// Comparison function: returns true if article 'a' should come before 'b'
bool DoublyLinkedList::compareArticles(Article* a, Article* b) {
    if (a->year != b->year)
        return a->year < b->year;
    return a->month < b->month;
}

// Splits the list starting at 'start' after 'step' nodes.
// Returns a pointer to the beginning of the next sublist.
Article* DoublyLinkedList::split(Article* start, int step) {
    if (!start)
        return nullptr;
    for (int i = 1; i < step && start->next; i++) {
        start = start->next;
    }
    Article* second = start->next;
    if (second)
        second->prev = nullptr;
    start->next = nullptr;
    return second;
}

// Merges two sorted lists (l1 and l2) and returns the head of the merged list.
Article* DoublyLinkedList::mergeLists(Article* l1, Article* l2) {
    Article dummy;
    Article* tail = &dummy;
    dummy.next = nullptr;

    while (l1 && l2) {
        if (compareArticles(l1, l2)) {
            tail->next = l1;
            l1->prev = tail;
            l1 = l1->next;
        }
        else {
            tail->next = l2;
            l2->prev = tail;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = (l1 ? l1 : l2);
    if (tail->next)
        tail->next->prev = tail;
    return dummy.next;
}

// Iterative (bottom-up) merge sort for the linked list.
Article* DoublyLinkedList::mergeSortIterative(Article* head) {
    if (!head)
        return head;

    // Count the number of nodes.
    int n = 0;
    Article* curr = head;
    while (curr) {
        n++;
        curr = curr->next;
    }

    Article dummy;
    dummy.next = head;

    // Merge sublists of increasing size.
    for (int step = 1; step < n; step *= 2) {
        Article* prev = &dummy;
        curr = dummy.next;
        while (curr) {
            Article* left = curr;
            Article* right = split(left, step);
            curr = split(right, step);
            prev->next = mergeLists(left, right);
            while (prev->next)
                prev = prev->next;
        }
    }

    Article* sortedHead = dummy.next;
    if (sortedHead)
        sortedHead->prev = nullptr;
    return sortedHead;
}

// Public function to sort the list by date (year, then month).
void DoublyLinkedList::sortByDate() {
    if (!head || !head->next)
        return;
    head = mergeSortIterative(head);
    // Update tail pointer.
    Article* temp = head;
    while (temp->next)
        temp = temp->next;
    tail = temp;
}

// Returns the total number of articles.
int DoublyLinkedList::countArticles() {
    return size;
}
