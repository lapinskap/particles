// constant buffer for the matrices, legacy from DirectX 8/9
cbuffer MatrixBuffer
{
    matrix worldMatrix[2];
    matrix viewMatrix;
    matrix projectionMatrix; 
};

cbuffer ViewBuffer
{
    float3 viewerPosition;
};

struct VertexInput
{
    float4 position: POSITION;
    float4 color: COLOR;
    float4 normal: NORMAL;
    uint instanceID: SV_INSTANCEID;
};

// TODO: learn about alignment and padding in HLSL
struct PixelInput
{
    float4 color: COLOR;
    float4 position: SV_POSITION;
    float4 normal: NORMAL;
    float3 viewDirection: VIEW_DIR;
};

PixelInput LightVertexShader(VertexInput input)
{
    PixelInput output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix[input.instanceID]);

    output.viewDirection = viewerPosition - output.position.xyz;
    output.viewDirection = normalize(output.viewDirection);

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the normal vector against the world matrix only.
    float4 normal = mul(input.normal, worldMatrix[input.instanceID]);
	
    // Normalize the normal vector.
    output.normal = normalize(normal);

    output.color = input.color;

    return output;
}
