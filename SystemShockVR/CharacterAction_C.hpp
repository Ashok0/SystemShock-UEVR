#pragma once

#include "uevr/API.hpp"

using namespace uevr;

/** The various states that the animation state machine can be in */
typedef enum eAnimationState
{
	ANIM_NONE,
	ANIM_CRYOBED_WAKE,
	ANIM_SURGICALBED_RIGHT_ENTER,
	ANIM_SURGICALBED_RIGHT_EXIT,
	ANIM_SURGICALBED_LEFT_ENTER,
	ANIM_SURGICALBED_LEFT_EXIT,
	ANIM_DEATH_P,
	ANIM_TERMINAL_USE
}
animation_State_t;

float ElapsedPlayTime = 0.0;
int Montage = 0;
bool CryobedWakeIsActive = false;
bool RightExitIsActive = false;
bool LeftExitIsActive = false;
bool TerminalIsActive = false;

extern bool RoomscaleMontageOverride;

class CharacterAction_C : private API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static auto result = API::get()->find_uobject<API::UClass>(L"BlueprintGeneratedClass /Game/Blueprints/Characters/Actions/CharacterAction.CharacterAction_C");
		return result;
	}

	static CharacterAction_C* get_instance()
	{
		auto klass = CharacterAction_C::static_class();
		CharacterAction_C* Object = nullptr;

		if (klass) {

			std::vector<CharacterAction_C*> List = klass->get_objects_matching<CharacterAction_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("CharacterAction_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"PersistentLevel.PAWN_Hacker_Implant_C") != std::wstring::npos)
				{
					// API::get()->log_info("CharacterAction_C::PersistentLevel.PAWN_Hacker_Implant_C found");
					Montage = Object->GetMontage();
					ElapsedPlayTime = Object->GetElapsedPlayTime();
					
					if ( (ElapsedPlayTime == 0.0) && (Montage == ANIM_SURGICALBED_RIGHT_ENTER) )
					{
						API::UObjectHook::set_disabled(true);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "false");
						RoomscaleMontageOverride = true;
					}
					else if ((ElapsedPlayTime == 0.0) && (Montage == ANIM_SURGICALBED_RIGHT_EXIT))
					{
						RightExitIsActive = true;
					}
					else if ((ElapsedPlayTime == -1.0) && (Montage == ANIM_SURGICALBED_RIGHT_EXIT) && (RightExitIsActive == true) )
					{
						RightExitIsActive = false;
						API::UObjectHook::set_disabled(false);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "true");
						RoomscaleMontageOverride = false;
					}
					else if ((ElapsedPlayTime == 0.0) && (Montage == ANIM_SURGICALBED_LEFT_ENTER))
					{
						API::UObjectHook::set_disabled(true);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "false");
						RoomscaleMontageOverride = true;
					}
					else if ((ElapsedPlayTime == 0.0) && (Montage == ANIM_SURGICALBED_LEFT_EXIT))
					{
						LeftExitIsActive = true;
					}
					else if ((ElapsedPlayTime == -1.0) && (Montage == ANIM_SURGICALBED_LEFT_EXIT) && (LeftExitIsActive == true))
					{
						LeftExitIsActive = false;
						API::UObjectHook::set_disabled(false);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "true");
						RoomscaleMontageOverride = false;
					}
					else if ((ElapsedPlayTime == 0.0) && (Montage == ANIM_DEATH_P))
					{
						API::UObjectHook::set_disabled(true);
					}
					else if ((ElapsedPlayTime == 0.0) && (Montage == ANIM_TERMINAL_USE))
					{
						TerminalIsActive = true;
						API::UObjectHook::set_disabled(true);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "false");
						RoomscaleMontageOverride = true;
					}
					else if ((ElapsedPlayTime == -1.0) && (Montage == ANIM_TERMINAL_USE) && (TerminalIsActive == true))
					{
						TerminalIsActive = false;
						API::UObjectHook::set_disabled(false);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "true");
						RoomscaleMontageOverride = false;
					}
					else if ((ElapsedPlayTime == 0.0) && (Montage == ANIM_CRYOBED_WAKE))
					{
						CryobedWakeIsActive = true;
						API::UObjectHook::set_disabled(true);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "false");
						RoomscaleMontageOverride = true;
					}
					else if ((ElapsedPlayTime == -1.0) && (Montage == ANIM_CRYOBED_WAKE) && (CryobedWakeIsActive == true) )
					{
						CryobedWakeIsActive = false;
						API::UObjectHook::set_disabled(false);

						auto& vr = API::get()->param()->vr;
						vr->set_mod_value("VR_RoomscaleMovement", "true");
						RoomscaleMontageOverride = false;
					}
				}
				else
				{
					// API::get()->log_info("CharacterAction_C::PersistentLevel.PAWN_Hacker_Implant_C not found");
				}
			}
		}
		return nullptr;
	}
	
	int GetMontage()
	{
		static const auto func = static_class()->find_function(L"GetMontage");

		if (!func) {
			// API::get()->log_error("GetMontage not found");
		}
		else
		{
			// API::get()->log_error("GetMontage found");
		}

		struct
		{
			API::UObject* Result;
		} params{ .Result = nullptr };

		process_event(func, &params);

		if (params.Result != nullptr)
		{
			std::wstring ObjName = params.Result->get_full_name();
			// API::get()->log_info("GetMontage: %ls", ObjName.c_str());

			if (ObjName.find(L"CH_Hacker_SurgicalBed_Right_Enter_Montage") != std::wstring::npos)
			{
				return ANIM_SURGICALBED_RIGHT_ENTER;
			}
			else if (ObjName.find(L"CH_Hacker_SurgicalBed_Right_Exit_Montage") != std::wstring::npos)
			{
				return ANIM_SURGICALBED_RIGHT_EXIT;
			}
			else if (ObjName.find(L"CH_Hacker_SurgicalBed_Left_Enter_Montage") != std::wstring::npos)
			{
				return ANIM_SURGICALBED_LEFT_ENTER;
			}
			else if (ObjName.find(L"CH_Hacker_SurgicalBed_Left_Exit_Montage") != std::wstring::npos)
			{
				return ANIM_SURGICALBED_LEFT_EXIT;
			}
			else if (ObjName.find(L"CH_Hacker_death_p_Montage") != std::wstring::npos)
			{
				return ANIM_DEATH_P;
			}
			else if (ObjName.find(L"CH_Hacker_Terminal_Use_Montage") != std::wstring::npos)
			{
				return ANIM_TERMINAL_USE;
			}
			else if (ObjName.find(L"CH_Hacker_Cryobed_Wake_Montage") != std::wstring::npos)
			{
				return ANIM_CRYOBED_WAKE;
			}
			else
			{
				return ANIM_NONE;
			}
		}
		else
		{
			return ANIM_NONE;
		}
	}

	float GetElapsedPlayTime()
	{
		static const auto func = static_class()->find_function(L"GetElapsedPlayTime");
		if (!func) {
			// API::get()->log_error("CharacterAction_C::GetElapsedPlayTime not found");
			return 0.0f;
		}
		else
		{
			// API::get()->log_error("CharacterAction_C::GetElapsedPlayTime found");
		}

		struct
		{
			float Result;
		} params{ .Result = 0.0 };

		process_event(func, &params);
		// API::get()->log_error("GetElapsedPlayTime: %f", params.Result);

		return params.Result;
	}
};