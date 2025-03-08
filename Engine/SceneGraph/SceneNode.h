#pragma once

#include "Common.h"

struct GraphicsState;

class SceneNode
{
public:

	virtual ~SceneNode() = default;

	std::vector<SceneNode*>& GetChildren() { return _children; }
	void Execute(GraphicsState& State);

protected:

	virtual void OnExecute(GraphicsState& State) = 0;

	std::vector<SceneNode*> _children;
};