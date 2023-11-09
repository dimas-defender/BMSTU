#ifndef OBJECTVISITOR_H
#define OBJECTVISITOR_H

#include <memory>

#include "object/compositeobject.h"
#include "object/point.h"
#include "object/model.h"
#include "object/basecamera.h"
#include "object/camera.h"

using namespace std;

class ObjectVisitor
{
public:
    virtual ~ObjectVisitor() = default;

    virtual void visit(CompositeObject& obj) = 0;
    virtual void visit(FrameModel& model) = 0;
    virtual void visit(Camera& camera) = 0;
    virtual void visit(Point& point) = 0;
};

#endif // OBJECTVISITOR_H
