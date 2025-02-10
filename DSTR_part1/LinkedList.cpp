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
	Node* current = head;
	while (current) {
		Node* temp = current;
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
	return newArticle;
}

void LinkedList::InsertArticle(Article article) {
	Node* newnode = new Node;

}



