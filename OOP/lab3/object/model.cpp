#include "model.h"

FrameModel::FrameModel(shared_ptr<ModelContent> cont) : content(cont) {}

FrameModel::~FrameModel() {}

bool FrameModel::isViewer()
{
    return false;
}

void FrameModel::accept(ObjectVisitor& vis)
{
    std::vector<Point>& points = (this->content)->getPoints();
    for (size_t i = 0; i < points.size(); i++)
        points[i].accept(vis);
}

ModelContent::ModelContent(const std::vector<Point> points, const std::vector<Edge> edges)
{
    this->points = points;
    this->edges = edges;
}

ModelContent::ModelContent(ModelContent &model)
{
    this->points = model.getPoints();
    this->edges = model.getEdges();
}

ModelContent::ModelContent(ModelContent &&model)
{
    this->points = model.getPoints();
    this->edges = model.getEdges();
}

ModelContent::~ModelContent() {}

std::vector<Point>& ModelContent::getPoints()
{
    return this->points;
}

std::vector<Edge> ModelContent::getEdges() const
{
    return this->edges;
}
