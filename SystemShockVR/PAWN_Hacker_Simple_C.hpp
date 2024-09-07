#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class PAWN_Hacker_Simple_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Characters/Hacker/PAWN_Hacker_Simple.PAWN_Hacker_Simple_C");
		}
		return result;
	}

	static PAWN_Hacker_Simple_C* get_instance()
	{
		auto klass = PAWN_Hacker_Simple_C::static_class();
		PAWN_Hacker_Simple_C* Object = nullptr;

		if (klass) {

			std::vector<PAWN_Hacker_Simple_C*> List = klass->get_objects_matching<PAWN_Hacker_Simple_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("PAWN_Hacker_Simple_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());				
			}

			return Object;
		}
		return nullptr;
	}

	bool get_IsTryingToLean()
	{
		return get_bool_property(L"IsTryingToLean");
	}

	void set_IsTryingToLean(const bool val)
	{
		set_bool_property(L"IsTryingToLean", val);
	}
};
