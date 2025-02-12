#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>


using namespace std;

struct Article {
	string title;
	string content;
	string subject;
	string date;
	Article* nextaddress;
};

class LinkedList {
private:
	Article* head = nullptr;
	Article* tail = nullptr;
	int size = 0;

public:
	LinkedList();
	~LinkedList();
	// function declarationsjjjj
	Article* createArticle(string title, string content, string subject, string date);
	void InsertArticle(Article* article); //insert into linked list
	void loadFromCSV(string filename);
	void displayArticles();
	void sortByDate();
	int countArticles();
	void searchFakeNews();
};
#endif