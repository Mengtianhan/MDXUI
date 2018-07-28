#pragma once
#include "MXml.h"

namespace DxUI{

	class MSTRING_API MSerialize
	{
	public:
		MSerialize();
		~MSerialize();

		//
		// 以读的方式打开文件
		//
		void ReadFile(const MString &filename);

		//
		// 为写而打开文件
		//
		void WriteFile(const MString &filename);

		//
		// 关闭文件，操作另一个文件时记得关闭当前文件
		// 对于写文件操作，在些完成之后记得关闭文件
		// 否则数据只会在内存之中
		// 当然在程序退出之时夜会调用该操作
		//
		void CloseFile();

		///
		/// \brief SaveFile
		/// 用于读取的文件但有修改的情况
		///
		void SaveFile();

		bool IsReadFile() const;

		virtual MString ToString() { return "MSerialize"; }


		//
		// 将数据写进文件
		// key xml 文件的key，节点之间以.分割
		// value 要写的值
		//
		template<class T>
		bool PutValue(const MString& key, const T& value){
			try{
				mXmlManager->PutValue(key, value);
				return true;
			}
			catch (...){
				return false;
			}
		}


		//
		// 将数据写进文件
		// key xml 文件的key，节点之间以.分割
		// values 以child的形式写进去
		// 只有xml文件能够使用该操作
		//
		template<class T>
		bool PutValues(const MString& key, const std::vector<T>& values){
			try{
				std::vector<mj::MAny> __Anys;
				for (auto s : values){
					__Anys.push_back(s);
				}
				mXmlManager->PutValue(key, __Anys);
				return true;
			}
			catch (...){
				return false;
			}
		}

		//
		// 都文件信息
		// 和上面的写文件一样
		// 怎么写进去的就怎么都出来即可
		//
		template<class T>
		T GetValue(const MString& key, const T& defaultvalue){
			try{
				mj::MAny __Any = mXmlManager->GetValue(key, defaultvalue);
				return mj::any_cast<T>(__Any);
			}
			catch (...){
				return defaultvalue;
			}
		}

		//
		// 和PutValues对应
		// 读取操作都需要一个默然的返回值
		// 在没有取得该值的时候使用默认值返回
		// 而且默认值还能够很好的标识出类型
		//
		template<class T>
		std::vector<T> GetValues(const MString& key, const T& defaultvalue){
			std::vector<T> __Result;
			try{
				std::vector<mj::MAny> __Anys = mXmlManager->GetValues(key, defaultvalue);
				for (auto __s : __Anys){
					__Result.push_back(mj::any_cast<T>(__s));
				}
			}
			catch (...){
				__Result.push_back(defaultvalue);
			}
			return __Result;
		}


		MString GetParentPath();
		void CreateFolder(const MString& path);
		bool XmlFileIsExist(const MString& fileName);

	private:
		MXml*    mXmlManager;
		MString  mXmlFile;
		bool    bIsRead;
	};
}

