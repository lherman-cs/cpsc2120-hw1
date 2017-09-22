#ifndef BOOK_H /* Prevent multiple inclusion... */
#define BOOK_H

#include "dictionary.h"
#include "page.h"
#include "node.h"

using namespace std;

class Book
{

private:
  Page **book;
  Dictionary<int> pages;
  Dictionary<IdNode *> inverted_index;
  int size;
  DictionaryNode<int> *add(string page, int weight,
                           DictionaryNode<int> *head);

public:
  Book(const char *src);
  ~Book();
  void random_walk();
  void search(string word);
  void print();
};

#endif
