//
// Created by adria on 16/09/2023.
//

#include "../../include/passmx/passmx.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

passmx::Passmx::Passmx(std::string secret, std::string passphrase, int length, bool verbose)
{
    // Flow control variables
    this->password_length = length;
    this->verbose = verbose;

    // Allocate secret and passphrase memory
    this->secret.content = (char *) malloc(sizeof(char) * secret.length());
    this->secret.length = secret.length();

    this->passphrase.content = (char *) malloc(sizeof(char) * passphrase.length());
    this->passphrase.length = passphrase.length();

    // Copy both strings to char array for better manipulation
    for (int i = 0; i < this->secret.length; i++) this->secret.content[i] = secret[i];
    for (int i = 0; i < this->passphrase.length; i++) this->passphrase.content[i] = passphrase[i];

    // Check if desired password length is higher than actual secret and password length
    int max_length = std::max(this->secret.length, this->passphrase.length);
    if (this->password_length > max_length)
    {
        if (this->verbose) std::cerr << "Password length can't be higher than strings length. Skipping desired length... Fail" << std::endl;
        this->password_length = max_length;
    }
}

passmx::Passmx::~Passmx()
{
    // Free all allocated memory
    free(this->secret.content);
    free(this->passphrase.content);

    for (int i = 0; i < this->password_length; i++)
    {
        free(this->secret_grouped[i].content);
        free(this->passphrase_grouped[i].content);
    }

    free(this->secret_grouped);
    free(this->passphrase_grouped);
}

std::string passmx::Passmx::generate_password()
{
    // Previous checks
    if (!this->is_prepared) this->prepare_strings();
    if (!this->is_grouped) this->prepare_groups();

    // Convert all groups to characters and push to string
    std::string password;
    for (int i = 0; i < this->password_length; i++)
    {
        int group_sum = 0;
        for (int j = 0; j < this->secret_grouped[i].length; j++) group_sum += (int) this->secret_grouped[i].content[j];
        for (int j = 0; j < this->passphrase_grouped[i].length; j++) group_sum += (int) this->passphrase_grouped[i].content[j];

        // Push character to string
        password.push_back(this->group_to_char(group_sum));
    }

    return password;
}

void passmx::Passmx::prepare_strings()
{
    // Previous checks
    // If already prepared, skip
    if (this->is_prepared) return;

    int max_length = std::max(this->secret.length, this->passphrase.length);

    // Try to fill secret with characters until its length == max_length
    // If length == max_length it will not do anything
    this->secret.content = (char *) realloc(this->secret.content, sizeof(char) * max_length);
    for (int i = this->secret.length; i < max_length; i++) this->secret.content[i] = this->passphrase.content[i]; // Fill with passphrase 'i' position character

    // Try to fill passphrase with characters until its length == max_length
    // If length == max_length it will not do anything
    this->passphrase.content = (char *) realloc(this->passphrase.content, sizeof(char) * max_length);
    for (int i = this->passphrase.length; i < max_length; i++) this->passphrase.content[i] = this->secret.content[i]; // Fill with secret 'i' position character

    // Flow control variables
    if (this->verbose)
    {
        std::cerr << std::endl;
        std::cerr << "Adding " << (max_length - this->secret.length) << " characters to secret keyword... Done" << std::endl;
        std::cerr << "Adding " << (max_length - this->passphrase.length) << " characters to passphrase... Done" << std::endl;
    }

    this->secret.length = max_length;
    this->passphrase.length = max_length;
    this->is_prepared = true;
}

void passmx::Passmx::prepare_groups()
{
    // Previous checks
    // If already grouped, skip
    if (this->is_grouped) return;
    if (!this->is_prepared) this->prepare_strings();

    // Allocate groups memory
    this->secret_grouped = (t_token *) malloc(sizeof(t_token) * this->password_length);
    this->passphrase_grouped = (t_token *) malloc(sizeof(t_token) * this->password_length);

    // Number of characters in a group
    int number_group_chars = (int) (this->secret.length / (this->password_length - 1));

    // Number of overflow characters in a group
    // At this point secret and passphrase has same length
    int number_overflow_chars = (this->secret.length % (this->password_length - 1));

    // Current group index position
    int current_group_position = 0;

    // If overflow characters then allocate it
    if (number_overflow_chars)
    {
        this->secret_grouped[current_group_position].content = (char *) malloc(sizeof(char) * number_overflow_chars);
        this->secret_grouped[current_group_position].length = number_overflow_chars;

        this->passphrase_grouped[current_group_position].content = (char *) malloc(sizeof(char) * number_overflow_chars);
        this->passphrase_grouped[current_group_position].length = number_overflow_chars;

        current_group_position++;
    }

    // For every group allocate it
    for (; current_group_position < this->password_length; current_group_position++)
    {
        this->secret_grouped[current_group_position].content = (char *) malloc(sizeof(char) * number_group_chars);
        this->secret_grouped[current_group_position].length = number_group_chars;

        this->passphrase_grouped[current_group_position].content = (char *) malloc(sizeof(char) * number_group_chars);
        this->passphrase_grouped[current_group_position].length = number_group_chars;
    }

    current_group_position = 0;
    for (int character_index = 0, group_index = 0, length_group_limit = this->secret_grouped[current_group_position].length;
         character_index < this->secret.length;
         character_index++, group_index++)
    {
        // If character index is higher or equal than group limit length, it will get into the next group
        // Having:
        //  string : hello world
        //  groups : he, llo, \swo, rld
        // then:
        //  character_index = 0 (h)
        //  length_group_limit = 2 (current group length)
        // When character_index surpass 2, it will get into next group:
        //  character_index = 2 (l)
        //  length_group_limit = 3 (current group length)
        if (character_index >= length_group_limit)
        {
            if (++current_group_position >= this->password_length) current_group_position = this->password_length;
            length_group_limit += this->secret_grouped[current_group_position].length;
            group_index = 0;
        }

        this->secret_grouped[current_group_position].content[group_index] = this->secret.content[character_index];
        this->passphrase_grouped[current_group_position].content[group_index] = this->passphrase.content[character_index];
    }

    // Flow control variables
    if (this->verbose)
    {
        std::cerr << std::endl;
        std::cerr << "Splitting overflow group of " << number_overflow_chars << " characters... Done" << std::endl;
        std::cerr << "Splitting " << (this->password_length - number_overflow_chars) << " normal groups of " << number_group_chars << " characters each... Done" << std::endl;

        std::cerr << std::endl << "Split table of " << this->password_length << " groups" << std::endl;
        for (int i = 0; i < this->password_length; i++) std::cerr << i + 1 << ". " << this->secret_grouped[i].content << " || " << this->passphrase_grouped[i].content << std::endl;
    }

    this->is_grouped = true;
}

char passmx::Passmx::group_to_char(int group_sum)
{
    // Values in the ascii table from 48 to 126
    return (char) (((group_sum - 48) % (126 - 48 + 1)) + 48);
}
