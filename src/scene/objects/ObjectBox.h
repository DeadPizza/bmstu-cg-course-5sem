#pragma once
#include "scene/SceneObject.h"

class ObjectBox : public SceneObject
{
    friend class SceneObjectMenu;
public:
    ObjectBox()
    {
        m_data.object_type = 2;
    }

    SceneObject *Clone() {return new ObjectBox(*this);}
    
    bool ParamsAccept(SceneObjectMenu &visitor) override;
};