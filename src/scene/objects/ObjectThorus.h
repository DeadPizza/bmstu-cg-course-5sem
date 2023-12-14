#pragma once
#include "scene/SceneObject.h"

class ObjectThorus : public SceneObject
{
    friend class SceneObjectMenu;
public:
    ObjectThorus()
    {
        m_data.object_type = 5;
    }
        SceneObject *Clone() {return new ObjectThorus(*this);}
    
    bool ParamsAccept(SceneObjectMenu &visitor) override;
};