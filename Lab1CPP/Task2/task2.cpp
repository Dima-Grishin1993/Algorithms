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

const Data kMaxValue = 1000;

void task2(Array *arr)
{
    size_t n = array_size(arr);

    Data counts[kMaxValue + 1] = {};

    for (size_t i = 0; i < n; ++i)
    {
        Data value = array_get(arr, i);

        if (value > kMaxValue)
        {
            cerr << "Value out of range [0, " << kMaxValue << "]: "
                 << value << "\n";
            return;
        }

        ++counts[value];
    }

    for (size_t value = 0; value <= kMaxValue; ++value)
    {
        if (counts[value] == 2)
        {
            cout << value << "\n";
        }
    }
}

int main(int argc, char **argv) 
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    Array *arr = NULL;
    ifstream input(argv[1]);

    arr = array_create_and_read(input);
    task2(arr);
    array_delete(arr);
    
    return 0;
}