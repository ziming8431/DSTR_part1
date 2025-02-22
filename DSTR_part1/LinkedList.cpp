#include "DoublyLinkedList.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cctype>
#include <cmath>   
#include <iomanip>

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
        year = 0;
        month = 0;
        return;
    }
    if (dayWithComma.back() != ',') {
        year = 0;
        month = 0;
        return;
    }
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
    getline(file, line);  // Skip header.
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
    loadToTxt("displaytext.txt");
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

void DoublyLinkedList::displayArticles() const {
    Article* current = head;
    const size_t maxTitleLength = 150;  // Truncate titles longer than 150 characters

    while (current) {
        string displayTitle = current->title;
        if (displayTitle.length() > maxTitleLength) {
            displayTitle = displayTitle.substr(0, maxTitleLength) + "...";
        }
        cout << "Year: " << current->year << ", Month: " << current->month << " | ";
        cout << "Title: " << displayTitle << "\n";
        cout << "---------------------------\n";
        current = current->next;
    }
}


// ===================== Utility Functions =====================
bool DoublyLinkedList::isValidRow(const Article& article) {
    if (article.title.empty() || article.text.empty() || article.subject.empty() || article.date.empty())
        return false;
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
bool DoublyLinkedList::compareArticles(Article* a, Article* b) const {
    if (a->year != b->year)
        return a->year < b->year;
    return a->month < b->month;
}

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

Article* DoublyLinkedList::mergeSortIterative(Article* head) {
    if (!head)
        return head;
    int n = 0;
    Article* curr = head;
    while (curr) {
        n++;
        curr = curr->next;
    }
    Article dummy;
    dummy.next = head;
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

void DoublyLinkedList::MergeSort(size_t& memoryUsed) {
    if (!head || !head->next) return;
    memoryUsed += sizeof(Article); // Dummy node in mergeSortIterative
    head = mergeSortIterative(head);
    Article* temp = head;
    while (temp->next) temp = temp->next;
    tail = temp;
}

int DoublyLinkedList::countArticles() const {
    return size;
}

// ===================== Bubble Sort =====================
void DoublyLinkedList::swapArticleData(Article* a, Article* b) {
    swap(a->title, b->title);
    swap(a->text, b->text);
    swap(a->subject, b->subject);
    swap(a->date, b->date);
    swap(a->year, b->year);
    swap(a->month, b->month);
}

void DoublyLinkedList::swapNodes(Article* a, Article* b) {
    if (a == b || !a || !b) return;

    // Handle adjacent nodes
    if (a->next == b) {
        Article* aPrev = a->prev;
        Article* bNext = b->next;

        if (aPrev) aPrev->next = b;
        else head = b;

        if (bNext) bNext->prev = a;
        else tail = a;

        b->next = a;
        a->prev = b;
        a->next = bNext;
        b->prev = aPrev;
    }
    else {
        return;
    }
}

void DoublyLinkedList::bubbleSort(size_t& memoryUsed) {
    if (!head || !head->next) return;
    bool swapped;
    Article* end = nullptr;
    do {
        swapped = false;
        Article* current = head;
        while (current->next != end) {
            if (!compareArticles(current, current->next)) {
                swapNodes(current, current->next);
                swapped = true;
                Article* temp = current;
                current = current->prev;
                if (!current->prev) head = current;
                if (!current->next->next) tail = current->next;
            }
            current = current->next;
        }
        end = current;
    } while (swapped);
    // Minimal extra memory (just pointers), no significant allocation
}



// ===================== Quick Sort =====================
Article* DoublyLinkedList::partition(Article* low, Article* high) {
    Article* pivot = high;
    Article* i = low->prev;
    for (Article* j = low; j != high; j = j->next) {
        if (compareArticles(j, pivot)) {
            i = (i == nullptr) ? low : i->next;
            swapArticleData(i, j);
        }
    }
    i = (i == nullptr) ? low : i->next;
    swapArticleData(i, high);
    return i;
}

void DoublyLinkedList::quickSortRec(Article* low, Article* high, size_t& memoryUsed) {
    if (high != nullptr && low != high && low != high->next) {
        memoryUsed += sizeof(Article*); // Stack frame for pointers
        Article* p = partition(low, high);
        quickSortRec(low, p->prev, memoryUsed);
        quickSortRec(p->next, high, memoryUsed);
    }
}

void DoublyLinkedList::quickSort(size_t& memoryUsed) {
    if (!head || !head->next) return;
    cout << "Sorting using Quick Sort..." << endl;
    quickSortRec(head, tail, memoryUsed);
}

// ===================== Search Function =====================

void DoublyLinkedList::searchArticles() {
    string searchType;
    cout << "\n=== Article Search ===" << endl;
    cout << "Search by: " << endl;
    cout << "1. Year" << endl;
    cout << "2. Month" << endl;
    cout << "3. Subject" << endl;
    cout << "4. Keyword in Title" << endl;
    cout << "5. Keyword in Text" << endl;
    cout << "Enter choice (1-5): ";
    getline(cin, searchType);

    if (searchType == "1") {
        int year;
        cout << "Enter year to search for: ";
        string yearStr;
        getline(cin, yearStr);
        try {
            year = stoi(yearStr);
        }
        catch (...) {
            cout << "Invalid year format. Please enter a number." << endl;
            return;
        }
        searchByYear(year);
    }
    else if (searchType == "2") {
        int month;
        cout << "Enter month (1-12) to search for: ";
        string monthStr;
        getline(cin, monthStr);
        try {
            month = stoi(monthStr);
            if (month < 1 || month > 12) {
                cout << "Month must be between 1 and 12." << endl;
                return;
            }
        }
        catch (...) {
            cout << "Invalid month format. Please enter a number between 1 and 12." << endl;
            return;
        }
        searchByMonth(month);
    }
    else if (searchType == "3") {
        string subject;
        cout << "Enter subject to search for: ";
        getline(cin, subject);
        searchBySubject(subject);
    }
    else if (searchType == "4") {
        string keyword;
        cout << "Enter keyword to search in titles: ";
        getline(cin, keyword);
        searchByKeywordInTitle(keyword);
    }
    else if (searchType == "5") {
        string keyword;
        cout << "Enter keyword to search in article text: ";
        getline(cin, keyword);
        searchByKeywordInText(keyword);
    }
    else {
        cout << "Invalid choice. Please try again." << endl;
    }
}

void DoublyLinkedList::searchByYear(int year) {
    int total = countArticles();
    Article** results = new Article * [total];
    int resultCount = 0;
    Article* current = head;
    while (current) {
        if (current->year == year)
            results[resultCount++] = current;
        current = current->next;
    }
    displaySearchResults(results, resultCount, "year", to_string(year));
    delete[] results;
}

void DoublyLinkedList::searchByMonth(int month) {
    int total = countArticles();
    Article** results = new Article * [total];
    int resultCount = 0;
    Article* current = head;
    while (current) {
        if (current->month == month)
            results[resultCount++] = current;
        current = current->next;
    }
    // Convert month number to month name.
    const string monthNames[] = { "January", "February", "March", "April", "May", "June",
                                  "July", "August", "September", "October", "November", "December" };
    string monthName = (month >= 1 && month <= 12) ? monthNames[month - 1] : "Unknown";
    displaySearchResults(results, resultCount, "month", monthName);
    delete[] results;
}

void DoublyLinkedList::searchBySubject(const string& subject) {
    int total = countArticles();
    Article** results = new Article * [total];
    int resultCount = 0;
    Article* current = head;
    while (current) {
        if (containsIgnoreCase(current->subject, subject))
            results[resultCount++] = current;
        current = current->next;
    }
    displaySearchResults(results, resultCount, "subject", subject);
    delete[] results;
}

void DoublyLinkedList::searchByKeywordInTitle(const string& keyword) {
    int total = countArticles();
    Article** results = new Article * [total];
    int resultCount = 0;
    Article* current = head;
    while (current) {
        if (containsIgnoreCase(current->title, keyword))
            results[resultCount++] = current;
        current = current->next;
    }
    displaySearchResults(results, resultCount, "title keyword", keyword);
    delete[] results;
}

void DoublyLinkedList::searchByKeywordInText(const string& keyword) {
    int total = countArticles();
    Article** results = new Article * [total];
    int resultCount = 0;
    Article* current = head;
    while (current) {
        if (containsIgnoreCase(current->text, keyword))
            results[resultCount++] = current;
        current = current->next;
    }
    displaySearchResults(results, resultCount, "text keyword", keyword);
    delete[] results;
}

void DoublyLinkedList::displaySearchResults(Article** results, int resultCount, const string& searchType, const string& searchTerm) {
    cout << "\n=== Search Results ===" << endl;
    cout << "Showing articles matching " << searchType << ": " << searchTerm << endl;
    cout << "Found " << resultCount << " matching articles" << endl << endl;
    if (resultCount > 0) {
        int displayCount = (resultCount < 20 ? resultCount : 20);
        for (int i = 0; i < displayCount; i++) {
            cout << "Title: " << results[i]->title << endl;
            cout << "Subject: " << results[i]->subject << endl;
            cout << "Date: " << results[i]->date << endl;
            string excerpt = results[i]->text.substr(0, 100);
            cout << "Text: " << excerpt << "..." << endl;
            cout << "------------------------" << endl;
        }
        if (resultCount > 20)
            cout << "... and " << (resultCount - 20) << " more results (showing first 20)" << endl;
    }
    else {
        cout << "No matching articles found." << endl;
    }
}

bool DoublyLinkedList::containsIgnoreCase(const string& source, const string& search) {
    string lowerSource = source;
    string lowerSearch = search;
    transform(lowerSource.begin(), lowerSource.end(), lowerSource.begin(), ::tolower);
    transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
    return (lowerSource.find(lowerSearch) != string::npos);
}
// ===================== Clone Function =====================
DoublyLinkedList* DoublyLinkedList::clone() const {
    DoublyLinkedList* newList = new DoublyLinkedList();
    Article* current = head;
    while (current) {
        Article* newArticle = new Article;
        newArticle->title = current->title;
        newArticle->text = current->text;
        newArticle->subject = current->subject;
        newArticle->date = current->date;
        newArticle->year = current->year;
        newArticle->month = current->month;
        newArticle->next = nullptr;
        newArticle->prev = nullptr;
        newList->InsertArticle(newArticle);
        current = current->next;
    }
    return newList;
}

// ===================== Linear search Functions =====================
int TrueMonthCounter[12] = { 0 }; //for storing true news for every month
int FakeMonthCounter[12] = { 0 }; //for storing fake news for every month

void DoublyLinkedList::AnalyseTrueArticles() {

    Article* search = head;
    while (search != nullptr) {

        string dateStr = search->date;
        istringstream iss(dateStr);
        string categoryStr = search->subject;


        string month_str, day_with_comma, year_str;
        if (iss >> month_str >> day_with_comma >> year_str) { //if the date column is fitting the format
            if (year_str == "2016" && categoryStr == "politicsNews") { //if the article is in 2016 and category is politicalNews
                TrueMonthMatching(month_str);
                search = search->next; //proceed to the next node
            }
            else {
                search = search->next; //proceed to the next node
            }
        }
        else {
            search = search->next; //pcoceed to the next node
        }
    }
    DisplayPercentage();
}

void DoublyLinkedList::TrueMonthMatching(const string& month_str) {
    if (month_str == "January" || month_str == "Jan") {
        TrueMonthCounter[0] += 1; //indicates January counter, 0+1
    }
    else if (month_str == "February" || month_str == "Feb") {
        TrueMonthCounter[1] += 1;
    }
    else if (month_str == "March" || month_str == "Mar") {
        TrueMonthCounter[2] += 1;
    }
    else if (month_str == "April" || month_str == "Apr") {
        TrueMonthCounter[3] += 1;
    }
    else if (month_str == "May") {
        TrueMonthCounter[4] += 1;
    }
    else if (month_str == "June" || month_str == "Jun") {
        TrueMonthCounter[5] += 1;
    }
    else if (month_str == "July" || month_str == "Jul") {
        TrueMonthCounter[6] += 1;
    }
    else if (month_str == "August" || month_str == "Aug") {
        TrueMonthCounter[7] += 1;
    }
    else if (month_str == "September" || month_str == "Sep") {
        TrueMonthCounter[8] += 1;
    }
    else if (month_str == "October" || month_str == "Oct") {
        TrueMonthCounter[9] += 1;
    }
    else if (month_str == "November" || month_str == "Nov") {
        TrueMonthCounter[10] += 1;
    }
    else if (month_str == "December" || month_str == "Dec") {
        TrueMonthCounter[11] += 1;
    }

}


void DoublyLinkedList::AnalyseFakeArticles() {

    Article* search = head;
    while (search != nullptr) {
        string dateStr = search->date;
        istringstream iss(dateStr);
        string categoryStr = search->subject;

        string monthVal, dayVal, yearVal;
        if (iss >> monthVal >> dayVal >> yearVal) { //if the date column is fitting the format;
            if (yearVal == "2016" && categoryStr == "politics") { //if the article is in 2016 and category is politics
                FakeMonthMatching(monthVal);
                search = search->next; //proceed to the next node
            }
            else {
                search = search->next; //proceed to the next node
            }
        }
        else {
            search = search->next; //pcoceed to the next node
        }
    }
}

void DoublyLinkedList::FakeMonthMatching(const string& month_str) {
    if (month_str == "January" || month_str == "Jan") {
        FakeMonthCounter[0] += 1; //indicates January counter, 0+1
    }
    else if (month_str == "February" || month_str == "Feb") {
        FakeMonthCounter[1] += 1;
    }
    else if (month_str == "March" || month_str == "Mar") {
        FakeMonthCounter[2] += 1;
    }
    else if (month_str == "April" || month_str == "Apr") {
        FakeMonthCounter[3] += 1;
    }
    else if (month_str == "May") {
        FakeMonthCounter[4] += 1;
    }
    else if (month_str == "June" || month_str == "Jun") {
        FakeMonthCounter[5] += 1;
    }
    else if (month_str == "July" || month_str == "Jul") {
        FakeMonthCounter[6] += 1;
    }
    else if (month_str == "August" || month_str == "Aug") {
        FakeMonthCounter[7] += 1;
    }
    else if (month_str == "September" || month_str == "Sep") {
        FakeMonthCounter[8] += 1;
    }
    else if (month_str == "October" || month_str == "Oct") {
        FakeMonthCounter[9] += 1;
    }
    else if (month_str == "November" || month_str == "Nov") {
        FakeMonthCounter[10] += 1;
    }
    else if (month_str == "December" || month_str == "Dec") {
        FakeMonthCounter[11] += 1;
    }
}

void DoublyLinkedList::DisplayPercentage() {
    // Display header

    cout << "\nPercentage of Fake Political News Articles in 2016\n" << endl;

    // Display monthly percentages
    const string months[] = { "January", "February", "March", "April", "May", "June",
                               "July", "August", "September", "October", "November", "December" };

    for (int i = 0; i < 12; i++)
    {
        double monthlyTotal = (TrueMonthCounter[i] + FakeMonthCounter[i]); //calculate the total news for each month
        double monthlyPercentage = (FakeMonthCounter[i] / monthlyTotal) * 100;  //find out the percentage of fake news in each month 

        cout << left << setw(10) << months[i] << " | ";

        // Print stars (each star represents roughly 1%)
        int stars = static_cast<int>(monthlyPercentage);
        for (int j = 0; j < stars; j++) {
            cout << "*";
        }

        // Print percentages 
        cout << " " << fixed << setprecision(1) << monthlyPercentage << "% \n";
    }
    cout << "\nNote: Each '*' represents approximately 1% of news articles that were fake political news.\n" << endl;
}

