#pragma once

#include "uevr/API.hpp"

using namespace uevr;

extern bool IsBracket;
extern bool ContextMode;

class WIDGET_PlayerHUD_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"WidgetBlueprintGeneratedClass /Game/Blueprints/UI/HUD/Widgets/WIDGET_PlayerHUD.WIDGET_PlayerHUD_C");
		}
		return result;
	}

	static WIDGET_PlayerHUD_C* get_instance()
	{
		auto klass = WIDGET_PlayerHUD_C::static_class();
		WIDGET_PlayerHUD_C* Object = nullptr;

		if (klass) {

			std::vector<WIDGET_PlayerHUD_C*> List = klass->get_objects_matching<WIDGET_PlayerHUD_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("WIDGET_PlayerHUD_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"WIDGET_PlayerHUD_C") != std::wstring::npos)
				{
					// API::get()->log_info("WIDGET_PlayerHUD_C found");
				}
				else
				{
					// API::get()->log_info("WIDGET_PlayerHUD_C not found");
				}

				if (IsBracket == false)
				{
					Object->set_ShouldShowBrackets(false);
					Object->ShowTargetBrackets(false);
				}
				else
				{
					Object->set_ShouldShowBrackets(true);
					Object->ShowTargetBrackets(true);
				}

				bool bIsMFDVisible = Object->get_is_mfd_visible();

				if (ContextMode == true) {
					Object->SetMouseCursorVisibility(false);
				}
				else
				{
					Object->SetMouseCursorVisibility(true);
				}

				if (bIsMFDVisible == true)
				{
					return Object; /* Return most recent object */
				}
			}
			return Object;
		}
		return nullptr;
	}

	bool get_is_mfd_visible()
	{
		return get_bool_property(L"bIsMFDVisible");
	}

	void set_ShouldShowBrackets(const bool val)
	{
		set_bool_property(L"ShouldShowBrackets", val);
	}

	void SetMouseCursorVisibility(bool Visible)
	{
		static const auto func = static_class()->find_function(L"SetMouseCursorVisibility");
		if (!func) {
			// API::get()->log_info("SetMouseCursorVisibility not found");
			return;
		}
		else
		{
			// API::get()->log_info("SetMouseCursorVisibility found");
		}

		struct
		{
			bool Visible;
		} params{ .Visible = Visible };

		process_event(func, &params);
	}

	void ShowTargetBrackets(bool Visible)
	{
		static const auto func = WIDGET_PlayerHUD_C::static_class()->find_function(L"ShowTargetBrackets");
		if (!func) {
			// API::get()->log_info("ShowTargetBrackets not found");
			return;
		}
		else
		{
			// API::get()->log_info("ShowTargetBrackets found");
		}

		struct
		{
			bool Visible;
		} params{ .Visible = Visible };

		process_event(func, &params);
	}

	void set_IsTryingToVaporizeInventory(const bool val)
	{
		set_bool_property(L"IsTryingToVaporizeInventory", val);
	}
};
