#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class CON_Hacker_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Characters/Hacker/CON_Hacker.CON_Hacker_C");
		}
		return result;
	}

	static CON_Hacker_C* get_instance()
	{
		auto klass = CON_Hacker_C::static_class();
		CON_Hacker_C* Object = nullptr;

		if (klass) {
			std::vector<CON_Hacker_C*> List = klass->get_objects_matching<CON_Hacker_C>();
			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("CON_Hacker_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"CON_Hacker_C") != std::wstring::npos)
				{
					// API::get()->log_info("CON_Hacker_C found");
				}
				else
				{
					// API::get()->log_info("CON_Hacker_C not found");
				}
			}

			return Object;
		}
		return nullptr;
	}

	void ForceHideCrosshairs(bool value)
	{
		static const auto func = CON_Hacker_C::static_class()->find_function(L"ForceHideCrosshairs");
		if (!func) {
			// API::get()->log_info("ForceHideCrosshairs not found");
			return;
		}
		else
		{
			// API::get()->log_info("ForceHideCrosshairs found");
		}

		struct
		{
			bool HideState;
		} params{ .HideState = value };

		process_event(func, &params);
	}
};