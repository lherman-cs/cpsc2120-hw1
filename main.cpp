#include <string>
#include <iostream>
#include "book.h"

using namespace std;

int main(void)
{
    Book book = Book("webpages.txt");

    book.random_walk();
    book.search("olympiad");

    return 0;
}