
//
// MFile.h 用来处理一个文件的
//
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <regex>
#include <fstream>


//====================================================
namespace mj{
	template<template<typename E, typename Allocator = std::allocator<E>> class C>
	void split(C<std::string>& c, const std::string& value, const std::string& sp){
		c.clear();
		auto beg = value.begin();
		std::string test(value.begin(), value.end());
		while (beg != value.end()){
			while (*beg == ' ')
				++beg;
			auto pos = test.find(sp);
			if (pos == std::string::npos){
				break;
			}
			auto end = beg + pos;
			auto temp = end;
			while (*temp == ' ')
				++temp;
			std::string t(beg, temp);
			c.push_back(t);
			beg = end + sp.size();
			while (*beg == ' ')
				++beg;
			test.assign(beg, value.end());
			pos = test.find(sp);
			if (pos == std::string::npos){
				c.push_back(test);
				break;
			}
			end = beg + pos;
			temp = end;
			while (*temp == ' ')
				--temp;
			t.assign(beg, temp);
			c.push_back(t);
			beg = end + sp.size();
			while (*beg == ' ')
				++beg;
			test.assign(beg, value.end());
		}
	}

	class MFile
	{
	public:
		MFile() :m_FileName(""), b_isApp(false){}

		~MFile(){}

		void setFile(const std::string& filename){
			m_FileName = filename;
		}

		void setApp(bool isApp){
			b_isApp = isApp;
		}

		template < typename T, typename A, template<typename T1,typename A1>class C>
		void ReadFile(C<T, A>& result, const std::string& group, const char end = '\n'){
			std::ifstream inFile(m_FileName);
			if (inFile.fail()){
                                throw std::runtime_error("unabel open file "+m_FileName +" for read......");
			}
			std::string str_file;
			while (std::getline(inFile, str_file)){
				if (str_file.empty())
					continue;
				str_file.erase(std::find(str_file.begin(), str_file.end(), end), str_file.end());
				boost::algorithm::trim(str_file);
				if (str_file.empty())
					continue;
                                std::string  group_str;
                                if(str_file.size()<2)
                                    group_str = str_file;
                                else
                                    group_str.assign(str_file.begin() + 1, str_file.end() - 1);
				if (!group.empty()){
					if (group_str != group)
						continue;
					while (std::getline(inFile, str_file)){
						str_file.erase(std::find(str_file.begin(), str_file.end(), end), str_file.end());
						try{
							result.push_back(str_file);
						}
						catch (...){
							throw std::runtime_error("read data and have a error happen......");
						}
					}//---------while
				} //------------if
				else{
					str_file.erase(std::find(str_file.begin(), str_file.end(), end), str_file.end());
					try{
						result.push_back(str_file);
					}
					catch (...){
						throw std::runtime_error("read data and have a error happen......");
					}
				}
			}
		}

		template<typename T, typename U,
			template<typename K, typename V,
			typename Cmp = std::less<K>,
			typename Allocator = std::allocator<std::pair<const K, V>>> class M>
		void ReadFile(M<T, U>& result,
			const std::string& group, const std::string& _reg = "=",
			const char discard = '#'){
				std::ifstream inFile(m_FileName);
				if (inFile.fail()){
					throw std::runtime_error("unabel open file for read......");
				}
				std::string str_file;
				std::regex reg(_reg);
				std::vector<std::string> temp;
				while (std::getline(inFile, str_file)){
					if (str_file.empty())
						continue;
					str_file.erase(std::find(str_file.begin(), str_file.end(), discard), str_file.end());
					boost::algorithm::trim(str_file);
					if (str_file.empty())
						continue;

                                        std::string group_str;
                                        if(str_file.size()<2)
                                            group_str = str_file;
                                        else
                                            group_str.assign(str_file.begin() + 1, str_file.end() - 1);
					if (!group.empty()){
						if (group_str != group)
							continue;
						while (std::getline(inFile, str_file)){
							str_file.erase(std::find(str_file.begin(), str_file.end(), discard), str_file.end());
							std::smatch reg_result;
							if (std::regex_search(str_file, reg_result, reg)){
								boost::algorithm::trim(str_file);
								boost::algorithm::split(temp, str_file, boost::algorithm::is_any_of(std::string(reg_result[0])));
								if (std::find(temp.begin(), temp.end(), "") != temp.end()){
									continue;
								}

								for (size_t j = 0; j<(temp.size() % 2 == 0 ? temp.size() : temp.size() - 1); j += 2){
									boost::algorithm::trim_right(temp[2 * j]);
									boost::algorithm::trim_left(temp[2 * j + 1]);
									try{
										result.insert(std::make_pair(boost::lexical_cast<T>(temp[2 * j]),
											boost::lexical_cast<U>(temp[2 * j + 1])));
									}
									catch (...){
										throw std::runtime_error("read data and have a error happen......");
									}
								}
							}
							else{
								break;
							}
						}//---------while
					} //------------if
					else{
						std::smatch reg_result;
						if (std::regex_search(str_file, reg_result, reg)){
							boost::algorithm::trim(str_file);
							boost::algorithm::split(temp, str_file, boost::algorithm::is_any_of(std::string(reg_result[0])));
							if (std::find(temp.begin(), temp.end(), "") != temp.end()){
								continue;
							}

							for (size_t j = 0; j<(temp.size() % 2 == 0 ? temp.size() : temp.size() - 1); j += 2){
								boost::algorithm::trim_right(temp[2 * j]);
								boost::algorithm::trim_left(temp[2 * j + 1]);
								try{
									result.insert(std::make_pair(boost::lexical_cast<T>(temp[2 * j]),
										boost::lexical_cast<U>(temp[2 * j + 1])));
								}
								catch (...){
									throw std::runtime_error("read data and have a error happen......");
								}
							}
						}
						else{
							break;
						}
					}
				}
				inFile.close();
			}


		//--------------------------------------------------------------------------
		// 写文件
		// info -------> map 需要写入文件的内容
		// group ------> string 写入文件的组
		// _reg  ------> string 分隔符
		//---------------------------------------------------------------------------
		template<typename T, typename U,
			template<typename K, typename V,
			typename Cmp = std::less<K>,
			typename Allocator = std::allocator<std::pair<const K, V>>> class M>
		void WriteFile(M<T, U>& info, const std::string& group, const std::string& _reg = "="){
				std::ofstream outFile;
				if (b_isApp){
					outFile.open(m_FileName, std::ios_base::app);
				}
				else
					outFile.open(m_FileName);

				if (!outFile){
					throw std::runtime_error("Unable open file for write......");
				}
				if (!group.empty()){
					outFile << "[" << group << "]" << "\n";
				}
				
				std::for_each(info.begin(), info.end(), [&](std::pair<T, U> item){
					outFile << item.first << " " << _reg << " " << item.second << "\n";
				});
				outFile.close();
			}

		//-------------------------------------------------------------------------
		// 读取通用文件
		// result---->map 储存读取的结果
		// group ---->string 需要读取的组名
		// _reg  ---->string key和值之间的分隔符
		// _sp   ---->string 值之间的分隔符
		// discard--->string 需要丢弃的部分
		//-------------------------------------------------------------------------
		template<typename T, typename U,
			template<typename E, typename Allocator = std::allocator<E>> class C,
			template<typename K, typename V,
			typename Cmp = std::less<K>,
			typename Allocator = std::allocator<std::pair<const K, V>>> class M>
		void ReadFile(M<T, C<U>>& result,
			const std::string& group, const std::string& _reg = "=", const std::string& _sp = "\\s+",
			const char discard = '#'){
				std::ifstream inFile(m_FileName);
				if (inFile.fail()){
					throw std::runtime_error("unabel open file for read....");
				}
				std::string str_file;
				std::regex reg(_reg);
				std::regex sp(_sp);
				std::vector<std::string> temp;
				std::vector<std::string> v_str;
				while (std::getline(inFile, str_file)){
					if (str_file.empty())
						continue;
					str_file.erase(std::find(str_file.begin(), str_file.end(), discard), str_file.end());
					boost::algorithm::trim(str_file);
					if (str_file.empty())
						continue;
					std::string match_str(str_file.begin() + 1, str_file.end() - 1);
					if (!group.empty()){
						if (!(match_str == group))
							continue;

						while (std::getline(inFile, str_file)){
							str_file.erase(std::find(str_file.begin(), str_file.end(), discard), str_file.end());
							boost::algorithm::trim(str_file);
							std::smatch reg_result;
							if (std::regex_search(str_file, reg_result, reg)){
								split(temp, str_file, std::string(reg_result[0]));
								std::for_each(temp.begin(), temp.end(), [&](std::string& str){
									boost::algorithm::trim(str);
								});
								if (temp.size() != 2)
									continue;
								std::smatch sp_result;
								if (std::regex_search(temp[1], sp_result, sp)){
									boost::algorithm::split(v_str, temp[1],
										boost::algorithm::is_any_of(std::string(sp_result[0])));
									std::for_each(v_str.begin(), v_str.end(), [&](std::string& str){
										boost::algorithm::trim(str);
									});
									if (std::find(v_str.begin(), v_str.end(), "") != v_str.end()){
										continue;
									}
								}
								C<U> elements;
								std::for_each(v_str.begin(), v_str.end(), [&](std::string str){
									elements.push_back(boost::lexical_cast<U>(str));
								});
								result.insert(std::make_pair(boost::lexical_cast<T>(temp[0]),
									elements));
							}
							else{
								break;
							}
						}
					}
					else{
						std::smatch reg_result;
						if (std::regex_search(str_file, reg_result, reg)){
							split(temp, str_file, std::string(reg_result[0]));
							std::for_each(temp.begin(), temp.end(), [&](std::string& str){
								boost::algorithm::trim(str);
							});
							if (temp.size() != 2)
								continue;
							std::smatch sp_result;
							if (std::regex_search(temp[1], sp_result, sp)){
								boost::algorithm::split(v_str, temp[1],
									boost::algorithm::is_any_of(std::string(sp_result[0])));
								std::for_each(v_str.begin(), v_str.end(), [&](std::string& str){
									boost::algorithm::trim(str);
								});
								if (std::find(v_str.begin(), v_str.end(), "") != v_str.end()){
									continue;
								}
							}

							try{
								C<U> elements;
								std::for_each(v_str.begin(), v_str.end(), [&](std::string str){
									elements.push_back(boost::lexical_cast<U>(str));
								});
							
								result.insert(std::make_pair(boost::lexical_cast<T>(temp[0]),
									elements));
							}
							catch (...){
								throw std::runtime_error("read error have a error happen......");
							}
						}
						else{
							break;
						}
					}
					
				}
				inFile.close();
			}


		//-------------------------------------------------------------------
		// 写文件
		// info 需要写入的信息
		// group 写入的组
		// 默认使用序号来表示键
		//-------------------------------------------------------------------
		template<typename T,
			template<typename E, typename Allocator = std::allocator<E>> class C>
		void WriteFile(C<T>& info, const std::string& group, const std::string& _reg = "="){
			std::ofstream outFile;
			if (b_isApp){
				outFile.open(m_FileName, std::ios_base::app);
			}
			else
				outFile.open(m_FileName);

			if (!outFile){
				throw std::runtime_error("Unable open file for write.....");
			}
			outFile << "[" << group << "]" << "\n";
			int i = 1;
			std::for_each(info.begin(), info.end(), [&](T item){
				outFile << i << " " << _reg << " " << item << "\n";
				++i;
			});
			outFile.close();
		}

		//---------------------------------------------------------------------------
		// 打印信息
		//---------------------------------------------------------------------------
		template<typename T,
			template<typename E, typename Allocator = std::allocator<E>> class C>
		void printAll(C<T>& c){
			if (c.empty())
				return;
			for (auto& _c : c){
				std::cout << _c << std::endl;
			}
		}

		//--------------------------------------------------------
		// 打印信息
		//--------------------------------------------------------
		template<typename K,typename U,typename C,typename A,
			template<typename K1, typename U1, typename C1, typename A1>class M>
		void printAll(M<K, U, C, A>& info){
			for (auto& it : info){
				std::cout << it.first << " " << it.second << std::endl;
			}
		}

		//-------------------------------------------------------------------------
		// 打印信息
		// m 从文件中读取的通用信息
		//-------------------------------------------------------------------------
		template<typename T, typename U,
			template<typename E, typename Allocator = std::allocator<E>> class C,
			template<typename K, typename V,
			typename Cmp = std::less<K>,
			typename Allocator = std::allocator<std::pair<const K, V>>> class M>
		void printAll(M<T, C<U>>& m){
				if (m.empty())
					return;
				auto it = m.begin();
				while (it != m.end()){
					std::cout << it->first << " : ";
					std::copy(it->second.begin(), it->second.end(), std::ostream_iterator<U>(std::cout, " "));
					std::cout << "\n";
					++it;
				}

			}

	private:
		std::string		m_FileName;
		bool			b_isApp;
	};
}
