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

/* Load book from the source path given */
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

  size = pages.get_num_keys();
  book = new Page *[size];
  while (fin >> value)
  {
    // Process the string s here...
    if (value == "NEWPAGE")
    {
      fin >> page;
      lookup_id = pages[page];

      // Get the id from pages if it is set.
      // Else, load the prev_id and increment it
      if (lookup_id != -1)
        id = lookup_id;
      else
      {
        id = prev_id;
        book[++id] = new Page(page);
        prev_id = id;
      }
    }
    else if (is_link(value))
    {
      // Add the link to book if it goes to a valid webpage
      if (pages.find(value))
      {
        book[id]->link = new DictionaryNode<int>(
            value, pages[value], book[id]->link);

        book[id]->num_links++;
        pages[page] = id;
      }
    }
    // Add the word to book
    else
    {
      book[id]->word = new SetNode(value, book[id]->word);
      pages[page] = id;

      // Add the word to the inverted_index
      inverted_index[value] = new IdNode(id, inverted_index[value]);
    }
  }
  fin.close();
  random_walk();
}

Book::~Book()
{
  // Delete book
  for (int i = 0; i < this->size; i++)
  {
    if (book[i])
    {
      delete book[i];
    }
  }
  delete[] book;

  // Delete inverted_index
  string *keys = inverted_index.keys();
  for (int i = 0; i < inverted_index.get_num_keys(); i++)
  {
    delete inverted_index[keys[i]];
  }
  delete[] keys;
}

/* Stabilize probability by randomly walking down all the pages */
void Book::random_walk()
{
  int i, j, t, id;
  DictionaryNode<int> *cursor;

  // Give each page initial weight 1 / N
  for (i = 0; i < this->size; i++)
    book[i]->weight = 1 / double(this->size);

  // Repeat 50 times
  for (j = 0; j < 50; j++)
  {
    // Give each page initial new_weight 0.1 / N
    for (i = 0; i < this->size; i++)
      book[i]->new_weight = 0.1 / double(this->size);

    // Adjust each page's weight that is in the link list
    for (i = 0; i < this->size; i++)
    {
      cursor = book[i]->link;
      t = book[i]->num_links;
      while (cursor != NULL)
      {
        id = cursor->value;
        // If id is not set, lookup the table
        if (id == -1)
        {
          id = pages[cursor->key];
          cursor->value = id;
        }
        book[id]->new_weight += 0.9 * book[i]->weight / t;
        cursor = cursor->next;
      }
    }
    // Set each page's weight to the new weight
    for (i = 0; i < this->size; i++)
      book[i]->weight = book[i]->new_weight;
  }
}

/* Search word in the book and give the rank associated with it */
void Book::search(string word)
{
  IdNode *cursor = inverted_index[word];
  DictionaryNode<int> *weight_node = NULL, *weight_cursor, *del;
  Page *page;
  int weight;
  while (cursor != NULL)
  {
    page = book[cursor->id];
    weight = (int)(page->weight * 100 * size);
    weight_node = add(page->name, weight, weight_node);
    cursor = cursor->next;
  }

  weight_cursor = weight_node;
  while (weight_cursor != NULL)
  {
    cout << weight_cursor->value << " " << weight_cursor->key << endl;

    del = weight_cursor;
    weight_cursor = weight_cursor->next;
    delete del;
  }
}

/* 
  Helper to sort the weights. The performance can be significantly improved
  by using a binary tree. But, maybe for later :)
*/
DictionaryNode<int> *Book::add(string page, int weight,
                               DictionaryNode<int> *head)
{
  if (head == NULL || weight > head->value)
    return new DictionaryNode<int>(page, weight, head);
  head->next = add(page, weight, head->next);
  return head;
}