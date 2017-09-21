#ifndef DICTIONARY_H /* Prevent multiple inclusion... */
#define DICTIONARY_H

#include <iostream>
#include <string>
#include "node.h"

using namespace std;

template <typename T>
class Dictionary
{

private:
  int size;
  int num_keys;
  DictionaryNode<T> **table;

  int hash(string s, int size);
  DictionaryNode<T> **allocate(int size);
  void scale_up();

public:
  Dictionary();
  Dictionary(int size);
  ~Dictionary();
  void insert(string key, const T &value);
  bool find(string key);
  T &operator[](string key);
  string *keys();
  T *values();
  int get_num_keys();
  int get_size();
  void print();
};

#endif

/* Default constructor */
template <typename T>
Dictionary<T>::Dictionary()
{
  this->size = 1024;
  this->num_keys = 0;
  this->table = allocate(this->size);
}

/* Allocate dictionary with specified size */
template <typename T>
Dictionary<T>::Dictionary(int size)
{
  this->size = size;
  this->num_keys = 0;
  this->table = allocate(this->size);
}

/* Deallocate all allocated memory from this instance */
template <typename T>
Dictionary<T>::~Dictionary()
{
  for (int i = 0; i < size; i++)
  {
    DictionaryNode<T> *cursor = table[i];
    while (cursor != NULL)
    {
      DictionaryNode<T> *del = cursor;
      cursor = cursor->next;
      delete del;
    }
  }
  delete[] table;
}

/* Hash string to an int */
template <typename T>
int Dictionary<T>::hash(string s, int size)
{
  int x = 7;
  unsigned int i, h = 0;
  for (i = 0; i < s.length(); i++)
    h = (h * x + s[i]) % size;
  return h;
}

/* Allocate a dictionary of pointers to nodes, all initialized to NULL */
template <typename T>
DictionaryNode<T> **Dictionary<T>::allocate(int size)
{
  DictionaryNode<T> **new_table = new DictionaryNode<T> *[size];
  for (int i = 0; i < size; i++)
    new_table[i] = NULL;
  return new_table;
}

/* Scale up dictionary by two. */
template <typename T>
void Dictionary<T>::scale_up()
{
  int new_size = size * 2;
  DictionaryNode<T> **new_table = allocate(new_size);
  for (int i = 0; i < this->size; i++)
  {
    DictionaryNode<T> *cursor = table[i];
    while (cursor != NULL)
    {
      int new_h = hash(cursor->key, new_size);
      new_table[new_h] = new DictionaryNode<T>(
          cursor->key,
          cursor->value,
          new_table[new_h]);

      DictionaryNode<T> *del = cursor;
      cursor = cursor->next;
      delete del;
    }
  }
  delete[] this->table;
  this->table = new_table;
  this->size = new_size;
}

/* Insert value to dictionary. */
template <typename T>
void Dictionary<T>::insert(string key, const T &value)
{
  if (num_keys == size)
    scale_up();

  int h = hash(key, size);

  if (find(key))
    table[h]->value = value;
  else
  {
    table[h] = new DictionaryNode<T>(key, value, table[h]);
    num_keys++;
  }
}

/* Return true if key exists in dictionary */
template <typename T>
bool Dictionary<T>::find(string key)
{
  int h = hash(key, size);
  for (DictionaryNode<T> *cursor = table[h]; cursor != NULL;
       cursor = cursor->next)
  {
    if (cursor->key == key)
      return true;
  }
  return false;
}

/* Overload dict[key] */
template <typename T>
T &Dictionary<T>::operator[](string key)
{
  int h = hash(key, size);
  for (DictionaryNode<T> *cursor = table[h]; cursor != NULL;
       cursor = cursor->next)
  {
    if (cursor->key == key)
      return cursor->value;
  }

  insert(key, T());
  return table[hash(key, size)]->value;
}

/* Return all the keys */
template <typename T>
string *Dictionary<T>::keys()
{
  string *keys = new string[this->num_keys];
  int j = 0, i;
  DictionaryNode<T> *cursor;

  for (i = 0; i < this->size; i++)
  {
    cursor = table[i];
    while (cursor)
    {
      keys[j++] = cursor->key;
      cursor = cursor->next;
    }
  }
  return keys;
}

/* Return all the values */
template <typename T>
T *Dictionary<T>::values()
{
  T *values = new T[this->num_keys];
  int j = 0, i;
  DictionaryNode<T> *cursor;

  for (i = 0; i < this->size; i++)
  {
    cursor = table[i];
    while (cursor)
    {
      values[j++] = cursor->value;
      cursor = cursor->next;
    }
  }
  return values;
}

/* Gettter for num_keys */
template <typename T>
int Dictionary<T>::get_num_keys()
{
  return this->num_keys;
}

/* Getter for size */
template <typename T>
int Dictionary<T>::get_size()
{
  return this->size;
}

/* Print all the keys */
template <typename T>
void Dictionary<T>::print()
{
  for (int i = 0; i < size; i++)
  {
    DictionaryNode<T> *cursor = table[i];
    while (cursor)
    {
      cout << cursor->key << endl;
      cursor = cursor->next;
    }
  }
}