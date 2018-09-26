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