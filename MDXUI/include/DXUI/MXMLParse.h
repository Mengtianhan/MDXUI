#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <HPath_File.h>
#include <vector>


class MXMLParse
{
public:
	MXMLParse();
	~MXMLParse();

	void    ReadFile(const std::string& filename);
	void    WriteFile(const std::string& filename);

	template<class T>
	T CheckData(const std::string& key, T defaurt = T()){
		return pTreeParse.get<T>(key, defaurt);
	}

	template<class T>
	std::vector<T> CheckChilds(const std::string& key){
		if (!boost::algorithm::iends_with(mFileName, ".xml")){
			box::Error_MessageBox("当前文件类型不支持该操作");
			return std::vector<T>();
		}
		std::vector<T> __result;
		auto item = pTreeParse.get_child_optional(key);
		if (item){
			std::string __str = key;
			int pos = key.find_last_of('.');
			if (pos == std::string::npos)
				return __result;
			__str.assign(key.begin(), key.begin() + pos);
			auto diff_aps = pTreeParse.get_child(__str);
			if (!diff_aps.empty()){
				auto iter = diff_aps.begin();
				while (iter != diff_aps.end()){
					__result.push_back(iter->second.get_value<T>());
					++iter;
				}
			}
		}
		return __result;
	}

	template<class T>
	void	WriteData(const std::string& key, const T& value){
		pTreeParse.put(key, value);
	}

	template<class T>
	void WriteChild(const std::string& key, const T& value){
		if (!boost::algorithm::iends_with(mFileName, ".xml")){
			box::Error_MessageBox("当前文件类型不支持该操作");
			return;
		}
		pTreeParse.add(key, value);
	}

    ///
    /// \brief CloseFile
    /// 关闭时如果是读取忽悠
    /// 如果是写将保存文件
    ///
	void CloseFile();

    ///
    /// \brief SaveFile
    /// 如果是读取重新保存文件
    /// 如果是写保存文件
    ///
    void SaveFile();
private:
	bool						   bIsWrite;
	std::string					   mFileName;
	boost::property_tree::ptree    pTreeParse;
	int							   mFileType; // {1,2,3,4} 1 xml 2 json 3 ini 4 info
};

