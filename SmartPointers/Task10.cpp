#include <iostream>
#include <memory>

std::unique_ptr<int[]> createArray(int n) {
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = i + 1;
    }
    return arr;
}

int Task10() {
    int n = 10;
    std::unique_ptr<int[]> array = createArray(n);
    
    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}