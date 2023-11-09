#ifndef MODEL_H
#define MODEL_H

#include "simpleobjects.h"
#include "point.h"
//#include "manager/controlmanager.h"

typedef struct
{
    size_t src;
    size_t dst;
} Edge;

class ModelContent
{
public:
    ModelContent() = default;
    ModelContent(const std::vector<Point> points, const std::vector<Edge> edges);
    ModelContent(ModelContent &model);
    ModelContent(ModelContent &&model);
    ~ModelContent();

    std::vector<Point>& getPoints();
    std::vector<Edge> getEdges() const;

private:
    std::vector<Point> points;
    std::vector<Edge> edges;
};

class FrameModel : public VisibleObject
{
    friend class DrawManager;
public:
    FrameModel(shared_ptr<ModelContent> cont);
    ~FrameModel();

    bool isViewer();

    void accept(ObjectVisitor& vis);
private:
    shared_ptr<ModelContent> content;
};

#endif // MODEL_H
