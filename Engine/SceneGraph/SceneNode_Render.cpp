#include "SceneNode_Render.h"
#include "GraphicsState.h"
#include "Shader.h"

void SceneNode_Render::OnExecute(GraphicsState& state)
{
	if (state.shader)
	{
		state.shader->Render(*state.d3D, state);
	}
}