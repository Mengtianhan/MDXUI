### MDXUI
一个基于DirectX11的DirectUI框架，几乎涵盖有常有的GUI组件，支持3D模型的渲染，具体的使用文档关注微信公众号《C/C++编程教室》
由于一些别的原因，此处不提供源代码，提供编译好的x86和x64版本二进制文件，基于win10+vs2013
使用的DX版本为：DXSDK_Feb10
在使用该框架前先安装DX运行库，可以直接下载DXSDK_Feb10.exe默认安装就好


### 怎么使用C++进行编程
* 一个最简单的创建程序
```
#define DXUI_WINDOW
#include <DxApplication.h>
#include <DxMainWindow.h>
using namespace DxUI;

//
// 入口函数
//
int main(int argc, char* argv[])
{
	CDxApplication* App = CDxApplication::Instance();
	CDxMainWindow Window;
	Window.Show();
	return App->Run();
}
```
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/10.PNG)

* 添加一个二维曲线的绘图窗口
```
#define DXUI_WINDOW
#include <DxApplication.h>
#include <DxMainWindow.h>
#include <DxPlotWindow.h>
using namespace DxUI;

int main(int argc, char* argv[])
{
	CDxApplication* App = CDxApplication::Instance();
	CDxMainWindow Window;
	TL::Vector<double> xs, ys;
	xs = xs.range(-180, 180,1);
	ys = xs.tranform_copy([](double v){return std::sin(v / 180 * mj::PI); });
	CDxPlotWindow PlotWindow;
	PlotWindow.PlotLine("sin", xs, ys);
	Window.SetCentralWidget(&PlotWindow);
	Window.Show();
	return App->Run();
}
```
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/11.PNG)
```
#define DXUI_WINDOW
#include <DxApplication.h>
#include <DxMainWindow.h>
#include <DxPlotWindow.h>
using namespace DxUI;

int main(int argc, char* argv[])
{
	CDxApplication* App = CDxApplication::Instance();
	CDxMainWindow Window;
	TL::Vector<double> xs, ys;
	xs = xs.range(-180, 180,1);
	ys = xs.tranform_copy([](double v){return std::sin(v / 180 * mj::PI); });
	CDxPlotWindow PlotWindow;
  PlotWindow.SetExtendEnabled(false);
	PlotWindow.PlotLine("sin", xs, ys);
	Window.SetCentralWidget(&PlotWindow);
	Window.Show();
	return App->Run();
}
```
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/12.PNG)

### 使用lua脚本进行编程
```
require("include.DxUI")
require("include.filesystem")
require("include.matrix")

window = {}
list = {}

btn = {}
plot = {}
sphere = {}
plane = {}
point = {}
tabwindow = {}


function ListEvent(curIndex,PreIndex)
	if curIndex == -1 then
		return
	end
	InfoBox(list:GetTextByIndex(curIndex),window)
end


function Clicked(isClicled)
	if isClicled == false then
		return
	end
	local curWindow = tabwindow:CurrentWindow()
	curWindow:SaveToFile()
end

	

function init()
	App = DxUI:App()
	window = DxUI:Widget("CDxWidget")
	tabwindow = DxUI:Widget("CDxTabWidget")
	window:SetSize(800,600)
	window:SetCaptionHeight(40)
	window:SetSizeBox({2,0,2,2})
	local ef = window:GetEffects()
	ef:SetColor(0,{255,255,255})
	list = DxUI:Widget("CDxListWindowEx")
	plot = DxUI:Widget("MPlotWindow") -- MPolarPlotWindowEx
	plane = DxUI:Widget("MPlaneWindow")
	sphere = DxUI:Widget("MSpherePlot")
	point = DxUI:Widget("MPlanePointWindow")
	btn = DxUI:Widget("CDxPushButton")
	btn:SetText("保存到文件")
	list:SetText("Lua List Window")
	list:AddItems({1,2,3,4,5,6,7,8})
	local gl = DxUI:Widget("CDxGridLayout")
	gl:SetMargin(50)
	gl:AddWidget(tabwindow ,0,0,1,1)
	window:SetLayout(gl)
	list:Event("SelectedChanged","ListEvent")
	tabwindow:AddTab(sphere,"球面方向图")
	tabwindow:AddTab(plane,"二维平面图")
	tabwindow:AddTab(plot,"二维曲线图")
	tabwindow:AddTab(point,"平面点阵图")
	tabwindow:AddTab(list,"列表框")
	point:LoadPointFromFile("ArrayTest -504 - try1.txt")
	window:AddChild(btn)
	btn:SetGeometyDim({1.0,-150},{0.0,5},{0.0,100},{0.0,40})
	btn:Event("Clicked","Clicked")
	plot:Load("plot.txt")
end

function show()
	window:Show()
	App:Run()
end

function dealfile(folder)
	local files = filesystem:findfiles(folder,"*.txt")
	plot:Clear()
	for i=1,#files do
		local filename = files[i]
		local mat = matrix:load(filename)
		if mat:size() > 0 then
			local mxs = mat:col(1)
			local mys = mat:col(2)
			local xs = mxs:detach()
			local ys = mys:detach()
			plot:PlotLine(filename,xs,ys)
		end
	end
end

init()
show()
```
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/13.PNG)

### MDXUI和OpenCV对接
将opencv文件夹下面的.h和.cpp架子进工程——为了减少依赖，这两个文件直接以源码的方式提供
```
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "DxOpenCvWindow.h"
#include <DxMainWindow.h>
#include <DxListWindow.h>
#include <DxLayout.h>
#include <MFileSystem.h>
#include <DxPushButton.h>
#include <DxCommDialog.h>

using namespace MFileSystem;
using namespace DxUI;
using namespace mj;




int main(int argc, char* argv[])
{
	DxUI::CDxApplication* App = DxUI::CDxApplication::Instance();
	using namespace cv;
	DxUI::CDxMainWindow wind;
	bool isOk = false;

	DxUI::CDxWindow CenterWindow;
	cv::Mat mat = imread("workbase.JPG");
	DxUI::CDxOpenCvWindow OpenCvWindow;
	wind.SetCentralWidget(&CenterWindow);
	DxUI::CDxListWindow ListWindow;
	DxUI::CDxGridLayout lg;
	CenterWindow.SetLayout(&lg);
	DxUI::CDxPushButton Button;
	DxUI::CDxPushButton Button2;
	lg.AddWidget(&OpenCvWindow, 0, 0, 10, 10);
	lg.AddWidget(&ListWindow, 0, 10, 9, 3);

	lg.AddWidget(&Button2, 9, 11, 1, 1);
	lg.AddWidget(&Button, 9, 12, 1, 1);
	Button.SetText("摄像头");
	Button2.SetText("拍照");
	std::vector<MString> files;
	MFindFiles(".", "*.JPG", files);
	ListWindow.AddItems(files);
	OpenCvWindow.ShowMat(mat);

	ListWindow.Event_SelectedChanged += [&](int index1, int index2, DxUI::CDxWidget* sender){
		auto item = ListWindow.GetItemByIndex(index1);
		if (item){
			mat = imread(item->Text().toStdString().c_str());
			OpenCvWindow.ShowMat(mat);
		}
	};

	Button.Event_Clicked += [&](bool isclicked, DxUI::CDxWidget* sender){
		if (isOk){
			isOk = false;
			return;
		}
		isOk = true;
		std::async(std::launch::async, [&](){
			VideoCapture capture(0);
			while (isOk){
				Mat frame;
				capture >> frame;
				OpenCvWindow.ShowMat(frame);
				::Sleep(30);
			}
			capture.release();
			OpenCvWindow.ShowMat(mat);
		});
	};

	Button2.Event_Clicked += [&](bool isClicked, DxUI::CDxWidget* sender){
		MString FileName = DxUI::CDxCommDialog::GetSaveFileNameW(sender, L"图片文件.png|*.png", L"png");
		if (FileName.empty()){
			return;
		}
		OpenCvWindow.SaveToFile(FileName);
	};
	wind.ShowMaximized();
	return App->Run();
}
```
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/13.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/14.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/15.PNG)
如果电脑有摄像头，可以点击按钮摄像头打开摄像头看看效果。


* 下面展示一些使用该框架编写的软件的界面：
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/1.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/2.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/3.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/4.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/5.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/6.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/7.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/8.PNG)
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/9.PNG)
