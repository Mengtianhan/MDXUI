///////////////////////////////////////////////////////////////////////////////
//
// MToolLib ---- Mengjin Tool Library
// Copyright (C) 2014-2018 by Mengjin (sh06155@hotmail.com)
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
#include "MParamWrap.h"
#include "MString.h"

using mj::MString;

namespace TL{

	//
	// MTypeValue holder 住 MMultType 里面各类型的值
	//
	template<class T>
	class MTypeValue{
	public:
		typedef T value_type;
	private:
		T mValue;
	public:
		MTypeValue() :mValue(T()){}
		MTypeValue(const T& val) :mValue(val){}
		MTypeValue(const MTypeValue& val) :mValue(val.mValue){}
		MTypeValue(T&& val) :mValue(std::forward<T>(val)){}


		template<class U>
		MTypeValue(const MTypeValue<U> &val) : mValue(val.ref()){}

		template<class U>
		MTypeValue(MTypeValue<U>&& val) : mValue(val.ref()){}



		MTypeValue& operator=(const MTypeValue& other){
			mValue = other.mValue;
			return *this;
		}

		MTypeValue& operator=(const T& val){
			mValue = val;
			return *this;
		}

		MTypeValue& operator=(T&& val){
			mValue = std::forward<T>(val);
			return *this;
		}

		MTypeValue& operator=(MTypeValue&& val){
			mValue = std::forward<T>(val.mValue);
			return *this;
		}

		template<class U>
		MTypeValue& operator=(const MTypeValue<U>& val){
			mValue = val.ref();
			return *this;
		}

		template<class U>
		MTypeValue& operator=(MTypeValue<U>&& val){
			mValue = val.ref();
			return *this;
		}

		operator T() const{ return mValue; }
		operator T() { return mValue; }

		//
		// 需要修改原始数据的时候如果不能直接赋值的方式修改那么就是用引用或者指针进行修改
		//
		const T& ref() const{ return mValue; }
		T& ref(){ return mValue; }

		const T* ptr() const{ return &mValue; }
		T* ptr() { return &mValue; }

		friend std::ostream& operator<<(std::ostream& os, const MTypeValue& val){
			os << val.mValue;
			return os;
		}

	};


	//
	// MMultType 类似于标准库的tuple的存在
	//
	template<class>
	class MMultType;

	template<>
	class MMultType<mjTL::MTypeList<>>{
		typedef MEmptyType type;
		typedef MNullType next;
		static const unsigned mSize = 0;
	public:
		bool Equal(const MMultType<mjTL::MTypeList<>>& other) const{
			return true;
		}

		bool Less(const MMultType<mjTL::MTypeList<>>& other) const{
			return true;
		}
	};

	template<class T, class...Args>
	class MMultType<mjTL::MTypeList<T, Args...>> : public MTypeValue<T>,
		public MMultType<mjTL::MTypeList<Args...>>
	{
	public:
		typedef MMultType<mjTL::MTypeList<T, Args...>> self_type;
		typedef MTypeValue<T> type;
		typedef mjTL::MTypeList<T, Args...> value_type;
		typedef typename MMultType<mjTL::MTypeList<Args...>>  next;
		static const unsigned mSize = 1 + sizeof...(Args);

		MMultType() :
			type(),
			next()
		{}

		explicit MMultType(const T& val, const Args&... args) :
			type(val),
			next(args...)

		{}


		template<class...Other>
		MMultType(const MMultType<Other...>& other) :
			type(other.self()),
			next(other.parent())
		{}

		template<class...Other>
		MMultType(MMultType<Other...>&& other) :
			type(other.self()),
			next(other.parent())

		{}

		template<class...Other>
		MMultType& operator=(const MMultType<Other...>& other){
			(type&) = other.self();
			(next&)(*this) = other.parent();
			return *this;
		}

		next&  parent(){
			return *this;
		}

		const next& parent() const{
			return *this;
		}

		self_type& self(){
			return *this;
		}

		const self_type& self() const{
			return *this;
		}

		bool Equal(const self_type& other) const{
			T val1 = TL::get<0>(*this);
			T val2 = TL::get<0>(other);
			if (val1 != val2){
				return false;
			}
			const next& __parent = parent();
			return __parent.Equal(other.parent());
		}

		bool Less(const self_type& other) const{
			T val1 = TL::get<0>(*this);
			T val2 = TL::get<0>(other);
			if (val1 > val2){
				return false;
			}
			const next& __parent = parent();
			return __parent.Less(other.parent());
		}
	};

	template<class T, class... Args>
	static inline bool operator==(const MMultType<mjTL::MTypeList<T, Args...>>& left, const MMultType<mjTL::MTypeList<T, Args...>>& right){
		return left.Equal(right);
	}

	template<class T, class... Args>
	static inline bool operator!=(const MMultType<mjTL::MTypeList<T, Args...>>& left, const MMultType<mjTL::MTypeList<T, Args...>>& right){
		return !(left.Equal(right));
	}

	template<class T, class... Args>
	static inline bool operator<(const MMultType<mjTL::MTypeList<T, Args...>>& left, const MMultType<mjTL::MTypeList<T, Args...>>& right){
		return left.Less(right) && !(left.Equal(right));;
	}

	template<class T, class... Args>
	static inline bool operator>(const MMultType<mjTL::MTypeList<T, Args...>>& left, const MMultType<mjTL::MTypeList<T, Args...>>& right){
		return !left.Less(right) && !(left.Equal(right));
	}

	//
	// MMultType 类型不能直接操作,需要使用下面的这些接口进行操作
	// 对于每一个类型不能够直接操作,需要使用 MTypeValue 来进行操作
	//
	template<class T, class... Args>
	MTypeValue<T>& reference_help(const MMultType<mjTL::MTypeList<T, Args...>>& obj, MIntType<0>){
		typename MMultType<mjTL::MTypeList<T, Args...>>::type& cur_obj = const_cast<MMultType<mjTL::MTypeList<T, Args...>>&>(obj);
		return cur_obj;
	}

	template<int N, class T, class... Args>
	MTypeValue<typename mjTL::index_type<mjTL::MTypeList<T, Args...>, N>::type>& reference_help(
		const MMultType<mjTL::MTypeList<T, Args...>>& obj, MIntType<N>)
	{
		typename MMultType<mjTL::MTypeList<T, Args...>>::next& next_obj = const_cast<MMultType<mjTL::MTypeList<T, Args...>>&>(obj);
		return reference_help(next_obj, MIntType<N - 1>());
	}

	//
	// 获取指定对象的操作
	//	
	template<int N, class T, class...Args>
	MTypeValue<typename mjTL::index_type<typename mjTL::MTypeList<T, Args...>, N>::type>& reference_at(const MMultType<mjTL::MTypeList<T, Args...>>& obj)
	{
		MIntType<N> index;
		return reference_help(obj, index);
	}

	//
	// 返回const引用
	//
	template<int N, class T, class...Args>
	const MTypeValue<typename mjTL::index_type<typename mjTL::MTypeList<T, Args...>, N>::type>& const_reference_at(const MMultType<mjTL::MTypeList<T, Args...>>& obj)
	{
		MIntType<N> index;
		return reference_help(obj, index);
	}

	//
	// 解引用
	//
	template<int N, int M>
	struct MMultTypeTieHelp{
		template<class T, class T1, class...Args>
		static void Apply(const T& obj, T1& res, Args&...agrs)
		{
			auto out = const_reference_at<N>(obj);
			res = out;
			MMultTypeTieHelp<N + 1, M>::Apply(obj, agrs...);
		}
	};

	template<int M>
	struct MMultTypeTieHelp <M, M>{
		template<class T, class T1>
		static void Apply(const T& obj, T1& res)
		{
			auto& out = const_reference_at<M>(obj);
			res = out;
		}
	};

	// 
	// 将 MMultType 内部数据解引用出来
	//
	template<class T, class...Args>
	inline void tie(const MMultType<mjTL::MTypeList<T, Args...>>& obj,
		T& res, Args&...args)
	{
		typedef MMultType<mjTL::MTypeList<T, Args...>> type;
		MMultTypeTieHelp<0, type::mSize - 1>::Apply(obj, res, args...);
	}

	//
	// 获取类型大小
	//
	template<class T, class...Args>
	inline size_t type_num(const MMultType<mjTL::MTypeList<T, Args...>>& obj){
		return 1 + sizeof...(Args);
	}

	template<int N, int M>
	struct MMultTypePrintHelp{
		template<class T>
		static void Apply(const T& obj, std::ostream& os, const char* spliter)
		{
			os << const_reference_at<N>(obj) << spliter;
			MMultTypePrintHelp<N + 1, M>::Apply(obj, os, spliter);
		}
	};

	template<int M>
	struct MMultTypePrintHelp <M, M>{
		template<class T>
		static void Apply(const T& obj, std::ostream& os, const char* spliter)
		{
			os << const_reference_at<M>(obj);
		}
	};

	//
	// 实现打印操作
	//
	template<class T, class...Args>
	std::ostream& operator<<(std::ostream& os, const MMultType<mjTL::MTypeList<T, Args...>>& obj){
		typedef MMultType<mjTL::MTypeList<T, Args...>> type;
		MMultTypePrintHelp<0, type::mSize - 1>::Apply(obj, os, "\t");
		return os;
	}

	template<class T, class...Args>
	void print(const MMultType<mjTL::MTypeList<T, Args...>>& obj, std::ostream& os, const char* spliter, const char* end = "\n"){
		typedef MMultType<mjTL::MTypeList<T, Args...>> type;
		MMultTypePrintHelp<0, type::mSize - 1>::Apply(obj, os, spliter);
		os << end;
	}

	//
	// 生成一个多类型的数据
	//
	template<class T, class...Args>
	MMultType<mjTL::MTypeList<T, Args...>> make_multtype(const T& t1, const Args&...args){
		return MMultType<mjTL::MTypeList<T, Args...>>(t1, args...);
	}

	//
	// 两个多类型合成一个多类型
	//

	template<int N, int M, int I, int J>
	struct MMultTypeCombineHelp{
		template<class T1, class T2>
		static void Apply(T1& obj1, T2& obj2){
			auto& val1 = reference_at<N>(obj1);
			auto& val2 = reference_at<I>(obj2);
			val1 = val2;
			MMultTypeCombineHelp<N + 1, M, I + 1, J>::Apply(obj1, obj2);
		}
	};

	template<int N, int M, int J>
	struct MMultTypeCombineHelp<N, M, J, J>{
		template<class T1, class T2>
		static void Apply(T1& obj1, T2& obj2){
			auto& val1 = reference_at<N>(obj1);
			auto& val2 = reference_at<J>(obj2);
			val1 = val2;
		}
	};

	template<int N, int M, int J>
	struct MMultTypeCombineHelp<N, M, J, -1>{
		template<class T1, class T2>
		static void Apply(T1& obj1, T2& obj2){
			;
		}
	};


	template<class...Args1, class...Args2>
	MMultType<mjTL::MTypeList<Args1..., Args2...>> multtype_cat(
		const MMultType<mjTL::MTypeList<Args1...>>& obj1,
		const MMultType<mjTL::MTypeList<Args2...>>& obj2)
	{
		typedef MMultType<mjTL::MTypeList<Args1..., Args2...>> RType;
		RType res;
		MMultTypeCombineHelp<0, RType::mSize - 1, 0, sizeof...(Args1)-1>::Apply(res, obj1);
		MMultTypeCombineHelp<sizeof...(Args1), RType::mSize - 1, 0, sizeof...(Args2)-1>::Apply(res, obj2);
		return res;
	}

	//
	// 让字符串转数组转换成带类型的tuple
	//
	template<size_t index, size_t M, class T>
	struct MTypeConvert;

	template<size_t index, size_t M, class ... Args>
	struct MTypeConvert<index, M, MMultType<mjTL::MTypeList<Args...>>>{
		typedef typename mjTL::index_type<mjTL::MTypeList<Args...>, index>::type Type;

		template<class T>
		struct Apply{
			Apply(T t) :mT(t){}
			inline void apply(std::vector<MString>& v)
			{
				MString str = v.front();
				str.trim();
				v.erase(v.begin());
				auto tt = multtype_cat(mT, make_multtype(str.ToOtherType<Type>()));
				MTypeConvert<index + 1, M, MMultType<mjTL::MTypeList<Args...>>>::Apply<decltype(tt)>(tt).apply(v);
			}
			T mT;
		};

	};

	template<size_t M, class ... Args>
	struct MTypeConvert<M, M, MMultType<mjTL::MTypeList<Args...>>>{
		typedef  typename mjTL::index_type<mjTL::MTypeList<Args...>, M - 1>::type Type;
		template<class T>
		struct Apply{
			Apply(T t) :mT(t){}
			inline void apply(std::vector<MString>& v)
			{
				throw mT;
			}
			T mT;
		};

	};

	template<class...Args>
	bool FromStr(const MString& str, MMultType<mjTL::MTypeList<Args...>>& output, const std::string& spliter = " \t,:"){
		MString Mstr = str;
		std::vector<MString> v;
		Mstr.split(spliter, v);
		if (v.size() < sizeof...(Args)){
			return false;
		}
		try{
			MTypeConvert<0, sizeof...(Args), MMultType<mjTL::MTypeList<Args...>>>::Apply<MMultType<mjTL::MTypeList<>>>(MMultType<mjTL::MTypeList<>>()).apply(v);
			return false;
		}
		catch (MMultType<mjTL::MTypeList<Args...>> e){
			output = std::forward<MMultType<mjTL::MTypeList<Args...>>&&>(e);
			return true;
		}
		catch (...){
			return false;
		}
	}

	template<class...Args>
	inline mj::MString ToStr(const MMultType<mjTL::MTypeList<Args...>>& obj, const char* spliter){
		std::stringstream s;
		print(obj, s, spliter, "");
		return s.str();
	}

	template<class...Args>
	inline std::istream& operator>>(std::istream& is, std::tuple<Args...>& t){
		std::string str;
		std::getline(is, str);
		if (str.empty())
			return is;
		t = FromStr(str, t);
		return is;
	}

	template<int M, class T>
	void get(const T& val)
	{
		static_assert(MFalse::value, "Function Not Support This Action");
	}

	template<int M, class...Args>
	typename mjTL::index_type<mjTL::MTypeList<Args...>, M>::type& get(const TL::MMultType<mjTL::MTypeList<Args...>>& val)
	{
		typedef typename mjTL::index_type<mjTL::MTypeList<Args...>, M>::type __Type;
		MTypeValue<__Type>&  out = reference_at<M>(val);
		return out.ref();
	}

	//
	// 兼容标准库 tuple
	//
	template<int M, class...Args>
	typename mjTL::index_type<mjTL::MTypeList<Args...>, M>::type get(const std::tuple<Args...>& val){
		return std::get<M>(val);
	}
}

//
// 标准库的 get 兼容 MMultType
//
namespace std{
	template<int M, class...Args>
	typename mjTL::index_type<mjTL::MTypeList<Args...>, M>::type& get(const TL::MMultType<mjTL::MTypeList<Args...>>& val){
		return TL::get<M>(val);
	}
}