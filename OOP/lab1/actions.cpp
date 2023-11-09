#include "actions.h"

static void move_point(point_t &point, const struct move &mo)
{
    point.x += mo.dx;
    point.y += mo.dy;
    point.z += mo.dz;
}

static void move_points(point_t *nodes, int nodes_num, const struct move &mo, point_t &center)
{
    for (int i = 0; i < nodes_num; i++)
        move_point(nodes[i], mo);
    move_point(center, mo);
}

int move_object(object_t &object, const struct move &params)
{
    int rc = check_object(object);
    if (rc)
        return rc;

    move_points(object.nodes, object.nodes_num, params, object.center);
    return rc;
}

static void scale_point(point_t &point, const struct scale &sc, point_t &center)
{
    point.x = sc.kx * point.x + center.x * (1 - sc.kx);
    point.y = sc.ky * point.y + center.y * (1 - sc.ky);
    point.z = sc.kz * point.z + center.z * (1 - sc.kz);
}

static void scale_points(point_t *nodes, int nodes_num, const struct scale &sc, point_t &center)
{
    for (int i = 0; i < nodes_num; i++)
        scale_point(nodes[i], sc, center);
}

int scale_object(object_t &object, const struct scale &params)
{
    int rc = check_object(object);
    if (rc)
        return rc;

    scale_points(object.nodes, object.nodes_num, params, object.center);
    return rc;
}

static float radians(int angle)
{
    return angle * M_PI / 180;
}

static struct trig count_trig(const struct rotate &ro)
{
    struct trig tg;
    tg.cos_x = cos(radians(ro.dgx));
    tg.cos_y = cos(radians(ro.dgy));
    tg.cos_z = cos(radians(ro.dgz));
    tg.sin_x = sin(radians(ro.dgx));
    tg.sin_y = sin(radians(ro.dgy));
    tg.sin_z = sin(radians(ro.dgz));
    return tg;
}

static void rotate_OZ(point_t &point, const struct trig &tg)
{
    float x = point.x, y = point.y;
    point.x = x * tg.cos_z - y * tg.sin_z;
    point.y = x * tg.sin_z + y * tg.cos_z;
}

static void rotate_OX(point_t &point, const struct trig &tg)
{
    float y = point.y, z = point.z;
    point.y = y * tg.cos_x - z * tg.sin_x;
    point.z = z * tg.cos_x + y * tg.sin_x;
}

static void rotate_OY(point_t &point, const struct trig &tg)
{
    float x = point.x, z = point.z;
    point.x = x * tg.cos_y + z * tg.sin_y;
    point.z = z * tg.cos_y - x * tg.sin_y;
}

static void rotate_point(point_t &point, const struct trig &tg)
{
    rotate_OX(point, tg);
    rotate_OY(point, tg);
    rotate_OZ(point, tg);
}

static void rotate_points(point_t *nodes, int nodes_num, const struct trig &tg)
{
    for (int i = 0; i < nodes_num; i++)
        rotate_point(nodes[i], tg);
}

static int move_by_center(object_t &object, point_t &center, int move_flag)
{
    struct move mo;
    mo.dx = move_flag * center.x;
    mo.dy = move_flag * center.y;
    mo.dz = move_flag * center.z;
    int rc = move_object(object, mo);
    return rc;
}

int rotate_object(object_t &object, const struct rotate &params)
{
    int rc = check_object(object);
    if (rc)
        return rc;

    struct trig tg = count_trig(params);
    point_t center = object.center;
    move_by_center(object, center, -1);
    rotate_points(object.nodes, object.nodes_num, tg);
    move_by_center(object, center, 1);
    return rc;
}
