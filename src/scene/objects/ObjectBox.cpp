#include "scene/objects/ObjectBox.h"
#include "scene/SceneObjectMenu.h"

bool ObjectBox::ParamsAccept(SceneObjectMenu &visitor)
{
    return visitor.ParamsVisit(*this);
}