#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class WIDGET_MainMenu_InGame_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"WidgetBlueprintGeneratedClass /Game/Blueprints/UI/MainMenu/WIDGET_MainMenu_InGame.WIDGET_MainMenu_InGame_C");
		}
		return result;
	}

	static WIDGET_MainMenu_InGame_C* get_instance()
	{
		auto klass = WIDGET_MainMenu_InGame_C::static_class();
		WIDGET_MainMenu_InGame_C* Object = nullptr;

		if (klass) {

			std::vector<WIDGET_MainMenu_InGame_C*> List = klass->get_objects_matching<WIDGET_MainMenu_InGame_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("WIDGET_MainMenu_InGame_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"GI_SinglePlayer_C") != std::wstring::npos)
				{
					// API::get()->log_info("WIDGET_MainMenu_InGame_C found");
					if (Object->get_IsMainMenuEnabled() == true) {
						return Object;
					}
				}
			}

			return Object;
		}
		return nullptr;
	}

	bool get_IsMainMenuEnabled()
	{
		return get_bool_property(L"IsMainMenuEnabled");
	}
};
