#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <iostream>
#include <memory>
#include <vector>

class ObjectVisitor;
class BaseObject;

using namespace std;

typedef vector<shared_ptr<BaseObject>>::iterator ObjIter;

class BaseObject
{
public:
    BaseObject() = default;

    virtual bool isVisible() = 0;
    virtual bool isComposite();
    virtual bool isViewer() = 0;

    virtual size_t getSize();

    virtual void addObject(shared_ptr<BaseObject> obj){};
    virtual void removeObject(ObjIter it){};

    virtual ObjIter begin(){return ObjIter();};
    virtual ObjIter end(){return ObjIter();};

    virtual void accept(ObjectVisitor& vis) = 0;
};

#endif // BASEOBJECT_H
