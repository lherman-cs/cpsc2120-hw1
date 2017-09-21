#ifndef BOOK_H /* Prevent multiple inclusion... */
#define BOOK_H

#include "dictionary.h"
#include "page.h"
#include "node.h"

using namespace std;

class Book
{

private:
  Dictionary<Page *> book;
  Dictionary<SetNode *> index_table;

public:
  Book(const char *src);
  ~Book();
  void random_walk();
  void search(string word);
  void print();
};

#endif
