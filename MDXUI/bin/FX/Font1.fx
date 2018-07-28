//--------------------------------------------------------------------------//

Texture2D BoundTexture;
matrix WorldMatrix;
matrix ProjectionMatrix;

//--------------------------------------------------------------------------//


struct VSSceneIn
{
	float3 pos : POSITION;
	float4 colour : COLOR;
	float2 tex : TEXCOORD;
};

struct PSSceneIn
{
	float4 pos : SV_Position;
	float4 colour : COLOR;
	float2 tex : TEXCOORD;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

BlendState BM_NORMAL
{
	AlphaToCoverageEnable = false;
	BlendEnable[0] = true;
	SrcBlendAlpha = INV_DEST_ALPHA;		// INV_DEST_ALPHA
	DestBlendAlpha = ONE;		// ONE;
	SrcBlend = SRC_ALPHA;		// SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;		// INV_SRC_ALPHA;
};

BlendState Font_Blend
{
	AlphaToCoverageEnable = false;
	BlendEnable[0] = true;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	SrcBlend = ONE;
	DestBlend = ZERO;
};

BlendState BM_RTT_PREMULTIPLIED
{
	AlphaToCoverageEnable = false;
	BlendEnable[0] = true;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = INV_SRC_ALPHA;
	SrcBlend = ONE; 
	DestBlend = INV_SRC_ALPHA;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState BorderSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Border;
	AddressV = Border;
};

RasterizerState clipRasterstate
{
	DepthClipEnable = false;
	FillMode = Solid;
	CullMode = None;
	ScissorEnable = true;
};

RasterizerState noclipRasterstate
{
	DepthClipEnable = false;
	FillMode = Solid;
	CullMode = None;
	ScissorEnable = false;
};

// Vertex shader
PSSceneIn VSMain(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0.0;
	//output our final position
   	output.pos = float4(input.pos, 1);//mul( float4(input.pos, 1), WorldMatrix );
   	output.pos = output.pos;//mul( output.pos, ProjectionMatrix);
   	output.tex = input.tex;
	output.colour = input.colour;
	return output;
}

// Pixel shader
float4 PSMain(PSSceneIn input) : SV_Target
{
	float4 colour = BoundTexture.Sample(BorderSampler, input.tex);
	colour.rgb = 1;	
//	clip(colour.a - 0.1);
	return colour* input.colour;
}



// Techniques
technique11 BM_NORMAL_Clipped_Rendering
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSMain()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(BM_NORMAL, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);// BM_NORMAL Font_Blend
		SetRasterizerState(clipRasterstate);
	}
}

technique11 BM_NORMAL_Unclipped_Rendering
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSMain()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(BM_NORMAL, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);// BM_NORMAL
		SetRasterizerState(noclipRasterstate);
	}
}

technique11 BM_RTT_PREMULTIPLIED_Clipped_Rendering
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSMain()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(BM_RTT_PREMULTIPLIED, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(clipRasterstate);
	}
}

technique11 BM_RTT_PREMULTIPLIED_Unclipped_Rendering
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSMain()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(BM_RTT_PREMULTIPLIED, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(noclipRasterstate);
	}
}
