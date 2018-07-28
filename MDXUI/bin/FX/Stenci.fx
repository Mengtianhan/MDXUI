//--------------------------------------------------
// 平行光照的基本的信息
//--------------------------------------------------
Texture2D TexColor;
SamplerState SampleColor{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

cbuffer EyePosition
{
	float4 EyePos;
	float4 LightPos;
	float4 LidghtDir;
};

cbuffer LightColor{
	float4 MatAmbient;
	float4 MatDiffuse;
	float4 MatSpecular;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
	float MatPower;

};
cbuffer Perbuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix project_matrix;
};

struct VS_INPOT
{
	float4 Pos : POSITION;
	float3 Norm: TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm: TEXCOORD0;
	float4 ViewDirection : TEXCOORD1;
	float4 LightVector : TEXCOORD2;
};

struct VS_Input1{
	float4 Pos : POSITION;
	float2 Tex: TEXCOORD;
};

struct PS_Input1{
	float4 Pos : SV_POSITION;
	float2 Tex: TEXCOORD;
};

PS_Input1 VS_Main(VS_Input1 input)
{
	PS_Input1 psOut = (PS_Input1)0;
	psOut.Pos = mul(input.Pos, world_matrix);
	psOut.Pos = mul(psOut.Pos,view_matrix);
	psOut.Pos = mul(psOut.Pos,project_matrix);
	psOut.Tex = input.Tex;
	return psOut;
}

float4 PS_Main(PS_Input1 input) :SV_TARGET
{
	return TexColor.Sample(SampleColor,input.Tex);
}

RasterizerState CounterClockFrontRS
{
	CullMode = NONE;
	FillMode = SOLID;
	FrontCounterClockwise = true;
};

technique11 LightTech
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS_Main()) );
		SetPixelShader( CompileShader(ps_5_0, PS_Main()) );
		SetRasterizerState(CounterClockFrontRS);
	}
}