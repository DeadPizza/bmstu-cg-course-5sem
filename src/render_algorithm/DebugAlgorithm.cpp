#include "render_algorithm/DebugAlgorithm.h"

#include "render_algorithm/RenderAlgorithmVisitor.h"

void DebugAlgorithm::Init(Application &app)
{
}

void DebugAlgorithm::Update(Application &app)
{
}

bool DebugAlgorithm::Accept(RenderAlgorithmVisitor &visitor)
{
    return visitor.Visit(*this);
}