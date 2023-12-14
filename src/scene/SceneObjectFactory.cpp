#include "scene/SceneObjectFactory.h"

#include "scene/objects/ObjectSphere.h"
#include "scene/objects/ObjectPlane.h"
#include "scene/objects/ObjectBox.h"
#include "scene/objects/ObjectHexPrism.h"
#include "scene/objects/ObjectTriangle.h"
#include "scene/objects/ObjectThorus.h"


const char* SceneObjectTypeLiterals[] = {
    "Сфера",
    "Плоскость",
    "Прямоугольный параллелепипед",
    "Шестиугольная призма",
    "Треугольник",
    "Тор"};

std::unique_ptr<SceneObject> generateSceneObject(SceneObjectType obj_type)
{
    switch (obj_type)
    {
        case SceneObjectType::SPHERE: return std::make_unique<ObjectSphere>();
        case SceneObjectType::PLANE: return std::make_unique<ObjectPlane>();
        case SceneObjectType::BOX: return std::make_unique<ObjectBox>();
        case SceneObjectType::HEX_PRISM: return std::make_unique<ObjectHexPrism>();
        case SceneObjectType::TRIANGLE: return std::make_unique<ObjectTriangle>();
        case SceneObjectType::THORUS: return std::make_unique<ObjectThorus>();
    }
    return nullptr;
}