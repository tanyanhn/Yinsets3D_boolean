
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

// main() section
int main()
{
    // vector<pair<int, int>> vec;
    // for (int i = 0; i < 10; ++i)
    // {
    //     vec.push_back(make_pair(i, 2 * i));
    // }

    // auto beg = vec.begin(), end = vec.end();

    // end = remove(beg, end, pair<int, int>(4, 8));

    // for_each(vec.begin(), vec.end(), [](pair<int, int> p) { cout << " ( " << p.first << " ," << p.second << " )" << endl; });

    // vector<pair<int, int>>::iterator &&e = vec.end();

    // vector<pair<int, int>> &rVec = vec;

    // if (e == rVec.end())
    // {
    //     cout << " e == vec.end() \n";
    // }

    // pair<int, int> &rPair = rVec[6];

    // rPair.second = 12;

    // for_each(vec.begin(), vec.end(), [](pair<int, int> p) { cout << " ( " << p.first << " ," << p.second << " )" << endl; });

    vector<vector<vector<pair<int, int>>>> vvVec;

    int size = 5;
    vvVec.resize(size);
    for (int i = 0; i < size; ++i)
    {
        vvVec[i].resize(size);
        for (int j = 0; j < size; ++j)
        {
            vvVec[i][j].resize(size);
            for (int k = 0; k < size; ++k)
            {
                vvVec[i][j][k].first = j;
                vvVec[i][j][k].second = k;
            }
        }
    }

    auto &rvVec = vvVec[3];
    auto &rrVec = rvVec[4];
    auto &rPair = rrVec[2];

    rPair = make_pair(10, 31);

    // for (int i = 0; i < size; ++i)
    // {
    int i = 3;
    for (int j = 0; j < size; ++j)
    {
        for (int k = 0; k < size; ++k)
        {
            auto &p = vvVec[i][j][k];
            cout << " ( " << p.first << " ," << p.second << " )" << endl;
        }
    }
    // }

    return 0;
}