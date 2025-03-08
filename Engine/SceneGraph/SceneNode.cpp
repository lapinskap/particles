#include "SceneNode.h"
#include "GraphicsState.h"

void SceneNode::Execute(GraphicsState& State)
{
	OnExecute(State);

	if (_children.size() == 1)
	{
		 _children[0]->Execute(State);
		 return;
	}

	for (auto& child : _children)
	{
		GraphicsState StateCopy = State;
		child->Execute(StateCopy);
	}
}