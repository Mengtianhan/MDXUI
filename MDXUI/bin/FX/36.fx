//--------------------------------------------------
// 平行光照的基本的信息
//--------------------------------------------------
Texture2D TexColor;
matrix world_matrix;
matrix view_matrix;
matrix project_matrix;

SamplerState SampleColor{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


struct VS_Input{
	float4 Pos : POSITION;
	float2 Tex: TEXCOORD;
};

struct PS_Input{
	float4 Pos : SV_POSITION;
	float2 Tex: TEXCOORD;
};

PS_Input VS_Main(VS_Input input)
{
	PS_Input psOut = (PS_Input)0;
	psOut.Pos = mul(input.Pos,world_matrix);
	psOut.Pos = mul(psOut.Pos,view_matrix);
	psOut.Pos = mul(psOut.Pos,project_matrix);
	psOut.Tex = input.Tex;
	return psOut;
}

float4 PS_Main(PS_Input input) :SV_TARGET
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