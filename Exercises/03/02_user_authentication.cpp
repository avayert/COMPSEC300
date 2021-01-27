#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// for getpass()
#include <unistd.h>

// libsodium is required to run this program
#include <sodium.h>


// map of username -> hash
using users_t = std::unordered_map<std::string, std::string>;


users_t read_users() {
    std::ifstream in("pw.data");
    if (!in) {
        return {};
    }

    users_t ret;

    std::vector<std::string> entries;
    entries.reserve(2);

    std::string line;
    while (std::getline(in, line)) {
        entries.clear();

        std::stringstream ss(line);

        std::string entry;
        while (std::getline(ss, entry, '|')) {
            entries.emplace_back(entry);
        }

        if (entries.size() != 2)
            continue;

        // can't emplace_back pre-c++20 seemingly
        ret[entries[0]] = entries[1];
    }

    return ret;
}

void write_users(const users_t& users) {
    // write atomically as not to destroy any data
    std::ofstream out("pw.data.tmp");
    for (auto [username, hash] : users) {
        out << username << '|'
            << hash  << '\n';
    }
    out.close();
    std::rename("pw.data.tmp", "pw.data");
}

std::pair<std::string, std::string> get_username_and_password() {
    std::string username;
    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::string password = getpass("Password: ");

    return {username, password};
}

std::optional<std::string> hash_password(const std::string& password) {
    char hashed_password[crypto_pwhash_STRBYTES];

    // crypto_pwhash_str automatically handles salting for us.
    // https://libsodium.gitbook.io/doc/password_hashing/default_phf#password-storage
    const auto oom = crypto_pwhash_str(
        hashed_password,
        password.c_str(),
        password.length(),
        crypto_pwhash_OPSLIMIT_SENSITIVE,
        crypto_pwhash_MEMLIMIT_SENSITIVE
    );

    if (oom) {
        return std::nullopt;
    }

    return hashed_password;
}



void add_user(users_t& users) {
    const auto [username, password] = get_username_and_password();

    if (users.find(username) != std::end(users)) {
        std::cout << "An user with that name already exists!\n\n";
        return;
    }

    if (std::any_of(
        std::begin(username),
        std::end(username),
        [](char c) { return !std::isalnum(c); }
    )) {
        std::cout << "Username must only contain alphanumeric ASCII characters.\n\n";
        return;
    }

    const auto hash = hash_password(password);

    if (!hash) {
        std::cout << "Could not add user. Please try again later." << std::endl;
        return;
    }


    users[username] = hash.value();

    // write here just in case
    write_users(users);

    std::cout << "User successfully created.\n\n";
}

void log_in(const users_t& users) {
    const auto [username, password] = get_username_and_password();

    const auto elem = users.find(username);
    if (elem == std::end(users)) {
        std::cout << "No user with name '" << username << "'\n";
        return;
    }

    if (crypto_pwhash_str_verify(
        elem->second.c_str(),
        password.c_str(),
        password.length()
    )) {
        std::cout << "Password does not match." << std::endl;
        return;
    }

    std::cout << "Congratulations. You have logged in." << std::endl;
    std::exit(0);
}


int main() {
    auto users = read_users();

    if (sodium_init() == -1) {
        std::cout << "Could not initialize libsodium." << std::endl;
        return 1;
    }

    while (true) {
        std::string input;
        std::cout << "Please choose an action:\n"
                     "(l)og in\n"
                     "(a)dd user\n"
                     ">>> ";
        std::getline(std::cin, input);

        if (input.length() != 1) {
            std::cout << "Invalid input\n\n";
        } else if (std::tolower(input[0]) == 'a') {
            add_user(users);
        } else if (std::tolower(input[0]) == 'l') {
            log_in(users);
        } else {
            std::cout << "Not a valid option.\n\n";
        }
    }
}
