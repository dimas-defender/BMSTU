#ifndef INPUT_H
#define INPUT_H

#include "object.h"
#include "defines.h"
#include <cstdlib>
#include <cstdio>

struct load_file
{
    const char* file_name;
};

int load_file(object_t &object, const struct load_file &lf);

#endif // INPUT_H
