### MDXUI
一个基于DirectX11的DirectUI框架，几乎涵盖有常有的GUI组件，支持3D模型的渲染，具体的使用文档关注微信公众号《C/C++编程教室》
由于一些别的原因，此处不提供源代码，提供编译好的x86和x64版本二进制文件，基于win10+vs2013
使用的DX版本为：DXSDK_Feb10
在使用该框架前先安装DX运行库，可以直接下载DXSDK_Feb10.exe默认安装就好


### 怎么使用C++进行编程

MDXUI原生就是C++编写，目前除了提供了对lua的支持外只暴漏了C++接口，其实有考虑过将其做成com组件，让其他语言也能够使用该框架，最后想想发现这是自己想多了，所以我们只能选择C++。

碍于篇幅的问题，下面简单介绍一下MDXUI的入门。如果熟悉Qt的话那么对MDXUI的使用就相当的简单，因为该GUI框架使用的便是Qt的Layout思想，也模拟了Qt的信号槽，自由度却在Qt之上，当然为了方便自然也参考了C#的事件委托，下面展示怎么连接一个按钮的点击事件：

* 使用connect的方式关联该事件
```
void fun(bool isclicked,CDxWidget* sender){}

CDxPushButton But;

// 将按钮的点击事件和一个自由函数关联起来
TL::MTLDataBind::Connect<CDxPushButton>(MSIGNAL(CDxPushButton, Clicked, &But), TL::MSLOT(fun));

class Test{
public:
	void mfun(bool isclicked,CDxWidget* sender){}
};

Test test;
// 将按钮的点击事件和一个成员函数关联起来
TL::MTLDataBind::Connect<CDxPushButton>(MSIGNAL(CDxPushButton, Clicked, &But), TL::MSLOT(&Test::mfun,&test));

```

* 使用委托的方式将关联起
```
CDxPushButton But;

// 使用lambda表达式
But.Event_Clicked += [](bool isclicked,CDxWidget* sender){};

// 使用自由函数
But.Event_Clicked += fun;

// 使用成员函数
But.Event_Clicked += TL::MSLOT(&Test::mfun,&test);
```



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
这个程序很简单，就是简单的创建一个窗口，然后让其显示
![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/10.PNG)



* 添加一个二维曲线的绘图窗口

如果只是添加一个窗口那么很简单，直接在主窗口上设置CentraWidget就好，当然对于CDxMainWindow来说设置这个窗口是必须的，可以在该窗CentraWidget上设置Layout，在Layout中添加窗口即可。

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
对于CDxPlotWindow来说默认是扩展的，什么是扩展呢？可以通过观察这个例子和下面被设置为非扩展的区别。

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

除了使用C++编程之外，其实我们还可以使用Lua进行编程，对于某些小程序来说，使用Lua可以很方便，不需要什么编译过程，还可以灵活对其修改，当然Lua还可以编写模块在C++中加载，这样就可以对模块进行灵活修改。

为了更好的启动Lua程序，我们提供一个C++编写的启动程序，当在同目录中发现main.lua文件时，该文件会被执行，然后会从main.lua文件中获取MainWindow让其显示出来，当然LuaExe.exe还可以通过命令行启动，第一个参数为启动的lua文件，第二个参数为需要显示的窗口名。

```
LuaExe.exe main.lua MainWindow
```

该效果等同于双击运行LuaExe.exe效果。

* 下面展示一个稍微复杂点的例子
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
	point:LoadPointFromFile("ArrayTest.txt")
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

![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/16.PNG)

![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/17.PNG)

![Image text](https://github.com/Mengtianhan/MDXUI/blob/master/Pic/18.PNG)


### MDXUI对OpenCV的支持
将opencv文件夹下面的.h和.cpp架子进工程——为了减少依赖，这两个文件直接以源码的方式提供，由于作者本人对opencv并未研究过，只对cv::Mat进行了一些简单的查看，所以提供的解决方案是基于cv::Mat,未必是最好的。
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
