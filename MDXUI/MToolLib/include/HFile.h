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
#include <memory>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <iterator>
#include <typeinfo>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <tuple>

namespace hlw{


//------------------------------------------------------------
//读文件，主要用于读配置文件，结果中first表示key，second表示value
//------------------------------------------------------------
template<typename T,typename U,
         template<typename K,typename V,
                  typename Cmp = std::less<K>,
                  typename Allocator = std::allocator<std::pair<const K,V>>> class M>
void ReadFile(M<T,U>& result,const std::string& filename,
              const std::string& group, const std::string& _reg = "=",
              const char discard = '#'){
    std::ifstream inFile(filename);
    if(inFile.fail()){
        throw std::runtime_error("unabel open file for read......");
    }
    std::string str_file;
    std::regex reg(_reg);
    std::vector<std::string> temp;
    while(std::getline(inFile,str_file)){
        str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
        boost::algorithm::trim(str_file);
        if(str_file.empty())
            continue;
        std::string  group_str(str_file.begin()+1,str_file.end()-1);
        if(group_str != group)
            continue;
        while(std::getline(inFile,str_file)){
            str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
            if(str_file.empty())
                continue;
            std::smatch reg_result;
            if(std::regex_search(str_file,reg_result,reg)){
                boost::algorithm::split(temp,str_file,boost::algorithm::is_any_of(std::string(reg_result[0])));
                if(std::find(temp.begin(),temp.end(),"")!=temp.end()){
                    continue;
                }
                for(int j=0;j<(temp.size()%2==0?temp.size():temp.size()-1);j+=2){
                    std::string key = temp[2*j];
                    boost::algorithm::trim(key);
                    std::string value = temp[2*j+1];
                    boost::algorithm::trim(value);
                    result.insert(std::make_pair(boost::lexical_cast<T>(key),
                                  boost::lexical_cast<U>(value)));
                }
            }
            else{
                break;
            }
        }
    }
    inFile.close();
}

//---------------------------------------------------------------
//用于读通用文件，first表示key，second中包含有按_sp分隔的所有数据
//---------------------------------------------------------------
template<typename T,typename U,
         template<typename E,typename Allocator = std::allocator<E>> class C,
         template<typename K,typename V,
                  typename Cmp = std::less<K>,
                  typename Allocator = std::allocator<std::pair<const K,V>>> class M>
void ReadFile(M<T,C<U>>& result,const std::string& filename,
              const std::string& group, const std::string& _reg = "=",const std::string& _sp = "\\s+",
              const char discard = '#'){
    std::ifstream inFile(filename);
    if(inFile.fail()){
        throw std::runtime_error("unabel open file for read!!!!");
    }
    std::string str_file;
    std::regex reg(_reg);
    std::regex sp(_sp);
    std::vector<std::string> temp;
    std::vector<std::string> v_str;
    while(std::getline(inFile,str_file)){
        str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
        if(str_file.empty())
            continue;
        boost::algorithm::trim(str_file);
        std::string  group_str(str_file.begin()+1,str_file.end()-1);
        if(group_str != group)
            continue;
        while(std::getline(inFile,str_file)){
            str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
            boost::algorithm::trim(str_file);
            if(str_file.empty())
                continue;
            std::smatch reg_result;
            if(std::regex_search(str_file,reg_result,reg)){
                split(temp,str_file,std::string(reg_result[0]));
                if(temp.size()!=2)
                    continue;
                std::smatch sp_result;
                if(std::regex_search(temp[1],sp_result,sp)){
                    boost::algorithm::split(v_str,temp[1],
                            boost::algorithm::is_any_of(std::string(sp_result[0])));
                    if(std::find(v_str.begin(),v_str.end(),"")!=v_str.end()){
                        continue;
                    }
                }
                C<U> elements;
                std::for_each(v_str.begin(),v_str.end(),[&](std::string str){
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
    inFile.close();
}

struct null_type{
};

template<typename T0,typename T1,typename T2>
void ReadFile(std::vector<std::tr1::tuple<T0,T1,T2>>& result,const std::string& filename,
              const std::string& group = "",const std::string& _sp = "\\s+",
              const char discard = '#' ){

    std::ifstream inFile(filename);
    if(inFile.fail()){
        throw std::runtime_error(filename + " File : unable open file for read!!!!");
    }
    std::string str_file;
    std::regex sp(_sp);
    std::vector<std::string> temp;
    if(group.empty()){
        result.clear();
        while(std::getline(inFile,str_file)){
            str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
            std::smatch sp_result;
            if(std::regex_search(str_file,sp_result,sp)){
                boost::algorithm::split(temp,str_file,
                                        boost::algorithm::is_any_of(std::string(sp_result[0])));
                if(std::find(temp.begin(),temp.end(),"")!=temp.end()){
                    continue;
                }
            }
            else{
                continue;
            }
            if(temp.size()<3)
                continue;
            try{
                result.push_back(std::make_tuple(boost::lexical_cast<T0>(temp[0]),
                                 boost::lexical_cast<T1>(temp[1]),boost::lexical_cast<T2>(temp[2])));
            }catch(boost::bad_lexical_cast e){
                std::cerr<<e.what();
            }
        }
        inFile.close();
    }
    else{
        result.clear();
        while(std::getline(inFile,str_file)){
            str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
            if(str_file.empty())
                continue;
            boost::algorithm::trim(str_file);
            std::string  group_str(str_file.begin()+1,str_file.end()-1);
            if(group_str != group)
                continue;
            while(std::getline(inFile,str_file)){
                str_file.erase(std::find(str_file.begin(),str_file.end(),discard),str_file.end());
                boost::algorithm::trim(str_file);
                if(str_file.empty())
                    continue;
                std::smatch sp_result;
                if(std::regex_search(str_file,sp_result,sp)){
                    boost::algorithm::split(temp,str_file,
                                            boost::algorithm::is_any_of(std::string(sp_result[0])));
                    if(std::find(temp.begin(),temp.end(),"")!=temp.end()){
                        continue;
                    }
                    if(temp.size()<3)
                        continue;
                    try{
                        result.push_back(std::make_tuple(boost::lexical_cast<T0>(temp[0]),
                                         boost::lexical_cast<T1>(temp[1]),boost::lexical_cast<T2>(temp[2])));
                    }
                    catch(boost::bad_lexical_cast e){
                        std::cerr<<e.what();
                    }
                }
                else{
                    break;
                }

            }
            break;
        }
        inFile.close();
    }
}

//------------------------------------------------------
//写文件，用于写配置文件，key和value之间用_reg来分隔
//------------------------------------------------------
template<typename T,typename U,
         template<typename K,typename V,
                  typename Cmp = std::less<K>,
                  typename Allocator = std::allocator<std::pair<const K,V>>> class M>
void WriteFile(M<T,U>& info,const std::string& filename,bool isApp,
               const std::string& group,const std::string& _reg = "="){
    std::ofstream outFile;
    if(isApp){
        outFile.open(filename,std::ios_base::app);
    }
    else
        outFile.open(filename);

    if(!outFile){
        throw std::runtime_error("Unable open file for write!!!");
    }
    outFile<<"["<<group<<"]"<<"\n";
    std::for_each(info.begin(),info.end(),[&](std::pair<T,U> item){
        outFile<<item.first<<" "<<_reg<<" "<<item.second<<"\n";
    });
    outFile.close();
}


template<typename T,
         template<typename E,typename Allocator = std::allocator<E>> class C>
void WriteFile(C<T>& info,const std::string& filename,bool isApp,
               const std::string& group,const std::string& _reg = "="){
    std::ofstream outFile;
    if(isApp){
        outFile.open(filename,std::ios_base::app);
    }
    else
        outFile.open(filename);

    if(!outFile){
        throw std::runtime_error("Unable open file for write!!!");
    }
    outFile<<"["<<group<<"]"<<"\n";
    int i = 1;
    std::for_each(info.begin(),info.end(),[&](T item){
        outFile<<i<<" "<<_reg<<" "<<item<<"\n";
        ++i;
    });
    outFile.close();
}
}


