//=====================================================//
Texture2D MWindowTexture;
matrix   MWorldMatrix;  // 世界变换矩阵
matrix   MVPMatrix;	// 视图投影矩阵
matrix   MWVPMatrix;	// 世界变换视图投影矩阵
float3   EyePosW;
//
// 常量文件坐标
//
cbuffer cbFixed
{
	float2 gTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};

//
// 输入顶点描述
//
struct VSSceneIn
{
	float3 pos : POSITION;
	float4 colour : COLOR;
    	float2 tex : TEXCOORD;
};

//
// 顶点描述
//
struct VertexOut
{
	float3 CenterW : POSITION;
	float4 Colour  : COLOR;
	float2 SizeW   : SIZE;
};

//
// 几何顶点描述
//
struct GeoOut
{
	float4 PosH    : SV_POSITION;
	float4 Colour  : COLOR;
    float2 Tex     : TEXCOORD;
    uint   PrimID  : SV_PrimitiveID;
};

VertexOut VS(VSSceneIn vin)
{
	VertexOut vout 	= (VertexOut)0;
	vout.CenterW 	= mul(float4(vin.pos,1),MWorldMatrix).xyz;
	vout.Colour  	= vin.colour;
	vout.SizeW   	= vin.tex;
	return vout;
}

//
// 输入像素描述
//
struct PSSceneIn
{
	float4 pos : SV_Position;
	float4 colour : COLOR;
	float2 tex : TEXCOORD;
};


//
// 深度模板描述
//
DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};


//
// 深度模板描述
//
DepthStencilState LessEqualDSS
{
	DepthEnable = TRUE; 
	DepthFunc = LESS_EQUAL; 
}; 

BlendState AlphaBlend 
{
	AlphaToCoverageEnable = false; 
	BlendEnable[0] = true; 
	SrcBlendAlpha = INV_DEST_ALPHA;	
	DestBlendAlpha = ONE; 
	SrcBlend = SRC_ALPHA; 
	DestBlend = INV_SRC_ALPHA; 
};

//
// 纹理采样描述
//
SamplerState WindowSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Border;
	AddressV = Border;
};

//
// 光栅化
//
RasterizerState Rasterstate
{
	DepthClipEnable = false;
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
	ScissorEnable = false;
};

//
// 顶点着色器
//
PSSceneIn VSMain(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0.0;
	output.pos = mul(float4(input.pos,1),MWVPMatrix);
	output.tex = input.tex;
	output.colour = input.colour;
	return output;
}

//
// 像素着色器
//
float4 PSMain(PSSceneIn input) : SV_Target
{
	return input.colour;
	float4 colour = MWindowTexture.Sample(WindowSampler, input.tex);
	return input.colour*colour;
}


//
// 几何着色器
//
[maxvertexcount(4)]
void GS(point VertexOut gin[1], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{	
	//
	// 广告版技术
	//
	float3 up 	= float3(0.0f, 1.0f, 0.0f);
	float3 look 	= EyePosW - gin[0].CenterW;
	look.y 		= 0.0f; 
	look 		= normalize(look);
	float3 right 	= cross(up, look);

	//
	//计算四边形的四个顶点.
	//
	float halfWidth  = 0.5f*gin[0].SizeW.x;
	float halfHeight = 0.5f*gin[0].SizeW.y;


	
	float4 v[4];
	v[0] = float4(gin[0].CenterW + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].CenterW + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].CenterW - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].CenterW - halfWidth*right + halfHeight*up, 1.0f);

	float dx = halfWidth;
	float dy = halfHeight;
	float __x = gin[0].CenterW.x;
	float __y = gin[0].CenterW.y;
	float __z = gin[0].CenterW.z;

	v[0] = float4(__x - dx ,__y + dy,__z,1.0f);
	v[1] = float4(__x + dx ,__y + dy,__z,1.0f);
	v[2] = float4(__x - dx ,__y - dy,__z,1.0f);
	v[3] = float4(__x + dx ,__y - dy,__z,1.0f);

	GeoOut gout = (GeoOut)0;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH	  = mul(v[i], MVPMatrix);
		gout.Colour   = gin[0].Colour;
		gout.Tex      = gTexC[i];
		gout.PrimID   = primID;
		triStream.Append(gout);
	}
}

//
// 像素着色器
//
float4 PS(GeoOut input) : SV_Target
{
	return input.Colour;
}

//
// 通道
//
technique11 MDx11WindowTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSMain())); 
		SetGeometryShader(NULL); 
		SetPixelShader(CompileShader(ps_5_0, PSMain())); 
		SetDepthStencilState(LessEqualDSS, 0); 
		SetRasterizerState(Rasterstate); 
	}
}

technique11 MDx11WindowTechWithGS
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
        SetDepthStencilState(DisableDepth, 0);
		SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFFFFFFFF);
		SetRasterizerState(Rasterstate); 
    }
}


