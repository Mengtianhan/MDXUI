

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DxConfig.h"

namespace DxUI{
	class MSerialize;
}

//-----------------------------
// 管理工程类型
// 以及对工程信息的创建
//-----------------------------

class DXUI_API MProjectType
{
public:
	MProjectType();
	virtual ~MProjectType();

	virtual void InitProject();

	//
	// 创建自己
	//
	virtual MProjectType* CreateThis();
	virtual void Destroy();

	//-----------------------------------------------------
	// SetProjectInfo   设置工程信息，工程路径包括工程名 比如：
	//                  工程名为 Test
	//                  路径为   E:/Project
	//                  那么设置的就是 E:/Project/Test
	// SetProjectType   工程类型使用无符号整数来表示
	//                  好处是可以方便枚举
	//                  字符串转换到整数也很方便
	// CreateProject    在设置好工程类型和工程信息后就可以创建工程了
	//-------------------------------------------------------
	void SetProjectInfo(const MString& projectinfo);
	void SetProjectType(unsigned type);
	void SetProjectType(const MString& strType);
	void SetModuleName(const MString& moduleName);


	//-------------------------------------------------------
	// 如有必要可用重写该函数
	// CreateProject 用于新建工程
	// OpenProject   用于打开工程
	// DealProject   用于另存为之后需要处理的一些文件信息
	//    比如log文件的重新定位
	//    config文件的重新定位
	//    data文件的重新定位等
	//    NewProjName 可带后可不带后缀
	//    带后缀被当作工程文件处理
	//    不带后缀被当作工程文件夹处理
	//-------------------------------------------------------
	virtual bool CreateProject();
	virtual bool OpenProject(const MString& FileName);
	virtual bool DealProject(const MString& NewProjName);

	virtual bool ClearProject();

	//------------------------------------------------------
	// 工程是否已经创建
	//------------------------------------------------------
	bool HaveBeenCreated();

	//---------------------------------------
	// GetProjectName 返回的只是工程的名字
	// GetProjectPath 返回的是工程所在的全路径
	// LoggerFile     返回当前工程的Logger文件
	// ConfigFile     返回当前工程的Config文件
	// DataFolderPath 返回储存当前工程数据的路径
	// GetProjectType 工程类型使用无符号整数来标定
	// ModuleName     工程实例的模块名
	// DataHeaderFile 记录数据的头文件
	//---------------------------------------
	const MString& GetProjectName() const;
	const MString& GetProjectPath() const;
	const MString& LoggerFile() const;
	const MString& ConfigFile() const;
	const MString& DataFolderPath() const;
	const MString& ModuleName() const;
	const MString& DataHeaderFile() const;

	//----------------------------------
	// 获取用于序列化的指针
	//----------------------------------
	DxUI::MSerialize* AppSerailPtr() const;

	unsigned GetProjectType();

	//
	// 修改工程扩展名
	//
	static void RegisterProjectExtension(const MString& extension);
	static MString ProjectExtension();

private:
	bool            bIsCreate{ false };
	MString         mProjectInfo; // 保存工程的全名包括路径和自身的名字
	unsigned        mProjectType; // 无工程时该值为-1
	MString         mProjectName;
	MString         mProjectPath;

	MString         mLoggerFile;  // 记录logger文件
	MString         mConfigFile;  // 记录config文件
	MString         mDataFolder;  // 记录保存数据的文件夹路径
	MString         mDataHeaderFile; // 记录数据的头文件

	MString         mModuleName;  // 创建工程的模块名

	DxUI::MSerialize*    pAppSerial;      // 管理工程数据,主要用于读盘和存盘

	static MString  mExtensionName;  // 工程扩展名
};

