#include "scene/objects/ObjectHexPrism.h"
#include "scene/SceneObjectMenu.h"

bool ObjectHexPrism::ParamsAccept(SceneObjectMenu &visitor)
{
    return visitor.ParamsVisit(*this);
}