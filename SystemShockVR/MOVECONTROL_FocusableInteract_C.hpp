#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class MOVECONTROL_FocusableInteract_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Interactables/MOVECONTROL_FocusableInteract.MOVECONTROL_FocusableInteract_C");
		}
		return result;
	}

	static MOVECONTROL_FocusableInteract_C* get_instance()
	{
		auto klass = MOVECONTROL_FocusableInteract_C::static_class();
		MOVECONTROL_FocusableInteract_C* Object = nullptr;

		if (klass) {

			std::vector<MOVECONTROL_FocusableInteract_C*> List = klass->get_objects_matching<MOVECONTROL_FocusableInteract_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("MOVECONTROL_FocusableInteract_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"MOVECONTROL_FocusableInteract_C") != std::wstring::npos)
				{
					// API::get()->log_info("MOVECONTROL_FocusableInteract_C found");
				}
				else
				{
					// API::get()->log_info("MOVECONTROL_FocusableInteract_C not found");
				}

				bool ShouldUseCharacterMovement = Object->get_ShouldUseCharacterMovement();
				bool IsExpiring = Object->get_IsExpiring();

				if ( (ShouldUseCharacterMovement == false) && (IsExpiring == false) )
				{
					return Object; /* Return most recent object */
				}
			}

			return Object;
		}
		return nullptr;
	}

	bool get_ShouldUseCharacterMovement()
	{
		return get_bool_property(L"ShouldUseCharacterMovement");
	}

	bool get_IsExpiring()
	{
		return get_bool_property(L"IsExpiring");
	}

	
};
