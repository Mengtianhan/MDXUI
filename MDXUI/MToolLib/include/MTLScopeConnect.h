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
#include <MTLConnect.h>
#include <MTLDataBind.h>
#include <MFunctionTraits.h>
#include <MEmptyType.h>
#include <MTLSlot.h>

namespace TL{
	template<class SendType,class ReceiveType = MEmptyType>
	class MScopeConnect : public MNoCopy
	{
	public:
		MScopeConnect(const std::shared_ptr<MConnect<SendType>>& con) :pCon(con)
		{}

		MScopeConnect& operator=(const std::shared_ptr<MConnect<SendType>>& con){
			if (pCon){
				pCon->DisConnect();
			}
			pCon = con;
			bIsDestroy = false;
			return *this;
		}

		~MScopeConnect(){
			DisConnect();
		}

		void Connect(){
			if (bIsDestroy == true){
				return;
			}
			if (pCon){
				pCon->Connect();
			}
		}

		void DisConnect(){
			if (pCon){
				pCon->DisConnect();
			}
		}

		void Block(bool isblock){
			if (pCon){
				pCon->Block(isblock);
			}
		}
	private:
		std::shared_ptr<MConnect<SendType>> pCon;
	};
}