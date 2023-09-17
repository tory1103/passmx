//
// Created by adria on 16/09/2023.
//

#ifndef PASSMX_TOKEN_H
#define PASSMX_TOKEN_H

// Struct that works like a string, contains content array and length
typedef struct t_token
{
    // Characters data
    char *content;

    // Number of characters
    int length;
} t_token;

#endif //PASSMX_TOKEN_H
