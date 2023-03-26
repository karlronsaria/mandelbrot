#include <iostream>

namespace debug {
    template <typename T>
    void pass(const char * description, T value) {
        std::cout << description << ": " << value << '\n';
    }
};
