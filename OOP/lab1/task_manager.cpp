#include "task_manager.h"

int task_manager(call &task)
{
    int  rc = OK;
    static object_t object = init();

    switch (task.act)
    {
    case INIT:
        break;
    case LOAD_FILE:
        rc = load_file(object, task.lf);
        break;
    case DRAW:
        rc = draw_object(object, task.dr);
        break;
    case MOVE:
        rc = move_object(object, task.mo);
        break;
    case SCALE:
        rc = scale_object(object, task.sc);
        break;
    case ROTATE:
        rc = rotate_object(object, task.ro);
        break;
    case QUIT:
        delete_object(object);
        break;
    default:
        rc = UNKNOWN_ACT;
    }
    return rc;
}
