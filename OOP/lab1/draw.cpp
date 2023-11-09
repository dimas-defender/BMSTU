#include "draw.h"

static struct line_t get_line(link_t *links, point_t *nodes, int i)
{
    line_t line;

    int ind1 = links[i].p_1;
    int ind2 = links[i].p_2;

    line.x_1 = nodes[ind1].x;
    line.y_1 = -nodes[ind1].y;
    line.x_2 = nodes[ind2].x;
    line.y_2 = -nodes[ind2].y;

    return line;
}

int draw_object(const object_t &object, struct draw &dr)
{
    int rc = check_object(object);
    if (rc)
        return rc;

    clear_scene(dr);
    for (int i = 0; i < object.links_num; i++)
    {
        line_t line = get_line(object.links, object.nodes, i);
        add_line(dr, line);
    }
    return rc;
}
