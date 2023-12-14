#pragma once

#include "ui/UIWindow.h"

#include "render_algorithm/RenderAlgorithm.h"

class UIAlgorithmProperties : public UIWindow
{
public:
    void Init(Application &app) override;
    bool Update(Application &app) override;

private:
    std::vector<std::shared_ptr<RenderAlgorithm>> m_render_algos;
    int m_cur_algo;
};