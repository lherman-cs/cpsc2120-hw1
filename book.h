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
  Dictionary<BSTNode *> cache;
  int size;
  BSTNode *insert(string page, int weight, BSTNode *head);
  void print_in_reverse_order(BSTNode *head);
  void delete_tree(BSTNode *head);
  void random_walk();

public:
  Book(const char *src);
  ~Book();
  void search(string word);
  void print();
};

#endif
