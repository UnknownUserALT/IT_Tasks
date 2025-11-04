#include <iostream>
#include <memory>
#include <vector>

void processA(std::shared_ptr<std::vector<int>> buf) {
    for (auto& element : *buf) {
        element += 1;
    }
}

void processB(std::shared_ptr<std::vector<int>> buf) {
    for (auto& element : *buf) {
        element *= 2;
    }
}

int Task9() {
    std::shared_ptr<std::vector<int>> buffer = std::make_shared<std::vector<int>>();
    
    for (int i = 1; i <= 10; ++i) {
        buffer->push_back(i);
    }
    
    processA(buffer);
    processB(buffer);
    
    for (const auto& element : *buffer) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    return 0;
}