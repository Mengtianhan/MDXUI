#pragma once
#include <memory>
#include "MTLClassFactory.h"
#include "MString.h"
#include <MTLProperty.h>

namespace TL{
	template<class Factory = MTLClassFactory>
	class  MObjectBase
	{
	public:
		typedef Factory MClassFactory;
	public:
		MObjectBase(){}
		virtual ~MObjectBase(){}
		const Factory* GetObjFactory() const{ return Factory::Instance(); }
		Factory* GetObjFactory(){ return Factory::Instance(); }
	};

	class MTLObject : public MObjectBase<>,public MTLProperty
	{
	public:
		MTLObject(){}
		virtual ~MTLObject(){ DestroySelf(); }
		void DestroySelf(){ NotifyAll(MPropertyNotifer(MTLObject, DestroySelf)); }
		virtual mj::MString ToString() const{ return "MObject"; }
	};
}