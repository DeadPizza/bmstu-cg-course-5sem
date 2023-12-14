#include "scene/objects/ObjectSphere.h"
#include "scene/SceneObjectMenu.h"

bool ObjectSphere::ParamsAccept(SceneObjectMenu &visitor)
{
    return visitor.ParamsVisit(*this);
}