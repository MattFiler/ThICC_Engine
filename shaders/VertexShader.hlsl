struct VS_INPUT
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float4 color: COLOR;
	float2 texcoord: TEXCOORD;
};

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

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;

	output.worldPos = mul(float4(input.pos,1.0f), world);
	output.Pos = mul(output.worldPos, view);
	output.Pos = mul(output.Pos, projection);

	output.Norm = mul(input.norm, rot);

	output.Color = input.color;

	output.texCoord = input.texcoord;
	return output;
}