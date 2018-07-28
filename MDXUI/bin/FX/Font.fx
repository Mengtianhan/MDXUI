//--------------------------------------------------------------------------//

Texture2D FontTexture;

matrix WorldMatrix;
matrix ViewProjectMatrix;

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
	DepthEnable =  FALSE;
	DepthWriteMask = ZERO;
};

BlendState Font_Blend
{
	AlphaToCoverageEnable = false;
	BlendEnable[0] = true;
	SrcBlendAlpha = INV_DEST_ALPHA;	
	DestBlendAlpha = ONE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};


SamplerState FontSampler
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
	ScissorEnable = false;//true;
};



// Vertex shader
PSSceneIn VSMain(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0.0;
   	output.pos = mul(float4(input.pos, 1.f),WorldMatrix);
//	output.pos = mul(output.pos,ViewProjectMatrix);
   	output.tex = input.tex;
	output.colour = input.colour;
	return output;
}

// Pixel shader
float4 PSMain(PSSceneIn input) : SV_Target
{
	float4 colour = FontTexture.Sample(FontSampler, input.tex);
	colour.rgb = 1;	
	return colour* input.colour;
}

float4 PSFrame(PSSceneIn input) : SV_Target
{
	return input.colour;
}

float4 PSFrameHeader(PSSceneIn input) : SV_Target
{
	float4 colour = FontTexture.Sample(FontSampler, input.tex);
	if(colour.r < 0.2 && colour.g < 0.2 && colour.b <0.2)
		colour.rgb = 1.f;
	input.colour.a = 1.f;
	return colour;//*input.colour;
}


// Techniques
technique11 FontRenderTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSMain()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(Font_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(clipRasterstate);
	}
}

// Frame
technique11 FontFrameTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSFrame()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(Font_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(clipRasterstate);
	}
}

// FrameHeader
technique11 FontFrameHeaderTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSFrameHeader()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(Font_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(clipRasterstate);
	}
}