#include "DxOpenCvWindow.h"
#include <DxResource.h>

using namespace DxUI;
#ifdef _DEBUG
#pragma comment(lib,"opencv_core343d")
#pragma comment(lib,"opencv_imgcodecs343d")
#pragma comment(lib,"opencv_highgui343d")
#pragma comment(lib,"opencv_video343d")
#pragma comment(lib,"opencv_videoio343d")
#pragma comment(lib,"opencv_videostab343d")
#else
#pragma comment(lib,"opencv_core343")
#pragma comment(lib,"opencv_imgcodecs343")
#pragma comment(lib,"opencv_highgui343")
#pragma comment(lib,"opencv_video343")
#pragma comment(lib,"opencv_videoio343")
#pragma comment(lib,"opencv_videostab343")
#endif

CDxOpenCvWindow::CDxOpenCvWindow()
{
	mClearColor = RgbI(255, 255, 255);
	mEffects.SetColor(Dx_Normal, RgbI(255, 255, 255));
	mEffects.SetColor(Dx_Hover, RgbI(255, 255, 255));
	mEffects.SetColor(Dx_Clicked, RgbI(255, 255, 255));
}


CDxOpenCvWindow::~CDxOpenCvWindow()
{
}


void CDxOpenCvWindow::ShowMat(const cv::Mat& mat){
	std::unique_lock<std::recursive_mutex> lock(m_mtx);
	safe_release(p_Bitmap);
	m_Mat = mat;
	UpdateBitmap();
	if (p_Bitmap){
		OnPainterEvent();
	}
}

void CDxOpenCvWindow::SaveToFile(const MString& FileName){
	std::unique_lock<std::recursive_mutex> lock(m_mtx);
	if (p_Bitmap == nullptr){
		return;
	}
	if (m_Mat.empty()){
		return;
	}
	using namespace cv;
	int channel = m_Mat.channels();
	int h = m_Mat.rows;
	int w = m_Mat.cols;
	BYTE* ptr = m_Mat.data;
	bool isConitnue = m_Mat.isContinuous();
	DWORD dwSize = w * h * 4;
	BYTE* m_pBits = (BYTE *)malloc(dwSize);
	memset(m_pBits, 0, dwSize);
	BYTE *pPiexl;

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			pPiexl = LPBYTE(m_pBits + y * w * 4 + x * 4);
			if (channel == 3){
				Vec3b col = m_Mat.ptr<Vec3b>(y)[x];
				pPiexl[0] = col(2);
				pPiexl[1] = col(1);
				pPiexl[2] = col(0);
				pPiexl[3] = 255;
			}
			else if (channel == 4){
				Vec4b col = m_Mat.ptr<Vec4b>(y)[x];
				pPiexl[0] = col(2);
				pPiexl[1] = col(1);
				pPiexl[2] = col(0);
				pPiexl[3] = col(3);
			}
		}
	}


	

	IWICBitmap* pWicBitmap = nullptr;
	CDxResource::GetWICFactory()->CreateBitmapFromMemory(
		w, 
		h, 
		GUID_WICPixelFormat32bppPRGBA,
		w * 4,
		dwSize,
		m_pBits,
		&pWicBitmap);
	free(m_pBits);
	
	IWICStream* pStream{ nullptr };
	CDxResource::GetWICFactory()->CreateStream(&pStream);
	if (pStream == nullptr)
		return;

	pStream->InitializeFromFilename(FileName.c_wstr(), GENERIC_WRITE);
	IWICBitmapEncoder* Encoder{ nullptr };
	IWICBitmapFrameEncode* FrameEncoder{ nullptr };
	CDxResource::GetWICFactory()->CreateEncoder(GUID_ContainerFormatPng, NULL, &Encoder);
	if (Encoder == nullptr)
		return;

	Encoder->Initialize(pStream, WICBitmapEncoderNoCache);
	Encoder->CreateNewFrame(&FrameEncoder, NULL);
	FrameEncoder->Initialize(NULL);
	FrameEncoder->SetSize(w, h);
	WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
	FrameEncoder->SetPixelFormat(&format);
	FrameEncoder->WriteSource(pWicBitmap, NULL);
	FrameEncoder->Commit();
	Encoder->Commit();
	safe_release(pWicBitmap);
	safe_release(FrameEncoder);
	safe_release(Encoder);
	safe_release(pStream);
}

void CDxOpenCvWindow::UpdateBitmap(){
	if (p_CachRendTargert == nullptr){
		return;
	}
	using namespace cv;
	safe_release(p_Bitmap);
	if (m_Mat.empty()){
		return;
	}
	int channel = m_Mat.channels();
	int h = m_Mat.rows;
	int w = m_Mat.cols;
	BYTE* ptr = m_Mat.data;
	bool isConitnue = m_Mat.isContinuous();
	DWORD dwSize = w * h * 4;
	BYTE* m_pBits = (BYTE *)malloc(dwSize);
	memset(m_pBits, 0, dwSize);
	BYTE *pPiexl;

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			pPiexl = LPBYTE(m_pBits + y * w * 4 + x * 4);
			if (channel == 3){
				Vec3b col = m_Mat.ptr<Vec3b>(y)[x];
				pPiexl[0] = col(0);
				pPiexl[1] = col(1);
				pPiexl[2] = col(2);
				pPiexl[3] = 255;
			}
			else if (channel == 4){
				Vec4b col = m_Mat.ptr<Vec4b>(y)[x];
				pPiexl[0] = col(0);
				pPiexl[1] = col(1);
				pPiexl[2] = col(2);
				pPiexl[3] = col(3);
			}
		}
	}

	HRESULT hr = p_CachRendTargert->CreateBitmap(
		D2D1::SizeU(w, h),
		m_pBits,
		w * 4,
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
		&p_Bitmap);// 
	if (FAILED(hr)){
		DxTRACE(L"´´½¨Î»Í¼Ê§°Ü[%1]\n", hr);
	}
	free(m_pBits);
}

void CDxOpenCvWindow::OnRendWindow(IPainterInterface* painter){
	std::unique_lock<std::recursive_mutex> lock(m_mtx);
	if (painter->GetPainterEnginer() == nullptr){
		return;
	}
	if (p_CachRendTargert != painter->GetPainterEnginer()){
		p_CachRendTargert = (ID2D1RenderTarget*)(painter->GetPainterEnginer());
		UpdateBitmap();
	}
	if (p_Bitmap == nullptr){
		UpdateBitmap();
	}
	if (p_Bitmap == nullptr){
		return;
	}
	p_CachRendTargert->DrawBitmap(p_Bitmap,
		D2D1::RectF(mRendArea.Left(), mRendArea.Top(),
		mRendArea.Right(), mRendArea.Bottom()));
}