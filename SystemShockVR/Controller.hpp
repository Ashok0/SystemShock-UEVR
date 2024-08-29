#pragma once

#include "uevr/API.hpp"

using namespace uevr;

extern UEVR_Rotatorf ref;

class Controller : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"Class /Script/Engine.Controller");
		}
		return result;
	}

	static Controller* get_instance()
	{
		auto klass = Controller::static_class();
		Controller* Object = nullptr;

		if (klass) {
			std::vector<Controller*> List = klass->get_objects_matching<Controller>();
			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("Controller: Object %d of %d, Object name: %ls",i, List.size(), ObjName.c_str());

				if (ObjName.find(L"CON_Hacker_C") != std::wstring::npos)
				{
					// API::get()->log_info("Controller found");
					return Object;
				}
				else
				{
					// API::get()->log_info("Controller not found");
				}
			}

			return Object;
		}
		return nullptr;
	}

	void SetControlRotation(UEVR_Rotatorf* const value)
	{
		static const auto func = Controller::static_class()->find_function(L"SetControlRotation");
		if (!func) {
			// API::get()->log_info("SetControlRotation not found");
			return;
		}
		else
		{
			// API::get()->log_info("SetControlRotation found");
		}

		struct
		{
			UEVR_Rotatorf NewRotation;
		} params{ .NewRotation = *value };

		process_event(func, &params);
	}

	UEVR_Rotatorf GetControlRotation()
	{
		static const auto func = Controller::static_class()->find_function(L"GetControlRotation");
		if (!func) {
			// API::get()->log_error("GetControlRotation not found");
			return { 0 };
		}
		else
		{
			// API::get()->log_info("GetControlRotation found");
		}

		struct
		{
			UEVR_Rotatorf rotation;
		} params{ 0 };

		process_event(func, &params);
		return params.rotation;
	}
};