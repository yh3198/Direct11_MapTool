// VS
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
	float3 Texture : TEXTURE;
};
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR0;
	float3 Texture : TEXTURE;
};

// GS
struct GS_INPUT
{
	float4 Position : SV_POSITION;
	float3 Texture : TEXTURE;
};
struct GS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Texture : TEXTURE;
	uint index : SV_RenderTargetArrayIndex;
};

// PS
struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float3 Texture : TEXTURE;
	uint Index : SV_RenderTargetArrayIndex;
};

// CB
cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};

cbuffer cbFog : register(b1)
{
	float4	g_ClipPlane:	packoffset(c0);
	float4	g_vCameraPos:	packoffset(c1);
	float4	g_FogColor:		packoffset(c2);
	float	g_FogMode : packoffset(c3.x);   // = FOGMODE_LINEAR;
	float	g_FogDensity : packoffset(c3.y);
	float	g_FogStart : packoffset(c3.z);
	float	g_FogEnd : packoffset(c3.w);
}

cbuffer cbTanLight : register(b2)
{
	matrix  g_matNoraml;
	float3  cb_vLightVector;
	float3  cb_vEyePos;
	float3  cb_vEyeDir;
}

cbuffer cbObjectNeverChanges : register(b3)
{
	float4	cb_AmbientLightColor: packoffset(c0);
	float4	cb_DiffuseLightColor: packoffset(c1);
	float3	cb_SpecularLightColor: packoffset(c2);
	float	cb_SpecularPower : packoffset(c2.w);
}


VS_OUTPUT Water_VS(VS_INPUT vIn)
{
	GS_INPUT output = (GS_INPUT)0; // √ ±‚»≠
	float4 vWorldPos = mul(vIn.Position, g_matWorld);
	output.Position = vWorldPos;
	output.Texture = normalize(vIn.Position.xyz);
	return output;
}

[maxvertexcount(18)]
void Cube_GS(triangle GS_INPUT input[3], inout TriangleStream<INPUT_PS> cubeMapStream)
{

}