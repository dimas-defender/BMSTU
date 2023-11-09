#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "defines.h"
#include "object.h"

struct draw
{
    QGraphicsScene *QS;
    int h;
    int w;
};

struct line_t
{
    int x_1;
    int y_1;
    int x_2;
    int y_2;
};

void add_line(struct draw &dr, const line_t line);
void clear_scene(struct draw &dr);

#endif // DRAW_UTILS_H
