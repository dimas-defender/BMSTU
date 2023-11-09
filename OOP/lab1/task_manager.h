#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "object.h"
#include "defines.h"
#include "input.h"
#include "draw.h"
#include "actions.h"

enum task_num
{
    INIT,
    LOAD_FILE,
    DRAW,
    MOVE,
    SCALE,
    ROTATE,
    QUIT
};

struct call
{
    task_num act;
    union
    {
        struct load_file lf;
        struct draw dr;
        struct move mo;
        struct scale sc;
        struct rotate ro;
    };
};

int task_manager(call &task);

#endif // TASK_MANAGER_H
