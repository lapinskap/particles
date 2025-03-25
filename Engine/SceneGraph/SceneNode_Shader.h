#pragma once

#include "SceneNode.h"

class Shader;

class SceneNode_Shader : public SceneNode
{
public:

	SceneNode_Shader(std::shared_ptr<Shader> shader);
	void PreInitialize(D3D& d3D, std::unique_ptr<SceneData>& sceneData) override;
	void PostInitialize(std::unique_ptr<SceneData>& sceneData) override;

	// Shader creates the scene description with the data it will need
	
	void PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData) override;
	void PostExecute(const std::unique_ptr<SceneData>& sceneData) override;

protected:

	D3D* _d3D = nullptr;
	std::shared_ptr<Shader> _shader;
};