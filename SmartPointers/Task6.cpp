#include <iostream>
#include <memory>

int Task6() {
    // Создаем shared_ptr
    std::shared_ptr<int> ptr1 = std::make_shared<int>(42);
    std::cout << "После создания ptr1:" << std::endl;
    std::cout << "use_count() = " << ptr1.use_count() << std::endl;

    // Создаем копии
    std::shared_ptr<int> ptr2 = ptr1;
    std::shared_ptr<int> ptr3 = ptr1;

    std::cout << "\nПосле создания ptr2 и ptr3:" << std::endl;
    std::cout << "ptr1.use_count() = " << ptr1.use_count() << std::endl;
    std::cout << "ptr2.use_count() = " << ptr2.use_count() << std::endl;
    std::cout << "ptr3.use_count() = " << ptr3.use_count() << std::endl;

    // Уничтожаем одну из копий
    std::cout << "\nУничтожаем ptr2..." << std::endl;
    ptr2.reset();  // или просто выходим из области видимости

    std::cout << "После уничтожения ptr2:" << std::endl;
    std::cout << "ptr1.use_count() = " << ptr1.use_count() << std::endl;
    std::cout << "ptr3.use_count() = " << ptr3.use_count() << std::endl;

    return 0;
}