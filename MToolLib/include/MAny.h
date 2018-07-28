#pragma once

#include <type_traits>
#include <typeinfo>
#include "HEnableIf.h"
#include <utility>
#include <type_traits>
namespace mj{
	//-------------------------------------------------------------
	// 使用模板元特性构建一个表示任意类型的东西
	//-------------------------------------------------------------
	class MAny{
	public:
		//---------------------------------------------------
		// 占位标识
		// 此处作为基类的接口，目的是消除派生类中的模板参数
		//----------------------------------------------------
		class MPlaceholder
		{
		public: 
			virtual ~MPlaceholder()
			{
			}

		public: 
			virtual const std::type_info & type() { return typeid(nullptr); }
			virtual MPlaceholder * clone() const{ return nullptr; }
                        virtual bool equals(const MPlaceholder* other) const{return false;}
		};

		template<typename T>
		class holder : public MPlaceholder
		{
		public: 
			holder(const T & value)
				: held(value)
			{
			}
			holder(T&& value)
				: held(static_cast<T&& >(value))
			{
			}

		public: 
			virtual const std::type_info & type()
			{
					return typeid(T);
			}

			virtual MPlaceholder* clone() const
			{
				return new holder(held);
			}


		public: 
			T held;
		private: 
			holder & operator=(const holder &) = delete;
		};

	public:
		MAny(): mValue(nullptr)
		{
		}

		template<typename T>
		MAny(const T & value)
			: mValue(new holder<typename std::decay<const T>::type>(value))
		{
		}

		MAny(const MAny & other)
			: mValue(other.mValue ? other.mValue->clone() : 0)
		{
		}

		MAny(MAny&& other)
			: mValue(other.mValue)
		{
			other.mValue = nullptr;
		}


		template<typename T>
		MAny(T&& value
			, typename disable_if<std::is_same<MAny&, T> >::type* = 0 
			, typename disable_if<std::is_const<T> >::type* = 0) 
			: mValue(new holder<typename std::decay<T>::type >(static_cast<T&&>(value)))
		{
		}

		~MAny(){
			if (mValue){
				delete mValue;
				mValue = nullptr;
			}	
		}

		MAny& swap(MAny& rhs)
		{
			std::swap(mValue, rhs.mValue);
			return *this;
		}

		MAny & operator=(const MAny& rhs)
		{
			MAny(rhs).swap(*this);
			return *this;
		}

		MAny& operator=(MAny&& rhs)
		{
			rhs.swap(*this);
			MAny().swap(rhs);
			return *this;
		}

		template <class ValueType>
		MAny& operator=(ValueType&& rhs)
		{
			MAny(static_cast<ValueType&&>(rhs)).swap(*this);
			return *this;
		}


		bool empty() const
		{
			return !mValue;
		}

		void clear()
		{
			MAny().swap(*this);
		}

		const std::type_info & type() const
		{
			return mValue ? mValue->type() : typeid(void);
		}

                //
                // 仅仅判断是否是同一类型,如果是同一类型即判断为相等
                //
                friend bool operator==(const MAny& first,const MAny& second){
                    return strcmp(first.type().name(),second.type().name()) == 0?true:false;
                }

	private:
		template<typename T>
		friend T* any_cast(MAny*) ;

		template<typename T>
		friend T* unsafe_any_cast(MAny*);

		MPlaceholder* mValue;
	};

	inline void swap(MAny& lhs, MAny& rhs)
	{
		lhs.swap(rhs);
	}

	class  bad_any_cast : public std::bad_cast
	{
	public:
		virtual const char * what() const
		{
			return "mj::bad_any_cast: "
				"failed conversion using mj::any_cast";
		}
	};

	template<typename T>
	T* any_cast(MAny* operand){
		return operand &&
		operand->type() == typeid(T)
		? &static_cast<MAny::holder<T>*>(operand->mValue)->held
		: 0;
	}

	template<typename T>
	inline const T* any_cast(const MAny* operand)
	{
		return any_cast<T>(const_cast<MAny*>(operand));
	}

	template<typename T>
	T any_cast(MAny& operand)
	{
		typedef typename std::remove_reference<T>::type nonref;
		nonref* result = any_cast<nonref>(&operand);
		if (!result)
			throw bad_any_cast();
		typedef typename if_<
			std::is_reference<T>,
			T,
			typename std::add_reference<T>::type
		>::type ref_type;

		return static_cast<ref_type>(*result);
	}

	template<typename T>
	inline T any_cast(const MAny& operand)
	{
		typedef typename std::remove_reference<T>::type nonref;
		return any_cast<const nonref &>(const_cast<MAny&>(operand));
	}

	template<typename T>
	inline T any_cast(MAny&& operand)
	{
		static_assert(
			std::is_rvalue_reference<T&&>::value 
			|| std::is_const< typename std::remove_reference<T>::type >::value,
			"mj::any_cast shall not be used for getting nonconst references to temporary objects"
			);
		return any_cast<T>(operand);
	}

	template<typename T>
	inline T* unsafe_any_cast(MAny* operand)
	{
		return &static_cast<MAny::holder<T> *>(operand->mValue)->held;
	}

	template<typename T>
	inline const T* unsafe_any_cast(const MAny* operand)
	{
		return unsafe_any_cast<T>(const_cast<MAny*>(operand));
	}
}
