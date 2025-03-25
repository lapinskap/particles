#include "SceneNode.h"

SceneNode::~SceneNode()
{
	for (auto child : _children)
	{
		delete child;
	}
}

SceneNode* SceneNode::AddChild(SceneNode* node)
{
	_children.push_back(node);
	return node;
}