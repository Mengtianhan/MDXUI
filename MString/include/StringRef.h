#pragma once

#ifndef _WIN64
#ifndef uint64
typedef unsigned __int32 uint64;
#endif
#else
#ifndef uint64
typedef unsigned __int64 uint64;
#endif
#endif
//
// 修改下面的预定于宏可修改预分配内存大小 预定义为8k
//
#ifndef MSTRINGMAXMEMERYSIZE
#define MSTRINGMAXMEMERYSIZE 8096
#endif

namespace mj{

	template<typename T>
	class  StringRef{
	public:
        StringRef(T* t = nullptr) :p_Ptr(t){
			init();
		}

		StringRef(const StringRef& p) :p_Ptr(p.p_Ptr){
			init();
		}

		StringRef& operator=(const StringRef& p){
			if (p_Ptr != p.p_Ptr){
				if (p_Ptr){
                    p_Ptr->RemRc();
				}
                p_Ptr = p.p_Ptr;
                init();
			}
			return *this;
		}

		~StringRef(){
			if (p_Ptr){
				p_Ptr->RemRc();
			}
		}

		T* operator->() const{
			return p_Ptr;
		}

		T& operator*() const{
			return *p_Ptr;
		}

		operator char*() const{
			return p_Ptr->c_str();
		}

	private:
		T* p_Ptr;

		void init(){
			if (!p_Ptr){
				return;
			}
			if (!(p_Ptr->IsShareable())){
				p_Ptr = new T(*p_Ptr);
			}
			p_Ptr->AddRc();
		}
	};


	///
	///  实现引用计数
	///

    ///
    /// \brief The StringRefrenceObj class
    ///
    class   StringRefrenceObj{
	public:
		void AddRc();
		void DecRc();
		void RemRc();
		void MarkUnshareable();
		bool IsShareable();
		bool IsShared();
        uint64 Count();
	private:
        uint64 mCount;
		bool  bIsShared;
	protected:
		StringRefrenceObj();
		StringRefrenceObj(const StringRefrenceObj&);
		StringRefrenceObj& operator=(const StringRefrenceObj&);
		virtual ~StringRefrenceObj();
	};
}


//
// 自动加载lib
//
//=========================================================================
//
//
// x64
//
#ifdef _WIN64

#ifdef _DEBUG
#pragma  comment(lib,"MStringd_x64")
#else
#pragma  comment(lib,"MString_x64")
#endif

#else


//
// x86
//
#ifdef _DEBUG
#pragma  comment(lib,"MStringd")
#else
#pragma  comment(lib,"MString")
#endif

#endif // _WIN64


