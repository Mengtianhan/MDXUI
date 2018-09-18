

///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution and is Copyright
// 2017-2018 by Mengjin (sh06155@hotmail.com).
// The latest version may be obtained from [******************]
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IObjectBase.h"
#include <objbase.h>
#include "IGUIDInterface.h"

class IServiceInterface;

template<class T,const IID& iid>
class IComBaseInterface : public virtual IObjectBase
{
public:
    IComBaseInterface():mRefNum(1){}
    virtual ~IComBaseInterface(){}

    virtual HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppv){
        *ppv = 0;
        if(riid == iid){
            //
            // 获取指定类型的指针
            //
            *ppv = static_cast<T*>(this);
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    ULONG __stdcall AddRef(){
        if (mRefNum == 0)
            return mRefNum;
        ::InterlockedIncrement(&mRefNum);
        return mRefNum;
    }

    ULONG __stdcall Release(){
        ::InterlockedDecrement(&mRefNum);
        if (mRefNum == 0)
            delete this;
        return mRefNum;
    }

protected:
    ULONG  mRefNum{1};
};
