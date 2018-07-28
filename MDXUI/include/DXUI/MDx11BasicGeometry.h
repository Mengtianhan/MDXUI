
//------------------------------------
// 用于构造一些简单的几何图形
// 方便使用
//------------------------------------
#pragma once

#include "DxConfig.h"



class DXUI_API MDx11BasicGeometry
{
public:
	MDx11BasicGeometry();
	~MDx11BasicGeometry();

	enum DATA_TYPE{
		POSITION = 0, //顶点坐标
		NORMAL,		  //顶点法线
		TANGENT,	  //顶点切线
		TEX,		  //顶点纹理
		INDEX		  //所以数据
	};

	//------------------------------------------------
	// 几何图形的基本顶点信息
	// 不需要直接拿去用
	// 可以根据需要提取所需数据
	//-----------------------------------------------
	struct Base_Vertex
	{
		Base_Vertex(){}
		Base_Vertex(const XMFLOAT3 _pos, XMFLOAT3 _normal, XMFLOAT3 _tangent, XMFLOAT2 _tex) :
			pos(_pos), normal(_normal), tangent(_tangent), tex(_tex){}

		XMFLOAT3	pos;
		XMFLOAT3	normal;
		XMFLOAT3	tangent;
		XMFLOAT2	tex;
	};

	//-------------------------------------------------
	// 保存几何图形的顶点和索引
	//-------------------------------------------------
	struct MeshData
	{
		std::vector<Base_Vertex>	vertices;
		std::vector<UINT>			indices;
	};


	//--------------------------------------------------------------------
	// 创建一个简单的立方体
	//--------------------------------------------------------------------
	MeshData CreateBox(float width, float height, float depth, MeshData& mesh);

	//---------------------------------------------------------------------
	// 创建一个网格:width, height. With m * n sub-grids.
	//---------------------------------------------------------------------
	MeshData CreateGrid(float width, float height, UINT m, UINT n, MeshData& mesh);

	//---------------------------------------------------------------------
	// 创建一个圆柱体
	//---------------------------------------------------------------------
	MeshData CreateCylinder(float topRadius, float bottomRadius, float height, int slice, int stack, MeshData& mesh);

	//---------------------------------------------------------------------
	// 添加圆柱体的上面盖子
	//---------------------------------------------------------------------
	void  AddCylinderTopCap(float topRadius, float bottomRadius, float height, int slice, int stack, MeshData& mesh);

	//---------------------------------------------------------------------
	// 添加圆柱体的下面盖子
	//---------------------------------------------------------------------
	void AddCylinderBottomCap(float topRadius, float bottomRadius, float height, int slice, int stack, MeshData& mesh);

	//--------------------------------------------------------------------
	// 创建一个球形
	//--------------------------------------------------------------------
	MeshData CreateSphere(float radius, int slice, int stack, MeshData& mesh);
};

