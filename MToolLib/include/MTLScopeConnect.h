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