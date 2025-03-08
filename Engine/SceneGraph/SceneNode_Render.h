#pragma once

#include "SceneNode.h"

class SceneNode_Render : public SceneNode
{
protected:

	virtual void OnExecute(GraphicsState& state) override;
};