#include "draw_utils.h"

void add_line(struct draw &dr, const line_t line)
{
    dr.QS->addLine(line.x_1, line.y_1, line.x_2, line.y_2);
}

void clear_scene(struct draw &dr)
{
    dr.QS->clear();
}
