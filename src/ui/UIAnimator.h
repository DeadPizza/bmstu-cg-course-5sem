#pragma once

#include <vector>
#include <memory>
#include <string>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "ui/UIWindow.h"

class UIAnimator : public UIWindow
{
    struct KeyFrame
    {
        glm::vec3 position;
        glm::quat rotation;
    };

public:
    void Init(Application &app) override;
    bool Update(Application &app) override;

    void SaveState(std::ostream &f) override;
    void RestoreState(std::istream &f) override;

private:
    std::vector<KeyFrame> m_key_frames;
    double begin_time;
};