#include "SceneNode_Transform.h"
#include "SceneTraversalState.h"
#include "SceneData.h"

void SceneNode_Transform::PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>&)
{
	state.transform = transform * state.transform;
}