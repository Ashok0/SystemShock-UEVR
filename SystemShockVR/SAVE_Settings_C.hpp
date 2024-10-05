#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class SAVE_Settings_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/UI/HUD/Widgets/Settings/SAVE_Settings.SAVE_Settings_C");
		}
		return result;
	}

	static SAVE_Settings_C* get_instance()
	{
		auto klass = SAVE_Settings_C::static_class();
		SAVE_Settings_C* Object = nullptr;

		if (klass) {

			std::vector<SAVE_Settings_C*> List = klass->get_objects_matching<SAVE_Settings_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("SAVE_Settings_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"SAVE_Settings_C") != std::wstring::npos)
				{
					// API::get()->log_info("SAVE_Settings_C found");
					Object->set_CrouchToggle(true);
					Object->set_FocusAimToggle(false);
					Object->SetFOV(120.0, true);
					Object->ApplyFOV(120.0, true);
					Object->SetReloadCanUseBatteries(false);
					Object->ApplyReloadCanUseBatteries(false);
				}
				else
				{
					// API::get()->log_info("SAVE_Settings_C not found");
				}
			}
		}
		return nullptr;
	}

	void set_CrouchToggle(const bool val)
	{
		set_bool_property(L"CrouchToggle", val);
	}

	void set_FocusAimToggle(const bool val)
	{
		set_bool_property(L"FocusAimToggle", val);
	}

	void SetFOV(float NewValue, bool IsRealspace)
	{
		static const auto func = SAVE_Settings_C::static_class()->find_function(L"SetFOV");
		if (!func) {
			// API::get()->log_info("SetFOV not found");
			return;
		}
		else
		{
			// API::get()->log_info("SetFOV found");
		}

		struct
		{
			float NewValue;
			bool IsRealspace;
		} params{ .NewValue = NewValue, .IsRealspace = IsRealspace };

		process_event(func, &params);
	}

	void ApplyFOV(float NewValue, bool IsRealspace)
	{
		static const auto func = SAVE_Settings_C::static_class()->find_function(L"ApplyFOV");
		if (!func) {
			// API::get()->log_info("ApplyFOV not found");
			return;
		}
		else
		{
			// API::get()->log_info("ApplyFOV found");
		}

		struct
		{
			float NewValue;
			bool IsRealspace;
		} params{ .NewValue = NewValue, .IsRealspace = IsRealspace };

		process_event(func, &params);
	}

	void SetReloadCanUseBatteries(bool NewValue)
	{
		static const auto func = SAVE_Settings_C::static_class()->find_function(L"SetReloadCanUseBatteries");
		if (!func) {
			// API::get()->log_info("SetReloadCanUseBatteries not found");
			return;
		}
		else
		{
			// API::get()->log_info("SetReloadCanUseBatteries found");
		}

		struct
		{
			bool NewValue;
		} params{ .NewValue = NewValue };

		process_event(func, &params);
	}

	void ApplyReloadCanUseBatteries(bool NewValue)
	{
		static const auto func = SAVE_Settings_C::static_class()->find_function(L"ApplyReloadCanUseBatteries");
		if (!func) {
			// API::get()->log_info("ApplyReloadCanUseBatteries not found");
			return;
		}
		else
		{
			// API::get()->log_info("ApplyReloadCanUseBatteries found");
		}

		struct
		{
			bool NewValue;
		} params{ .NewValue = NewValue };

		process_event(func, &params);
	}
};
