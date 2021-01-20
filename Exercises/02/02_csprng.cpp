/*
 * 02_csprng.cpp
 *
 * Reads and writes cryptographically secure random data
 *
 * NOTE: this program is Linux-specific
 */

#include <iostream>
#include <fstream>
#include <array>


constexpr size_t BUFFER_SIZE = 32;


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

    // read in chunks of N to avoid memory issues
    std::array<char, BUFFER_SIZE> buffer;
    size_t read = 0;
    while (n) {
        urandom.read(buffer.data(), std::min(n, BUFFER_SIZE));
        if ((read = urandom.gcount()) == 0) {
            std::cerr << "Reading from /dev/urandom failed" << std::endl;
            return 1;
        }

        out.write(buffer.data(), std::min(n, BUFFER_SIZE));
        if (out.bad()) {
            std::cerr << "Writing to output file failed" << std::endl;
            return 1;
        }

        n -= read;
    }
}
