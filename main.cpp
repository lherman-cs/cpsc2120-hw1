#include <string>
#include <iostream>
#include "book.h"

using namespace std;

int main(void)
{
    string input;
    Book book = Book("webpages.txt");

    cout << endl
         << "Search: ";
    cin >> input;
    do
    {
        book.search(input);
        cout << endl
             << "Search: ";
    } while (cin >> input);

    return 0;
}