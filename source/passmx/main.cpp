//
// Created by adria on 16/09/2023.
//

#include <cstdlib>
#include <iostream>
#include <ostream>
#include "../../include/passmx/passmx.h"

void print_usage()
{
    std::cout << "Passmx - Password Mixer" << std::endl
              << "================================" << std::endl
              << "Usage mode: passmx -s 'secret_key' -p 'passphrase' ... [OPTIONS]" << std::endl
              << "Generates passwords by mixing secret and passphrase" << std::endl
              << std::endl
              << "\t-h, --help      \tDisplays this help menu" << std::endl
              << "\t-s, --secret    \tSecret keyword to use on generation" << std::endl
              << "\t-p, --passphrase\tPassphrase to use on generation" << std::endl
              << "\t-l, --length    \tDesired password length (cannot be higher than actual secret and passphrase length)" << std::endl
              << "\t-v, --verbose   \tVerbose output. Show step by step information" << std::endl
              << std::endl
              << "Showcase:" << std::endl
              << "\tpassmx -s mySecretKey -p 'https://google.com myUserMail@gmail.com' -v -l 16" << std::endl
              << "\tpassmx -s mySecretKey -p 'i like peanuts' -v" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        print_usage();
        return 0;
    }

    // Variables
    std::string secret, passphrase;
    int password_length = 99999;
    bool verbose = false;

    // Arguments
    for (int argument_index = 1; argument_index < argc; argument_index++)
    {
        std::string arg = argv[argument_index];
        if (arg == "-h" || arg == "--help")
        {
            print_usage();
            return 0;
        } else if (arg == "-v" || arg == "--verbose")
        {
            verbose = true;
        } else if (arg == "-s" || arg == "--secret")
        {
            if (argument_index + 1 < argc) secret = argv[++argument_index];
            else
            {
                print_usage();
                return 2;
            }
        } else if (arg == "-p" || arg == "--passphrase")
        {
            if (argument_index + 1 < argc) passphrase = argv[++argument_index];
            else
            {
                print_usage();
                return 3;
            }
        } else if (arg == "-l" || arg == "--length")
        {
            if (argument_index + 1 < argc) password_length = strtol(argv[++argument_index], nullptr, 10);
            else
            {
                print_usage();
                return 4;
            }
        } else
        {
            std::cout << "Invalid argument_index: " << arg << ". Try --help for more details." << std::endl;
            return 1;
        }
    }

    passmx::Passmx generator(secret, passphrase, password_length, verbose);
    std::cout << generator.generate_password() << std::endl;

    return 0;
}
