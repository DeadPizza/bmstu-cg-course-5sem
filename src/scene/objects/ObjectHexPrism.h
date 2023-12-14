#pragma once
#include "scene/SceneObject.h"

class ObjectHexPrism : public SceneObject
{
    friend class SceneObjectMenu;
public:
    ObjectHexPrism()
    {
        m_data.object_type = 3;
    }

    SceneObject *Clone() { return new ObjectHexPrism(*this); }

    bool ParamsAccept(SceneObjectMenu &visitor) override;
};