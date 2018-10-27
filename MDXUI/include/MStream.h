

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
#include "MLoggerStream.h"
#include <MTLEvent.h>

class MAppManager;

class DXUI_API MStream : public TL::MTLObject
{
    DECLARE_CLASS(MStream)
	typedef std::function<void(const MString&)> HookFunType;
public:
    MStream();
    ~MStream();
	static void BindAppManager(MAppManager *pAppManager);
	static void SetHookFun(HookFunType fun);
	static void SetLogFile(const MString& FileName);
	static MLoggerStream& Log();
    template<class T>
    MStream& operator<<(const T& val){
		mHookFun(val);
		Log()<< val;
		ToLogFile(val);
        return *this;
    }

    template<class T,class...Args>
    MStream& log(const char* sig,const T& value,const Args&... args){
        try{
            MString::setFloatPrecision(6);
			MString str = MString::Format(sig, value, args...);
			mHookFun(str);
			Log() << str;
			ToLogFile(str);
            MString::setFloatPrecision(3);
        }
        catch(std::runtime_error e){
            std::cerr<<e.what()<<std::endl;
        }
        return *this;
    }

    ///
    /// \brief TransData
    /// \param data
    /// 传输数据
    ///
    void TransData(const MString& data);

	//
	// 传输二进制数据
	//
	bool TransData(const char* buffer, __int64 len);

    ///
    /// \brief TransParam
    /// \param param
    ///
    bool TransParam(const MString& param);

	

private:
	void ToLogFile(const MString& str);
    static MLoggerStream   mStreamObj;
	static TL::MTLVoidEvent<MString>   mHookFun;
	static std::ofstream   mLogStream;
};

