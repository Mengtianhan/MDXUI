//======================================
// 一个简单的分配器
//======================================
#pragma once
#include <memory>

namespace mj{
	//=====================================
	// 内存池
	//=====================================
	class MPool{
	private:
		struct Link{
			Link* next{ nullptr };
		};
		struct Chunk{
			enum{ size = 8 * 1024 - 16 };  // 8k字节
			char mem[size];
			Chunk* next{ nullptr };
		};

		Chunk*		 pChunks{ nullptr };
		const size_t mSize{ sizeof(Link) };
		Link*		 pHead{ nullptr };
		__int64		 mCurrentSize{ 0 };
		MPool(const MPool&) = delete;
		MPool& operator=(const MPool&) = delete;

		void IncreaceMem();

	public:
		MPool(size_t n);
		~MPool();
		void* Alloc();
		void Free(void*);
		__int64 CurrentMem();
	};


	//===================================
	// 分配器
	//====================================
	template<class T>
	class MAllocator{
		MPool mPool;
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* point;
		typedef const T* const_point;
		typedef T& reference;
		typedef const T& const_reference;

		MAllocator() throw():
			mPool(sizeof(T))
		{
		}

		template<class U>
		MAllocator(const MAllocator<U>& other) throw(){}

		~MAllocator()throw(){}

		point address(reference r){
			return &r;
		}
		const_point address(const_reference cr){
			return &cr;
		}

		point allocator(size_t n){
			point p = reinterpret_cast<point>(mPool.Alloc());
			for (int i = 1; i < n; ++i){
				mPool.Alloc();
			}
			return p;
		}

		void deallocator(point p, size_t n){
			for (int i = 0; i < n; ++i){
				mPool.Free(p);
				p = p - sizeof(point);
			}
		}

		void construct(point p, const T& value){
			new(p)T(value);
		}

		void destroy(point p){
			p->~T();
		}

		size_type max_size() throw(){
			return size_type(-1) / sizeof(T);
		}

		__int64 CurrentMem(){
			return mPool.CurrentMem();
		}

		void clear(){
			mPool.~MPool();
		}

		template<class U>
		using Other = MAllocator<U>;	
	};
//	template<class T>
//	MPool MAllocator<T>::mPool(sizeof(T));
}
