#include <fstream>
#include <iostream>
#include <string.h>
#include "book.h"

using namespace std;

/* Load book from the source path given */
Book::Book(const char *src)
{
  ifstream fin;
  string page, value;
  int prev_id = -1, id = -1, lookup_id = -1,
      cntr = 0, i = 0;
  char loading[] = {'/', '-', '\\', '|'};

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

  // Loading starts
  cout << "Loading... " << loading[0] << flush;

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
    else if (pages.find(value))
    {
      // Add the link to book if it goes to a valid webpage
      book[id]->link = new DictionaryNode<int>(
          value, pages[value], book[id]->link);

      book[id]->num_links++;
      pages[page] = id;
    }
    // Add the word to book
    else
    {
      book[id]->word = new SetNode(value, book[id]->word);
      pages[page] = id;

      // Add the word to the inverted_index
      inverted_index[value] = new IdNode(id, inverted_index[value]);
    }

    // Loading bar logic
    if (cntr % 250000 == 0)
    {
      i++;
      cout << '\b' << loading[i % 4] << flush;
    }
    cntr++;
  }

  fin.close();
  random_walk();
  cout << '\b' << "done" << flush;
  cout << endl;
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

  // Delete BST caches
  keys = cache.keys();
  for (int i = 0; i < cache.get_num_keys(); i++)
  {
    delete_tree(cache[keys[i]]);
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

/* 
  Search word in the book and give the rank associated with it.
  The word then will be stored in cache to siginificantly improve
  the performance in the afterward search.
*/
void Book::search(string word)
{
  BSTNode *head;

  if (cache.find(word))
    head = cache[word];

  else
  {
    IdNode *cursor = inverted_index[word];
    head = NULL;
    Page *page;
    int weight;
    while (cursor != NULL)
    {
      page = book[cursor->id];
      weight = (int)(page->weight * 100 * size);
      head = insert(page->name, weight, head);
      cursor = cursor->next;
    }
    cache.insert(word, head);
  }

  print_in_reverse_order(head);
}

/*
  insert, print_in_reverse_order, and delete_tree are helpers 
  using Binary search tree to reverse sort the output.
*/
BSTNode *Book::insert(string page, int weight, BSTNode *head)
{
  if (head == NULL)
    return new BSTNode(page, weight);

  if (weight < head->weight)
    head->left = insert(page, weight, head->left);
  else
    head->right = insert(page, weight, head->right);

  return head;
}

void Book::print_in_reverse_order(BSTNode *head)
{
  if (head == NULL)
    return;

  print_in_reverse_order(head->right);
  cout << head->weight << " " << head->page << endl;
  print_in_reverse_order(head->left);
}

void Book::delete_tree(BSTNode *head)
{
  if (head != NULL)
  {
    delete_tree(head->left);
    delete_tree(head->right);
    delete head;
  }
}