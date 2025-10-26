#include <iostream>
#include <vector>

int SumOfVectorValues() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    int* p = &vec[0];
    int sum = 0;
    
    for (size_t i = 0; i < vec.size(); i++) {
        sum += *(p + i);
    }
    
    std::cout << "Сумма: " << sum << std::endl;
    
    return 0;
}