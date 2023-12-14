#pragma once

#include "render_algorithm/DebugAlgorithm.h"
#include "render_algorithm/RayTracingAlgorithm.h"
#include "render_algorithm/BPTAlgorithm.h"

class RenderAlgorithmVisitor
{
public:
    bool Visit(DebugAlgorithm &algo);
    bool Visit(RayTracingAlgorithm &algo);
    bool Visit(BPTAlgorithm &algo);
};