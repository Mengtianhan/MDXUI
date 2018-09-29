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
	ef = window:GetEffects()
	ef:SetColor(0,{255,255,255})
	list = DxUI:Widget("CDxListWindowEx")
	plot = DxUI:Widget("MPolarPlotWindowEx") -- MPolarPlotWindowEx
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
	point:LoadPointFromFile("Test.txt")
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




