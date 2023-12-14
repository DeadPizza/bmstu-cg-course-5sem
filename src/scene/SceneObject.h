#pragma once

#include <cstdint>

class SceneObjectMenu;

class SceneObject
{
public:
    struct CommonData
    {
        int32_t object_type;
        float extra_attr[2] = { 0 };
        bool attr_flag = false;
        float attr[12] = { 0 };
    };

    int selected_material = 0;

    virtual bool ParamsAccept(SceneObjectMenu &visitor) = 0;
    virtual SceneObject *Clone() = 0;

    CommonData GetData() const { return m_data; }
    CommonData m_data;
private:
};