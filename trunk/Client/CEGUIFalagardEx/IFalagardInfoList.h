#pragma once

namespace CEGUI
{
	class IFalagardInfoList
	{
	public:
		virtual void AddInfo(const String32& strInfo) = 0;
		virtual void RemoveAllInfo() = 0;
	};
}