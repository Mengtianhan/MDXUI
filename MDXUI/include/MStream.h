

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MLoggerStream.h"

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
		if (mHookFun){
			mHookFun(val);
		}
		Log()<< val;
		ToLogFile(val);
        return *this;
    }

    template<class T,class...Args>
    MStream& log(const char* sig,const T& value,const Args&... args){
        try{
            MString::setFloatPrecision(6);
			MString str = MString::Format(sig, value, args...);
			if (mHookFun){
				mHookFun(str);
			}
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
	static HookFunType	   mHookFun;
	static std::ofstream   mLogStream;
};

