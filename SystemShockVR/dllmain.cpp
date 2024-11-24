#define  _CRT_SECURE_NO_WARNINGS 1
#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <ctime>
#include <iostream>
#include <fstream>

#include "cxxtimer.hpp"

#include "uevr/Plugin.hpp"
#include "CH_Hacker_AnimBP_C.hpp"
#include "CharacterAction_C.hpp"
#include "COMP_HackerInventory_C.hpp"
#include "COMP_MoveControlManager_C.hpp"
#include "CON_Hacker_C.hpp"
#include "Controller.hpp"
#include "INTERACT_Laptop_C.hpp"
#include "MOVECONTROL_FocusableInteract_C.hpp"
#include "PAWN_Hacker_Simple_C.hpp"
#include "PAWN_SystemShockCharacter_C.hpp"
#include "SAVE_Settings_C.hpp"
#include "WIDGET_InventoryContextMenu_C.hpp"
#include "WIDGET_MainMenu_InGame_C.hpp"
#include "WIDGET_PlayerHUD_C.hpp"
#include "pch.h"

void smooth_rotation(XINPUT_STATE* state);
void smooth_rotation_cyberspace(XINPUT_STATE* state);
void send_key(WORD key, bool key_up);
void anim_cine_intro_height_recalibration();
void reset_height();
void pawn_Process(uint8_t current_pawn_state);

typedef struct _TIMER_STRUCT
{
    bool* XPressed;
    std::time_t* TimeStamp;
}   TIMER_STRUCT;

/** The various states that the pawn state machine can be in */
typedef enum ePawnState
{
    PAWN_UNKNOWN,
    PAWN_PLAYERGHOST,
    PAWN_HACKERSIMPLE,
    PAWN_HACKERIMPLANT,
    PAWN_AVATAR,
}
pawn_State_t;

using namespace uevr;

#define PLUGIN_LOG_ONCE(...) {\
    static bool _logged_ = false; \
    if (!_logged_) { \
        _logged_ = true; \
        API::get()->log_info(__VA_ARGS__); \
    }}

#define INPUT_DEADZONE_HI  ( 0.90f * FLOAT(0x7FFF) )  // Default to 90% of the +/- 32767 range.
#define INPUT_DEADZONE_MED ( 0.45f * FLOAT(0x7FFF) )  // Default to 45% of the +/- 32767 range.
#define INPUT_DEADZONE_LO  ( 0.01f * FLOAT(0x7FFF) )  // Default to 01% of the +/- 32767 range.

// Instantiate cxxtimer::Timer object
cxxtimer::Timer timer;

bool KEY_DN = false;

bool IsLaptop = false;

bool IsMFDPrev = false;
bool IsMFDCurrent = false;
bool IsContextMenu = false;
bool ContextMode = false;
bool SendKeyDebounce = false;
bool VaporizeInventory = false;

bool IsMovementPrev = true;
bool IsMovementCurrent = true;

bool ActivateIK = false;
bool DeactivateIK = false;

bool IsCrosshairPrev = false;
bool IsCrosshairCurrent = false;

bool IsAlivePrev = false;
bool IsAliveCurrent = false;

bool IsMainMenuPrev = false;
bool IsMainMenuCurrent = false;

float pose_y_current = 0.0;
float pose_y_max = 0.0;
float pose_y_min = 0.0;

bool IsPhysicalCrouching = false;
bool IsCrouchingCurrent = false;

bool IsInteractMode = false;
bool InteractToggle = false;
bool IsEquipped = false;
bool IKSanityCheck = false;
bool InteractExit = false;
bool RoomscaleMontageOverride = false;
bool IsLean = false;

bool IsSprinting = false;
bool IsSmoothTurn = false;
bool InitSmoothTurn = false;
char snap_angle;

bool IsBracket = false;
bool ButtonsDownY = false;

bool disable_equip = false;
bool validate_equip = false;

uint8_t last_pawn_state = PAWN_UNKNOWN;
uint8_t current_pawn_state = PAWN_UNKNOWN;

UEVR_Vector3f pose;
UEVR_Quaternionf rot, offset;
UEVR_Rotatorf ref;
float controller_rot_y = 0.0f;

float old_position[3] = { 0.0 };
float difference[3] = { 0.0 };
float difference_adjusted[3] = { 0.0 };
float abs_difference[3] = { 0.0 };
float old_difference = 0.0;

bool melee_attack = false;
bool rapier_attack = false;
bool melee_equipped = false;
bool rapier_equipped = false;
bool melee_debounce = false;
bool melee_dn = false;
bool melee_fwd = false;

int melee_dn_count = 0;
int melee_fwd_count = 0;

class SystemShockPlugin : public uevr::Plugin {
public:
    SystemShockPlugin() = default;

    void on_dllmain() override {}

    void on_initialize() override {
        // Logs to the appdata UnrealVRMod log.txt file
        API::get()->log_info("%s", "Salt the fries");
    }

    void on_pre_engine_tick(API::UGameEngine* engine, float delta) override
    {
        PLUGIN_LOG_ONCE("Pre Engine Tick: %f", delta);

        /* ========== GESTURE MELEE ========== */
        const UEVR_VRData* vr = API::get()->param()->vr;

        if (!vr->is_runtime_ready())
            return;

        UEVR_Vector3f left_position, right_position;
        UEVR_Quaternionf left_rotation, right_rotation;

        vr->get_pose(vr->get_right_controller_index(), &right_position, &right_rotation);

        difference[0] = right_position.x - old_position[0];
        abs_difference[0] = abs(difference[0]);
        difference[1] = right_position.y - old_position[1];
        abs_difference[1] = abs(difference[1]);
        difference[2] = right_position.z - old_position[2];
        abs_difference[2] = abs(difference[2]);

        difference_adjusted[2] = difference[2] + old_difference;
        // API::get()->log_info("speed.y = %f", difference[1]);
        // API::get()->log_info("speed.z = %f", difference[2]);

        if (difference[1] <= -.02)
        {
            melee_dn = true;
        }

        if (difference_adjusted[2] <= -.012)
        {
            melee_fwd = true;           
        }

        if ((melee_dn == true) && (melee_fwd == true) && (melee_debounce == true))
        {
            melee_attack = true;

            melee_dn = false;
            melee_fwd = false;

            melee_dn_count = 0;
            melee_fwd_count = 0;
        }

        if (difference[2] <= -.02)
        {
            rapier_attack = true;
        }

        old_position[0] = right_position.x;
        old_position[1] = right_position.y;
        old_position[2] = right_position.z;

        old_difference = 0;
        if (difference[2] < 0)old_difference = difference[2];

        if (melee_fwd == true)melee_fwd_count++;
        if (melee_dn == true)melee_dn_count++;

        if ((melee_dn_count > 1) || (melee_fwd_count > 1))
        {
            melee_dn = false;
            melee_fwd = false;

            melee_dn_count = 0;
            melee_fwd_count = 0;
        }

        melee_debounce = true;
    }

    void on_post_engine_tick(API::UGameEngine* engine, float delta) override
    {
        PLUGIN_LOG_ONCE("Post Engine Tick: %f", delta);
    }

    void on_pre_slate_draw_window(UEVR_FSlateRHIRendererHandle renderer, UEVR_FViewportInfoHandle viewport_info) override {
        PLUGIN_LOG_ONCE("Pre Slate Draw Window");
    }

    void on_post_slate_draw_window(UEVR_FSlateRHIRendererHandle renderer, UEVR_FViewportInfoHandle viewport_info) override {
        PLUGIN_LOG_ONCE("Post Slate Draw Window");
    }

    //*******************************************************************************************
// This is the controller input routine. Everything happens here.
//*******************************************************************************************
    void on_xinput_get_state(uint32_t* retval, uint32_t user_index, XINPUT_STATE* state) {

        if (state != NULL) {
          
            /* ========== CAMERA FIX ========== */

            pawn_Process(current_pawn_state);

            const auto pawn = API::get()->get_local_pawn(0);

            if (pawn->get_full_name().starts_with(L"PAWN_Avatar")) {

                current_pawn_state = PAWN_AVATAR;
            }
            else if (pawn->get_full_name().starts_with(L"PAWN_PlayerGhost"))
            {
                current_pawn_state = PAWN_PLAYERGHOST;
            }
            else if (pawn->get_full_name().starts_with(L"PAWN_Hacker_Simple"))
            {
                current_pawn_state = PAWN_HACKERSIMPLE;
            }
            else if (pawn->get_full_name().starts_with(L"PAWN_Hacker_Implant"))
            {
                current_pawn_state = PAWN_HACKERIMPLANT;
            }

            /* ========== MAIN GAME FIXES ========== */

            if (current_pawn_state == PAWN_HACKERSIMPLE) /* Intro fix */
            {
                const auto PHSC = PAWN_Hacker_Simple_C::get_instance();

                if (PHSC)
                {
                    IsLean = PHSC->get_IsTryingToLean();

                    if (IsLean)
                    {
                        PHSC->set_IsTryingToLean(false); /* Disable lean input as this can freeze movement in VR */
                    }
                }

                const auto ILC = INTERACT_Laptop_C::get_instance();
                if (ILC)
                {
                    IsLaptop = ILC->get_IsInteracting();

                    if (IsLaptop == true)
                    {                        
                        API::UObjectHook::set_disabled(true); /* Fix shaky cam during intro */
                        anim_cine_intro_height_recalibration(); /* Disabling hooks requires height recalibration for laptop sequence */
                    }
                }           
            }
            else
            {
                IsLaptop = false;
            }

            if (current_pawn_state == PAWN_AVATAR)
            {
                smooth_rotation_cyberspace(state);

                if (state->Gamepad.sThumbRY <= -200)
                {
                    state->Gamepad.wButtons |= XINPUT_GAMEPAD_B;
                }
                else if (state->Gamepad.sThumbRY >= 200)
                {
                    state->Gamepad.wButtons |= XINPUT_GAMEPAD_A;
                }
            }

            if (current_pawn_state == PAWN_HACKERIMPLANT)
            {
                /* ========== CONFIGURE IN-GAME SETTINGS ========== */

                const auto SSC = SAVE_Settings_C::get_instance();

                const auto PHSC = PAWN_Hacker_Simple_C::get_instance();

                if (PHSC)
                {
                    IsLean = PHSC->get_IsTryingToLean();

                    if (IsLean)
                    {
                        PHSC->set_IsTryingToLean(false); /* Disable lean input as this can freeze movement in VR */
                    }
                }

                /* ========== GESTURE MELEE ========== */

                COMP_HackerInventory_C::get_instance();                    

                if (melee_attack == true && melee_equipped == true) /* Check if gesture performed while pipe or wrench is equipped */
                {
                    if (state->Gamepad.bRightTrigger >= 200)
                    {
                        state->Gamepad.bRightTrigger = 0; /* Perform heavy melee */
                    }
                    else
                    {
                        state->Gamepad.bRightTrigger = 200; /* Perform light melee */
                    }
                    melee_attack = false;
                }

                if (rapier_attack == true && rapier_equipped == true) /* Check if gesture performed while rapier is equipped */
                {
                    if (state->Gamepad.bRightTrigger >= 200)
                    {
                        state->Gamepad.bRightTrigger = 0; /* Perform heavy melee */
                    }
                    else
                    {
                        state->Gamepad.bRightTrigger = 200; /* Perform light melee */
                    }
                    rapier_attack = false;
                }                

                /* ========== INGAME MENU FIXES ========== */

                IsMainMenuPrev = IsMainMenuCurrent;

                const auto WMMIGC = WIDGET_MainMenu_InGame_C::get_instance();

                if (WMMIGC) {

                    IsMainMenuCurrent = WMMIGC->get_IsMainMenuEnabled();

                    if (IsMainMenuPrev != IsMainMenuCurrent)
                    {
                        if (IsMainMenuCurrent == false)
                        {
                            // API::get()->log_info("Main menu open");
                            auto& vr = API::get()->param()->vr;
                            vr->set_snap_turn_enabled(true);
                        }
                        else
                        {
                            // API::get()->log_info("Main menu closed");
                            auto& vr = API::get()->param()->vr;
                            vr->set_snap_turn_enabled(false); /* Prevent snap turn when camera superimposed with menu screen */
                        }
                    }
                }

                /* ========== PHYSICAL CROUCHING ========== */

                const auto hmd_index = API::get()->param()->vr->get_hmd_index();

                API::get()->param()->vr->get_pose(hmd_index, &pose, &rot);
                API::get()->param()->vr->get_rotation_offset(&offset);
                // API::get()->log_info("pose.y = %f", pose.y);

                pose_y_current = pose.y;

                if (pose_y_current >= pose_y_max)
                {
                    pose_y_max = pose_y_current; /* Track max position of HMD */
                    // API::get()->log_info("pose_y_max = %f", pose_y_max);
                }
                else if (pose_y_current <= pose_y_min)
                {
                    pose_y_min = pose_y_current; /* Track min position of HMD */
                    // API::get()->log_info("pose_y_min = %f", pose_y_min);
                }

                if ( (IsPhysicalCrouching == false) && (pose_y_current < (pose_y_max-0.4) ) ) /* Check HMD position delta to detect if player is physically crouched */
                {
                    IsPhysicalCrouching = true;
                    // API::get()->log_info("IsPhysicalCrouching = 1");

                    KEY_DN = true;
                    send_key(VK_LCONTROL, KEY_DN);
                }
                else if ( (IsPhysicalCrouching == true) && (pose_y_current > (pose_y_min + 0.4)))
                {
                    IsPhysicalCrouching = false;
                    // API::get()->log_info("IsPhysicalCrouching = 0");

                    KEY_DN = true;
                    send_key(VK_LCONTROL, KEY_DN);
                }                

                /* ========== DEATH ANIMATION FIX ========== */

                IsAlivePrev = IsAliveCurrent;

                const auto PSSC = PAWN_SystemShockCharacter_C::get_instance();

                if (PSSC) {

                    IsAliveCurrent = PSSC->get_IsAlive();

                    if (IsAlivePrev != IsAliveCurrent)
                    {
                        if (IsAliveCurrent == false)
                        {
                            API::UObjectHook::set_disabled(true); /* Disable hooks if killed to allow death animations to play in VR */

                            auto& vr = API::get()->param()->vr;
                            vr->set_mod_value("VR_RoomscaleMovement", "false");
                        }
                        else
                        {
                            API::UObjectHook::set_disabled(false);

                            auto& vr = API::get()->param()->vr;
                            vr->set_mod_value("VR_RoomscaleMovement", "true");
                        }
                    }
                }

                /* ========== MONTAGE FIX ========== */

                const auto CAC = CharacterAction_C::get_instance();

                /* ========== CROSSHAIR TOGGLE ========== */                

                const auto CMCMC = COMP_MoveControlManager_C::get_instance();

                if (CMCMC)
                {
                    IsCrouchingCurrent = CMCMC->get_IsCrouching(); /* Verify player not crouching before attempting to enable Interact mode as cursor requires default collision cylinder settings */
                }

                /* Toggle between interact and combat mode with right trigger */

                IsCrosshairPrev = IsCrosshairCurrent;

                if (state->Gamepad.bLeftTrigger <= 100)
	            {
	                IsCrosshairCurrent = false;  
	            }           
	            else if (state->Gamepad.bLeftTrigger >= 200) 
	            {   
                    IsCrosshairCurrent = true;
	            }

                if (IsCrosshairPrev != IsCrosshairCurrent)
                {
                    if ( (IsMFDCurrent == false) && (IsMovementCurrent == true) && (IsAliveCurrent == true) && (IsMainMenuCurrent == false) && (InteractExit == false) && (IsCrouchingCurrent == false) ) /* Verify if cursor should be hidden */
                    {
                        if (IsCrosshairCurrent == false)
                        {
                            auto& vr = API::get()->param()->vr;

                            const auto CHC = CON_Hacker_C::get_instance();
                            if (CHC) {
                                CHC->ForceHideCrosshairs(true); /* Hide crosshair */
                                IsBracket = false;
                            }

                            if (RoomscaleMontageOverride == false)
                            {                               
                                vr->set_mod_value("VR_RoomscaleMovement", "true"); /* Re-enable Roomscale */
                            }

                            const auto AnimBP = CH_Hacker_AnimBP_C::get_instance();
                            if (AnimBP) {
                                AnimBP->SetUseIK(true); /* Enable IK for motion controls */
                            }

                            vr->set_mod_value("UI_Y_Offset", "-0.200000"); /* Adjust UI placement */

                            if (IsEquipped == false)
                            {
                                InteractToggle = true; /* If inventory item is holstered when exiting Interact mode, force weapon draw animation to show item */
                                COMP_HackerInventory_C::get_instance();
                            }

                            IKSanityCheck = false;
                            IsInteractMode = false;
                        }
                        else
                        {
                            const auto CHC = CON_Hacker_C::get_instance();
                            if (CHC) {
                                CHC->ForceHideCrosshairs(false); /* Show crosshair */
                                IsBracket = true;
                            }

                            auto& vr = API::get()->param()->vr;
                            vr->set_mod_value("VR_RoomscaleMovement", "false"); /* Temporarily disable Roomscale to disable pushback for Interact mode */
                                                                                  
                            const auto AnimBP = CH_Hacker_AnimBP_C::get_instance();
                            if (AnimBP) {
                                AnimBP->SetUseIK(false); /* Disable IK for Interact mode */
                            }

                            vr->set_mod_value("UI_Y_Offset", "0.000000"); /* Adjust UI placement */

                            if (IsEquipped == true)
                            {
                                InteractToggle = true; /* If inventory item is equipped when exiting combat mode, force weapon holster animation to hide item */
                                COMP_HackerInventory_C::get_instance();
                            }

                            IsInteractMode = true;
                        }
                    }
                }

                if ( (IsInteractMode == true) && (IsEquipped == false) )
                {
                    IKSanityCheck = true; /* Check if any events occur in Interact mode that would require motion controls to be re-enabled */

                    if (IsMFDCurrent == false)
                    {
                        // Vibration flag, clear it on Y button release.
                        if (ButtonsDownY == true)
                        {
                            if (timer.count<std::chrono::milliseconds>() >= 500) /* Open MFD if Y held down 500ms in Interact mode */
                            {
                                KEY_DN = true;
                                send_key(VK_TAB, KEY_DN);

                                timer.stop();
                                timer.reset();

                                IsMFDCurrent = true;
                                ButtonsDownY = false;
                            }

                            if (!(state->Gamepad.wButtons & XINPUT_GAMEPAD_Y))  /* Open menu if Y held down <500ms in Interact mode */
                            {
                                ButtonsDownY = false;

                                timer.stop();

                                if (timer.count<std::chrono::milliseconds>() < 500)
                                {
                                    KEY_DN = true;
                                    send_key(VK_ESCAPE, KEY_DN);
                                }

                                timer.reset();
                                IsMainMenuCurrent = true;
                            }
                        }
                        else 
                        {
                            if (state->Gamepad.wButtons & XINPUT_GAMEPAD_Y)
                            {
                                ButtonsDownY = true;

                                // Start the timer
                                timer.start();                              
                            }
                        }
                    }
                }

                // API::get()->log_error("IsInteractMode = %d, IsEquipped = %d, InteractExit = %d, IsCrouchingCurrent = %d, IsMFDCurrent = %d, IsMainMenuCurrent = %d, IsMovementCurrent = %d, IsAliveCurrent = %d, IsCrosshairCurrent = %d, IKSanityCheck = %d", IsInteractMode, IsEquipped, InteractExit, IsCrouchingCurrent, IsMFDCurrent, IsMainMenuCurrent, IsMovementCurrent, IsAliveCurrent, IsCrosshairCurrent, IKSanityCheck);

                if (IKSanityCheck == true)
                {
                    if ((IsEquipped == true) || (InteractExit == true) || (IsCrouchingCurrent == true) || (IsMFDCurrent == true) || (IsMainMenuCurrent == true)) /* Restore motion controls if pickup item, montage, or crouching is active */
                    {
                        // API::get()->log_error("InteractExit = %d", InteractExit);

                        const auto CHC = CON_Hacker_C::get_instance();
                        if (CHC) {
                            CHC->ForceHideCrosshairs(true);
                            IsBracket = false;
                        }

                        auto& vr = API::get()->param()->vr;
                        vr->set_mod_value("VR_RoomscaleMovement", "true");

                        const auto AnimBP = CH_Hacker_AnimBP_C::get_instance();
                        if (AnimBP) {
                            AnimBP->SetUseIK(true);
                        }

                        vr->set_mod_value("UI_Y_Offset", "-0.200000");

                        if (IsEquipped == false) /* If inventory item is holstered while restoring motion controls, force weapon draw animation to show item */
                        {
                            InteractToggle = true;
                            COMP_HackerInventory_C::get_instance();

                            IsCrosshairPrev = true;
                            IsCrosshairCurrent = true;
                        }

                        InteractExit = false;
                        IsInteractMode = false;
                        IKSanityCheck = false;
                    }
                }

                if (IsCrosshairCurrent == false && IsMovementCurrent == true) /* Force hide crosshair when returning to combat mode after exiting puzzle */
                {
                    const auto CHC = CON_Hacker_C::get_instance();
                    if (CHC) {
                        CHC->ForceHideCrosshairs(true);
                        IsBracket = false;
                    }
                }

                /* ========== MFD FIX ========== */

                IsMFDPrev = IsMFDCurrent;

                const auto WICMC = WIDGET_InventoryContextMenu_C::get_instance();

                if (WICMC)
                {
                    IsContextMenu = WICMC->get_IsInventoryContextMenuEnabled();
                }

                const auto HUD = WIDGET_PlayerHUD_C::get_instance();
                if (HUD) {
                    IsMFDCurrent = HUD->get_is_mfd_visible();

                    if (VaporizeInventory == false) {
                        HUD->set_IsTryingToVaporizeInventory(false); /* Prevent RT from vaporizing items */
                    }
                    else
                    {
                        HUD->set_IsTryingToVaporizeInventory(true); /* Vaporize items on rebinded input */
                    }
                }

                if (IsMFDPrev != IsMFDCurrent)
                {
                    if (IsMFDCurrent == true)
                    {
                        // API::get()->log_error("MFD Found");
                        auto& vr = API::get()->param()->vr;
                        vr->set_snap_turn_enabled(false); /* Disable snap turn when MFD is active */
                    }
                    else
                    {
                        // API::get()->log_error("MFD Not Found");
                        auto& vr = API::get()->param()->vr;
                        vr->set_snap_turn_enabled(true);
                    }
                }

                if (IsMFDCurrent == true) /* Remap inventory controls for VR */
                {
                    if (IsContextMenu == true)
                    {
                        if (!(state->Gamepad.wButtons & XINPUT_GAMEPAD_Y))
                        {
                            ContextMode = true;
                            SendKeyDebounce = false;
                        }
                        
                        if ((state->Gamepad.wButtons & XINPUT_GAMEPAD_Y) && ContextMode == true)
                        {
                            if (SendKeyDebounce == false)
                            {
                                SendKeyDebounce = true;
                                KEY_DN = true;
                                send_key(VK_RETURN, KEY_DN);
                            }
                        }

                        if (state->Gamepad.sThumbRY <= -200)
                        {
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
                        }
                        else if (state->Gamepad.sThumbRY >= 200)
                        {
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
                        }
                        else if (state->Gamepad.sThumbRX <= -200)
                        {
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
                        }
                        else if (state->Gamepad.sThumbRX >= 200)
                        {
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
                        }

                        if (state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
                        {
                            state->Gamepad.wButtons = state->Gamepad.wButtons & ~XINPUT_GAMEPAD_LEFT_SHOULDER;  /* Disable LGrip */
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP; /* Cycle context menu with grip buttons */
                        }
                        else if (state->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
                        {
                            state->Gamepad.wButtons = state->Gamepad.wButtons & ~XINPUT_GAMEPAD_RIGHT_SHOULDER; /* Disable RGrip */
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN; /* Cycle context menu with grip buttons */
                        }
                    }
                    else
                    {
                        ContextMode = false;

                        if (state->Gamepad.bLeftTrigger > 0 && validate_equip == false) {
                            disable_equip = true; /* Set flag to prevent auto equipping items while opening inventory when LT is held down */
                        }
                        else if (state->Gamepad.bLeftTrigger == 0)
                        {
                            disable_equip = false;
                        }
                        
                        if (timer.count<std::chrono::milliseconds>() >= 1000)
                        {
                            VaporizeInventory = true;
                            timer.stop();
                            timer.reset();
                        }

                        if (state->Gamepad.wButtons & XINPUT_GAMEPAD_A) {                           

                            timer.start();
                        }
                        else
                        {
                            VaporizeInventory = false;
                            timer.stop();
                            timer.reset();
                        }

                        if (state->Gamepad.bLeftTrigger >= 200) {
                            if(disable_equip == false) state->Gamepad.wButtons = (state->Gamepad.wButtons | XINPUT_GAMEPAD_A);
                        }
                        else {
                            state->Gamepad.wButtons = (state->Gamepad.wButtons & ~XINPUT_GAMEPAD_A);
                        }

                        validate_equip = true;
                    }

                    if (state->Gamepad.bRightTrigger >= 200) {
                        state->Gamepad.bLeftTrigger = 200;
                    }
                    else {
                        state->Gamepad.bLeftTrigger = 0;
                    }                    
                }
                else
                {
                    validate_equip = false;
                }

                /* ========== KEYPAD CIRCUIT PUZZLE FIX ========== */
                       
                IsMovementPrev = IsMovementCurrent;

                const auto MC = MOVECONTROL_FocusableInteract_C::get_instance();
                if (MC) {

                    IsMovementCurrent = MC->get_ShouldUseCharacterMovement();

                    if (IsMovementPrev != IsMovementCurrent)
                    {
                        if (IsMovementCurrent == false)
                        {
                            // API::get()->log_info("Camera is framed. IK disabled.");
                            DeactivateIK = true; /* Fis cursor alignment issues if focused on puzzle */
                        }
                        else
                        {
                            // API::get()->log_info("Camera is not framed. IK enabled.");
                            ActivateIK = true; /* Re-enable motion controls after player exits puzzle */
                        }
                    }
                }

                if (DeactivateIK == true)
                {
                    const auto AnimBP = CH_Hacker_AnimBP_C::get_instance();
                    if (AnimBP) {
                        AnimBP->SetUseIK(false);                        
                    }

                    reset_height(); /* Recalibrate height for keypads and puzzles */

                    auto& vr = API::get()->param()->vr;
                    vr->set_mod_value("UI_Distance", "0.500000");
                    vr->set_mod_value("UI_Size", "0.500000");
                    vr->set_mod_value("UI_Y_Offset", "0.000000");
                    vr->set_mod_value("VR_RoomscaleMovement", "false");
                    vr->set_snap_turn_enabled(false);

                    IsBracket = true;
                    DeactivateIK = false;
                }

                if (ActivateIK == true)
                {
                    const auto AnimBP = CH_Hacker_AnimBP_C::get_instance();
                    if (AnimBP) {
                        AnimBP->SetUseIK(true);    
                    }

                    auto& vr = API::get()->param()->vr;
                    vr->set_mod_value("UI_Distance", "2.000000");
                    vr->set_mod_value("UI_Size", "2.000000");
                    vr->set_mod_value("UI_Y_Offset", "-0.200000");
                    vr->set_mod_value("VR_RoomscaleMovement", "true");
                    vr->set_snap_turn_enabled(true);

                    if(IKSanityCheck == true)InteractExit = true;
                    if(IsInteractMode == false)IsBracket = false;
                    ActivateIK = false;
                }

                /* =================== REBIND GRIP BUTTONS =================== */

                if ((IsMFDCurrent == false) && (IsMainMenuCurrent == false))
                {
                    if (abs(state->Gamepad.sThumbLX) <= INPUT_DEADZONE_MED && abs(state->Gamepad.sThumbLY) <= INPUT_DEADZONE_MED)
                    {
                        IsSprinting = false; /* Disable sprinting state */
                    }

                    if (IsSprinting)
                    {
                        state->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_THUMB; /* Enable sprinting */
                    }

                    if (state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
                    {
                        state->Gamepad.wButtons = state->Gamepad.wButtons & ~XINPUT_GAMEPAD_LEFT_SHOULDER; /* Disable LGrip */

                        if (abs(state->Gamepad.sThumbLX) >= INPUT_DEADZONE_HI || abs(state->Gamepad.sThumbLY) >= INPUT_DEADZONE_HI)
                        {
                            IsSprinting = true;  /* Enable sprinting state */
                        }
                    }

                    if (state->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
                    {
                        state->Gamepad.wButtons = state->Gamepad.wButtons & ~XINPUT_GAMEPAD_RIGHT_SHOULDER; /* Disable RGrip */

                        if (state->Gamepad.sThumbLX >= INPUT_DEADZONE_LO)
                        {
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT; /* Remap DPad Right for hotbar */
                        }
                        else if (state->Gamepad.sThumbLX <= -INPUT_DEADZONE_LO)
                        {
                            state->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT; /* Remap DPad Left for hotbar */
                        }

                        state->Gamepad.sThumbLX = 0; /* Prevent rotating while holding RGrip */
                    }
                }
            }

            /* =================== SMOOTH TURNING =================== */

            if ((current_pawn_state == PAWN_HACKERSIMPLE) || (current_pawn_state == PAWN_HACKERIMPLANT)) /* Verify game has loaded before attempting smooth turn */
            {
                auto& vr = API::get()->param()->vr;
                char snap_angle[16] = { 0 };
                vr->get_mod_value("VR_SnapturnTurnAngle", snap_angle, sizeof(snap_angle));
                int snap_angle_int = atoi(snap_angle);

                if ( (snap_angle_int == 359) && ((IsMFDCurrent == false) && (IsMovementCurrent == true) && (IsMainMenuCurrent == false) || (current_pawn_state == PAWN_HACKERSIMPLE)) ) /* Verify if smooth turning conditions have been met */
                {
                    IsSmoothTurn = true;
                    vr->set_snap_turn_enabled(false);

                    if (InitSmoothTurn == false)
                    {
                        InitSmoothTurn = true;
                    }

                    smooth_rotation(state);
                }
                else if (snap_angle_int != 359)
                {
                    if (IsSmoothTurn == true) /* Disable snap turn when angle decreased in UEVR overlay */
                    {
                        IsSmoothTurn = false;
                        InitSmoothTurn = false;

                        vr->set_snap_turn_enabled(true);
                    }
                }
            }

            /* ========== MANUAL HEIGHT CALIBRATION ========== */

           if (state->Gamepad.bLeftTrigger >= 200 && (state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)) {
               reset_height();
           }           
        }
    }
};

void smooth_rotation(XINPUT_STATE* state)
{
    if (state->Gamepad.sThumbRX >= INPUT_DEADZONE_HI) /* Fast speed smooth rotation */
    {
        ref.yaw += 3;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX > INPUT_DEADZONE_MED) && (state->Gamepad.sThumbRX < INPUT_DEADZONE_HI)) /* Medium speed smooth rotation */
    {
        ref.yaw += 2;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX > INPUT_DEADZONE_LO) && (state->Gamepad.sThumbRX <= INPUT_DEADZONE_MED)) /* Slow speed smooth rotation */
    {
        ref.yaw += 1;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if (state->Gamepad.sThumbRX <= -INPUT_DEADZONE_HI) /* Fast speed smooth rotation */
    {
        ref.yaw -= 3; // = controller_rot_y;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX < -INPUT_DEADZONE_MED) && (state->Gamepad.sThumbRX > -INPUT_DEADZONE_HI)) /* Medium speed smooth rotation */
    {
        ref.yaw -= 2; // = controller_rot_y;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX < -INPUT_DEADZONE_LO) && (state->Gamepad.sThumbRX >= -INPUT_DEADZONE_MED)) /* Medium speed smooth rotation */
    {
        ref.yaw -= 1; // = controller_rot_y;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else
    {
        const auto CONT = Controller::get_instance();
        if (CONT) {
            ref = CONT->GetControlRotation(); /* Get camera settings before attempting smooth rotation */
        }

        // API::get()->log_info("Old SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
}

void smooth_rotation_cyberspace(XINPUT_STATE* state)
{
    if (state->Gamepad.sThumbRX >= INPUT_DEADZONE_HI) /* Fast speed smooth rotation */
    {
        ref.yaw += 2;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX > INPUT_DEADZONE_MED) && (state->Gamepad.sThumbRX < INPUT_DEADZONE_HI)) /* Medium speed smooth rotation */
    {
        ref.yaw += 1.5;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX > INPUT_DEADZONE_LO) && (state->Gamepad.sThumbRX <= INPUT_DEADZONE_MED)) /* Slow speed smooth rotation */
    {
        ref.yaw += .5;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if (state->Gamepad.sThumbRX <= -INPUT_DEADZONE_HI) /* Fast speed smooth rotation */
    {
        ref.yaw -= 2; // = controller_rot_y;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX < -INPUT_DEADZONE_MED) && (state->Gamepad.sThumbRX > -INPUT_DEADZONE_HI)) /* Medium speed smooth rotation */
    {
        ref.yaw -= 1.5; // = controller_rot_y;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else if ((state->Gamepad.sThumbRX < -INPUT_DEADZONE_LO) && (state->Gamepad.sThumbRX >= -INPUT_DEADZONE_MED)) /* Medium speed smooth rotation */
    {
        ref.yaw -= .5; // = controller_rot_y;
        const auto CONT = Controller::get_instance();
        if (CONT) {
            CONT->SetControlRotation(&ref);
        }

        // API::get()->log_info("New SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
    else
    {
        const auto CONT = Controller::get_instance();
        if (CONT) {
            ref = CONT->GetControlRotation(); /* Get camera settings before attempting smooth rotation */
        }

        // API::get()->log_info("Old SetControlRotation x = %f y = %f z = %f", ref.pitch, ref.yaw, ref.roll);
    }
}

void send_key(WORD key, bool key_dn)
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    if (key_dn == true) 
    {
        key_dn = false;

        input.ki.dwFlags = 0;
        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

void anim_cine_intro_height_recalibration() {

    auto& api = API::get();
    auto vr = api->param()->vr;
    UEVR_Vector3f origin{};
    vr->get_standing_origin(&origin);

    UEVR_Vector3f hmd_pos{};
    UEVR_Quaternionf hmd_rot{};
    vr->get_pose(vr->get_hmd_index(), &hmd_pos, &hmd_rot);

    origin.y = (hmd_pos.y)-(float)0.3; /* Recalibrate height for intro laptop animation */

    vr->set_standing_origin(&origin);
}

void reset_height() {

    auto& api = API::get();
    auto vr = api->param()->vr;
    UEVR_Vector3f origin{};
    vr->get_standing_origin(&origin);

    UEVR_Vector3f hmd_pos{};
    UEVR_Quaternionf hmd_rot{};
    vr->get_pose(vr->get_hmd_index(), &hmd_pos, &hmd_rot);

    origin.y = hmd_pos.y;

    vr->set_standing_origin(&origin);

    pose_y_min = 0.0;
    pose_y_max = 0.0;
}

void pawn_Process(uint8_t current_pawn_state)
{
   auto& vr = API::get()->param()->vr;

   if (last_pawn_state != current_pawn_state)
   {
       switch (current_pawn_state)
       {
            case PAWN_PLAYERGHOST:
                // API::get()->log_info("New pawn found: PlayerGhost %d %d", last_pawn_state, current_pawn_state); /* Title screen or cutscene */
                vr->set_aim_method(0);
                vr->set_snap_turn_enabled(true);
                vr->set_decoupled_pitch_enabled(true);
                vr->set_mod_value("VR_RoomscaleMovement", "false");
                vr->set_mod_value("UI_Distance", "2.000000");
                vr->set_mod_value("UI_Size", "2.000000");
                vr->set_mod_value("UI_Y_Offset", "-0.200000");
                vr->set_mod_value("VR_CameraForwardOffset", "0.000000");
                vr->set_mod_value("VR_CameraRightOffset", "0.000000");
                vr->set_mod_value("VR_CameraUpOffset", "0.000000");
                API::UObjectHook::set_disabled(true);
                reset_height();

                break;
            case PAWN_HACKERSIMPLE:
                // API::get()->log_info("New pawn found: HackerSimple %d %d", last_pawn_state, current_pawn_state); /* Animation */
                vr->set_aim_method(2);
                vr->set_snap_turn_enabled(true);
                vr->set_decoupled_pitch_enabled(true);
                vr->set_mod_value("VR_RoomscaleMovement", "false");
                vr->set_mod_value("UI_Distance", "2.000000");
                vr->set_mod_value("UI_Size", "2.000000");
                vr->set_mod_value("UI_Y_Offset", "-0.200000");
                vr->set_mod_value("VR_CameraForwardOffset", "0.000000");
                vr->set_mod_value("VR_CameraRightOffset", "0.000000");
                vr->set_mod_value("VR_CameraUpOffset", "0.000000");
                API::UObjectHook::set_disabled(false);
                reset_height();

                break;
            case PAWN_HACKERIMPLANT:
                // API::get()->log_info("New pawn found: HackerImplant %d %d", last_pawn_state, current_pawn_state); /* Main game */
                vr->set_aim_method(2);
                vr->set_snap_turn_enabled(true);
                vr->set_decoupled_pitch_enabled(true);
                vr->set_mod_value("VR_RoomscaleMovement", "true");
                vr->set_mod_value("UI_Distance", "2.000000");
                vr->set_mod_value("UI_Size", "2.000000");
                vr->set_mod_value("UI_Y_Offset", "-0.200000");
                vr->set_mod_value("VR_CameraForwardOffset", "0.000000");
                vr->set_mod_value("VR_CameraRightOffset", "0.000000");
                vr->set_mod_value("VR_CameraUpOffset", "0.000000");
                API::UObjectHook::set_disabled(false);
                reset_height();

                break;
            case PAWN_AVATAR:
                // API::get()->log_info("New pawn found: Avatar %d %d", last_pawn_state, current_pawn_state); /* Cyberspace */
                vr->set_aim_method(2);
                vr->set_snap_turn_enabled(false);
                vr->set_decoupled_pitch_enabled(false);
                vr->set_mod_value("VR_RoomscaleMovement", "true");
                vr->set_mod_value("UI_Distance", "2.000000");
                vr->set_mod_value("UI_Size", "2.000000");
                vr->set_mod_value("UI_Y_Offset", "-0.200000");
                vr->set_mod_value("VR_CameraForwardOffset", "0.000000");
                vr->set_mod_value("VR_CameraRightOffset", "0.000000");
                vr->set_mod_value("VR_CameraUpOffset", "0.000000");
                API::UObjectHook::set_disabled(true);
                reset_height();

                break;
            default:
                break;
       }
   }

   last_pawn_state = current_pawn_state;
}

// Actually creates the plugin. Very important that this global is created.
// The fact that it's using std::unique_ptr is not important, as long as the constructor is called in some way.
std::unique_ptr<SystemShockPlugin> g_plugin{ new SystemShockPlugin() };