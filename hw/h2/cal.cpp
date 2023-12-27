#include <iostream>
#include <cmath>

using namespace std;

int main() {
    int add_mem;

    cout << "Please input the memory you want to add: ";

    cin >> add_mem;

    int mem = 256 + add_mem - 96;

    int num_process = floor(mem / 48);

    cout << "The number of processes that can be run is: " << num_process << endl;

    double cpu_util = 1 - pow(0.9, num_process);

    cout << "The CPU utilization is: " << cpu_util << endl;

    return 0;
}