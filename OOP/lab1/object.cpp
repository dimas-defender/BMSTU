#include "object.h"

object_t init()
{
    object_t object;
    object.links_num = 0;
    object.nodes_num = 0;
    object.links = NULL;
    object.nodes = NULL;
    return object;
}

int check_object(const object_t &object)
{
    if (!object.nodes || !object.links)
        return ARR_NULL;
    if (object.nodes_num <= 0)
        return NODES_ERR;
    if (object.links_num < 0)
        return LINKS_ERR;
    return OK;
}

void delete_object(object_t &object)
{
    if (object.nodes_num)
    {
        free(object.nodes);
        free(object.links);
        object.nodes_num = 0;
        object.links_num = 0;
    }
}
