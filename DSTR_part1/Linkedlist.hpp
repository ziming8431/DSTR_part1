#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>


using namespace std;

struct Article{
	string title;
	string content;
	string subject;
	string date;
};

struct Node {
	Article article;
	Node* nextaddress;
};

class LinkedList {
private:
	Node* head = nullptr;
	Node* tail = nullptr;
	int size;

public:
	LinkedList();
	~LinkedList();
	// function declarations
	Article* createArticle(string title, string content, string subject, string date);
	void InsertArticle(Article article);
	void loadFromCSV(string filename);
	void displayArticles();
	void sortByDate();
	int countArticles();
	void searchFakeNews();
};
#endif