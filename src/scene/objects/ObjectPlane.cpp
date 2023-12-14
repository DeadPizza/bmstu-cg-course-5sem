#include "scene/objects/ObjectPlane.h"
#include "scene/SceneObjectMenu.h"

bool ObjectPlane::ParamsAccept(SceneObjectMenu &visitor)
{
    return visitor.ParamsVisit(*this);
}