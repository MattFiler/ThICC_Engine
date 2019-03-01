struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos: POSITION;
	float4 Norm : NORMAL;
	float4 Color : COLOR;
	float2 texCoord : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix	world;
	matrix	view;
	matrix	projection;
	matrix	rot;
	float4	lightCol;
	float4	ambientCol;
	float3 lightPos;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);


float4 main(VS_OUTPUT input) : SV_TARGET
{
	// return interpolated color
	float4 vertexCol = input.Color * myTexture.Sample(mySampler, input.texCoord);
	float3 lightDir = normalize(input.worldPos - lightPos);
	float4 diffuse = saturate(max(0.0f, dot(lightDir, normalize(input.Norm))) * lightCol);
	return saturate((diffuse + ambientCol) * vertexCol);
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}