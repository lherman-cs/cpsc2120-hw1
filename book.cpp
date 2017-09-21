#include <fstream>
#include <iostream>
#include <string.h>
#include <assert.h>
#include "book.h"

using namespace std;

bool is_link(string s)
{
  string url_specifier = "http://";
  int specifier_len = url_specifier.size(), s_len = s.size(), j = 0;

  for (int i = 0; i < specifier_len; i++)
  {
    if (j >= s_len || url_specifier[i] != s[j++])
      return false;
  }
  return true;
}

Book::Book(const char *src)
{
  ifstream fin;
  string page, value;

  fin.open(src);
  while (fin >> value)
  {
    // Process the string s here...
    if (value == "NEWPAGE")
    {
      fin >> page;
      if (!book.find(page))
        book.insert(page, new Page());
    }
  }

  // Reset file pointer
  fin.clear();
  fin.seekg(0, ios::beg);

  while (fin >> value)
  {
    // Process the string s here...
    if (value == "NEWPAGE")
      fin >> page;
    else if (is_link(value))
    {
      if (book.find(value))
      {
        book[page]->link = new SetNode(value, book[page]->link);
        book[page]->num_links++;
      }
    }
    else
    {
      book[page]->word = new SetNode(value, book[page]->word);
      book[page]->num_words++;

      // Add word to the index table
      index_table[value] = new SetNode(page, index_table[value]);
    }
  }
  fin.close();
}

Book::~Book()
{
  // Delete book
  string *keys = book.keys();
  for (int i = 0; i < book.get_num_keys(); i++)
  {
    delete book[keys[i]];
  }
  delete[] keys;

  // Delete index_table
  keys = index_table.keys();
  for (int i = 0; i < index_table.get_num_keys(); i++)
  {
    delete index_table[keys[i]];
  }
  delete[] keys;
}

void Book::random_walk()
{
  int i, j, N = book.get_num_keys(), t;
  Page **values = book.values();
  Page *page;
  SetNode *cursor;

  // Give each page initial weight 1 / N
  for (i = 0; i < N; i++)
    values[i]->weight = 1 / double(N);

  for (j = 0; j < 50; j++)
  {
    for (i = 0; i < N; i++)
      values[i]->new_weight = 0.1 / double(N);
    for (i = 0; i < N; i++)
    {
      page = values[i];
      cursor = page->link;
      t = page->num_links;
      while (cursor != NULL)
      {
        book[cursor->value]->new_weight += 0.9 * page->weight / t;
        cursor = cursor->next;
      }
    }
    for (i = 0; i < N; i++)
      values[i]->weight = values[i]->new_weight;
  }

  delete[] values;
}

void Book::search(string word)
{
  SetNode *cursor = index_table[word];
  Page *page;
  double weight;
  int N = book.get_num_keys();
  while (cursor != NULL)
  {
    page = book[cursor->value];
    weight = page->weight * 100 * N;
    cout << weight << " " << cursor->value << endl;
    cursor = cursor->next;
  }
}

void Book::print()
{
  string *keys = book.keys();
  for (int i = 0; i < book.get_num_keys(); i++)
  {
    cout << book[keys[i]]->weight << endl;
  }
  delete[] keys;
}