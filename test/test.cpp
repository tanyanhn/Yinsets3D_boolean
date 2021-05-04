
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>
#include "../src/Triangle.h"
using namespace std;
using namespace YSB;

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

    /**
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
    **/

    /**
    Triangle<Real, 3> tri;
    cout << "tri's identity :" << tri.id() << endl;

    tri.id() += 11;
    cout << "tri's identity :" << tri.id() << endl;

    const Triangle<Real, 3> ctri;
    cout << "ctri's identity :" << ctri.id() << endl;

    ctri.id(21 - ctri.id());
    cout << "ctri's identity :" << ctri.id() << endl;
    **/

    // map<int, vector<int>> m;
    // m.insert({4, vector<int>(1, 3)});

    // int size = 10, innersize = size * 100;
    // vector<map<int, int>> vecmap(size);
    // for (int i = 0; i < size; ++i)
    // {
    //     for (int j = 0; j < innersize; ++j)
    //     {
    //         vecmap[i].insert({j, j});
    //     }
    // }

    // for (int i = 0; i < size; ++i)
    // {
    //     for (auto &&it : vecmap[i])
    //     {
    //         // it.second = -2;
    //     }
    // }

    // for (int i = 0; i < size; i += 3)
    // {
    //     auto it = vecmap[i].begin();
    //     for (int j = 0; j < innersize; j += 100)
    //     {
    //         cout << "vecmap :" << i << ", " << j << " = " << it->second << endl;
    //         advance(it, 100);
    //     }
    // }

    // vector<int> v0, v1;
    // for (int i = 0; i < 10; ++i)
    // {
    //     v0.push_back(i * -1);
    //     v1.push_back(i);
    // }

    // vector<int> *pv[2] = {&v0, &v1};

    // for (int i = 0; i < 10; ++i)
    // {
    //     pv[0]->operator[](i) *= 3;
    //     pv[1]->operator[](i) *= 11;
    // }

    // for (int i = 0; i < 10; ++i)
    // {
    //     cout << "v0[i] : " << v0[i] << ", v1[i] : " << v1[i] << endl;
    // }

    // map<pair<int, int>, double> m;

    // for (int i = 0; i < 10; ++i)
    // {
    //     pair<int, int> id({i, 2 * i});
    //     m[id] = i;
    // }

    // for (auto it = m.begin(); it != m.end(); ++it)
    // {
    //     if (int(it->second) % 2 == 1)
    //         m.erase(it);
    // }

    // int i = 0;
    // for (auto &&it : m)
    // {
    //     pair<int, int> id({i, 2 * i});
    //     cout << "id : " << id.first << " : " << it.second << endl;
    //     ++i;
    // }

    // int row, column;
    // cin >> row;

    // vector<vector<Triangle<Real, 3>>> a(row); //row决定最里面层容器大小，vector<int>(column)决定外层容器的类型和大小
    // for (int k = 0; k < row; k++)
    //     a[k].reserve(1); //row*row矩阵
    //使用空间
    // for (int j = 0; j < row; j++)
    //     for (int k = 0; k < row; k++)
    //         a[j].push_back(rand() % 100);
    // int c = 1;
    // for (auto &&v : a)
    // {
    //     c++;
    //     for (int k = 0; k < row; k++)
    //     {
    //         Real r[3] = {1.0 * k * c, 1.0 * k, 1.0 * k};
    //         vector<Point<Real, 3>> p(3, Point<Real, 3>(r));
    //         v.emplace_back(p.data());
    //     }
    // }

    // for (int j = 0; j < 2; j++)
    // {
    //     cout << endl
    //          << endl
    //          << "\n";
    //     for (int k = 0; k < row; k++)
    //     {
    //         cout << a[j][k].vert(0)[0] << "     ";
    //     }
    // }

    // map<int, int> v;
    // for (auto i = 0; i < 10; ++i)
    //     v[i] = i * i;

    // for (auto it = v.begin(); it != v.end(); ++it)
    // {
    //     if (it->first == 3)
    //     {
    //         v.erase(it);
    //         v.insert({3, 21});
    //     }
    //     if (it->first == 4)
    //     {
    //         v.erase(it);
    //         v.insert({4, 22});
    //     }
    // }

    // for (auto &&it : v)
    // {
    //     if (it.first == 5)
    //     {
    //         v.erase(5);
    //     }

    //     cout << it.second << "  ";
    // }

    // map<int, vector<int>> m;
    // for (auto i = 0; i < 10; ++i)
    //     m[i].push_back(i * i);

    // cout << endl
    //      << m[1].size() << endl;

    cout << ((0) ? (-1) : 1) << endl;
    cout << ((1) ? (-1) : 1) << endl;

    return 0;
}