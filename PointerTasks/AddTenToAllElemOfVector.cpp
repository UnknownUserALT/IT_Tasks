#include <iostream>
#include <vector>

int AddTenToAllElem() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    int* p = vec.data();
    
    for (size_t i = 0; i < vec.size(); i++) {
        *(p + i) += 10;
    }
    
    std::cout << "Элементы после увеличения: ";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}