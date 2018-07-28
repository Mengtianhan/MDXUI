#pragma once
#include "MString.h"
#include <MAny.h>
#include <vector>
#include <mutex>
using mj::MString;
using mj::MAny;
class MXMLParse;
namespace DxUI{
	class MSTRING_API MXml
	{
	public:
		MXml();
		~MXml();

		void    ReadFile(const MString& filename);
		void    WriteFile(const MString& filename);
		void	PutValue(const MString& key, const MAny& value);
		void    PutValue(const MString& key, const std::vector<MAny>& values);

		MAny GetValue(const MString&key, const MAny& defaultvalue);
		std::vector<MAny> GetValues(const MString& key, const MAny& defaultvalue);

		void	CloseFile();
		void    SaveFile();
	private:
		MXMLParse* pXml{ nullptr };
	};

}

