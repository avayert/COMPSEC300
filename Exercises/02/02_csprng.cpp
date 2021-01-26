/*
 * 02_csprng.cpp
 *
 * Reads and writes cryptographically secure random data
 *
 * NOTE: this program is Linux-specific
 */

#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template <typename T>
constexpr auto _n_digits(T n) {
    T ret = 0;
    while (n) {
        ret++;
        n /= 10;
    }
    return ret;
}

constexpr auto length = _n_digits(std::numeric_limits<size_t>::max()) - 1;


std::size_t safe_input() {
    std::string input;

    while (true) {
        std::cout << "How many bytes of data do you want to write?" << std::endl;
        std::getline(std::cin, input);

        if (length <= input.length()) {
            std::cout << "Size too large. Please try again." << std::endl;
            continue;
        }

        auto not_number = [] (char c) { return !std::isdigit(c); };
        if (std::any_of(input.begin(), input.end(), not_number)) {
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }

        // I can't believe there's no simpler way to do this...
        std::size_t ret;
        std::stringstream ss(input);
        ss >> ret;
        return ret;
    }
}

int main() {
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    if (!urandom) {
        std::cerr << "Could not open /dev/urandom" << std::endl;
        return 1;
    }

    std::ofstream out("output.bin", std::ios::binary);
    if (!out) {
        std::cerr << "Could not write output file." << std::endl;
        return 1;
    }

    auto n = safe_input();

    std::copy_n(
        std::istreambuf_iterator<char>(urandom),
        n,
        std::ostreambuf_iterator<char>(out)
    );
}
