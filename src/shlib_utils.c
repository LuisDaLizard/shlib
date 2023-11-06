//
// Created by Luis Tadeo Sanchez on 10/23/23.
//

#include "shlib_internal.h"
#include <stdio.h>
#include <stdlib.h>

char *utils_read_file(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *text = malloc(length + 1);
    text[length] = 0;

    size_t res = fread(text, 1, length, file);

    if (res < length)
    {
        free(text);
        fclose(file);
        return 0;
    }

    fclose(file);
    return text;
}

unsigned char *utils_read_file_bytes(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (!file)
        return 0;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *bytes = malloc(length);

    size_t res = fread(bytes, 1, length, file);

    if (res < length)
    {
        free(bytes);
        fclose(file);
        return 0;
    }

    fclose(file);
    return bytes;
}