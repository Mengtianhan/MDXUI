require("MDXUI")

DxUI = {}


--
-- Event 目前所支持事件
-- Clicked  -->CDxPushButton CDxStatusButton CDxUserButton CDxHexagonButton CDxFlatButton CDxFlatButtonEx CDxCheckBox CDxRadioButton CDxGroupBox
-- SelectedChanged --> CDxListWindow CDxListWindowEx CDxListWidget CDxComboBox
-- SelectedIterms ItemChecked ItemClicked ItemClickedPoint DoubleClicked ---->CDxListWindow CDxListWindowEx
-- ValueChanged -->CDxSpinBox
-- ClickedItem DBClickedItem RBClickedItem  --->CDxTreeWindow
-- TextChanged TextEnterFinished  --> CDxEdt
-- TableSizeChanged ItemTextChanged RightButtonClicked --->CDxTableWindow
--

-- 
--  窗口创建函数
--  使用窗口类名作为参数创建
--  创建失败会有相关信息显示
--
function DxUI:Widget(resource)
	local window = {}
	window = MDXUI.CreateWindow(resource)
	return window
end




--
-- Application
--
function DxUI:App()
	App = {}
	App = MDXUI.Application()
	--[[
	local metatable = getmetatable(App)
	metatable.__index = metatable
	metatable.MainThreadID = Application.MainThreadID
	metatable.CurrentThreadID = Application.CurrentThreadID
	metatable.Run = Application.Run
	--]]
	return App
end

--
-- XML
--
function DxUI:Xml()
	local xml = {}
	xml = MDXUI.Xml()
	--[[
	local metatable = getmetatable(xml)
	metatable.__index = metatable

	metatable.PutValue = MXML.PutValue
	metatable.PutValues = MXML.PutValues
	metatable.GetValue = MXML.GetValue
	metatable.GetValues = MXML.GetValues
	metatable.ReadFile = MXML.ReadFile
	metatable.WriteFile = MXML.WriteFile
	metatable.Close = MXML.Close
	metatable.GetParentPath = MXML.GetParentPath
	metatable.CreateFolder = MXML.CreateFolder
	--]]
	return xml
end
