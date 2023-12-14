#pragma once
#include "scene/SceneObject.h"

class ObjectTriangle : public SceneObject
{
    friend class SceneObjectMenu;
public:
    ObjectTriangle()
    {
        m_data.object_type = 4;
    }
    SceneObject *Clone() { return new ObjectTriangle(*this); }

    bool ParamsAccept(SceneObjectMenu &visitor) override;
};