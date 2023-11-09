#include "input.h"

static int read_center(point_t &center, FILE *f)
{
    int rc = OK;
    if (fscanf(f, "%f %f %f", &center.x, &center.y, &center.z) != 3)
        rc =  FILE_ERR;
    return rc;
}

static int read_nodes_num(int &nodes_num, FILE *f)
{
    int rc = OK;
    if (fscanf(f, "%d", &nodes_num) != 1)
        rc =  FILE_ERR;
    else if (nodes_num <= 0)
        rc = NODES_ERR;
    return rc;
}

static int read_links_num(int &links_num, FILE *f)
{
    int rc = OK;
    if (fscanf(f, "%d", &links_num) != 1)
        rc =  FILE_ERR;
    else if (links_num < 0)
        rc = LINKS_ERR;
    return rc;
}

static int alloc_nodes(point_t **nodes, const int &nodes_num)
{
    *nodes = (point_t*)malloc(nodes_num * sizeof(point_t));
    int rc = OK;
    if (!*nodes)
        rc = ALLOC_ERR;
    return rc;
}

static int alloc_links(link_t **links, const int &links_num)
{
    *links = (link_t*)malloc(links_num * sizeof(link_t));
    int rc = OK;
    if (!*links)
        rc = ALLOC_ERR;
    return rc;
}

static void free_nodes(point_t *nodes)
{
    free(nodes);
}

static void free_links(link_t *links)
{
    free(links);
}

static int read_node(point_t *nodes, int i, FILE *f)
{
    int rc = OK;
    if (fscanf(f, "%f %f %f", &nodes[i].x, &nodes[i].y, &nodes[i].z) != 3)
        rc = FILE_ERR;
    return rc;
}

static int read_nodes(point_t **nodes, const int &nodes_num, FILE *f)
{
    int rc = alloc_nodes(nodes, nodes_num);

    for (int i = 0; !rc && i < nodes_num; i++)
        rc = read_node(*nodes, i, f);

    if (rc == FILE_ERR)
        free_nodes(*nodes);
    return rc;
}

static int read_link(link_t *links, int i, FILE *f)
{
    int rc = OK;
    if (fscanf(f, "%d %d", &links[i].p_1, &links[i].p_2) != 2)
        rc = FILE_ERR;
    return rc;
}

static int read_links(link_t **links, const int &links_num, FILE *f)
{
    int rc = alloc_links(links, links_num);

    for (int i = 0; !rc && i < links_num;  i++)
        rc = read_link(*links, i, f);

    if (rc == FILE_ERR)
        free_links(*links);
    return rc;
}

static int check_link(link_t *links, int i, const int &nodes_num)
{
    int rc = OK;
    if (links[i].p_1 < 0 || links[i].p_2 < 0 || links[i].p_1 > nodes_num - 1 || links[i].p_2 > nodes_num - 1)
        rc =  WRONG_LINKS;
    return rc;
}

static int check_model(object_t &obj)
{
    int rc = OK;
    for (int i = 0; !rc && i < obj.links_num; i++)
        rc = check_link(obj.links, i, obj.nodes_num);
    return rc;
}

static void copy_model(object_t &object, object_t &new_obj)
{
    object = new_obj;
}

static int open_file(FILE **f, const char *name)
{
    int rc = OK;
    if (!name)
        rc =  WRONG_FILE;
    *f = fopen(name, "r");
    if (!*f && !rc)
        rc =  WRONG_FILE;
    return rc;
}

static int read_model(object_t &obj, FILE *f)
{
    int rc = read_center(obj.center, f);
    if (!rc)
        rc = read_nodes_num(obj.nodes_num, f);
    if (!rc)
        rc = read_links_num(obj.links_num, f);
    if (!rc)
    {
        rc = read_nodes(&obj.nodes, obj.nodes_num, f);
        if (!rc)
        {
            rc = read_links(&obj.links, obj.links_num, f);
            if (rc)
                free_nodes(obj.nodes);
        }
    }
    if (!rc)
    {
        rc = check_model(obj);
        if (rc)
            delete_object(obj);
    }
    return rc;
}

int load_file(object_t &object, const struct load_file &lf)
{
    FILE *f = NULL;
    if (open_file(&f, lf.file_name) != OK)
        return WRONG_FILE;

    object_t new_obj = init();
    int rc = read_model(new_obj, f);

    fclose(f);

    if (!rc)
    {
        delete_object(object);
        copy_model(object, new_obj);
    }
    return rc;
}
