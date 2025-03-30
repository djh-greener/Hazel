#pragma once
#include"Hazel/Scene/Entity.h"
namespace Hazel {
	class GlobalData
	{
	private:
		GlobalData() = default;
		GlobalData(const GlobalData&) = delete;
		GlobalData& operator=(const GlobalData&) = delete;

	public:
		static GlobalData& GetGlobalData()
		{
			static GlobalData Data;
			return Data;
		}
		Entity SelectedEntity;
		
	};
}
