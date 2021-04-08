
#include <iostream>
#include <cmath>
using namespace std;

// main() section
int main()
{
    float x;
    float y;

    x = -1.0;
    y = -2.5;
    cout << "atan2(" << y << "," << x << "): " << atan2(y, x) << endl;

    x = 11.0;
    y = 22.5;
    cout << "atan2(" << y << "," << x << "): " << atan2(y, x) << endl;

    for (int i = 0; i < 0; ++i)
    {
        x = -4.0;
        y = 0;
        cout << "atan2(" << y << "," << x << "): " << atan2(y, x) << endl;
    }

    return 0;
}