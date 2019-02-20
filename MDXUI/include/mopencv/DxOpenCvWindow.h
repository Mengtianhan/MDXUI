#pragma once
#include <opencv2/core/mat.hpp>
#include "DxWindow.h"


namespace DxUI{
	//+-----------------------------------
	//
	// 用于支持 opencv
	//
	//+-----------------------------------
	class CDxOpenCvWindow :
		public CDxWindow
	{
	public:
		CDxOpenCvWindow();
		~CDxOpenCvWindow();
		void	ShowMat(const cv::Mat& mat);
		void	SaveToFile(const MString& FileName);
	protected:
		virtual void UpdateBitmap();
		void OnRendWindow(IPainterInterface* painter);
	private:
		ID2D1Bitmap*		p_Bitmap{ nullptr };
		ID2D1RenderTarget*	p_CachRendTargert{ nullptr };
		cv::Mat				m_Mat;
		std::recursive_mutex m_mtx;
	};
}


