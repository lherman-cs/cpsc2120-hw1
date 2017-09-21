#ifndef PAGE_H
#define PAGE_H

#include "node.h"

using namespace std;

struct Page
{
    SetNode *link;
    SetNode *word;
    double weight;
    double new_weight;
    int num_links;
    int num_words;

    Page()
    {
        link = NULL;
        word = NULL;
        weight = 0.0;
        new_weight = 0.0;
        num_links = 0;
        num_words = 0;
    }

    Page(const Page &rhs)
    {
        num_links = 0;
        num_words = 0;

        for (SetNode *cursor = rhs.link; cursor; cursor = cursor->next)
        {
            link = new SetNode(cursor->value, link);
            num_links++;
        }
        for (SetNode *cursor = rhs.word; cursor; cursor = cursor->next)
        {
            word = new SetNode(cursor->value, word);
            num_words++;
        }

        weight = rhs.weight;
        new_weight = rhs.new_weight;
        num_links = rhs.num_links;
    }

    ~Page()
    {
        SetNode *cursor_link = link;
        while (cursor_link != NULL)
        {
            SetNode *del = cursor_link;
            cursor_link = cursor_link->next;
            delete del;
        }

        SetNode *cursor_word = word;
        while (cursor_word != NULL)
        {
            SetNode *del = cursor_word;
            cursor_word = cursor_word->next;
            delete del;
        }
    }
};

#endif