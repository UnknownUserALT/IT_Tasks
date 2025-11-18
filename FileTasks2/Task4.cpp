#include <iostream>
#include <fstream>
#include <vector>

int Task4() {
    int numbers[] = {15, 23, 7, 42, 18, 91, 3, 56, 77, 10};
    const int count = sizeof(numbers) / sizeof(numbers[0]);
    
    std::ofstream outFile("../numbers.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
        return 1;
    }
    
    outFile.write(reinterpret_cast<const char*>(numbers), sizeof(numbers));
    outFile.close();
    
    std::cout << "Массив записан в файл numbers.bin" << std::endl;
    
    std::ifstream inFile("../numbers.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Ошибка открытия файла для чтения!" << std::endl;
        return 1;
    }
    
    inFile.seekg(0, std::ios::end);
    size_t file_size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    
    std::cout << "Размер файла: " << file_size << " байт" << std::endl;
    
    std::vector<int> buffer(count);
    
    inFile.read(reinterpret_cast<char*>(buffer.data()), file_size);
    inFile.close();
    
    std::cout << "Прочитанные числа: ";
    int sum = 0;
    for (int i = 0; i < count; ++i) {
        std::cout << buffer[i];
        if (i < count - 1) {
            std::cout << ", ";
        }
        sum += buffer[i];
    }
    std::cout << std::endl;
    
    std::cout << "Сумма всех чисел: " << sum << std::endl;
    
    return 0;
}