Texture2D Texture0[4] : register (t0);		// �ؽ��� �������� 0���� ���� �������ڴ�.
Texture2D AlphaTexture : register (t1);		// �ؽ��� �������� 0���� ���� �������ڴ�.
Texture2D AlphaImageTex1 : register (t2);
Texture2D AlphaImageTex2 : register (t3);
Texture2D AlphaImageTex3 : register (t4);
Texture2D AlphaImageTex4 : register (t5);


SamplerState Sample0 : register (s0);	// �ؽ��ĸ� ��Ʈ�� �ϱ� ���� ����

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // ���ؽ� ������
	float3 Normal : NORMAL;		   // ���ؽ� ���
	float4 Diffuse : COLOR0;	   // ���ؽ� ����
	float2 Texture : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 c : SV_Target;
};

VS_OUTPUT VS(in float3 p : POSITION, in float3 n : NORMAL, in float4 c : COLOR, in float2 t : TEXTURE)
{
	VS_OUTPUT Output;
	float fColor = p.x / g_MeshColor.y;
	Output.Diffuse = float4(fColor, fColor, fColor, fColor);
	Output.Position = float4(p, 1.0f);
	Output.Texture = t;
	Output.Normal = n;
	return Output;
}
//Texture0[0].Sample(Sample0, input.Texture);
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 test = float4(0.0f, 0.0f, 0.0f, 0.0f);
	return test;
	//return input.Diffuse;
}