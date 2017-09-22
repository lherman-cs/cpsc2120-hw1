#include <string>
#include <iostream>
#include "book.h"

using namespace std;

int main(void)
{
    string input;
    Book book = Book("webpages.txt");

    while (cin >> input)
    {
        book.search(input);
    }

    return 0;
}