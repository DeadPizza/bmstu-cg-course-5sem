#pragma once

#include "scene/objects/ObjectPlane.h"
#include "scene/objects/ObjectSphere.h"
#include "scene/objects/ObjectBox.h"
#include "scene/objects/ObjectHexPrism.h"
#include "scene/objects/ObjectTriangle.h"
#include "scene/objects/ObjectThorus.h"

class SceneObjectMenu
{
public:
    SceneObjectMenu()
        : m_cur_object(0), delete_cur(false)
    {}

    bool ParamsVisit(ObjectPlane &obj);
    bool ParamsVisit(ObjectSphere &obj);
    bool ParamsVisit(ObjectBox &obj);
    bool ParamsVisit(ObjectHexPrism &obj);
    bool ParamsVisit(ObjectTriangle &obj);
    bool ParamsVisit(ObjectThorus &obj);

    void Reset() { m_cur_object = 0; delete_cur = false; clone_cur = false;}
    void ResetDeleteCur() { delete_cur = false; clone_cur = false;}
    bool ShouldDeleteCur() { return delete_cur; }
    bool ShouldCloneCur() { return clone_cur; }
private:
    int m_cur_object;
    bool delete_cur, clone_cur;
};