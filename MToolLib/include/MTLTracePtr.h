#pragma once
#include <MTLSlot.h>
#include <MParamWrap.h>
#include <MTypeWrap.h>

namespace TL{


	template<class T,class DataBinder>
	class MTLTracePtr{
	public:
		MTLTracePtr():pPtr(nullptr){}

		MTLTracePtr(T* obj) :pPtr(obj){
		}

		MTLTracePtr(const MTLTracePtr& other){
			pPtr = other.pPtr;
			bIsAccepted = other.bIsAccepted;
			bIsAttach = other.bIsAttach;
		}

		MTLTracePtr& operator=(const MTLTracePtr& other){
			pPtr = other.pPtr;
			bIsAccepted = other.bIsAccepted;
			bIsAttach = other.bIsAttach;
			return *this;
		}

		template<class Other,class Other2>
		MTLTracePtr(const MTLTracePtr<Other, Other2>& _other){
			static_assert(MIsBDClass(T, Other), "不是继承自同一基类无法执行该操作");
			pPtr = _other.pPtr;
			bIsAccepted = _other.bIsAccepted;
			bIsAttach = _other.bIsAttach;
		}

		template<class Other, class Other2>
		MTLTracePtr(MTLTracePtr<Other, Other2>&& _other){
			static_assert(MIsBDClass(T, Other), "不是继承自同一基类无法执行该操作");
			pPtr = _other.pPtr;
			bIsAccepted = _other.bIsAccepted;
			bIsAttach = _other.bIsAttach;
		}

		template<class Other, class Other2>
		MTLTracePtr& operator=(MTLTracePtr<Other, Other2>&& _other){
			static_assert(MIsBDClass(T, Other), "不是继承自同一基类无法执行该操作");
			pPtr = _other.pPtr;
			bIsAccepted = _other.bIsAccepted;
			bIsAttach = _other.bIsAttach;
			return *this;
		}

		template<class Other, class Other2>
		MTLTracePtr& operator=(const MTLTracePtr<Other, Other2>& _other){
			static_assert(MIsBDClass(T, Other), "不是继承自同一基类无法执行该操作");
			pPtr = _other.pPtr;
			bIsAccepted = _other.bIsAccepted;
			bIsAttach = _other.bIsAttach;
			return *this;
		}

		//
		// 如果追踪对象提前销毁那么将连接断开
		//
		~MTLTracePtr(){
		}


		operator bool(){
			returnp !pPtr;
		}

		T* operator->(){ return pPtr; }
		T& operator*(){ return *pPtr; }
		T* get(){ return pPtr; }
		T* get() const{ return pPtr; }

		//
		// 是否可接收数据
		//
		bool isAccepted() const{
			return bIsAccepted;
		}

		void attach(){
			if (pPtr){
				DataBinder::Connect<T>("DestroySelf", &T::DestroySelf, pPtr,
					MSLOT(&MTLTracePtr::ObjectDestroy, this),
					*this,
					false);
				bIsAttach = true;
				bIsAccepted = true;
			}
		}

		void detach(){
			bIsAccepted = false;
			if (pPtr && bIsAttach){
				bIsAttach = false;
				DataBinder::DisConnect<T>("DestroySelf", &T::DestroySelf, pPtr,
					MSLOT(&MTLTracePtr<T, DataBinder>::ObjectDestroy, this),
					*this);
			}
		}
		void ObjectDestroy(){
			bIsAccepted = false;
			detach();
			std::cout << typeid(T).name() << std::endl;
			pPtr = nullptr;	
		}
	private:
		T*		pPtr{ nullptr };
		bool	bIsAttach{ false };
		bool	bIsAccepted{ true };
	};

	template<class T,class T2,class T3>
	bool operator==(const MTLTracePtr<T,T2>& ptr1, const MTLTracePtr<T,T3>& ptr2){
		if (ptr1.get() != nullptr && ptr1.get() == ptr2.get()){
			return true;
		}
		else if (ptr1.get() == nullptr && ptr1.get() == ptr2.get()){
			return true;
		}
		else{
			return false;
		}
		return true;
	}

	template<class T,class T2,class T3>
	bool operator!=(const MTLTracePtr<T,T2>& ptr1, const MTLTracePtr<T,T3>& ptr2){
		return !(ptr1 == ptr2);
	}

	struct MTLTracePtrEqual{
		template<class T,class T2,class T3>
		bool operator()(MTLTracePtr<T,T2>* ptr1, MTLTracePtr<T,T3>*ptr2){
			if (ptr1 == nullptr && ptr2 == nullptr)
				return true;
			else if (ptr1 != nullptr ptr2 != nullptr){
				return *ptr1 == *ptr2;
			}
			else{
				return false;
			}
		}
	};
}

//
// 包装类
//
template<class ReceiveType, class DataBinder, class Allocator>
class MTypeWrap<TL::MTLTracePtr<ReceiveType, DataBinder>, Allocator> : public Allocator
{
public:
	typedef TL::MTLTracePtr<ReceiveType, DataBinder> Type;
	typedef TL::MTLTracePtr<ReceiveType, DataBinder>& reference_type;
	typedef const TL::MTLTracePtr<ReceiveType, DataBinder>& const_reference_type;

public:
	MTypeWrap(const_reference_type val = Type()) :
		mVal(new MContentType(val))
	{
	}

	MTypeWrap& operator =(const_reference_type val){
		mVal = new MContentType(val);
		return *this;
	}

	operator Type(){
		return mVal->mVal;
	}

	reference_type toType(){
		return mVal->mVal;
	}

	const_reference_type toType() const{
		return mVal->mVal;
	}

	reference_type reference_obj(){
		return mVal->mVal;
	}

	const_reference_type reference_obj() const{
		return mVal->mVal;
	}

	MTypeWrap clone(){
		MTypeWrap newobj;
		newobj.mVal = new MContentType(*mVal);
		newobj.mVal->MarkUnshareable();
		return newobj;
	}
	MTypeWrap clone() const{
		return const_cast<MTypeWrap<Type, Allocator>*>(this)->clone();
	}

	void lock(){
		if (mVal->IsShared()){
			mVal = new MContentType(*mVal);
		}
		mVal->MarkUnshareable();
	}

	void unlock(){
		mVal->MarkShareable();
	}

	bool islock() const{
		return mVal->IsShared();
	}

	const std::type_info & type() const{
		return mVal->type();
	}
	void* Address() const{
		return &(mVal->mVal);
	}

	friend bool operator==(const MTypeWrap& left, const MTypeWrap& right){
		return left.mVal->mVal == right.mVal->mVal;
	}

	friend bool operator!=(const MTypeWrap& left, const MTypeWrap& right){
		return left.mVal->mVal != right.mVal->mVal;
	}
private:
	class MContentType : public MRefImpl<MAllocator<MPoolAllocator<MContentType>>>
	{
	public:
		MContentType(const_reference_type val = Type()) :
			MRefImpl<MAllocator<MPoolAllocator<MContentType>>>(),
			mVal(val)
		{}

		MContentType(const MContentType& other) :
			MRefImpl<MAllocator<MPoolAllocator<MContentType>>>(other),
			mVal(other.mVal)
		{}

		MContentType& operator=(const MContentType& other){
			MRefImpl<MAllocator<MPoolAllocator<MContentType>>>::operator=(other);
			mVal = other.mVal;
			return *this;
		}

		MContentType& operator =(const_reference_type val){
			MRefImpl<MAllocator<MPoolAllocator<MContentType>>>::operator=(*this);
			mVal = val;
			return *this;
		}

		operator Type(){
			return mVal;
		}
		reference_type toType(){
			return mVal;
		}

		const_reference_type toType() const{
			return mVal;
		}

		const std::type_info & type() const{
			return typeid(mVal);
		}
		Type   mVal;
	};
	MRefWrap<MContentType> mVal;
};