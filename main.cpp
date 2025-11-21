#include <iostream>
#include <string>
#include "pmr_vector.hpp"
#include "memory_pool_resourse.hpp"

struct Foo {
    int a;
    double b;
    std::string s;

    friend std::ostream& operator<<(std::ostream& os, const Foo& f) {
        return os << "{a=" << f.a << ", b=" << f.b << ", s=\"" << f.s << "\"}";
    }
};

template <typename T>
void interactive_mode() {
    memory_pool_resourse pool;
    pmr_vector<T> vec(&pool);

    while (true) {
        std::cout <<
            "Доступные команды:\n"
            "  push <x>        - добавить элемент\n"
            "  pop             - удалить последний элемент\n"
            "  print           - вывести элементы\n"
            "  size            - вывести size и capacity\n"
            "  reserve <n>     - зарезервировать память\n"
            "  get <i>         - вывести элемент по индексу\n"
            "  exit            - выход\n> ";

        std::string cmd;
        std::cin >> cmd;

        if (cmd == "push") {

            if constexpr (std::is_same_v<T, int>) {
                int x;
                std::cin >> x;
                vec.push_back(x);
            } else {
                Foo f;
                std::cin >> f.a >> f.b >> f.s;
                vec.push_back(f);
            }

        } else if (cmd == "pop") {
            if (vec.size()) vec.pop_back();
        } else if (cmd == "print") {
            std::cout << "Содержимое: ";
            for (auto &x : vec) {
                std::cout << x << " ";
            }

            std::cout << "\n\n";
        } else if (cmd == "size") {
            std::cout << "size = " << vec.size() << " capacity = " << vec.capacity() << "\n\n";
        } else if (cmd == "reserve") {
            size_t n;
            std::cin >> n;
            vec.reserve(n);
        } else if (cmd == "get") {
            size_t i;
            std::cin >> i;
            if (i < vec.size()) {
                std::cout << vec[i] << "\n\n";
            } else {
                std::cout << "Индекс вне диапазона\n\n";
            }
        } else if (cmd == "exit") {
            break;
        } else {
            std::cout << "Неизвестная команда\n\n";
        }
    }
}

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Запуск:\n"
                     "  ./main int\n"
                     "  ./main struct\n";
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "int") {
        interactive_mode<int>();
    } else if (mode == "struct") {
        interactive_mode<Foo>();
    } else {
        std::cout << "Неизвестный режим\n";
    }

    return 0;
}
