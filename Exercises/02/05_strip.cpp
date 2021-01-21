#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>


constexpr size_t BUFFER_SIZE = 1024;


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Invalid usage. Use " << argv[0] << " <filename>" << std::endl;
        return -1;
    }

    std::ifstream stream(argv[1], std::ios::binary);
    if (!stream) {
        std::cerr << "Could not open file with name " << argv[1] << std::endl;
        return 1;
    }

    // read and print in chunks to avoid issues with OOMing
    std::array<char, BUFFER_SIZE> buffer;
    while (stream.read(buffer.data(), BUFFER_SIZE)) {
        std::copy_if(
            std::begin(buffer),
            std::end(buffer),
            std::ostream_iterator<char>(std::cout),
            [](char c) { return std::isalnum(c) || c == ',' || c == '-'; }
        );
    }

    std::cout << '\n';
}
