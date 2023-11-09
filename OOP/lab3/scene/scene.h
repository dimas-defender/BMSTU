#ifndef SCENE_H
#define SCENE_H

#include "basescene.h"
#include "object/camera.h"

class Scene : public BaseScene
{
public:
    Scene();

    size_t getSize() override;

    ObjIter begin() override;
    ObjIter end() override;

    std::shared_ptr<BaseCamera> getCamera() override;
    void setCamera(ObjIter it) override;

    void addObject(const std::shared_ptr<BaseObject>& obj) override;
    void removeObject(ObjIter it) override;
};

#endif // SCENE_H
