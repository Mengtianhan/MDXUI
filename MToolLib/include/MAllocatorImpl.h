#pragma once

#include <vector>
#include <MSingleton.h>
#include <assert.h>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <MMap.h>
#include <map>
#include <boost/timer.hpp>
//
// 预定义简单的线程模型
//
struct SingleThreadMode{
	inline void Lock(){}
	inline void UnLock(){}
};

struct MultThreadMode{
	inline void Lock(){
		mMutex.lock();
	}

	inline void UnLock(){
		mMutex.unlock();
	}
	std::mutex mMutex;
};


template<class ThreadMode>
class MMemPool;
typedef MSingleton<MMemPool<SingleThreadMode>> MAllocator2;

//
// 所有派生自该类的子类都可以实现快速分配内存
//
class MObjectBase{
public:
	static void* operator new(std::size_t size);
	static void operator delete(void* p, std::size_t size);
	virtual ~MObjectBase(){}
};


//
// 如果想要修改内存池大小可以修改下面的宏定义
//
#ifndef MBLOCKSIZE
#define MBLOCKSIZE 4096
#endif

#ifndef MBLOCKNUM
#define MBLOCKNUM 255
#endif

/*************************************************************************************/
//
// row 内存布局描述
//     1 ---------------------
//     2 ---------------------
//     ........................
//     255--------------------
//
//
class MChunck{
public:
	MChunck();
	MChunck(const MChunck& other);
	MChunck& operator=(const MChunck& other);
	void Init(unsigned __int32 blocksize, unsigned __int8 bloks);
	void Release();
	void* Allocator(unsigned __int32 size);
	void Deallocator(void* p, unsigned __int32 size);
	unsigned char* pMemeray{ nullptr };
	unsigned __int8 mAvailbelIndex{ 0 };
	unsigned __int8 mAvailbelBlocks{ 0 };
	std::mutex		mMutex;
};

//
// 分配大小固定内存
//
class MFixAllocator{
public:
	MFixAllocator(unsigned __int32 blocksize, unsigned __int8 blocks);
	MFixAllocator(const MFixAllocator& other);
	MFixAllocator& operator=(const MFixAllocator& other);
	~MFixAllocator();

	void* Allocator();
	void Deallocator(void* p);
	bool Check(void* p, unsigned __int32 size, unsigned __int32& start, unsigned __int32& end);
private:
	unsigned __int32      mBlockSize;    // 一个chunck有多少的row有多少
	unsigned __int8		  mBlockNum;  // 一个chunck中有多少个row
	std::vector<MChunck>  mChuncks;
	MChunck*			  pAllocatorChunck{ nullptr };
	MChunck*			  pDeallocatorChunck{ nullptr };
	std::deque<void*>     mFastCache;// 告诉缓存期间
	std::recursive_mutex  mMutex;
};

//
// 内存池
//
template<class ThreadMode>
class MMemPool : public ThreadMode{
public:
	MMemPool(unsigned __int32 blocksize = MBLOCKSIZE, unsigned __int8 blocknum = MBLOCKNUM);
	~MMemPool();
	void* Allocator(unsigned __int32 size);
	void  Deallocator(void* p, unsigned __int32 size);
private:
	void processdellocator();

	std::vector<MFixAllocator>  mPool;
	std::deque<void*>			mUsedMemory; // 已被使用的内存
	void*				pCurrentMemeray{ nullptr };
	MFixAllocator*      pAllocator{ nullptr };
	MFixAllocator*		pDealloactor{ nullptr };
	unsigned __int32    mCurrentIndex{ 0 };
	unsigned __int32	mBlockSize;
	unsigned __int8     mBlockNum;
	std::thread			mThread;
	std::recursive_mutex mMutex;
	std::condition_variable mCondition;
	bool				bIsExit{ false };
	typedef std::pair<void*, unsigned __int32> CacheType;
	std::vector<CacheType> mNeedFreeMemory; // 需要释放的内存
	std::vector<std::vector<CacheType>> mCacheDeque;
	std::map<void*, unsigned __int32> mMerginMemory;   // 合并后的内存


	template<class CableLock>
	class UniquiLock{
	public:
		UniquiLock(CableLock* obj) :pObj(obj){
			pObj->Lock();
		}
		~UniquiLock(){
			pObj->UnLock();
		}
	private:
		CableLock*  pObj{ nullptr };
	};
};


//
// 实现两个操作符
//
void* MObjectBase::operator new(std::size_t size){

	void* ptr = nullptr;
	while (ptr == nullptr){
		ptr = MAllocator2::Instance()->Allocator(size);;
	}
	return ptr;
}
void MObjectBase::operator delete(void* p, std::size_t size){
	MAllocator2::Instance()->Deallocator(p, size);
}


//
// 实现chunck
//

MChunck::MChunck(){
}

void MChunck::Init(unsigned __int32 blocksize, unsigned __int8 bloks){
	try{
		pMemeray = new unsigned char[blocksize*bloks];
		mAvailbelIndex = 0;
		mAvailbelBlocks = bloks;
		unsigned char i = 0;
		unsigned char* p = pMemeray;
		for (i; i != bloks; p += blocksize){
			*p = ++i;
		}
	}
	catch (std::bad_alloc e){
		;
	}
}

MChunck::MChunck(const MChunck& other){
	pMemeray = other.pMemeray;
	mAvailbelIndex = other.mAvailbelIndex;
	mAvailbelBlocks = other.mAvailbelBlocks;
}

MChunck& MChunck::operator = (const MChunck& other){
	pMemeray = other.pMemeray;
	mAvailbelIndex = other.mAvailbelIndex;
	mAvailbelBlocks = other.mAvailbelBlocks;
	return *this;
}

void MChunck::Release(){
	if (pMemeray){
		delete[] pMemeray;
		pMemeray = nullptr;
	}
	mAvailbelIndex = 0;
	mAvailbelBlocks = -1;
}

void* MChunck::Allocator(unsigned __int32 size){
	if (!mAvailbelBlocks)
		return nullptr;
	std::unique_lock<std::mutex> lock(mMutex);
	unsigned char* pResult = pMemeray + (size*mAvailbelIndex);
	mAvailbelIndex = *pResult;
	--mAvailbelBlocks;
	return pResult;
}


void MChunck::Deallocator(void* p, unsigned __int32 size){
	//
	// 断言p属于当前chunck里面的内存
	//

	std::unique_lock<std::mutex> lock(mMutex);
	if (p < pMemeray){
		return;
	}
	unsigned char* releasebuffer = static_cast<unsigned char*>(p);
	if ((releasebuffer - pMemeray) % size != 0){
		return;
	}
	*releasebuffer = mAvailbelIndex;
	mAvailbelIndex = static_cast<unsigned char>((releasebuffer - pMemeray) / size);
	++mAvailbelBlocks;
}

//
// 实现固定大小内存分配器
//
MFixAllocator::MFixAllocator(unsigned __int32 blocksize, unsigned __int8 blocks) :mBlockNum(blocks), mBlockSize(blocksize),
pAllocatorChunck(nullptr), pDeallocatorChunck(nullptr)
{
	mChuncks.clear();
}

MFixAllocator::MFixAllocator(const MFixAllocator& other){
	mChuncks = other.mChuncks;
	pAllocatorChunck = other.pAllocatorChunck;
	pDeallocatorChunck = other.pDeallocatorChunck;
	mBlockNum = other.mBlockNum;
	mBlockSize = other.mBlockSize;
}

MFixAllocator& MFixAllocator::operator=(const MFixAllocator& other){
	mChuncks = other.mChuncks;
	pAllocatorChunck = other.pAllocatorChunck;
	pDeallocatorChunck = other.pDeallocatorChunck;
	mBlockNum = other.mBlockNum;
	mBlockSize = other.mBlockSize;
	return *this;
}

MFixAllocator::~MFixAllocator(){
	for (auto& m : mChuncks){
		m.Release();
	}
	mChuncks.clear();
}

void* MFixAllocator::Allocator(){
	//
	// 不使用告诉缓存
	//
	/*	if (!mFastCache.empty()){
	std::unique_lock<std::recursive_mutex> lock(mMutex);
	void* pResult =  mFastCache.front();
	mFastCache.pop_front();
	return pResult;
	}
	*/
	if (pAllocatorChunck && pAllocatorChunck->mAvailbelBlocks > 0){
		return pAllocatorChunck->Allocator(mBlockSize);
	}
	auto beg = mChuncks.begin();
	for (;;){
		if (beg == mChuncks.end()){
			mChuncks.push_back(MChunck());
			MChunck& chunck = mChuncks.back();
			chunck.Init(mBlockSize, mBlockNum);
			pAllocatorChunck = &chunck;
			pDeallocatorChunck = &(mChuncks.front());
			break;
		}
		if (beg->mAvailbelBlocks > 0){
			pAllocatorChunck = &(*beg);
			break;
		}
		++beg;
	}
	assert(pAllocatorChunck != nullptr);
	assert(pAllocatorChunck->mAvailbelBlocks > 0);
	return pAllocatorChunck->Allocator(mBlockSize);
}

//
// 内存归还不需要直接返回到Chunck中进行处理
// 
//
void MFixAllocator::Deallocator(void* p){
	pDeallocatorChunck->Deallocator(p, mBlockSize);
	//	std::unique_lock<std::recursive_mutex> lock(mMutex);
	//	mFastCache.push_back(p);
}

//
// 检查内存是否在当前分配器中
//
bool MFixAllocator::Check(void* p, unsigned __int32 size, unsigned __int32& start, unsigned __int32& end){
	unsigned char* ptr = static_cast<unsigned char*>(p);
	bool isExist = false;
	MChunck* pChunck = nullptr;
	for (auto& chunck : mChuncks){
		if (ptr >= chunck.pMemeray && ptr <= chunck.pMemeray + mBlockSize*(mBlockNum - 1)){
			pChunck = &chunck;
			isExist = true;
			break;
		}
	}
	if (isExist == false)
		return false;

	pDeallocatorChunck = pChunck;
	//
	// 进一步确定内存位置
	//
	unsigned __int32 diff = ptr - pChunck->pMemeray;
	unsigned __int32 mod = diff % mBlockSize;
	unsigned __int32 blocksizediff = mBlockSize - mod;

	if (blocksizediff < mBlockSize){
		start = blocksizediff;
	}
	else{
		start = 0;
	}

	if (size - start < mBlockSize){
		return false;
	}

	mod = size % mBlockSize;
	end = size - mod;
	return true;
}

/*************************************************************************************/
///
/// 内存池的实现
///
template<class ThreadMode>
MMemPool<ThreadMode>::MMemPool(unsigned __int32 blocksize, unsigned __int8 blocknum){
	mPool.clear();
	pCurrentMemeray = nullptr;
	mCurrentIndex = 0;
	mBlockSize = blocksize;
	mBlockNum = blocknum;
	mNeedFreeMemory.reserve(8196);
	mCacheDeque.reserve(8196);
	mThread = std::thread(std::bind(&MMemPool<ThreadMode>::processdellocator, this));
	
	
}

template<class ThreadMode>
MMemPool<ThreadMode>::~MMemPool(){
	bIsExit = true;
	mCondition.notify_all();
	if (mThread.joinable()){
		mThread.join();
	}
}

template<class ThreadMode>
void* MMemPool<ThreadMode>::Allocator(unsigned __int32 size){
	if (size > mBlockSize)
		return ::operator new(size);

	MMemPool<ThreadMode>::UniquiLock<MMemPool<ThreadMode>> lock(this);
	if (pAllocator == nullptr){
		mPool.push_back(MFixAllocator(mBlockSize, mBlockNum));
		pAllocator = &(mPool.back());
		pDealloactor = &(mPool.front());
	}
	if (pCurrentMemeray == nullptr){
		mCurrentIndex = 0;
		pCurrentMemeray = pAllocator->Allocator();
	}
	else if (mCurrentIndex + size > mBlockSize){
		unsigned char* p = static_cast<unsigned char*>(pCurrentMemeray);
		Deallocator(p + mCurrentIndex, mBlockSize - mCurrentIndex);
		pCurrentMemeray = pAllocator->Allocator();
		mCurrentIndex = 0;
	}
	unsigned char* p = static_cast<unsigned char*>(pCurrentMemeray);
	p += mCurrentIndex;
	mCurrentIndex += size;
	return p;
}

template<class ThreadMode>
void  MMemPool<ThreadMode>::Deallocator(void* p, unsigned __int32 size){
	if (size > mBlockSize){
		::operator delete(p);
		return;
	}
	
	mNeedFreeMemory.push_back(std::make_pair(p, size));
	if (mNeedFreeMemory.size() > 8000){
		std::unique_lock<std::recursive_mutex> lock(mMutex);
		mCacheDeque.push_back(mNeedFreeMemory);
		mNeedFreeMemory.clear();
		mCondition.notify_all();
	}
}

template<class ThreadMode>
void MMemPool<ThreadMode>::processdellocator(){
	std::mutex mtx;
	std::unique_lock<std::mutex> lock(mtx);
	while (1){
		mCondition.wait(lock);
		if (bIsExit){
			return;
		}
		if (mCacheDeque.empty())
			continue;

		
		while (true){
			if (mCacheDeque.empty())
				break;

			std::vector<std::pair<void*, unsigned __int32>> temp;
			{
				std::unique_lock<std::recursive_mutex> lock(mMutex);
				temp = mCacheDeque.front();
				mCacheDeque.erase(mCacheDeque.begin());
			}

			typedef std::pair<void*, unsigned __int32> PairType;
			std::sort(temp.begin(), temp.end(), [](PairType p1, PairType p2){
				return p1.first < p2.first; }
			);

			//
			// mMerginMemory;
			// 将内存合并到 mMerginMemory 中
			//
			for (auto& m : temp){
				unsigned char* p = static_cast<unsigned char*>(m.first);
				unsigned __int32 size = m.second;
				bool isExist = false;
				for (auto& m2 : mMerginMemory){
					unsigned char* p2 = static_cast<unsigned char*>(m2.first);
					unsigned __int32 size2 = m2.second;
					if (p2 + size2 == p){
						m2.second = size2 + size;
						isExist = true;
						break;
					}
				}
				if (isExist == false){
					mMerginMemory[p] = size;
				}
			} // end for (auto& m : tempmap)

			//
			// 检查 mMerginMemory 内存是否达到释放标准
			//
			temp.clear();

			
			auto beg = mMerginMemory.begin();

			int mapsize = mMerginMemory.size();

			//
			// 先自己合并一下内存
			//
			for (; beg != mMerginMemory.end();){
				unsigned char* p = static_cast<unsigned char*>(beg->first);
				unsigned __int32 size = beg->second;
				auto __temptr = beg;
				++beg;
				if (beg == mMerginMemory.end()){
					break;
				}
				unsigned char* p2 = static_cast<unsigned char*>(beg->first);
				unsigned __int32 size2 = beg->second;
				if (p + size == p2){
					__temptr->second += size2;
					mMerginMemory.erase(p2);
					beg = mMerginMemory.begin();
				}
			}
			beg = mMerginMemory.begin();
			for (; beg != mMerginMemory.end();){
				unsigned char* p = static_cast<unsigned char*>(beg->first);
				unsigned __int32 size = beg->second;
				unsigned __int32 startindex = 0, endindex = 0;
				MFixAllocator* pDeallocatorPtr = nullptr;
				//
				// 如果需要释放的内存达到row大小可以释放
				//
				if (size > mBlockSize){
					for (auto& pool : mPool){
						if (pool.Check(p, size, startindex, endindex)){
							pDeallocatorPtr = &pool;
							break;
						}
					}
				}

				if (pDeallocatorPtr){
					if (startindex > 0){
						mMerginMemory[p] = startindex;
					}
					else{
						mMerginMemory.erase(p);
						beg = mMerginMemory.begin();
					}
					for (; startindex != endindex; startindex += mBlockSize){
						pDeallocatorPtr->Deallocator(p + startindex);
					}
					if (size - endindex > 0){
						mMerginMemory[p + endindex] = size - endindex;

						//
						// 重置迭代器
						//
						beg = mMerginMemory.begin();
					}

				}
				else{
					++beg;
				}

			} // end for (; beg != mMerginMemory.end();)
			std::cout << "mMerginMemory.size = " << mapsize <<"\t"<< mMerginMemory.size() << std::endl;

		}// end while
			
	}// end while
}
