#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class COMP_MoveControlManager_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Characters/Hacker/ScriptedControls/COMP_MoveControlManager.COMP_MoveControlManager_C");
		}
		return result;
	}

	static COMP_MoveControlManager_C* get_instance()
	{
		auto klass = COMP_MoveControlManager_C::static_class();
		COMP_MoveControlManager_C* Object = nullptr;

		if (klass) {

			std::vector<COMP_MoveControlManager_C*> List = klass->get_objects_matching<COMP_MoveControlManager_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("COMP_MoveControlManager_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"PAWN_Hacker_Implant_C") != std::wstring::npos)
				{
					// API::get()->log_info("COMP_MoveControlManager_C found");
					return Object;
				}
				else
				{
					// API::get()->log_info("COMP_MoveControlManager_C not found");
				}				
			}
		}
		return nullptr;
	}

	bool get_IsCrouching()
	{
		return get_bool_property(L"IsCrouching");
	}
};
