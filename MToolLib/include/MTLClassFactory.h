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
