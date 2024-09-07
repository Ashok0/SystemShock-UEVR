#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class WIDGET_InventoryContextMenu_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"WidgetBlueprintGeneratedClass /Game/Blueprints/UI/HUD/Widgets/ContextMenu/WIDGET_InventoryContextMenu.WIDGET_InventoryContextMenu_C");
		}
		return result;
	}

	static WIDGET_InventoryContextMenu_C* get_instance()
	{
		auto klass = WIDGET_InventoryContextMenu_C::static_class();
		WIDGET_InventoryContextMenu_C* Object = nullptr;

		if (klass) {

			std::vector<WIDGET_InventoryContextMenu_C*> List = klass->get_objects_matching<WIDGET_InventoryContextMenu_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("PAWN_Hacker_Simple_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());				
			}

			return Object;
		}
		return nullptr;
	}

	bool get_IsInventoryContextMenuEnabled()
	{
		return get_bool_property(L"IsInventoryContextMenuEnabled");
	}
};
