#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "simpleobjects.h"

class BaseCamera : public InvisibleObject
{
    friend class TransformVisitor;
    friend class MoveVisitor;
    friend class RotateVisitor;
    friend class ScaleVisitor;
    friend class Visualizer;
public:
    BaseCamera();
    BaseCamera(const double x, const double y, const double z);
    BaseCamera(const BaseCamera &camera);
    BaseCamera(BaseCamera &&camera);
    ~BaseCamera();

    bool isViewer() override;

protected:
    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(const double x);
    void setY(const double y);
    void setZ(const double z);

    double x;
    double y;
    double z;
};

#endif // BASECAMERA_H
