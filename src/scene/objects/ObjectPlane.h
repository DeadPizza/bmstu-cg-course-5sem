#pragma once
#include "scene/SceneObject.h"

class ObjectPlane : public SceneObject
{
    friend class SceneObjectMenu;
public:
    ObjectPlane()
    {
        m_data.object_type = 1;
    }
    SceneObject *Clone() {return new ObjectPlane(*this);}

    bool ParamsAccept(SceneObjectMenu &visitor) override;
};