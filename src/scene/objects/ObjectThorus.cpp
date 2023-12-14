#include "scene/objects/ObjectThorus.h"
#include "scene/SceneObjectMenu.h"

bool ObjectThorus::ParamsAccept(SceneObjectMenu &visitor)
{
    return visitor.ParamsVisit(*this);
}