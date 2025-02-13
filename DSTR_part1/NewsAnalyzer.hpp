#ifndef NEWSANALYZER_HPP
#define NEWSANALYZER_HPP

#include <string>
#include "DoublyLinkedList.hpp"

using namespace std;


class NewsAnalyzer {
private:
	DoublyLinkedList& trueNews;
	DoublyLinkedList& fakeNews;

public:
	NewsAnalyzer(DoublyLinkedList& trueNewsList, DoublyLinkedList& fakeNewsList);
	void calculateFakeNews();
};

#endif