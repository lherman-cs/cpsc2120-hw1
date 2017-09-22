#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>

using namespace std;

struct IdNode
{
  int id;
  IdNode *next;
  IdNode(int id, IdNode *next)
  {
    this->id = id;
    this->next = next;
  }
  IdNode()
  {
    this->id = 0;
    this->next = NULL;
  }
  ~IdNode()
  {
  }
  IdNode(const IdNode &rhs)
  {
    id = rhs.id;
    next = rhs.next;
  }
};

struct SetNode
{
  string value;
  SetNode *next;
  SetNode(string value, SetNode *next)
  {
    this->value = value;
    this->next = next;
  }
  SetNode()
  {
    this->value = "";
    this->next = NULL;
  }
  ~SetNode()
  {
  }
  SetNode(const SetNode &rhs)
  {
    value = rhs.value;
    next = rhs.next;
  }
};

template <typename T>
struct DictionaryNode
{
  string key;
  T value;
  DictionaryNode *next;
  DictionaryNode(string k, const T &v, DictionaryNode *n)
      : key(k), value(v), next(n)
  {
  }

  DictionaryNode()
      : key(""), value(T()), next(NULL)
  {
  }
  ~DictionaryNode()
  {
  }
  DictionaryNode(const DictionaryNode<T> &rhs)
  {
    key = rhs.key;
    value = rhs.value;
    next = rhs.next;
  }
};

#endif