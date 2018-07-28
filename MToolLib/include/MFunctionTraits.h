#pragma once
#include <type_traits>
#include <tuple>
#include <vector>
#include <HTuple.h>

//
// 默认情况下不引入别的东西
// 如有必要使用方才引入boost
// 主要是使用模板参数类型推导上面
//
#ifdef __M_BOOST__
#include <type_traits>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_convertible.hpp>
#endif


#define HAS_MEMBER(member)  \
template<typename T, typename... Args>\
struct HasMemberFun_##member\
{ \
private:\
	template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	template<typename U> static std::false_type Check(...); \
public:\
enum{ value = std::is_same<decltype(Check<T>(0)), std::true_type>::value }; \
};


namespace mj{

#ifdef __M_BOOST__
    //
	// 模板参数类型自行推导
    //
	template<class T>
	struct param_type : boost::mpl::eval_if <
		boost::mpl::or_ < std::is_scalar<T>,
		std::is_reference < T >> ,
		boost::mpl::identity<T>, boost::add_reference <T >>
	{};

	template<class T>
	struct const_param_type : boost::mpl::eval_if <
		boost::mpl::or_ < std::is_scalar<T>,
		std::is_reference < T >> ,
		boost::mpl::identity<T>, boost::add_reference <T const >>
	{};
#endif


    //
	// 定义一个类型列表
    //
	template<class...Args>
	class MTypeList;

	template<>
	class MTypeList<>{};

	template<class T, class...Args>
	class MTypeList<T, Args...> : public MTypeList<Args...>
	{
	public:
		typedef T current_type;
		typedef MTypeList<Args...> parent_type;
	};

    //
	// 类型元素
    //
	template<size_t index, class T>
	struct MType_Element;

	template<class T, class ...Args>
	struct MType_Element<0, MTypeList<T, Args...>>{
		typedef T value_type;
		typedef MTypeList<T, Args...> type;
	};

	template<size_t index, class T, class...Args>
	struct MType_Element<index, MTypeList<T, Args...>> : MType_Element<index - 1, MTypeList< Args...>>
	{};

    //
	// 类型的元素个数
    //
	template<class T>
	struct MType_Size;

	template<class...Args>
	struct MType_Size<MTypeList<Args...>> : std::integral_constant<size_t, sizeof...(Args)>{};

    //
	// 元素类型
    //
	template<size_t index, class T>
	struct MAt;

	template<size_t index, class ...Args>
	struct MAt<index, MTypeList<Args...>>{
		typedef typename MType_Element<index, MTypeList<Args...>>::value_type type;
	};

    //
	// 压入类型
    //
	template<class T,class Type>
	struct Push_Back;

	template<class ...Args,class T>
	struct Push_Back<MTypeList<Args...>, T>{
		typedef MTypeList<Args..., T> type;
	};
    //
	// 函数特征性
    //
	template<class T>
	struct MFunction_Traits;

	template<typename R, typename... Args>
	struct MFunction_Traits<R(Args...)>
	{
		typedef R return_type;
		typedef MTypeList<Args...> arglist;
		enum { Args_count = sizeof...(Args) };
		template<unsigned int N>
		struct Args
		{
			static_assert(N < Args_count, "Args index out of range ...... ");
			typedef typename MAt<N, arglist>::type type;
		};
	};

	template<typename R, typename... Args>
	struct MFunction_Traits<R(*)(Args...)>
	{
		typedef R return_type;
		typedef MTypeList<Args...> arglist;
		enum { Args_count = sizeof...(Args) };
		template<unsigned int N>
		struct Args
		{
			static_assert(N < Args_count, "Args index out of range ...... ");
			typedef typename MAt<N, arglist>::type type;
		};
	};


     //
	// 针对成员函数进行特化
    //
    template <typename T,typename R,  typename... Args>
	struct MFunction_Traits<R(T::*)(Args...)>
	{
		typedef T class_type;
		typedef R return_type;
		typedef MTypeList<Args...> arglist;
		enum { Args_count = sizeof...(Args) };
		template<unsigned int N>
		struct Args
		{
			static_assert(N < Args_count, "Args index out of range ...... ");
			typedef typename MAt<N, arglist>::type type;
		};
	};

    //
	// 函数类型推断
	// 解决函数重载问题
    //
	template<typename ...Args>
        struct OverloadResolved{
		template<typename R>
		static auto static_doit(R(*f) (Args...), Args ... args) -> R { return f(args...); }
	};


        template<class T,class...Args>
        struct OverloadResolvedForMem{
            template<typename R>
            static auto static_doit(R(T::*f)(Args...), T* obj,Args&&... args) -> R {
                return (obj->*f)(args...);
            }
        };

	template<typename ...Args>
	auto deduce(Args...) -> OverloadResolved<Args...> { return OverloadResolved<Args...>(); }

        template<class T,class... Args>
        auto deduceformem(T*,Args...)->OverloadResolvedForMem<T,Args...>{
            return OverloadResolvedForMem<T,Args...>();
        }

	template<typename T>
	struct dummy : public T { };

        template<typename T>
        struct dummyformem : public T{};



    //
	// 生成一个整数系列
	// 是一个反向系列
    //
	template<size_t N>
	struct __Int : public __Int<N-1>{
		typedef __Int<N> type;
		static const size_t value = N;
	}; //整形外覆器

	template<>
	struct __Int<0>{
		typedef __Int<0> type;
		static const size_t value = 0;
	};

	template<size_t index,class T>
	struct __Int_At;

	template<size_t N>
	struct __Int_At<0, __Int<N>>{
		static const size_t value = __Int<N>::value;
	};

	template<size_t index,size_t N>
	struct __Int_At<index, __Int<N>> : public __Int_At<index - 1, __Int<N - 1>>{};

    //
	// 操纵函数的函数
	// 将tuple解包
    //
	template<size_t N>
	struct Apply{
		template<class F,class T,class...Args>
		static inline auto apply(F&& f, T&& t, Args&&...args)->decltype(Apply<N - 1>::apply(
			std::forward<F>(f),
			std::forward<T>(t),
			std::get<N - 1>(std::forward<T>(t)),
			std::forward<Args>(args)...))
		{
			return Apply<N - 1>::apply(
				std::forward<F>(f),
				std::forward<T>(t),
				std::get<N - 1>(std::forward<T>(t)),
				std::forward<Args>(args)...);
		}
	};

	template<>
	struct Apply<0>{
		template<class F,class T,class ...Args>
		static inline auto apply(F&& f, T&& t, Args&&...args)->decltype(std::forward<F>(f)(
			std::forward<Args>(args)...))
		{
			return std::forward<F>(f)(std::forward<Args>(args)...);
		}
	};



	template<class F, class T>
	inline auto ManualFun(F&& f, T&& t)->decltype(
		Apply<std::tuple_size<typename std::decay<T>::type>::value>::apply(
		std::forward<F>(f), std::forward<T>(t))
		)
	{
		return Apply<std::tuple_size<typename std::decay<T>::type>::value>::apply(
			std::forward<F>(f), std::forward<T>(t));
	}

    //
	// 操纵函数的函数
	// 将参数打包tuple
    //
	template<class F,class...Args>
	inline auto ManualFun(F&& f, Args...args)->decltype(
		Apply<sizeof...(Args)>::apply(
		std::forward<F>(f), std::move(std::make_tuple(args...)))
		)
	{
		return Apply<sizeof...(Args)>::apply(
			std::forward<F>(f), std::move(std::make_tuple(args...)));
	}
}


//
//外部包装函数
//
#define MRunFun(fun, ...) (mj::dummy<decltype(mj::deduce( __VA_ARGS__ ))> :: static_doit(fun, __VA_ARGS__) )

//
// 包装成员函数
//
#define MRunMemFun(fun,obj,...) (mj::dummyformem<decltype(mj::deduceformem(obj,__VA_ARGS__))>::static_doit(fun,obj,__VA_ARGS__))

//
// 包装任意函数
//
#define MRunAnyFun(fun,...) (fun(__VA_ARGS__))


//
// 解析参数，将参数打包
//
namespace mj{
    //
	// 将字符串解析为参数类型
    //
	template<class F, size_t M = 0, size_t N = MFunction_Traits<F>::Args_count>
	struct ParseStrToTuple{
		typedef typename MFunction_Traits<F>::template Args<M>::type type;
		typedef typename MFunction_Traits<F>::return_type r_type;
		template<class T>
		static inline r_type apply(F&& f, std::vector<std::string>& vStr, T&& args)
		{		
			MString str(vStr[M]);
			return ParseStrToTuple<F, M + 1, N>::apply(std::forward<F>(f), 
				vStr,std::tuple_cat(args, std::make_tuple(str.ToOtherType<type>())));
		}
	};

	template<class F,size_t N>
	struct ParseStrToTuple<F,N,N>{
		typedef typename MFunction_Traits<F>::return_type r_type;
		template<class T>
		static inline r_type apply(F&& f, std::vector<std::string>& vStr, T&& args)
		{
			return ManualFun(std::forward<F>(f), std::forward<T>(args));
		}
	};

	template<class F>
	inline auto ManualFun(F& f, std::vector<std::string>& vStr)->decltype(
		ParseStrToTuple<F, 0, MFunction_Traits<F>::Args_count>::apply(
		std::forward<F>(f),
		vStr, std::tuple<>())
		)
	{
		return ParseStrToTuple<F, 0, MFunction_Traits<F>::Args_count>::apply(std::forward<F>(f), 
			vStr, std::tuple<>());
	}

        inline std::vector<std::string> StringToVector(const MString& params, const MString& spliter){
		std::vector<std::string> result;
		params.split(spliter, result);
		return result;
	}

	template<class F>
	inline auto ManualFunWithString(F& f, const MString& params, const MString& spliter)->decltype(
		ManualFun(f, StringToVector(params, spliter))
		)
	{
		return ManualFun(f, StringToVector(params, spliter));
	}
}




