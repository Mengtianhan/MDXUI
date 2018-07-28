
cbuffer EyePosition
{
	float3 EyePos;
	float4 LightPos;
};

cbuffer isReflect{
	bool b_is_reflect;
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

cbuffer PerObject
{
	float4x4 g_worldViewProj;
};

struct VertexIn
{
	float3 posL: POSITION;
	float3 normal : NORMAL;
};

struct VertexOut
{
	float4 posH: SV_POSITION;
	float3 posL: POSITION;
	float3 normal : NORMAL;
};

TextureCube g_cubeMap;

SamplerState samTexture
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posL = vin.posL;
	vout.posH = mul(float4(vin.posL,1.f), g_worldViewProj).xyww;
	vout.normal = vin.normal;//mul(vin.normal,(float3x3)g_worldViewProj);
	return vout;
}

float4 PS(VertexOut pin): SV_TARGET
{
	if(b_is_reflect){
		float3 toEye = EyePos - pin.posH.xyz;
		float dis = length(toEye);
		toEye /= dis;
		float3 incident = -toEye;
		float3 relectVector = reflect(incident,pin.normal);
		return g_cubeMap.Sample(samTexture,relectVector);
	}
	else{

		return g_cubeMap.Sample(samTexture,pin.posL);
	}
}



RasterizerState CounterClockFrontRS
{
	FrontCounterClockwise = true;
};

DepthStencilState LessEqualDSS
{
    DepthFunc = LESS_EQUAL;
};

technique11 SkyBoxTech
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetPixelShader( CompileShader(ps_5_0, PS()) );
        	SetDepthStencilState(LessEqualDSS, 0);
		SetRasterizerState(CounterClockFrontRS);
	}
}

technique11 SkyBoxTech2
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetPixelShader( CompileShader(ps_5_0, PS()) );
        	SetDepthStencilState(LessEqualDSS, 0);
		SetRasterizerState(NULL);
	}
}