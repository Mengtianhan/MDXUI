//--------------------------------------------------------------------------//
Texture2D MWindowTexture;
matrix   MInstanceWorldMatrix;
matrix   MWorldMatrix;
matrix   MWVPMatrix;
matrix	 MMeshObjMatrix;
matrix   ViewMatrix;
matrix   ProjectMatrix;
matrix   ViewProjectMatrix;


struct VSSceneIn
{
    float3 pos : POSITION;
    float4 colour : COLOR;
    float2 tex : TEXCOORD;
};

struct VSObjMeshIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PSSceneIn
{
    float4 pos : SV_Position;
    float4 colour : COLOR;
    float2 tex : TEXCOORD;
};


struct PSObjMeshIn
{
    float4 PosH : SV_Position;
    float3 PosW  : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
};

//
// 针对2D绘制
//
struct VS2DIn
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct PS2DIn
{
    float4 PosH : SV_Position;
    float2 tex : TEXCOORD;
};


//
// 深度模板缓冲期
//
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState LessEqualDSS
{
    DepthEnable = TRUE; 
    DepthFunc = LESS_EQUAL; 
}; 


//
// 纹理采样器
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
    CullMode = None;
    FrontCounterClockwise = false;
    ScissorEnable = false;
};


RasterizerState RasterstateForObj
{
    DepthClipEnable = true;
    FillMode = Solid;
    CullMode = Back;
    FrontCounterClockwise = false;
    ScissorEnable = false;
};

RasterizerState RasterstateCWcullMode
{
    DepthClipEnable = false;
    FillMode = Solid;
    CullMode = None;
    FrontCounterClockwise = false;
    ScissorEnable = false;
};

RasterizerState RasterstateMeshMode
{
    DepthClipEnable = false;
    FillMode = Solid;//Wireframe;
    CullMode = Back;//;Back
    FrontCounterClockwise = false;
    ScissorEnable = false;
};

//
// 绘制背面
//
RasterizerState RasterstateBack
{
    DepthClipEnable = true;
    FillMode = Solid;//Wireframe;
    CullMode = Front;//;Back
    FrontCounterClockwise = false;
    ScissorEnable = false;
};

//
// 绘制前面
//
RasterizerState RasterstateFront
{
    DepthClipEnable = true;
    FillMode = Solid;//Wireframe;
    CullMode = Back;//;Back
    FrontCounterClockwise = false;
    ScissorEnable = false;
};

RasterizerState RasterstateDouble
{
    DepthClipEnable = true;
    FillMode = Solid;//Wireframe;
    CullMode = None;//;Back
    FrontCounterClockwise = false;
    ScissorEnable = false;
};

//
// Vertex shader
//
PSSceneIn VSMain(VSSceneIn input)
{
    PSSceneIn output = (PSSceneIn)0.0;
    output.pos = mul(float4(input.pos,1),MInstanceWorldMatrix);
    output.pos = mul(output.pos,MWorldMatrix);
    output.tex = input.tex;
    output.colour = input.colour;
    return output;
}

//
// VSMesh
//
PSSceneIn VSMesh(VSSceneIn input)
{
    PSSceneIn output = (PSSceneIn)0.0;
    output.pos = mul(float4(input.pos,1),MMeshObjMatrix);
    output.pos = mul(output.pos,ViewProjectMatrix);
    output.tex = input.tex;
    output.colour = input.colour;
    return output;
}

PSSceneIn VSThreeMesh(VSSceneIn input)
{
    PSSceneIn output = (PSSceneIn)0.0;
    output.pos = mul(float4(input.pos,1),MMeshObjMatrix); 
    output.pos = mul(output.pos,ViewProjectMatrix);
    output.tex = input.tex;
    output.colour = input.colour;
    return output;
}

//
// VSObjectMesh
//
PSObjMeshIn VSObjectMesh(VSObjMeshIn input)
{
    PSObjMeshIn output = (PSObjMeshIn)0.0;
    output.PosH = mul(float4(input.pos,1),MMeshObjMatrix);
    output.PosH = mul(output.PosH,MWorldMatrix);
    output.Tex = input.tex;
    output.Normal = mul(input.normal, (float3x3)MMeshObjMatrix);
    output.Normal = mul(output.Normal, (float3x3)MWorldMatrix);
    return output;
}


//
// PSMain 
//
float4 PSMain(PSSceneIn input) : SV_Target
{
    float4 colour = MWindowTexture.Sample(WindowSampler, input.tex);
    return input.colour*colour;
}

//
// PSMesh
//
float4 PSMesh(PSSceneIn input) : SV_Target
{
    float4 colour = MWindowTexture.Sample(WindowSampler, input.tex); 
    return input.colour;//*colour; 
}


float4 PSMeshWithTex(PSSceneIn input) : SV_Target
{
     float4 colour = MWindowTexture.Sample(WindowSampler, input.tex); 
     return input.colour*colour; 
}

//
// PSObjectMesh
//
float4 PSObjectMesh(PSObjMeshIn input) : SV_Target
{
    float4 colour = MWindowTexture.Sample(WindowSampler, input.Tex);
    return colour;
}

//
// 2D定点渲染
//
PS2DIn VSTwoMain(VS2DIn input)
{
    PS2DIn output = (PS2DIn)0.0;
    output.PosH = mul(float4(input.pos,1),MWorldMatrix);
    output.tex = input.tex;
    return output;
}

float4 PSTwoMain(PS2DIn input):SV_Target
{
    float4 colour = MWindowTexture.Sample(WindowSampler, input.tex);
    return colour;
}

//
// 3D顶点渲染
//
PSSceneIn VSThreeMain(VSSceneIn input)
{
    PSSceneIn output = (PSSceneIn)0.0;
    output.pos = mul(float4(input.pos,1), MMeshObjMatrix);
    output.pos = mul(output.pos,ViewProjectMatrix); 
    output.tex = input.tex;
    output.colour = input.colour;
    return output;
}

float4 PSThreeMain(PSSceneIn input):SV_Target
{
    float4 colour = MWindowTexture.Sample(WindowSampler, input.tex);
    return colour;
    return input.colour;
}

//
float4 PSThreeMainNoTex(PSSceneIn input):SV_Target
{
   return input.colour;
}

//
// Techniques
//
technique11 MDx11WindowTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetGeometryShader(NULL); 
        SetPixelShader(CompileShader(ps_5_0, PSMain())); 
        SetDepthStencilState(DisableDepth, 0); 
        SetRasterizerState(Rasterstate); 
    }
}

//
// 绘制3D对象
//
technique11 DrawMesh
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSMesh()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMesh()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateForObj); 
    }
}

//
// 绘制3D对象
//
technique11 Draw3DMesh
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMesh()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMesh()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateForObj); 
    }
}

technique11 Draw3DMeshDouble
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSThreeMesh()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSMesh()));
		SetDepthStencilState(LessEqualDSS, 0);
		SetRasterizerState(RasterstateDouble); // RasterstateForObj
	}
}


technique11 Draw3DMeshWitchTex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMesh()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMeshWithTex()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateForObj); 
    }
}

//
// 绘制对象
//
technique11 DrawObjectMesh
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSObjectMesh()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSObjectMesh()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateForObj); 
    }
}

//
// 绘制2D平面
//
technique11 Draw2DScanne
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSTwoMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSTwoMain()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateCWcullMode); 
    }
}

//
// 绘制3D平面
//
technique11 Draw3DScanne
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSThreeMain()));
        SetDepthStencilState(DisableDepth, 0);
        SetRasterizerState(RasterstateMeshMode);
    }
}

//
// 绘制3D平面无纹理
//
technique11 Draw3DScanneNoTex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSThreeMainNoTex()));
        SetDepthStencilState(DisableDepth, 0);
        SetBlendState(NULL, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetRasterizerState(RasterstateMeshMode);
    }
}
//
// 只绘制前面
//
technique11 DrawFrontScanne
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSThreeMain()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateFront);
    }
}

//
// 只绘制背面
//
technique11 DrawBackScanne
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSThreeMain()));
        SetDepthStencilState(LessEqualDSS, 0);
        SetRasterizerState(RasterstateBack); 
    }
}

//
// 绘制双面
//
technique11 DrawDoubleScanne
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSThreeMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSThreeMain()));
        SetDepthStencilState(DisableDepth, 0);
        SetRasterizerState(RasterstateDouble);
    }
}
