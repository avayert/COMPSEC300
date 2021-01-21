/*
 * 02_csprng.cpp
 *
 * Reads and writes cryptographically secure random data
 *
 * NOTE: this program is Linux-specific
 */

#include <algorithm>
#include <fstream>
#include <iostream>


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

    std::cout << "How many bytes of data do you want to write?" << std::endl;

    size_t n;
    std::cin >> n;

    std::copy_n(
        std::istreambuf_iterator<char>(urandom),
        n,
        std::ostreambuf_iterator<char>(out)
    );
}
