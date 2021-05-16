#include <iostream>
#include <vector>

using namespace std;

// clear vector with computational complexity O(1)
template <typename T>
void clear_container(vector<T> &vec)
{
    delete &vec;
    vec = vector<T>(0);
}

int main()
{
    vector<int> vec = {1, 2, 3, 4, 5};

    clear_container(vec);

    if(vec.empty())
        cout << "Empty!" << endl;

    return 0;
}
