#include "Shader.h"
#include "GraphicsState.h"
#include "Model.h"

Shader::Shader(D3D& d3D, const std::wstring& VertexShaderFilename, const std::wstring& PixelShaderFilename)
{
	// Initialize the vertex and pixel shaders.
	//InitializeShader(d3D, VertexShaderFilename, PixelShaderFilename);
}

void Shader::Render(D3D& d3D, const GraphicsState& GraphicsState)
{
	// Set the vertex input layout.
	//d3D.GetDeviceContext()->IASetInputLayout(_layout.get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	d3D.GetDeviceContext()->VSSetShader(_vertexShader.get(), nullptr, 0);
	d3D.GetDeviceContext()->PSSetShader(_pixelShader.get(), nullptr, 0);

	// Set the constant buffer.
	d3D.GetDeviceContext()->VSSetConstantBuffers(0, 1, &_matrixBuffer);

	SetShaderParameters(d3D, GraphicsState);

	// Render the triangle.
	d3D.GetDeviceContext()->DrawIndexed(GraphicsState.model->GetIndexCount(), 0, 0);
}

