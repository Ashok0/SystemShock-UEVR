#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class PAWN_SystemShockCharacter_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Characters/PAWN_SystemShockCharacter.PAWN_SystemShockCharacter_C");
		}
		return result;
	}

	static PAWN_SystemShockCharacter_C* get_instance()
	{
		auto klass = PAWN_SystemShockCharacter_C::static_class();
		PAWN_SystemShockCharacter_C* Object = nullptr;

		if (klass) {

			std::vector<PAWN_SystemShockCharacter_C*> List = klass->get_objects_matching<PAWN_SystemShockCharacter_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("PAWN_SystemShockCharacter_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"PAWN_Hacker_Implant_C") != std::wstring::npos)
				{
					// API::get()->log_info("PAWN_SystemShockCharacter_C found");
					return Object;
				}
				else
				{
					// API::get()->log_info("PAWN_SystemShockCharacter_C not found");
				}				
			}
		}
		return nullptr;
	}

	bool get_IsAlive()
	{
		return get_bool_property(L"IsAlive");
	}
};
