#include <iostream>
#include <memory_resource>
#include "memory_pool_resourse.hpp"
#include "pmr_vector.hpp"

int main() {
    std::cout << "=== PMR Vector Demo ===\n";

    // создаём наш пул
    memory_pool_resourse pool(64 * 1024);     // 64KB
    std::pmr::polymorphic_allocator<int> alloc(&pool);

    // создаём вектор, использующий наш пул
    pmr_vector<int> vec(&pool);

    std::cout << "Используется кастомный memory_resource (memory_pool_resourse)\n\n";

    while (true) {
        std::cout << "Доступные команды:\n"
                  << "  push <x>    - добавить элемент\n"
                  << "  pop         - удалить последний элемент\n"
                  << "  print       - вывести элементы\n"
                  << "  size        - размер вектора\n"
                  << "  reserve <n> - зарезервировать память\n"
                  << "  exit        - выход\n"
                  << "> ";

        std::string cmd;
        std::cin >> cmd;

        if (cmd == "push") {
            int x;
            std::cin >> x;
            vec.push_back(x);
            std::cout << "Добавлен элемент " << x << "\n\n";
        }
        else if (cmd == "pop") {
            if (!vec.empty()) {
                vec.pop_back();
                std::cout << "Удалён последний элемент\n\n";
            } else {
                std::cout << "Вектор пуст\n\n";
            }
        }
        else if (cmd == "print") {
            std::cout << "Содержимое: ";
            for (auto &v : vec) std::cout << v << " ";
            std::cout << "\n\n";
        }
        else if (cmd == "size") {
            std::cout << "size = " << vec.size()
                      << ", capacity = " << vec.capacity() << "\n\n";
        }
        else if (cmd == "reserve") {
            size_t n;
            std::cin >> n;
            vec.reserve(n);
            std::cout << "capacity увеличена до " << vec.capacity() << "\n\n";
        }
        else if (cmd == "exit") {
            std::cout << "Выход...\n";
            break;
        }
        else {
            std::cout << "Неизвестная команда\n\n";
        }
    }

    return 0;
}
