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
#include <map>
#include <string>
#include <vector>
#include <MSingleton.h>
#include "MTLConfig.h"
#include <memory>

namespace TL{
	class  MTLClassFactory : public MSingleton<MTLClassFactory>{
	public:
		MTLClassFactory(){}
		virtual ~MTLClassFactory(){}
	public:
		void* GetInstanceByName(const std::string& name){
			if (name.empty())
				return nullptr;
			if (mClassMap.count(name)){
				return mClassMap.at(name)();
			}
			return nullptr;
		}

		void MRegisterClass(const std::string& name, MCreateInstanceFun __class){
			if (name.empty())
				return;
			mRegisterClassName.push_back(name);
			mClassMap[name] = __class;
		}
		const std::vector<std::string> &GetRegisterClasses() const{
			return mRegisterClassName;
		}
		const std::vector<std::string>& GetRegisterClasses(){
			return mRegisterClassName;
		}

	private:
		std::map<std::string, MCreateInstanceFun>   mClassMap;
		std::vector<std::string>  mRegisterClassName;
	};

	template<class Factory>
	class MClassDynamic{
	public:
		MClassDynamic(const std::string& className, MCreateInstanceFun fun){
			Factory::Instance()->MRegisterClass(className, fun);
		}
	};
}
