#include "scene/objects/ObjectTriangle.h"
#include "scene/SceneObjectMenu.h"

bool ObjectTriangle::ParamsAccept(SceneObjectMenu &visitor)
{
    return visitor.ParamsVisit(*this);
}