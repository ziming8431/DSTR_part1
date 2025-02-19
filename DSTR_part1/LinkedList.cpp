#include "DoublyLinkedList.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cctype>
#include <cmath>    // for std::round
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

// ===================== Utility Functions =====================
bool DoublyLinkedList::isValidRow(const Article& article) {
    if (article.title.empty() || article.text.empty() || article.subject.empty() || article.date.empty())
        return false;
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
bool DoublyLinkedList::compareArticles(Article* a, Article* b) {
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

void DoublyLinkedList::sortByDate() {
    if (!head || !head->next)
        return;
    head = mergeSortIterative(head);
    Article* temp = head;
    while (temp->next)
        temp = temp->next;
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

void DoublyLinkedList::bubbleSort() {
    cout << "Starting Bubble Sort..." << endl;
    if (!head) return;
    bool swapped;
    int pass = 0;
    do {
        swapped = false;
        pass++;
        cout << "Pass " << pass << "..." << endl;
        Article* current = head;
        while (current && current->next) {
            if (!compareArticles(current, current->next)) {
                cout << "Swapping articles: \"" << current->title
                    << "\" with \"" << current->next->title << "\"" << endl;
                swapArticleData(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
    Article* temp = head;
    while (temp->next)
        temp = temp->next;
    tail = temp;
    cout << "Bubble Sort completed in " << pass << " passes." << endl;
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

void DoublyLinkedList::quickSortRec(Article* low, Article* high) {
    if (high != nullptr && low != high && low != high->next) {
        Article* p = partition(low, high);
        quickSortRec(low, p->prev);
        quickSortRec(p->next, high);
    }
}

void DoublyLinkedList::quickSort() {
    cout << "Starting Quick Sort..." << endl;
    quickSortRec(head, tail);
    cout << "Quick Sort completed." << endl;
}

// ===================== Search Function =====================
// ===================== Search Functions =====================

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

// ===================== New Helper Functions for Binary Search Analysis =====================
// ===================== toArray() and Binary Search =====================
Article** DoublyLinkedList::toArray() const {
    int n = countArticles(); // now allowed because countArticles() is const
    Article** arr = new Article * [n];
    Article* current = head;
    for (int i = 0; i < n; i++) {
        arr[i] = current;
        current = current->next;
    }
    return arr;
}

int DoublyLinkedList::lowerBound(Article** arr, int n, int targetYear) {
    int low = 0, high = n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid]->year < targetYear) {
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }
    return low;
}

int DoublyLinkedList::upperBound(Article** arr, int n, int targetYear) {
    int low = 0, high = n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid]->year <= targetYear) {
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }
    return low;
}

// ===================== calculatePoliticalFakePercentage2016() =====================
double DoublyLinkedList::calculatePoliticalFakePercentage2016(const DoublyLinkedList& trueList,
    const DoublyLinkedList& fakeList) {
    // Both lists must be sorted by date for the binary search to be valid
    int trueSize = trueList.countArticles();
    int fakeSize = fakeList.countArticles();

    Article** trueArr = trueList.toArray();
    Article** fakeArr = fakeList.toArray();

    // range of articles from 2016
    int lbTrue = lowerBound(trueArr, trueSize, 2016);
    int ubTrue = upperBound(trueArr, trueSize, 2016);
    int lbFake = lowerBound(fakeArr, fakeSize, 2016);
    int ubFake = upperBound(fakeArr, fakeSize, 2016);

    int politicalTrueCount = 0;
    for (int i = lbTrue; i < ubTrue; i++) {
        string subj = toLowercase(trueArr[i]->subject);
        if (subj == "politicsnews") {
            politicalTrueCount++;
        }
    }
    int politicalFakeCount = 0;
    for (int i = lbFake; i < ubFake; i++) {
        string subj = toLowercase(fakeArr[i]->subject);
        if (subj == "politics") {
            politicalFakeCount++;
        }
    }

    int totalPolitical = politicalTrueCount + politicalFakeCount;
    double percentage = 0.0;
    if (totalPolitical > 0) {
        percentage = (politicalFakeCount * 100.0) / totalPolitical;
    }

    delete[] trueArr;
    delete[] fakeArr;
    return percentage;
}

// ===================== printMonthlyFakePoliticalNewsPercentage2016() =====================
void DoublyLinkedList::printMonthlyFakePoliticalNewsPercentage2016(
    const DoublyLinkedList& trueList,
    const DoublyLinkedList& fakeList
) {
    // We'll track monthly totals for 2016:
    int monthlyTotal[12] = { 0 };
    int monthlyFake[12] = { 0 };

    // 1) Traverse True List (subject == "politicsNews")
    Article* cur = trueList.head;
    while (cur) {
        if (cur->year == 2016) {
            string subj = toLowercase(cur->subject);
            if (subj == "politicsnews") {
                int m = cur->month;
                if (m >= 1 && m <= 12) {
                    monthlyTotal[m - 1]++;
                }
            }
        }
        cur = cur->next;
    }

    // 2) Traverse Fake List (subject == "politics")
    cur = fakeList.head;
    while (cur) {
        if (cur->year == 2016) {
            string subj = toLowercase(cur->subject);
            if (subj == "politics") {
                int m = cur->month;
                if (m >= 1 && m <= 12) {
                    monthlyTotal[m - 1]++;
                    monthlyFake[m - 1]++;
                }
            }
        }
        cur = cur->next;
    }

    static const string MONTHS[12] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    cout << "\nPercentage of Fake Political News Articles in 2016\n\n";
    for (int i = 0; i < 12; i++) {
        int total = monthlyTotal[i];
        int fake = monthlyFake[i];

        double percentage = 0.0;
        if (total > 0) {
            percentage = (fake * 100.0) / total;
        }

        // Number of '*' = integer rounding of the percentage
        int starCount = static_cast<int>(std::round(percentage));
        string stars(starCount, '*');

        // Print line with one decimal place
        cout << left << setw(10) << MONTHS[i] << " | "
            << stars << " "
            << fixed << setprecision(1) << percentage << "%" << endl;
    }

    cout << "\nNote: Each '*' represents 1% of fake political news articles.\n";
}



// ===================== Define the static arrays =====================
int TrueMonthCounter[12] = { 0 };
int FakeMonthCounter[12] = { 0 };

// ===================== Reset counters =====================
void DoublyLinkedList::resetMonthCounters() {
    for (int i = 0; i < 12; i++) {
        TrueMonthCounter[i] = 0;
        FakeMonthCounter[i] = 0;
    }
}

// ===================== AnalyseTrueArticles() =====================
void DoublyLinkedList::AnalyseTrueArticles() {
    // We'll scan the entire list for articles from 2016 whose subject is "politicsNews".
    // For each match, we increment TrueMonthCounter based on the month.
    Article* search = head;
    while (search) {
        // We'll parse date as strings rather than using parseDate, just like your snippet
        istringstream iss(search->date);
        string month_str, day_with_comma, year_str;
        if (iss >> month_str >> day_with_comma >> year_str) {
            // year_str is e.g. "2016"
            // subject is e.g. "politicsNews"
            // Check year and subject
            if (year_str == "2016") {
                string lowerSubj = toLowercase(search->subject);
                if (lowerSubj == "politicsnews") {
                    TrueMonthMatching(month_str);
                }
            }
        }
        search = search->next;
    }
}

// ===================== TrueMonthMatching() =====================
void DoublyLinkedList::TrueMonthMatching(const string& month_str) {
    // Convert the input to lowercase to handle "January"/"Jan"
    string lowerMonth = toLowercase(month_str);

    if (lowerMonth == "january" || lowerMonth == "jan") { TrueMonthCounter[0]++; }
    else if (lowerMonth == "february" || lowerMonth == "feb") { TrueMonthCounter[1]++; }
    else if (lowerMonth == "march" || lowerMonth == "mar") { TrueMonthCounter[2]++; }
    else if (lowerMonth == "april" || lowerMonth == "apr") { TrueMonthCounter[3]++; }
    else if (lowerMonth == "may") { TrueMonthCounter[4]++; }
    else if (lowerMonth == "june" || lowerMonth == "jun") { TrueMonthCounter[5]++; }
    else if (lowerMonth == "july" || lowerMonth == "jul") { TrueMonthCounter[6]++; }
    else if (lowerMonth == "august" || lowerMonth == "aug") { TrueMonthCounter[7]++; }
    else if (lowerMonth == "september" || lowerMonth == "sep" || lowerMonth == "sept") {
        TrueMonthCounter[8]++;
    }
    else if (lowerMonth == "october" || lowerMonth == "oct") { TrueMonthCounter[9]++; }
    else if (lowerMonth == "november" || lowerMonth == "nov") { TrueMonthCounter[10]++; }
    else if (lowerMonth == "december" || lowerMonth == "dec") { TrueMonthCounter[11]++; }
}

// ===================== AnalyseFakeArticles() =====================
void DoublyLinkedList::AnalyseFakeArticles() {
    // We'll scan for articles from 2016 whose subject is "politics"
    Article* search = head;
    while (search) {
        istringstream iss(search->date);
        string monthVal, dayVal, yearVal;
        if (iss >> monthVal >> dayVal >> yearVal) {
            if (yearVal == "2016") {
                string lowerSubj = toLowercase(search->subject);
                if (lowerSubj == "politics") {
                    FakeMonthMatching(monthVal);
                }
            }
        }
        search = search->next;
    }
}

// ===================== FakeMonthMatching() =====================
void DoublyLinkedList::FakeMonthMatching(const string& month_str) {
    // Convert to lowercase to unify matching
    string lowerMonth = toLowercase(month_str);

    if (lowerMonth == "january" || lowerMonth == "jan") { FakeMonthCounter[0]++; }
    else if (lowerMonth == "february" || lowerMonth == "feb") { FakeMonthCounter[1]++; }
    else if (lowerMonth == "march" || lowerMonth == "mar") { FakeMonthCounter[2]++; }
    else if (lowerMonth == "april" || lowerMonth == "apr") { FakeMonthCounter[3]++; }
    else if (lowerMonth == "may") { FakeMonthCounter[4]++; }
    else if (lowerMonth == "june" || lowerMonth == "jun") { FakeMonthCounter[5]++; }
    else if (lowerMonth == "july" || lowerMonth == "jul") { FakeMonthCounter[6]++; }
    else if (lowerMonth == "august" || lowerMonth == "aug") { FakeMonthCounter[7]++; }
    else if (lowerMonth == "september" || lowerMonth == "sep" || lowerMonth == "sept") {
        FakeMonthCounter[8]++;
    }
    else if (lowerMonth == "october" || lowerMonth == "oct") { FakeMonthCounter[9]++; }
    else if (lowerMonth == "november" || lowerMonth == "nov") { FakeMonthCounter[10]++; }
    else if (lowerMonth == "december" || lowerMonth == "dec") { FakeMonthCounter[11]++; }
}

// ===================== DisplayPercentage() =====================
void DoublyLinkedList::DisplayPercentage() {
    cout << "\n=== Percentage of Fake Political News Articles in 2016 (Linear Search) ===\n" << endl;

    static const string months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    for (int i = 0; i < 12; i++) {
        int total = TrueMonthCounter[i] + FakeMonthCounter[i];
        double monthlyPercentage = 0.0;
        if (total > 0) {
            monthlyPercentage = (FakeMonthCounter[i] * 100.0) / total;
        }

        // Print month
        cout << left << setw(10) << months[i] << " | ";

        // Print stars
        int stars = static_cast<int>(monthlyPercentage + 0.5); // round
        for (int j = 0; j < stars; j++) {
            cout << "*";
        }

        // Print percentage with one decimal place
        cout << " " << fixed << setprecision(1) << monthlyPercentage << "%\n";
    }

    cout << "\nNote: Each '*' represents approximately 1% of fake political news.\n" << endl;
}