#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class INTERACT_Laptop_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Art/Props/Hacker_Apartment/Laptop/INTERACT_Laptop.INTERACT_Laptop_C");
		}
		return result;
	}

	static INTERACT_Laptop_C* get_instance()
	{
		auto klass = INTERACT_Laptop_C::static_class();
		INTERACT_Laptop_C* Object = nullptr;

		if (klass) {

			std::vector<INTERACT_Laptop_C*> List = klass->get_objects_matching<INTERACT_Laptop_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("INTERACT_Laptop_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"INTERACT_Laptop_C") != std::wstring::npos)
				{
					// API::get()->log_info("INTERACT_Laptop_C found");
				}
				else
				{
					// API::get()->log_info("INTERACT_Laptop_C not found");
				}
			}
			return Object;  
		}
		return nullptr;
	}

	bool get_IsInteracting()
	{
		return get_bool_property(L"IsInteracting");		
	}
};