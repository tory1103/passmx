//
// Created by adria on 16/09/2023.
//

#ifndef PASSMX_PASSMX_H
#define PASSMX_PASSMX_H

#include <cstddef>
#include <iostream>
#include <string>
#include "./token.h"

namespace passmx
{
    class Passmx
    {
    private:
        // Flow control variables
        // Determines how the code should work depending on current states
        //
        // is_prepared -> Checks if the secret and passphrase are filled to have same length
        // is_grouped  -> Checks if the secret and passphrase are grouped in arrays
        // verbose     -> Determines when to log code output
        bool is_prepared = false, is_grouped = false, verbose = false;

        // Determines what final password length should be, max password length is max(secret, passphrase)
        int password_length = 0;

        // Token struct variables, used to manipulate characters data
        t_token secret, passphrase, *secret_grouped = nullptr, *passphrase_grouped = nullptr;

        // Fills secret and passphrase to have same length
        // On complete sets is_prepared to True
        void prepare_strings();

        // Splits the secret and passphrase in character groups
        // On complete sets is_grouped to True
        void prepare_groups();

        // Converts the sum of a characters group into a single character
        // This character is located in a range of the ascii table, not all ascii values are accepted
        char group_to_char(int group_sum);

    public:
        // Class constructor, allocates memory
        Passmx(std::string secret, std::string passphrase, int length, bool verbose = false);

        // Class destructor, free memory
        ~Passmx();

        // Generates a password by mixing the secret and passphrase.
        // Lets explain it with an example, we have 2 strings:
        // 	secret:     mySecret
        //  passphrase: my passphrase says im pretty
        //
        // First we will fill the string to have same length, it will use same position characters:
        // 	secret:     mySecrethrase says im pretty
        //  passphrase: my passphrase says im pretty
        //
        // Once we have the strings completed, now we will split in groups,
        // depending on desired string length, groups will have more or less characters:
        // 	secret:     mySecrethrase says im pretty -> 28 characters
        //  passphrase: my passphrase says im pretty -> 28 characters
        // 	Desired password length -> 8
        //
        // The math behind groups split is simple, we will always have some groups with same length and an overflow group,
        // Overflow group is: secret_length % (desired_password_length - 1) -> 0 characters
        // Normal groups are: secret_length / (desired_password_length - 1) -> 4 characters
        // In this case there is not an overflow group and 8 groups of 4 characters each
        // This is a representation of how groups will look when split:
        //  split secret:     mySe, cret, hras, e sa, ys i, m pr, etty (spaces also are characters)
        //  split passphrase: my p, assp, hras, e sa, ys i, m pr, etty
        //
        // Once we have split all characters in groups, now we can convert each group into a individual character
        // We will make the sum of every character in the groups and use this mathematical equation to determine which character it is:
        // ((group_sum - 48) % (126 - 48 + 1)) + 48 (character between 48 and 126 ascii characters)
        //
        // Group 1:
        //  mySe + my p = (109 + 121 + 83 + 101) + (109 + 121 + 32 + 112) = 374
        //  ((374 - 48) % (126 - 48 + 1)) + 48 = 58 (character :)
        // We repeat this process for every group
        std::string generate_password();
    };
}

#endif //PASSMX_PASSMX_H
