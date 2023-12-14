#pragma once
#include "scene/SceneObject.h"

class ObjectSphere : public SceneObject
{
    friend class SceneObjectMenu;
public:
    ObjectSphere()
    {
        m_data.object_type = 0;
    }

    SceneObject *Clone() {return new ObjectSphere(*this);}

    bool ParamsAccept(SceneObjectMenu &visitor) override;
};