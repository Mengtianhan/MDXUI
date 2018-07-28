//--------------------------------------------------------------------------//
matrix WorldMatrix;
matrix ViewProjectMatrix;
float  Width = 0.5f;
float  Height = 0.5f;

Texture2D PointTexture;
Texture2D TextTexture;
//======================================================================
// 纹理坐标
//======================================================================
cbuffer cbFixed
{
	float2 TextureCoord[4] = 
	{
		float2(0.01f, 0.99f),
		float2(0.01f, 0.01f),
		float2(0.99f, 0.99f),
		float2(0.99f, 0.01f)
	};
	float2 TextCoord[4] = 
	{
		float2(0.f, 1.f),
		float2(0.f, 0.f),
		float2(1.f, 1.f),
		float2(1.f, 0.f)
	};
};


struct VSSceneIn
{
	float3 pos : POSITION;
	float4 colour : COLOR;
};

struct VSSceneOut
{
	float3 pos : POSITION;
	float4 colour : COLOR;
};

struct PSSceneIn
{
	float4 pos    : SV_Position;
	float4 colour : COLOR;
};

struct GeoOut
{
	float4 PosH    : SV_POSITION;
    	float3 PosW    : POSITION;
	float4 colour  : COLOR;
	float2 Tex     : TEXCOORD;
    	uint   PrimID  : SV_PrimitiveID;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
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


SamplerState PointSampler
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
	ScissorEnable = false;
};


//=============================================================
// Vertex shader
//=============================================================
VSSceneOut VSMain(VSSceneIn input)
{
	VSSceneOut output = (VSSceneOut)0.0;
   	output.pos = input.pos;
	output.colour = input.colour;
	return output;
}


//=============================================================
// 几何着色器
//=============================================================
[maxvertexcount(4)]
void GS(point VSSceneOut gin[1], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GeoOut> triStream)
{	


	float halfWidth  = Width;
	float halfHeight = Height;
	float z = gin[0].pos.z;
	
	float4 v[4];
	v[0] = float4(gin[0].pos.x + halfWidth,gin[0].pos.y - halfHeight,z, 1.0f);
	v[1] = float4(gin[0].pos.x + halfWidth,gin[0].pos.y + halfHeight,z, 1.0f);
	v[2] = float4(gin[0].pos.x - halfWidth,gin[0].pos.y - halfHeight,z, 1.0f);
	v[3] = float4(gin[0].pos.x - halfWidth,gin[0].pos.y + halfHeight,z, 1.0f);
	

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH     = mul(v[i], WorldMatrix);
		gout.PosH     = mul(gout.PosH,ViewProjectMatrix);
		gout.PosW     = v[i].xyz;
		gout.colour   = gin[0].colour;
		gout.Tex      = TextureCoord[i];
		gout.PrimID   = primID;		
		triStream.Append(gout);
	}
}

[maxvertexcount(4)]
void GSForText(point VSSceneOut gin[1], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GeoOut> triStream)
{	


	float halfWidth  = Width*0.8f;
	float halfHeight = Height * 0.3f;
	float z = gin[0].pos.z;
	
	float4 v[4];
	v[0] = float4(gin[0].pos.x + halfWidth,gin[0].pos.y - halfHeight,z, 1.0f);
	v[1] = float4(gin[0].pos.x + halfWidth,gin[0].pos.y + halfHeight,z, 1.0f);
	v[2] = float4(gin[0].pos.x - halfWidth,gin[0].pos.y - halfHeight,z, 1.0f);
	v[3] = float4(gin[0].pos.x - halfWidth,gin[0].pos.y + halfHeight,z, 1.0f);
	
	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH     = mul(v[i], WorldMatrix);
		gout.PosH     = mul(gout.PosH,ViewProjectMatrix);
		gout.PosW     = v[i].xyz;
		gout.colour   = gin[0].colour;
		gout.Tex      = TextCoord[i];
		gout.PrimID   = primID;		
		triStream.Append(gout);
	}
}

//=============================================================
// Pixel shader
//=============================================================
float4 PSMain(GeoOut input) : SV_Target
{
	float4 col = PointTexture.Sample(PointSampler, input.Tex);
	if(col.r > 0.4f && col.g >= 0.4f && col.b >= 0.4f){
		col.a = 0;
	}
	else{
		col.rgb = 1.f;
	}
	return col*input.colour;
}

float4 PS(GeoOut input) : SV_Target
{
	float4 col = TextTexture.Sample(PointSampler, input.Tex);
//	col.rgb = 1.f; // 如果需要修改字体颜色 那么这里的注释必须放出来
	return col;
}

//==============================================================
// Techniques
//==============================================================
technique11 DrawPoint
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PSMain()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(Font_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(clipRasterstate);
	}
}

technique11 DrawText
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(CompileShader(gs_5_0, GSForText()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(Font_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(clipRasterstate);
	}
}