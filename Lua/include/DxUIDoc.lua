
--[[

-----------------------------------------------------------------------------------------------

--
-- 引入的公共函数
-- Box 函数只有第一个参数是必须的
--
ErrorBox      		// 错误对话框 参数 显示的错误信息[字符串],父窗口[DxUI:Widget],按钮的名称,标题
InfoBox				// 提示对话框 参数 显示的提示信息[字符串],父窗口[DxUI:Widget],按钮的名称,标题
QueBox				// 询问对话框 参数 显示的询问信息[字符串],父窗口[DxUI:Widegt],确定按钮名,取消按钮名,标题
QueInfoBox			// 选择对话框 参数 显示的询问信息[字符串],父窗口[DxUI:Widegt],是按钮名,否按钮名,取消按钮名 标题
Print				// 打印信息 只接受一个参数  功能不如lua自带的print,主要是可以兼容DxUI库
GetOpenFileNames	// 选择多个文件 参数 父窗口[DxUI],文本过滤
GetOpenFileName		// 选择一个文件 参数 父窗口[DxUI],文本过滤
GetSaveFileName		// 获取一个文件名用于保存 参数 父窗口[DxUI],文本过滤,默认文件后缀
GetSaveDirName		// 获取文件保存路径 参数 父窗口[DxUI]
GetColor			// 使用系统调色板获取一个颜色值 返回 r,g,b,a



--
--  DxUI 窗口组件类简要说明
--

-----------------------------------------------------------------------------------------------------


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


--------------------------------------------------------------------------------------------------------

--
-- 目前所支持的窗口类有
--  
-- CDxWidget  CDxWindow CDxLabel CDxEdit CDxEditWidget CDxTextBolck CDxSpinBox
-- CDxListWidget CDxListWindow CDxListWindowEx CDxComboBox CDxTreeWindow
-- CDxPushButton CDxStatusButton CDxUserButton CDxHexagonButton CDxFlatButton CDxFlatButtonEx CDxCheckBox CDxRadioButton CDxGroupBox
-- CDxTableWindow CDxTableWidget CDxTabWidget CDxStackedWidget
-- CDxWindowContainer 
-- CDxTopWindow CDxPopWindow CDxDialog
-- CDxSpliterWindow
-- CDxHSpliter CDxVSpliter
-- CDxProgressBar
-- CDxPicture
-- CDxNoCaptionPopWindow CDxNoCaptionDialog
-- CDxMultTreeWindow
-- CDxMenu  CDxAction
-- CDxLinkEdit
-- CDxHorizonLayout CDxVerticLayout CDxGridLayout
-- CDxItemWindow
-- CDxFloatWindow
-- CDxColorPalette
-- CDxAnimationWindow
-- CDx3DWidget
-- MContourWindow MPlaneWindowBase MPlaneWindow
-- MPolarPlotWindow MPlotWindow MPlotWindowBase
-- MSpherePlot MDcartplot3d MSmoothPlane MKPlot2d MKPlot3d MCylinderPlot
--
-- 有些窗口的专用接口目前还未提供
--




------------------------------------------------------------------------------------------------------------
--
-- 创建窗口对象
-- 参数窗口类名
--
CreateWindow

--
-- 创建一个应用实例 一个进程仅此一个
--
Application

--
-- 创建XML读取实列
--
Xml

SetText   // 设置窗口文本 参数字符串
SetTitle  // 设置窗口的标题  参数字符串
SetIcon   // 设置窗口图标   参数字符串
SetSize   // 设置窗口大小  参数 长 和 宽
SetFixSize   //设置窗口固定尺寸 参数 长 和 宽 
SetFixWidth  // 设置窗口固定宽度  参数 整形
SetFixHeight // 设置窗口固定高度  参数 整形


--
-- AddItem 用于列表框，导航树，下拉框 设置Item
-- 参数 [icon]文本内容
-- 参数 UI组件
-- 对于导航树 如果参数为字符串 将会返回新建的item对象
--
AddItem      

--
-- AddItems 用于列表框 下拉框 通过表添加多个item
-- {}
--
AddItems


--
-- AddChild 添加子窗口 参数为窗口组件
-- 
AddChild

--
-- 将窗口设置为tab窗口
-- 参数 窗口组件,Tab名
--
AddTab


SetMargin  			// 用于Layout 设置Margin
SetSeparation 		// 用于Layout 设置 间距

SetGeomety  		// 设置窗口设置 参数为 x,y,w,h
SetGeometyDim 		// 按照比例模式设置窗口尺寸 参数为4个表，每个表格式{0.0，20} 比例 + 偏移
SetBackGroundColor  // 设置窗口背景颜色 参数为一个颜色表{r,g,b,a} r[0-255]
SetCaptionHeight    // 设置窗口标题栏高度 参数为 h
SetToolTip          // 设置窗口组件的提示文本 参数 字符串
AppendText 			// 追加文本 参数字符串
Clear				// 清除组件上的信息
SetEnabeled			// 窗口组件是否可用 参数为 bool  true 可用 false 不可用
SetReadOnly			// 窗口组件为只读模式 参数为bool true 只读 false 可编辑
SetParent			// 设置父窗口
SetSpliterDirection	// 设置分割窗口的分割方向 [0,1]
SetFirstWindowRatio // 设置分割窗口第一个窗口所占比例
GetColor			// 获取调色板的颜色

SaveToFile			// 将窗口渲染到文件

--
-- ShowWindowByIndex CDxStakedWidget用于显示指定层窗口
-- 参数为 index 
--
ShowWindowByIndex


--
-- ShowWindowByWindow CDxStackedWidget 用于显示指定窗口
-- 参数为 窗口组件
--
ShowWindowByWindow

--
-- RemoveChild 移除子窗口
--
RemoveChild



--
-- CurrentWindow CDxStackedWidget当前所显示的窗口
--
CurrentWindow



--
-- CurrentIndex CDxStackedWidget当前所显示的窗口的索引
--
CurrentIndex


--
-- GetIndexByWindow CDxStackedWidget获取指定窗口的索引
-- 参数为窗口组件
--
GetIndexByWindow


--
-- GetWindowByIndex CDxStackedWidget获取指定索引的窗口楼
-- 参数为索引
--
GetWindowByIndex



SetSizeBox   				//	设置窗口的边框尺寸 参数为表 {left,top,right,bottom},{2,0,2,2} ,也可以是 四个整形
SetRoundRadius  			// 	设置窗口的圆角半径 参数为表 {x,y} {10,10} 也可以是 2 个整形
SetWindowShape  			// 	设置窗口形状 参数为整形 0---->矩形 1----->圆角矩形 2------->椭圆
SetBorderSize   			// 	设置窗口的border宽度  参数为整形
SetBorderColor  			// 	设置窗口的border颜色   参数为颜色表{r,g,b,a}
SetVisible      			// 	设置窗口是否可见 参数 bool
SetValue					//  设置窗口的数值 CDxSpinBox 参数为数字
GetValue					//	获取窗口数值 CDxSpinBox
SetRange					//  设置范围 对于CDxSpinBox 可以有3个参数 minva,maxval,decms 。对于CDxProgressBar只需要两个参数
ShowBorderEnable 			// 	窗口是否显示边框 参数 bool
GetEffects		 			//	获取窗口的效果 所有窗口效果都是通过Effects进行设置
GetText			 			//	获取窗口文本
Show 			 			// 	显示窗口  参数 bool true->最大化显示  可以无参 午餐普通显示
Hide			 			// 	隐藏窗口
Exec			 			//	用于对话框显示 对于弹出菜单的显示需要坐标参数 x,y
SetLayout		 			// 	窗口设置布局管理器 参数为布局管理
AddWidget		 			// 	布局管理器添加创就  参数为 ： 窗口，ir,ic,rspan,cspan,alignment

--
-- 连接窗口事件
-- 第一个参数为事件名，第二个参数为响应函数名
--
Event 


--
-- GetTextByIndex 列表框 下拉框 导航树 获取指定索引位置处的文本
--
GetTextByIndex


--
-- GetIntexByText 列表框 下拉框 导航树 获取指定文本的第一个索引
-- 参数为字符串
--
GetIntexByText


--
-- GetCurrentText 列表框 下拉框 导航树 获取当前选择的文本
--
GetCurrentText

--
-- GetCurrentIndex 列表框 下拉框 导航树 获取当前选择的索引
--
GetCurrentIndex


--
-- GetItemCounts 列表框 下拉框 导航树 获取item的大小
--
GetItemCounts

--
-- GetAllItems 列表框 下拉框 导航树 获取所有的item文本信息
--
GetAllItems



--
-- RemoveItemByIndex 列表框 下拉框 导航树 移除指定索引位置的item
--
RemoveItemByIndex


--
-- SetItemSelectedByIndex 列表框 下拉框 导航树 将指定索引的item设置为选中状态
--
SetItemSelectedByIndex


--
-- SetMultSelectedEnable 列表框 是否支持多选
-- 参数为bool
--
SetMultSelectedEnable


--
-- SetAllItemStatus 列表框 将所有参数设置为选中或非选中
-- 参数为bool
--
SetAllItemStatus

--
-- IncreaseIndex 列表框 将指定索引位置向下移动一位
--
IncreaseIndex

--
-- DecreaseIndex 列表框 将指定索引位置向上移动一位
--
DecreaseIndex


--
-- GetSelectedItems 列表框 获取所有选中的item
--
GetSelectedItems


--
-- SetCurrentByIndex 列表框 下拉框 将指定索引的item设置为选中
-- 参数 索引
--
GetSelectedItems

--
-- SetCurrentByText 列表框 下拉框 将指定文本的item设置为选中
-- 参数 字符串
--
SetCurrentByText

--
-- GetSelectedIndexs 列表框 获取所有选中的索引
--
GetSelectedIndexs
	

--
-- RemoveTreeItemByText 移除导航树指定文本的item
-- 参数 字符串
--
RemoveTreeItemByText
	
	
--
-- RemoveTreeItem 移除导航树的item
--
RemoveTreeItem 


--
-- SetTableSize 设置 Table的大小
-- 参数 行数 和 列数
--
SetTableSize


GetTableRows 					//	 获取Table的列数
GetTableColums					//	 获取Table的行数
TableAutoScale					//   Table自动适应
TableAvgSize					// 	 设置CDxTableWidget等分 参数 bool
SetTableItem					//   设置Table的单元内容 参数  row,col,val
SetTableWidget					//   设置Table的窗口  row,col,widget
GetTableItem					//   获取单元的内容  参数 row,col
SetTableData					//   设置表格内容 参数是一个矩阵 row,col,表
HorizonHeaderShowable 			// Table的水平表头是否显示 参数  bool
VerticalHeaderShowable			// Table的垂直表头是否显示 参数 bool
SetHorizonItemAutoWithContent 	// 水平表头是否随内容自动调整大小
SetVerticalItemAutoWithContent	// 垂直表头是否随内容自动调整大小
SetHorizonHeader				//	设置水平表头 参数为表
SetVerticalHeader				// 	设置垂直表头 参数为表
GetTableDatas					// 获取Table的所有数据 返回值 row col vals
InsertTableRows					// 插入行  一个参数表示在该行之前插入一个 两个参数表示在第一个参数前插入第二个参数指定的行
InsertTableCols					// 插入列 如上
AddTableRows					// 添加行 参数为行数
AddTableCols					// 添加列 参数为列数
RemoveTableRows					// 移动行 参数为开始的行位置 + 行数
RemoveTableCols					// 移除列 参数为开始列位置 + 列数


PlotLine						// 绘制一条曲线 参数为 曲线名，xs,ys,col,size
AppendLine						// 在制定曲线后面追加数据 曲线名  xs ，ys
AddMarker						// 添加mark [曲线名,marker名，x，col][mark名，x,y,col]
FixHorizonAxis					// 固定水平轴 bool minva maxval
FixVerticalAxis					// 固定垂直轴 bool minval maxval
AxisAutoScale					// 启动缩放
RemoveMarker					// 移除marker 参数 mark名
RemoveCurve						// 移除曲线  参数 曲线名
CurveIsExist					// 检查指定曲线是否存在

SetMajorGridColor				// 设置曲线边框颜色 参数  颜色表{r,g,b,a}
SetMinorGridColor				// 设置曲线虚线颜色
SetLabelTextColor				// 设置标签字体颜色
SetPlotBackGroundColor			// 设置绘图曲线的背景色
SetSelectedMarkerColor			// 设置选中的marker颜色
SetPlotTitile					// 设置plot的名称
SetPlotHTitile					// 设置水平轴文本
SetPlotVTitile					// 设置垂直轴文本
SetCurveColor					// 设置曲线颜色 参数 曲线名 颜色表
SetCurveSize					// 设置曲线大小 参数 曲线名 size
SetTraceMouse					// 是否追踪鼠标 参数 bool
SetSelectedCurveSize			// 设置选中曲线的大小
SetHighLightPointable			// 曲线是否用点来表示 参数[曲线名]bool
SetHighLightPointType			// 设置曲线高亮的表示方法 0 点 1 ×
SetHighLightPointColor			// 设置曲线高亮的表示颜色 参数 颜色表

Save							// 将plot保存到文件中  参数  文件名
Load							// 从文件中将plot加载出来 参数 文件名

SetDataShowRange				// 针对三维图形设置数据显示范围 参数 minval，maxval
SetInformation					// 针对三维图形设置一些信息文本 参数 字符串

SetNotNeedLine					// 绘制高亮时是否需要线 参数 曲线名 bool
SetStripLine					// 使用条状表示曲线 参数 曲线名 bool

PlotSurface						// 绘制表面 参数  xs,ys,[datas] 
SetShowLabelText				// 设置彩虹条上面的文本
SetPlaneDataRange				// 设置彩虹条上的映射范围  参数 boo  minval maxval
SetNeedColorLabel				// 设置是否需要彩虹条 参数 bool
SetXTitle						// 设置X轴的标题
SetYTitle						// 设置Y轴的标题
SetZTitle						// 设置Z轴的标题

SetPlaneData					// 设置平面数据 参数 row,col,datas
SetPlaneDataByAxis				// 根据坐标设置平面数据 x,y,val
SetPlaneDataByIndex				// 根据索引设置平面数据 r,c,val

LoadPointFromFile				// 从文件中加载点信息
SetPointRadius					// 设置点的半径
SetPointValue					// 设置点的值 参数  id ,val

SetDefaultColor					// 设置点的默认颜色
SetTextInfoColor				// 设置文本颜色
SetSelectedColor				// 设置选中的颜色 



-------------------------------------------------------------------------

--
-- Application
--
MainThreadID 					// 获取主线程 id
CurrentThreadID					// 获取当前线程id
Run								// Run


-------------------------------------------------------------------------

--
-- Xml 文件的读写
--
PutValue						// 写入数据 参数  key,val
PutValues						// 写入数据 参数  key,一张表
GetValue						// 获取数据 参数  key,defval
GetValues						// 获取数据 参数  key,defval
ReadFile						// 以读的方式打开文件  参数 文件名
WriteFile						// 以写的方式打开文件  参数 文件名
Close							// 关闭文件 以写的方式打开的文件将会保存
GetParentPath					// 获取当前操作文件的上层路径
CreateFolder					// 创建文件夹


--------------------------------------------------------------------------

--
-- Effects
--

SetColor  						// 设置颜色 参数 [0,1,2],颜色表
SetColors						// 设置渐变颜色 参数 [0,1,2],颜色{"0.0","ff0000ff","0.5,ff00ff","1.0,ffff"}
SetBitmaps						// 设置背景图
SetGradientDirect				// 设置渐变方式 参数[0,1]
SetGradientType					// 设置渐变类型 参数[0,1]
SetTextAlignment				// 设置文本对齐方式 参数[0,1,2,.....]
SetDisabelColor					// 设置不可用的颜色
SetGeometryShapeType			// 设置形状
SetTextDefaultColor				// 设置文本的默认颜色 
ClearTextDefaultColor			// 清除文本的默认颜色
ClearTextSelected				// 清除文本选中状态
ClearTextFontInfo				// 清除字体信息
ClearTextFontSize				// 清除字体大小
ClearTextColor					// 清除字体颜色
ClearUnderLine					// 清除字体下划线
ClearOverLine					// 清除字体上划线
ClearStrikeThrough				// 清除字体删除线	
ClearHighlight					// 清除文本高亮

AddHighlight					// 添加文本高亮  参数  iStart ,nLen,Col
SetHighlight					// 添加文本高亮  参数  iStart ,nLen,Col

AddStrikeThrough				// 添加文本删除线  参数  iStart ,nLen,Col 
SetStrikeThrough				// 添加文本删除线  参数  iStart ,nLen,Col
SetOverLine						// 添加文本上划线  参数  iStart ,nLen,Col	
AddOverLine						// 添加文本上划线  参数  iStart ,nLen,Col	
SetUnderLine					// 添加文本下划线  参数  iStart ,nLen,Col,[0,1,2,3]
AddUnderLine					// 添加文本下划线  参数  iStart ,nLen,Col,[0,1,2,3]	
SetTextColor					// 添加文本颜色  参数  iStart ,nLen,Col
AddTextColor					// 添加文本颜色  参数  iStart ,nLen,Col
SetTextFontSize					// 添加字体大小  参数  iStart ,nLen,Size
AddTextFontSize					// 添加字体大小  参数  iStart ,nLen,Size
SetTextFontProperty				// 设置字体属性  参数  iStart ,nLen,bool,[boo]
AddTextFontProperty				// 设置字体属性  参数  iStart ,nLen,bool,[boo]
SetTextFontInfo					// 设置字体信息  参数 iStart, nLen, fontName,Size,isBorder, isItalic
AddTextFontInfo					// 设置字体信息  参数 iStart, nLen, fontName,Size,isBorder, isItalic
SetDefaulFont					// 设置默认字体  参数 fontName, Size, isBorder, isItalic		
SetTextSelectedRange			// 设置文本选中状态 参数 iStart, nLen, col, col2
AddTextSelectedRange			// 设置文本选中状态 参数 iStart, nLen, col, col2



--
-- AddSpliter CDxMenu 添加分割线
--
AddSpliter   


--
-- AddAction 添加菜单项
-- 参数 [图标],名称
--
AddAction  

--
-- AddCheckableAction 添加可选菜单项
-- 参数 名称
--
AddCheckableAction 

--
-- AddMenu 添加子菜单
-- 参数 [图标],名称
--
AddMenu 


--
-- PushPictureFile CDxPicture 添加图片
-- 参数 图片文件
--
PushPictureFile

--
-- ShowPicByFileName CDxPicture显示指定文件的图片
-- 参数 图片文件名
--
ShowPicByFileName


--
-- ShowPicByIndex CDxPicture 显示指定索引的图片
-- 参数 索引号
--
ShowPicByIndex


--
-- PopPicture CDxPicture 弹出土拍你
--
PopPicture

--]]