#pragma once

#include <memory>

#include "scene/SceneObject.h"

enum class SceneObjectType
{
    SPHERE,
    PLANE,
    BOX,
    HEX_PRISM,
    TRIANGLE,
    THORUS
};

extern const char* SceneObjectTypeLiterals[6];

std::unique_ptr<SceneObject> generateSceneObject(SceneObjectType obj_type);