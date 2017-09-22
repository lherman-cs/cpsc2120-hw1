#include <fstream>
#include <iostream>
#include <string.h>
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
  int prev_id = -1, id = -1, lookup_id = -1;

  fin.open(src);
  while (fin >> value)
  {
    // Process the string s here...
    if (value == "NEWPAGE")
    {
      fin >> page;
      pages.insert(page, -1);
    }
  }

  // Reset file pointer
  fin.clear();
  fin.seekg(0, ios::beg);

  this->size = pages.get_num_keys();
  book = new Page *[this->size];
  while (fin >> value)
  {
    // Process the string s here...
    if (value == "NEWPAGE")
    {
      fin >> page;
      lookup_id = pages[page];
      if (lookup_id != -1)
        id = lookup_id;
      else
      {
        id = prev_id;
        book[++id] = new Page();
        book[id]->name = page;
        prev_id = id;
      }
    }
    else if (is_link(value))
    {
      if (pages.find(value))
      {
        book[id]->link = new DictionaryNode<int>(value, pages[value], book[id]->link);
        book[id]->num_links++;
        pages[page] = id;
      }
    }
    else
    {
      book[id]->word = new SetNode(value, book[id]->word);
      book[id]->num_words++;
      pages[page] = id;

      // Add word to the inverted_index
      inverted_index[value] = new IdNode(id, inverted_index[value]);
    }
  }
  fin.close();
}

Book::~Book()
{
  for (int i = 0; i < this->size; i++)
  {
    if (book[i])
    {
      delete book[i];
    }
  }
  delete[] book;

  // // Delete book
  // for (int i = 0; i < book.get_num_keys(); i++)
  // {
  //   delete book[keys[i]];
  // }
  // delete[] keys;

  // Delete inverted_index
  string *keys = inverted_index.keys();
  for (int i = 0; i < inverted_index.get_num_keys(); i++)
  {
    delete inverted_index[keys[i]];
  }
  delete[] keys;
}

void Book::random_walk()
{
  int i, j, t, id;
  DictionaryNode<int> *cursor;

  // Give each page initial weight 1 / N
  for (i = 0; i < this->size; i++)
    book[i]->weight = 1 / double(this->size);

  for (j = 0; j < 50; j++)
  {
    for (i = 0; i < this->size; i++)
      book[i]->new_weight = 0.1 / double(this->size);
    for (i = 0; i < this->size; i++)
    {
      cursor = book[i]->link;
      t = book[i]->num_links;
      while (cursor != NULL)
      {
        id = cursor->value;
        if (id == -1)
        {
          id = pages[cursor->key];
          cursor->value = id;
        }
        book[id]->new_weight += 0.9 * book[i]->weight / t;
        cursor = cursor->next;
      }
    }
    for (i = 0; i < this->size; i++)
      book[i]->weight = book[i]->new_weight;
  }
}

void Book::search(string word)
{
  IdNode *cursor = inverted_index[word];
  Page *page;
  int weight;
  while (cursor != NULL)
  {
    page = book[cursor->id];
    weight = (int)(page->weight * 100 * size);
    cout << weight << " " << page->name << endl;
    cursor = cursor->next;
  }
}

void Book::print()
{
  for (int i = 0; i < size; i++)
  {
    cout << book[i]->weight << endl;
  }
}