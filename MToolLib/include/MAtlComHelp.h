#pragma once
#include <comdef.h>
#include <atlbase.h>
#include <atlcom.h>

template<class BaseType,class TYPE>
struct ATL_NO_VTABLE MAtlIUnkwonImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public BaseType
{
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObject)
	{
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef(void){
		return InternalAddRef();
	}

	ULONG STDMETHODCALLTYPE Release(void){
		long l = InternalRelease();
		if (l == 0){
			delete this;
		}
		return l;
	}
};