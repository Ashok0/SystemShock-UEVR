#pragma once

#include "uevr/API.hpp"

using namespace uevr;

extern bool InteractToggle;
extern bool IsEquipped;
extern bool melee_equipped;
extern bool rapier_equipped;

class COMP_HackerInventory_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Inventory/COMP_HackerInventory.COMP_HackerInventory_C");
		}
		return result;
	}

	static COMP_HackerInventory_C* get_instance()
	{
		auto klass = COMP_HackerInventory_C::static_class();
		COMP_HackerInventory_C* Object = nullptr;

		if (klass) {
			std::vector<COMP_HackerInventory_C*> List = klass->get_objects_matching<COMP_HackerInventory_C>();
			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("COMP_HackerInventory_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"Implant_C") != std::wstring::npos)
				{
					// API::get()->log_info("COMP_HackerInventory_C found");
					Object->GetEquippedWeapon();

					if (InteractToggle == true)
					{
						Object->ToggleWeaponEquipState();
						InteractToggle = false;
					}
				}
				else
				{
					// API::get()->log_info("COMP_HackerInventory_C not found");
				}				
			}
		}
		return nullptr;
	}

	void ToggleWeaponEquipState()
	{
		static const auto func = COMP_HackerInventory_C::static_class()->find_function(L"ToggleWeaponEquipState");
		if (!func) {
			// API::get()->log_info("ToggleWeaponEquipState not found");
			return;
		}
		else
		{
			// API::get()->log_info("ToggleWeaponEquipState found");
		}

		struct
		{
			bool value;
		} params{ .value = false };

		process_event(func, &params);
	}

	void GetEquippedWeapon()
	{
		static const auto func = static_class()->find_function(L"GetEquippedWeapon");

		if (!func) {
			// API::get()->log_error("GetEquippedWeapon not found");
		}
		else
		{
			// API::get()->log_error("GetEquippedWeapon found");
		}

		struct
		{
			API::UObject* Weapon;
		} params{ .Weapon = nullptr };

		process_event(func, &params);

		if (params.Weapon != nullptr)
		{
			IsEquipped = true;

			std::wstring WeaponName = params.Weapon->get_full_name();
			// API::get()->log_info("GetEquippedWeapon: %ls", WeaponName.c_str());

			if ((WeaponName.find(L"Pipe") != std::wstring::npos) || (WeaponName.find(L"Wrench") != std::wstring::npos))
			{
				melee_equipped = true;
			}
			else
			{
				melee_equipped = false;
			}

			if (WeaponName.find(L"Rapier") != std::wstring::npos)
			{
				rapier_equipped = true;
			}
			else
			{
				rapier_equipped = false;
			}
		}
		else
		{
			IsEquipped = false;
		}
	}
};