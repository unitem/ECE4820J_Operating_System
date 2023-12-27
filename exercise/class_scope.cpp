#include <iostream>

using namespace std;

class Test
{
    int x;

public:
    void setX(int i) { x = getX() + i; }
    int getX() { return x; }
    int getX2() const { return x; }
};

int main()
{
    Test obj;
    obj.setX(10);
    cout << obj.getX() << endl;
    cout << obj.getX2() << endl;
    return 0;
}