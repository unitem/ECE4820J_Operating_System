#include <vector>
#include <string>
#include <iostream>

// 例2：std::vector和std::string的实际例子
int main() {
    std::string str1 = "aacasxs";
    std::vector<std::string> vec;
     
    vec.push_back(str1); // 传统方法，copy
    str1 = "asdasd";
    std::cout << str1 << std::endl;
    std::cout << vec[0] << std::endl;
    vec.push_back(std::move(str1)); // 调用移动语义的push_back方法，避免拷贝，str1会失去原有值，变成空字符串
    std::cout << str1 << std::endl;
    std::cout << vec[1] << std::endl;
    str1 = "sdfsdfsf";
    vec.emplace_back(std::move(str1)); // emplace_back效果相同，str1会失去原有值
    std::cout << str1 << std::endl;
    std::cout << vec[2] << std::endl;
    vec.emplace_back("axcsddcas"); // 当然可以直接接右值
}
