#pragma once
#include "MDirectPlotImpl.h"
#include "MDx11Point.h"
#include "xnacollision.h"
#include "MDcartplot3d.h"

using DxUI::BasicPC;
namespace DxUI{
	class MFTFont;
}
using DxUI::MFTFont;
//+--------------------------------
//
// class MUnitWindow
// 泡泡龙
//
//+--------------------------------
class DXUI_API MUnitWindow :
	public MDcartplot3d
{
	DECLARE_CLASS(MUnitWindow)

public:
	struct MVector3D{
		float x;
		float y;
		float z;
	};

	struct TextType{
		ID3D11ShaderResourceView* Tex{ nullptr };
		int width{ 0 };
		int height{ 0 };
	};

	typedef std::function<void(unsigned)>           ClickPointFun;

public:
	MUnitWindow();
	~MUnitWindow();

	//
	// 绘制点的函数集
	//
	virtual bool	LondUnitContent(){ return true; }
	void			SetPointBitmap(const MString& bitmap);
	void			SetOrgColor(float r, float g, float b, float a);
	void			SetPointColor(unsigned id, float r, float g, float b);
	void			LoadPointFromFile(const MString& unitfilename);
	void			SetPointRadius(float radius);
	void			SetPoint(std::vector<MDx11::DXUnitPointType> &points);
	void			SetPoint(std::vector<MVector3D> &points, unsigned baseid = 1);
	void			SetPointData(unsigned id, float value);
	void			BindClickPointFun(ClickPointFun fun);
	float			GetValueFromId(unsigned id) const;
	unsigned		GetPointsCount() const;
	void			ClearPoint();
	void			ClearPointColor();
	void			IsNeedShowPointInfo(bool isshow);
	void			IsNeedShowPointId(bool isshow);

protected:
	bool OnInitResource3D();
	bool BuidVertex();
	void readData(const MString& filename);
	void picked(float x, float y);
	void Frustum();
	void updateTetxInfo();
	void OnRender();
	void OnRender3D();
	void UnInitResource();
	void OnUpdateBitmap();
	void ReGenTextureForText(int width, int height);
	void OnPaintEventFor2D(IPainterInterface* painter);
	void OnMouseEvent(const DxUI::CDxMouseEvent& e);
	
private:
	ID3D11Buffer*               pPointVertexBuffer{ nullptr };
	ID3D11InputLayout*          pPointInputLayout{ nullptr };\
	MDx11LayoutManage*          pPointLayoutManager{ nullptr };
	unsigned                    mPointCount;  // 单元点数
	unsigned                    mCurrentBufferSize;
	float                       mZvalue{ 1.f };
	float                       mRadius{ 10.f };
	unsigned                    mPickedIndex{ -1 };  // 检查鼠标是否点中单元
	XMFLOAT4X4                  mPointWorldMatrix;         // 世界变换矩阵
	XMFLOAT4                    mOrgColor;
	XMFLOAT4                    mTextColor;
	XNA::AxisAlignedBox         mPointMeshBox;        // 矩形包装框
	XNA::Frustum                mPointCamFrustum;     // 裁剪框
	bool                        bIsNeedID{ true };
	bool                        bIsShowMsg{ true };


	ID2D1Bitmap*				pBitmap{ nullptr }; // 彩虹条
	ID3D11ShaderResourceView*   pRoundSRV{ nullptr };// 圆形纹理
	MString					    mRoundSRVBitmap;

	std::map<unsigned, BasicPC> mOrgVertexs;     // 原始顶点数据
	std::map<unsigned, BasicPC> mLVertexs;     // 未变换的顶点数据
	std::map<unsigned, BasicPC> mVisualPoints; // 保存可见的点
	std::map<unsigned, float>   mPointData;    // 保存测试单元的数据，比如幅值，比如相位
	std::map<unsigned, XMFLOAT4> mPointColors;
	std::map<mj::MString, TextType> mTextTextureInfo;
	std::map<mj::MString, TextType> mHTextTextureInfo; // 变换后的文本信息
	ClickPointFun               mClickFun{ nullptr };

	double                      mXOffset{ 0.0 };
	double                      mYOffset{ 0.0 };
	float                       mPointWidth{ 0.5 };
	float                       mPointHeight{ 0.5 };
	MFTFont*					p_DxFont{ nullptr };
};

