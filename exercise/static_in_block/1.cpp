#include <iostream>

using namespace std;

thread_local int y;

int test1(void) {
    static int x = 10;
    y = 10;    
    return x--;
}

int main() {
    for (int i = 0; i < 10; i++)
    {
        // cout << x << endl;
        cout << test1() << endl;
        cout << y << endl;
    }
    return 0;
}