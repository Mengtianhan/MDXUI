

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DxConfig.h"
#include <HVector.h>
#include <memory>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "MProjectType.h"
#include <unordered_map>
#include "MAppDataUnit.h"
#include "MAppWindow.h"
#include <MSingleton.h>
#include "MAppFrameData.h"
#include "MAppFrameUI.h"
#include "ICommunication.h"
#include "MSerialize.h"
#include "IFrameWindowInterface.h"
#include "MTreeModule.h"
#include "MAppEvent.h"
#include "MAppEventHandle.h"
#include <IMSql.h>
#include "MMenuAction.h"
//
// HAppFrameUI 管理公用的UI
// HAppFrameData 管理公用的Data
// MLogger 记录框架logger
//

class MAppData;
class MAppUI;
class MAppInstance;
class MAppParseFile;
class IDealDataInterface;
class MProjectType;
class MAppDataUnit;
class MAppWindow;
class MAppFrameData;
class MAppFrameUI;
class MLoggerBox;



namespace DxUI{
	class CDxWidget;
	struct IWindow;
}

//
// 工厂接口
//
class IFactoryInterface;

//
// App 的实例对象只有一个
// 管理整个App的数据以及UI信息
//
class DXUI_API MAppManager : public TL::MTLObject
{
public:
    friend class MFrameWindow;
    //
    // 当工程状态改变时执行的回掉函数
    //
    typedef std::function<void(int)> APPSTATUSCHANGEFUN;

    //
    // 当框架状态的工程改变时执行的回调函数
    //
    typedef std::function<void(MString)> PROJECTCHANGEFUN;

    //
    // 工程类型改变的回调函数
    //
    typedef APPSTATUSCHANGEFUN   APPPROJTYPECHANGEFUN;

    //
    // 在加载完工程后做些什么呢？
    //
    typedef std::function<void()>  FINISHEDLOADPROJFUN;



    //
    // 工程名的StrID，工程数据，工程UI
    //
    typedef TL::MMNoSqlDB<MString,MAppData*,MAppUI*> APPDATATYPE;

    //
    // 保存实例对象，对同一个实例仅创建一个对象,int 代表工程类型
    //
    typedef TL::MMNoSqlDB<MAppInstance*,MAppData*,MAppUI*,int> INSTACETYPE;

    //
    // 工程名的StrID，工程类型
    //
    typedef std::map<MString,MProjectType*> APPPROJTYPE;

    //
    // 模块名，模块实例指针
    //
    typedef std::map<MString,MAppInstance*>  APPMODULES;

    //
    // HAppDataBase，HAppWindow
    //
    typedef std::map<MAppDataUnit*,MAppWindow*> APPDATAUI;


	//
	// 工程相关的事件处理
	//
	typedef std::map<MString, MAppEventHandle*> PROJEVENTHANDLE;

    //
    // 工程名---数据处理器
    //
    typedef std::map<MString,IDealDataInterface*> PROJDATADEALER;


    typedef DxUI::IWindow*(*CreateInstanceFun)();


	//
	// 特定函数类型
	//
	typedef void(*NoArgrFunType)(TL::MTLObject*);


    explicit MAppManager();
    virtual ~MAppManager();

public:

    //
    // 当前整个应用程序的状态
    //
    enum APPSTATUS{
        TESTING = 0,
        PURSE,
        STOP,
    };

    //
    // 当前工程的状态，打开？新建？另存？
    //
    enum PROJECTSTATUS{
        NEWPROJECT = 0,
        OPENPROJECT,
        SAVEASPROJECT,
        NONESTASTUS
    };

    ///
    /// \brief BindInstance
    /// \param __AppManager
    /// 绑定外部App管理器
    /// 绑定之后使用Instance获取的便是绑定的
    ///
    static void BindInstance(MAppManager* __AppManager);

    ///
    /// \brief Instance
    /// \return
    /// 创建App管理的唯一实例
    /// 如果有使用 BindInstance 绑定实例，那么返回的便是外部实例
    ///
    static MAppManager* Instance();

    ///
    /// \brief DestroyInstance
    /// 销毁App管理的实例
    /// 只销毁自己创建出来的
    /// BindInstance 绑定的不处理
    ///
    static void DestroyInstance();


    ///
    /// \brief LoadWindowFromDll
    /// \param dllname Dll
    /// \param parent  父窗口 可以为nullptr
    /// \return
    /// 从dll模块中加载窗口模块
    ///
    static DxUI::IWindow* LoadWindowFromDll(const MString& dllname);


    ///
    /// \brief Register
    /// \param syskey 系统指定的key 可以是 HKEY_CLASS_ROOT,HKEY_CURRENT_USER,HKEY_LOCAL_MACHINE,HKEY_USERS,HKEY_CURRENT_CONFIG
    /// \param usekey 用户自定义key
    /// \param subkey 键
    /// \param value  值
    /// \return
    /// 写注册表
    /// eg:
    /// Register(HKEY_CURRENT_USER,"HelloWord","Value","1000")
    ///
    static bool Register(HKEY syskey,
                         const MString& usekey,
                         const MString& subkey,
                         const MString& value);


    ///
    /// \brief ReadRegister
    /// \param syskey 同Register
    /// \param usekey 同Register
    /// \param subkey 同Register
    /// \return
    /// 读取注册表信息
    /// eg:
    /// MString value = ReadRegister(HKEY_CURRENT_USER,"HelloWord","Value");
    ///
    static MString ReadRegister(HKEY syskey,
                                const MString& usekey,
                                const MString& subkey);

    ///
    /// \brief UnRegister
    /// \param syskey 同Register
    /// \param useKey 同Register
    /// \return
    /// 卸载注册表
    /// eg:
    /// UnRegister(HKEY_CURRENT_USER,"HelloWord")
    ///
    static bool UnRegister(HKEY syskey,const MString& useKey);



	///
	///
	///
	static void RunNoArgrFunction(TL::MTLObject* obj, NoArgrFunType fun);

	//
	// 提示信息对话框，仅有一个确认按钮
	//
	static int ErrorBox(const MString& info,DxUI::CDxWidget* Parent = nullptr,const MString& title = "错误");

	//
	// 提示信息对话框，仅有一个确认按钮
	//
	static int ShowBox(const MString& info, DxUI::CDxWidget* Parent = nullptr, const MString& title = "信息");

	//
	// 这个对话框有两个按钮，YES 和 NO 返回值为IDYES 和 IDNO
	//
	static int InfoBox(const MString& info, DxUI::CDxWidget* Parent = nullptr, const MString& title = "提示");

	//
	// 这个对话框有三个按钮，YES NO 和 CANCEL 返回值为IDYES IDNO 和 IDCANCEL
	//
	static int QueBox(const MString& info, DxUI::CDxWidget* Parent = nullptr, const MString& title = "询问");


    ///
    /// \brief ToString
    /// \return
    /// 转换为字符串
    /// 作为一个标准接口而存在
    ///
    virtual MString ToString(){return "MAppManager";}


    ///
    /// 获取AppData 应用程序访问数据的唯一入口
    ///
    template<class T = MAppData>
    T* AppData(const MString& projId = ""){
        if(mOpingProject.empty()){
            mOpingProject = mCurrentProject;
        }
        MString projName = mOpingProject;
        if(!projId.empty()){
            projName = projId;
        }   
		try{
			auto value = mAppDatas.item<0>(projName);
			MAppData* __Data = TL::get<1>(value);
			return dynamic_cast<T*>(__Data);
		}
		catch (...){
			return nullptr;
		}
    }

    ///
    /// 访问FrameData 数据的唯一入口
    ///
    template<class T = MAppFrameData>
    T* AppFramData(){
        try{
            T* __data = dynamic_cast<T*>(pFrameData);
            return __data;
        }
        catch(...){
            return nullptr;
        }
    }

    ///
    /// 获取AppUI
    /// 访问应用程序UI信息的唯一入口
    ///
    template<class T = MAppUI>
    T* AppUI(const MString& projId = ""){
        if(mOpingProject.empty()){
            mOpingProject = mCurrentProject;
        }
        MString projName = mOpingProject;
        if(!projId.empty()){
            projName = projId;
        }
		try{
			auto value = mAppDatas.item<0>(projName);
			MAppUI* __Data = TL::get<2>(value);
			return dynamic_cast<T*>(__Data);
		}
		catch (...){
			return nullptr;
		}
    }


    ///
    /// 获取AppFrameUI
    /// 访问应用程序FrameUI信息的唯一入口
    ///
    template<class T = MAppFrameUI>
    T* AppFrameUI(){
        try{
            T* __Ui = dynamic_cast<T*>(pFrameUI);
            return __Ui;
        }
        catch(...){
            return nullptr;
        }
    }

    ///
    /// 获取工程信息管理组件,同样是访问工程信息的唯一入口
    ///
    template<class T = MProjectType>
    T* ProjceType(const MString& projId = ""){
        if(mOpingProject.empty()){
            mOpingProject = mCurrentProject;
        }
        MString projName = mOpingProject;
        if(!projId.empty()){
            projName = projId;
        }
        if(mProjTypes.count(projName)){
            try{
				MProjectType* __Type = mProjTypes.at(projName);
                return dynamic_cast<T*>(__Type);
            }
            catch(...){
                return nullptr;
            }
        }
        else{
            return nullptr;
        }
    }


	///
	/// 获取信号处理器
	/// ProjId 需要访问的工程id
	///
	template<class T>
	T* AppSignalHandle(const MString& ProjId = ""){
		if (mOpingProject.empty()){
			mOpingProject = mCurrentProject;
		}
		MString projName = mOpingProject;
		if (!ProjId.empty()){
			projName = ProjId;
		}
		if (mProjEventHandle.count(projName)){
			auto ptr = mProjEventHandle.at(projName);
			try{
				T* __Ptr = dynamic_cast<T*>(ptr);
				return __Ptr;
			}
			catch (...){
				return nullptr;
			}
		}
		return nullptr;
	}


    ///
    /// 获取工程数据处理器
    /// ProjId 需要访问的工程id 默认为当前正在操作的工程
    ///
    template<class T>
    T* AppDataDealer(const MString& ProjId = ""){
        if(mOpingProject.empty()){
            mOpingProject = mCurrentProject;
        }
        MString projName = mOpingProject;
        if(!ProjId.empty()){
            projName = ProjId;
        }
        if(mProjDataDealer.count(projName)){
            auto ptr = mProjDataDealer.at(projName);
            try{
                T* __Ptr = dynamic_cast<T*>(ptr);
                return __Ptr;
            }
            catch(...){
                return nullptr;
            }
        }
        return nullptr;
    }

    ///
    /// 通过一个模块名获取一个模块实例
    /// 该模块名在创建工程之时由创建工程模块返回
    /// 获取该模块实例之后便可以创建该实例的工程
    /// moduleName 可以是dll名字也可以是指定的模块名
    ///
    MAppInstance* ModuleInstance(const MString& moduleName);

    ///
    /// \brief RegisterInstance
    /// \param moduleName
    /// \param instance
    /// 注册Instance
    /// 如果模块本身就在内存中存在那么直接注册即可
    ///
	void RegisterInstance(const MString& moduleName, MAppInstance* instance);

    ///
    /// \brief RemoveInstance
    /// \param moduleName
    /// 移除Instance
    ///
    void RemoveInstance(const MString& moduleName);

    ///
    /// 通过一个Nag的StriID 获取他的root名
    /// GetNagRootTextFromStrId 获取但是显示名
    /// GetNagRootStrIdFromStrId 获取的是StrID
    ///
    MString GetNagRootTextFromStrId(const MString& strId);
    MString GetNagRootStrIdFromStrId(const MString& strId);

    ///
    /// 设置工程信息
    /// 将工程添加进来进行管理
    /// 分离的数据，UI，工程信息通过该接口关联起来
    ///
    void PushProject(const MString& projid,
                     MAppData* __data, MAppUI*
					 __ui, MProjectType* __Type);


	///
	/// \brief
	/// 在另存工程的时候使用同一套UI
	/// 关键key会被修改
	/// 需要执行该函数进行相关更新
	///
	void UpdateProjectInfo(MAppUI* __ui, MProjectType* __Type);

    ///
    /// \brief ProjectList
    /// \return
    /// 获取工程列表
    ///
    TL::Vector<MString> ProjectList() const;


    //
    // CloseCurrent  关闭当前工程
    // CloseAll 关闭所有工程
    // CloseProject 关闭指定工程
    //

    ///
    /// \brief CloseCurrent
    /// 关闭当前工程
    ///
    void CloseCurrent();

    ///
    /// \brief CloseAll
    /// 关闭所有工程
    ///
    void CloseAll();

    ///
    /// \brief CloseProject
    /// \param projid
    /// 关闭指定工程
    ///
    void CloseProject(const MString& projid);

    //
    // 针对当前工程进行操作
    //

    ///
    /// \brief SaveCurrent
    /// \return
    /// 保存当前工程
    ///
    bool SaveCurrent();

    ///
    /// \brief SetCurrent
    /// \return
    /// 设置当前工程
    ///
    bool SetCurrent();

    ///
    /// \brief ValidateCurrentData
    /// \return
    /// 校验当前工程
    ///
    bool ValidateCurrentData();

    ///
    /// \brief UpdateCurrentWindow
    /// \return
    /// 更新当前工程的窗口
    ///
    bool UpdateCurrentWindow();

    //
    // 操作所有的工程
    // SaveAll 将UI数据保存到Data中
    // SetAll  将Data设置到UI中
    // SaveProject 将Data数据保存到文件中
    // ReadProject 从文件中读取数据到Data中
    //

    ///
    /// \brief SaveAll
    /// \return
    /// 保存所有工程
    /// 该过程是通过 SaveContext 将UI上面的信息保存到Data上
    ///
    bool SaveAll();

    ///
    /// \brief SetAll
    /// \return
    /// 设置所有工程
    /// 该过程是通过 SetContext 函数将Data上的数据设置到UI上
    ///
    bool SetAll();

    ///
    /// \brief ValidateAll
    /// \return
    /// 通过 ValidateData 校验UI的输入信息
    ///
    bool ValidateAll();

    ///
    /// \brief UpdateAllWindow
    /// \return
    /// 通过 UpdateWindow 对UI进行更新
    ///
    bool UpdateAllWindow();

    ///
    /// \brief SaveProject
    /// \param project
    /// \return
    /// 将指定工程的UI信息保存到Data，并从Data中保存到硬盘
    ///
    bool SaveProject(const MString& project = "");

    ///
    /// \brief ReadProject
    /// \param project
    /// \return
    /// 从硬盘中读取指定工程的数据到Data中，再设置到UI
    ///
    bool ReadProject(const MString& project = "");

    //
    // 操作全局数据
    //

    ///
    /// \brief SaveFrameData
    /// \return
    /// 将FrameUI信息通过 SaveContext 保存到 FrameData 再到硬盘
    ///
    bool SaveFrameData();

    ///
    /// \brief SetFrameData
    /// \return
    /// 读取 FrameData 然后通过 SetContext 设置到FrameUI
    ///
    ///
    bool SetFrameData();

    ///
    /// \brief UpdateFrameUI
    /// \return
    /// 更新FrameUI
    ///
    bool UpdateFrameUI();

    ///
    /// \brief ValidateFrameData
    /// \return
    /// 校验FrameUI的输入是否有效
    ///
    bool ValidateFrameData();

    //
    // 更新导航树
    //

    ///
    /// \brief UpdateNagTree
    /// \param isAll
    /// 更新导航树
    ///
    void UpdateNagTree(bool isAll = true);

    ///
    /// \brief PushNagTree
    /// \param itemID
    /// 添加导航菜单
    /// itemID 以 # 号分层 图标放在[]中
    /// eg :
    /// StrId = Project[:res/icon.png]#Item1[:res/xxxx]#Item2[xxxx]
    /// 结果就是
    /// Project
    ///     Item1
    ///         Item2
    ///
    void PushNagTree(const MString& itemID);

    ///
    /// \brief EraseNagTree
    /// \param itemID
    /// 删除导航信息
    ///
    void EraseNagTree(const MString& itemID);

    ///
    /// \brief SetCurrentNagItem
    /// \param itemId
    /// 让指定的item成选中状态
    ///
    void SetCurrentNagItem(const MString& itemId);

    ///
    /// \brief ModifyNagShowName
    /// \param itemId
    /// \param Name
    /// 修改指定的Item的显示名
    ///
    void ModifyNagShowName(const MString& itemId,const MString& Name);

    //
    // 修改当前操作的工程
    // 设置后接下来啊的操作都是对当前工程进行操作
    //

    ///
    /// \brief SetCurrentProject
    /// \param currentProjId
    /// 设置指定工程为当前工程
    ///
    void SetCurrentProject(const MString& currentProjId);

    ///
    /// \brief CurrentProject
    /// \return
    /// 获取当前工程
    ///
    const MString&  CurrentProject() const;

    //
    // ParentWindow 获取父窗口
    // AttachWindow 将窗口作为整个APP的父窗口
    // DettachWindow 不需要父窗口
    //

    ///
    /// \brief ParentWindow
    /// \return
    /// 获取父窗口
    ///
    DxUI::CDxWidget* ParentWindow();

    ///
    /// \brief AttachWindow
    /// \param __window
    /// 将指定窗口作为父窗口保存下来
    ///
	void AttachWindow(DxUI::CDxWidget* __window);

    ///
    /// \brief DettachWindow
    /// 不再需要父窗口
    ///
    void DettachWindow();

    //
    // 获取当前工作区域的窗口
    //

    ///
    /// \brief CurrentWindow
    /// \return
    /// 获取当前工作区域窗口
    ///
	DxUI::CDxWidget* CurrentWindow() const;


	//
	// 日志记录窗口
	//
	MLoggerBox* MLoggerWindow();

    //
    // InitWindow 初始化公共组件
    // NagTree 获取导航树组件
    // MsgOuput 获取窗口输出组件
    // MenuAndTool 获取菜单工具组件
    // NewForm 获取新建窗口组件
    // PopMenu 获取右键菜单
    //
	MTreeModule*  NagTree();


	//
	// 获取基本事件管理器
	//
	MAppEvent*  RootEvent();

    /// \brief InitWindow
    /// \param parent 父窗口
    /// \param __data FrameData
    /// \param __ui   FrameUI
    /// 初始化完成后将完成框架公用的组件
    ///
	void InitWindow(DxUI::CDxWidget* parent, MAppEvent* Event,MAppFrameData* __data = nullptr, MAppFrameUI* __ui = nullptr); // 初始化窗口

    ///
    /// \brief InstallCommunication
    /// \param __communication
    /// 安装通信模块
    ///
    void InstallCommunication(ICommunication* __communication);

    ///
    /// \brief InstanllProTypeFactor
    /// \param factor
    ///  安装工程管理器
    ///
    void InstallProTypeFactor(MProjectType* factor);



    ///
    /// \brief InstallAnyAction
    /// \param StrKey 关键字
    /// \param any    操作对象
    /// 下面的这个操作就强大了,能装载任意操作
    ///
    void InstallAnyAction(const MString& StrKey,const MAny& any);

    ///
    /// \brief RemoveAnyAction
    /// \param any
    /// 和 InstallAnyAction 对应,这是移除一个操作
    ///
    void RemoveAnyAction(const MAny& any);

    ///
    /// \brief GetAnyAction
    /// \return
    /// 在实例中获取该操作既可以完成任意委托,当然前提是要先安装的有委托
    ///
    const TL::Vector<MAny> &GetAnyAction() const;

    ///
    /// \brief GetAnyAction
    /// \param StrKey
    /// \return
    /// 获取指定操作
    ///
    MAny GetAnyAction(const MString& StrKey) const;




    ///
    /// \brief NewProjType
    /// \return
    /// 通过工厂创建工程管理器
    ///
    MProjectType* NewProjType() const;


    ///
    /// \brief SetFrameConfigFile
    /// \param configfile
    /// 修改FrameData的保存文件
    /// 但是没必要这么干
    ///
    void SetFrameConfigFile(const MString& configfile); // 设置Frame的config文件

    ///
    /// \brief FrameConfigFile
    /// \return
    /// 获取FrameData保存的文件
    ///
    const MString& FrameConfigFile() const; // Frame的config文件

    ///
    /// \brief GetFrameSerial
    /// \return
    ///
    DxUI::MSerialize*		GetFrameSerial();



	void					SetWorkWindowIndex(int index);
	DxUI::CDxStackedWidget* GetCurrentWorkWindow();
	DxUI::CDxStackedWidget* GetCenterWindow();


    ///
    /// \brief ShowWindow
    /// \param window
    /// 直接在 Frame 中切换窗口的显示
    ///
	DxUI::CDxWidget*  ShowWindow(DxUI::CDxWidget* window);  // 显示窗口工程
	int			  ShowWindowByIndex(unsigned index);
	void		  PushWindow(DxUI::CDxWidget* window);
	unsigned	  GetWindowCounts() const;

    ///
    /// \brief RemoveWindow
    /// \param window
    /// 将窗口从框架中移除
    ///
    void          RemoveWindow(DxUI::CDxWidget* window); // 移除窗口
	void		  RemoveWindowByIndex(unsigned index);


    ///
    /// \brief CloseApp
    /// \return
    /// 关闭App
    /// 返回值 1 标识需要保存工程
    /// 0 不需要保存工程
    /// -1 取消不关闭
    ///
    int CloseApp();

    ///
    /// \brief AppStatus
    /// \return
    /// 获取当前APP的状态
    /// 只有一个工程处于测试状态
    ///
    APPSTATUS AppStatus();

    ///
    /// \brief SetAppStatus
    /// \param status
    /// 设置当前APP状态
    /// 该函数具有发射作用
    /// 所以可以对该属性选择接收
    ///
    void SetAppStatus(APPSTATUS status);

    ///
    /// \brief ProjectStatus
    /// \param projid
    /// \return
    /// 获取当前工程的状态
    ///
    PROJECTSTATUS ProjectStatus(const MString &projid = "");

    ///
    /// \brief SetProjectStatus
    /// \param status
    /// \param ProjId
    /// 设置当前工程状态
    ///
    void SetProjectStatus(PROJECTSTATUS status, const MString& ProjId = "");

    ///
    /// \brief BindAppStatusFun
    /// \param fun
    /// 设置APP状态改变回调操作
    ///
    void BindAppStatusFun(APPSTATUSCHANGEFUN fun);

    ///
    /// \brief BindProjectChangeFun
    /// \param fun
    /// 设置工程改变时执行的操作
    ///
    void BindProjectChangeFun(PROJECTCHANGEFUN fun);

    ///
    /// \brief BindAppTypeChangeFun
    /// \param fun
    /// 程类型改变时执行的操作
    ///
    void BindAppTypeChangeFun(APPPROJTYPECHANGEFUN fun);

    ///
    /// \brief BindLoadFinishedProjFun
    /// \param fun
    /// 设置加载完工程后执行的操作
    ///
    void BindLoadFinishedProjFun(FINISHEDLOADPROJFUN fun);

    ////
    /// \brief BindSaveFinsihedProjFun
    /// \param fun
    /// 设置保存完工程后执行的操作
    ///
    void BindSaveFinsihedProjFun(FINISHEDLOADPROJFUN fun);

    ///
    /// \brief BindCloseFinishedProjFun
    /// \param fun
    /// 关闭工程后所执行的操作
    ///
    void BindCloseFinishedProjFun(FINISHEDLOADPROJFUN fun);

    ///
    /// \brief BindOpenfinishedProjFun
    /// \param fun
    /// 打开工程后所执行的操作
    ///
    void BindOpenfinishedProjFun(FINISHEDLOADPROJFUN fun);

    ///
    /// \brief HaveProject
    /// \param projId
    /// \return
    /// 查询当前是否还有工程
    ///
    bool HaveProject(const MString& projId = "");



    ///
    /// \brief CreateInstance
    /// \param modulename 模块名
    /// \return
    /// 创建模块实例
    ///
    MAppInstance* CreateInstance(const MString& modulename);

    ///
    /// \brief AppInstance
    /// \param projname
    /// \return
    /// 根据工程名获取创建当前工程的Instance
    ///
    MAppInstance* AppInstance(const MString& projname);

    ///
    /// \brief CreateProject
    /// \param modulename 模块名
    /// \param __Type 工程信息管理其
    /// \return
    /// 创建工程
    ///
    bool CreateProject(const MString& modulename,MProjectType* __Type);
	bool OpenProject(const MString& modulename, MProjectType* __Type);
	bool GenProjectWindow(const MString& modulename, MProjectType* __Type);

    ///
    /// \brief UpdateOperatorProj
    /// \param ProjID
    /// 更新当前操作的工程
    ///
    void UpdateOperatorProj(const MString& ProjID);


    ///
    /// \brief getTestingProjectID
    /// \return
    /// 获取正在测试的工程
    ///
    MString GetTestingProjectID() const;

    ///
    /// \brief RegisterEventFun
    /// \param menuName
    /// \param actionName
    /// \param fun
    /// 注册添加菜单事件
    ///
    void RegisterEventFun(const MString& menuName,const MString& actionName,std::function<bool()> fun);
    void AddMenuToFrame(const MString& menuName,unsigned pos);
    void RemoveMenuFromFrame(const MString& menuName);

    ///
    /// \brief RegisterFrameData
    /// \param FrameData
    /// 注册公共数据
    ///
    void RegisterFrameData(const TL::Vector<MAppDataUnit *> &FrameData);

    ///
    /// \brief RegisterFrameUI
    /// \param FrameUI
    /// 注册公共UI
    ///
    void RegisterFrameUI(const TL::Vector<MAppWindow *> &FrameUI);

    ///
    /// \brief RegisterFrameData
    /// \param FrameData
    /// 反注册公共数据
    ///
	void UnRegisterFrameData(const TL::Vector<MAppDataUnit *> &FrameData);

    ///
    /// \brief RegisterFrameUI
    /// \param FrameUI
    /// 反注册公共UI
    ///
	void UnRegisterFrameUI(const TL::Vector<MAppWindow *> &FrameUI);


    ///
    /// \brief GetFrameInterface
    /// \return
    /// 获取mainwindow的操作接口
    ///
    IFrameWindowInterface* GetFrameInterface();


	//
	// 获取数据库操作指针
	//
	std::shared_ptr<IMSql> GetSqlPtr();

    ///
    /// \brief HideNagTreeWindow
    /// \param ishide
    /// 隐藏导航树窗口
    ///
    void HideNagTreeWindow(bool ishide);

    ///
    /// \brief HideOutputWindow
    /// \param ishide
    /// 隐藏输出窗口
    ///
    void HideOutputWindow(bool ishide);

    ///
    /// \brief Lock
    /// 对测试工程进行保护
    ///
    void Lock();

    ///
    /// \brief UnLock
    /// 解锁保护工程
    ///
    void UnLock();

    ///
    /// \brief isLock
    /// \return
    /// 检查当前是否有锁定
    ///
    bool IsLock();

	void SetCurrentUserName(const MString& UserName);
	MString GetCurrentUserName() const;


	///
	/// \brief UpdateProjectForDataBase
	/// \return
	/// 数据库更新的时候触发该事件
	///
	void UpdateProjectForDataBase();


	//
	// 将工程写进数据库中
	//
	void UpdateProjectToDataBase(const MString& FileName,MProjectType* __Type);


	//
	// 从数据库中获取当前保存的工程列表
	// 按照时间顺序排序
	// 不存在的工程将会被移除
	//
	TL::Vector<std::pair<MString,MString>> GetProjectListFromDataBase();

	//
	// 从数据库中获取指定类型的工程
	//
	TL::Vector<std::pair<MString, MString>> GetProjectListFromDataBaseByType(int Type);


	//
	// 从数据库中获取指定用户的工程
	//
	TL::Vector<std::pair<MString, MString>> GetProjectListFromDataBaseByUser(const MString& userName);

	//
	// 从数据库中获取指定类型指定用户的工程
	//
	TL::Vector<std::pair<MString, MString>> GetProjectListFromDataBaseByUser(int Type,const MString& userName);

	MMenuActionManager*  GetMenuManager();

	///
	/// \brief FinishedSerail
	/// \return
	/// 线程加载完数据后执行函数
	///
	void FinishedSerail(const MString& ProjectName);
protected:
    bool    SaveUiToData(const MString& proj);
    MString GetProjFromInstance(MAppInstance* __instance);
    void    SaveDataDealer(const MString& Proj,IDealDataInterface* __DealData);
    void    EraseDataDealer(const MString& Proj);

	void    SaveEventHanle(const MString& Proj, MAppEventHandle* __DealData);
	void    EraseEventHanle(const MString& Proj);


protected:
    MString             mCurrentProject;            // 当前正在操作的工程 保存的是一个StrID
    MString             mOpingProject;               // 操作的工程，但未必是当前工程
    APPDATATYPE         mAppDatas;                  // 保存所有工程的数据 包括UI信息
    APPPROJTYPE         mProjTypes;                 // 工程信息,工程名 -----> ProjectType
    APPMODULES          mModuleInfo;                // 模块信息
    APPMODULES          mProjInstanceMap;           // 保存工程和instance的关系对应
	PROJEVENTHANDLE		mProjEventHandle;			// 保存工程和对应的事件处理
    PROJDATADEALER      mProjDataDealer;            // 保存工程和对应的数据处理器
  
    INSTACETYPE         mInstanceMap;               // instance和实例之间的映射
    DxUI::CDxWidget*    pParentWindow{nullptr};     // 父窗口,和DataManager一样仅有一个
    MProjectType*       pProjTypeFactor{nullptr};   // 工程类型的工厂对象



    //
    // 下面公用组件通过成员函数直接操作
    //
private:
    //
    // 下面的UI组件是整个程序共同使用的，所以在这里创建
    //
    bool					bIsInishedWindow{false};
	DxUI::CDxStackedWidget* pCenterWindow{ nullptr };
	MLoggerBox*				pLoggerWindow{ nullptr };
	MTreeModule*			pTreeWindow{ nullptr };
	int						mWorkWindowIndex{ -1 };

    //
    // APP相关的通用UI和Data
    //
    MAppFrameUI*        pFrameUI{nullptr};         // 全局UI
	MAppFrameData*      pFrameData{ nullptr };       // 全局数据
	MAppEvent*			pEvent{ nullptr };
    MString             mFrameConfigFile;          // 框架的配置文件


	MString				mCurrentSerialName;
	MMenuActionManager  mMenuManager;

    //
    // 记录APP当前状态,以及状态改变时执行的操作
    //
    APPSTATUS               mAppStatus{STOP};           // 当前是否正在操作工程,比如测试
    APPSTATUSCHANGEFUN      mAppStatusFun{nullptr};     // 工程状态改变时执行的函数
    PROJECTCHANGEFUN        mProjectChangeFun{nullptr}; // 工程改变时执行的回调函数
    APPPROJTYPECHANGEFUN    mAppTypeChangeFun{nullptr}; // 工程类型改变时执行的回调函数
    FINISHEDLOADPROJFUN     mLoadFinsihedProjFun{nullptr}; // 加载完工程后将会被执行的函数
    FINISHEDLOADPROJFUN     mSaveFinsihedProjFun{nullptr}; // 工程保存完成后会执行函数
    FINISHEDLOADPROJFUN     mCloseFinishedProjFun{nullptr}; // 关闭工程后执行的函数
    FINISHEDLOADPROJFUN     mOpenFinsihedProjFun{nullptr}; // 打开工程后所执行的操作
    //
    // mLoadFinsihedProjFun 和 mOpenFinsihedProjFun 不是负责同一件事
    // mLoadFinsihedProjFun 必须在加载完成所有工程配置后方才执行
    // mOpenFinsihedProjFun 打开工程之后便可执行
    //

    //
    // 记录工程的属性
    //
    TL::Map<MString,PROJECTSTATUS> mProjectProperty;


    //
    // 在有特殊需求的情况下可能会连HAppManager都会被重写,那么这就派上了用场
    //
    static MAppManager*				pAppManager;              // 创建App管理的唯一实例
                                                  // 可以通过外部进行创建，然后通过Frame进行绑定
    static bool						bInstanceIsThis;          // 用于判断AppManager是否就是自身
                                                  // 因为如果是自身的话那么在DestroyInstance的时候就可以进行delete

    //
    // 通信模块
    //
    ICommunication*      pCommunication{nullptr};
    MString              mTestingProject;
    bool                 bIsLock{false};


    //
    // hold住一个操作必要的时候可以提取用于辅助完成一些外部完成的任务
    //
    TL::Vector<MAny>          mAnyAction;
    std::map<MString,MAny>     mAnyActionMap;

    //
    // 从dll中加载模块
    //
    static std::unordered_map<MString,CreateInstanceFun> mModuleFuns;

    //
    // 多线程操作,对数据的操作放在线程中,对GUI的操作在主线程中
    //
private:
    void DealCommuniction(bool isok);


	MString						m_CurrentUserName;
    MString						mReadingProjectName;
    DxUI::MSerialize*			pSerial{nullptr};
	DxUI::MSerialize			mFrameConfigSerial;
	std::shared_ptr<IMSql>      m_SqlPtr;
};

