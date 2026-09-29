#include <iostream>
#include <fstream>
#include "array.h"

using namespace std;

Array *array_create_and_read(ifstream &input) 
{
    int n;
    input >> n;
    Array *arr = array_create(n);

    for (int i = 0; i < n; ++i) 
    {
        Data x;
        input >> x;
        array_set(arr, i, x);
    }

    return arr;
}

Data factorial(Data n)
{
    Data result = 1;
    for (Data i = 2; i <= n; ++i)
        result *= i;
    return result;
}

void task1(Array *arr) 
{
    size_t n = array_size(arr);

    for (size_t i = 0; i < n; ++i)
    {
        Data value = array_get(arr, i);
        array_set(arr, i, factorial(value));
    }

    for (size_t i = 0; i < n; ++i)
    {
        cout << array_get(arr, i) << "\n";
    }
}

int main(int argc, char **argv) 
{
    if (argc < 2)
    {
        cerr <<"Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    Array *arr = NULL;
    ifstream input(argv[1]);

    arr = array_create_and_read(input);
    task1(arr);
    array_delete(arr);
    
    return 0;
}