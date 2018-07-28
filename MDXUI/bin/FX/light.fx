//--------------------------------------------------
// 平行光照的基本的信息
//--------------------------------------------------
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


PS_INPUT VS_Main(VS_INPOT input)
{
	PS_INPUT psOut = (PS_INPUT)0;
	float4 world_pos = mul(input.Pos, world_matrix);
	psOut.ViewDirection = EyePos - world_pos;
	psOut.ViewDirection = normalize(psOut.ViewDirection);
	
	psOut.LightVector = LightPos - world_pos;
	psOut.LightVector = normalize(psOut.LightVector);
	psOut.LightVector.w = length(LightPos - world_pos);

	psOut.Pos = mul(world_pos,view_matrix);
	psOut.Pos = mul(world_pos,project_matrix);
	psOut.Norm = mul(input.Norm,world_matrix);
	psOut.Norm = mul(psOut.Norm,view_matrix);
	psOut.Norm = mul(psOut.Norm,project_matrix);
	return psOut;
}

float4 PS_Main(PS_INPUT input) :SV_TARGET
{
	float4 finalColor;
	float4 ambientColor, diffuseColor, specularColor;

	float3 lightVector = -LidghtDir.xyz;
	lightVector = normalize(lightVector);

	ambientColor = MatAmbient * LightAmbient;
	float diffuseFactor = dot(lightVector, input.Norm);
	if (diffuseFactor > 0.0){
		diffuseColor = MatDiffuse * LightDiffuse * diffuseFactor;
		float3 reflection = reflect(-lightVector, input.Norm);
		float specularFactor = pow(max(dot(reflection, input.ViewDirection.xyz), 0.0), MatPower);
		specularColor = MatSpecular * LightSpecular * specularFactor;
	}
	finalColor = saturate(ambientColor + diffuseColor + specularColor);
	return finalColor;
}


technique11 LightTech
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS_Main()) );
		SetPixelShader( CompileShader(ps_5_0, PS_Main()) );
	}
}