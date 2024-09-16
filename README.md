# System Shock Remake UEVR Plugin
This System Shock mod brings VR support complete with 6DOF motion controls to one of the greatest immersive sims of all time!  This VR mod consists of:

1) A UEVR profile and C++ plugin (Bundled together).  Required UEVR Nightly 940 or newer!
2) An optional PAK mod

The UEVR profile adds 6DOF motion controls to System Shock.  The C++ plugin adds a wide variety of advanced VR features and fixes to the game.  The PAK mod updates the button prompts to match Oculus Touch controllers.

## VR Mod Features
### UEVR Profile
* 6DOF motion controls.
* ADS support.  Projectiles are aligned with gun sights for added VR immersion.
* Disable On Screen Effects to fix intro cutscene.
* Fix issue with transparency effects only appearing in left eye.
* The culler is attached to the HMD with an increased FOV to prevent "blackout" when looking around in VR.
* Lua script with the following features:
  * Hides unnecessary floating arm meshes when weapons are holstered.
  * Motion control fixes to better align weapons with your hands in real life.

### VR Plugin
* In-game settings automatically configured for VR.
* Improved object interaction.  Hold **LT** to enable "interact" mode.  You do not need to enter "interact" mode to interact with objects, but this mode will display a cursor so you can more easily interact with things like weapons, ammo, switches, levers, etc.  Weapons are disabled in this mode as enabling motion controls prevents the cursor from working properly in VR. This mode also temporarily disables Roomscale which allows you to lean closely towards objects and walls without any pushback.  Releasing **LT**, crouching, or picking up auto equip items will exit "interact" mode and re-center you in your playspace.  "Interact" mode is also automatically activated and exited when you use or stop using a keypad, circuit puzzle, or vending machine.
* Motion controller gesture support.  You can melee attack with the lead pipe, wrench, or laser rapier by swinging your right hand forward in real life.  Holding **RT** while performing a gesture will perform a heavy melee attack.  You can alternatively melee with only **RT**.
* Improved VR controls when using menus.  The main menu can be accessed by holding **System** for less than 1 second.  The main menu can alternatively be accessed by entering Interact Mode (hold **LT**) and holding **Y** for less than 1 second.  The Multi-Function Display (MFD) can be accessed by holding **System** for greater than 1 second.  The MFD can alternatively be accessed by entering Interact Mode (hold **LT**) and holding **Y** for greater than 1 second. The mouse cursor can be controlled with **RS**.  Inventory items can be selected with **LT** and moved with **RS**.  Inventory sub-menu options (shown with **Y**) can also be selected with **Y**.  Inventory sub-menu options can be scrolled by flicking **RS** (Dpad) up/down/left/right.  Press **RT** to display additional information on selected inventory items.  Press **B** to sort items.  Hold **A** for 2 seconds to vaporize junk (note that food may also be vaporized). **LGrip** abd **RGrip** can be used to cycle the MFD tabs.   
* Roomscale support with Automatic Height Adjustment.
* Snap Turn support. Enabled by default.
* Smooth Turn support.  Press **L3+R3** to open up the UEVR overlay.  Select the "Input" tab and set the "Snap Turn Angle" to 359 (for Meta, Index, HTC, and Pimax controllers) or 2 (for Varjo controllers).  This enables smooth turning for seated play.
* Physical crouching support.  You can crouch and crawl through tight collision areas by crouching in real life.  The profile also prevents the camera from "rubber banding" to the ground when crouching is active.  You can recalibrate your height and physical crouching threshold by pressing **LT+L3**.  Note that pressing **X** to crouch will no longer lower the camera to the ground if physical crouching is active.  If you wish to disable physical crouching and use **X** to crouch for seated play, delete "17105528413843615245_props.json" from your UEVR global directory.
* Disables leaning as this causes movement to freeze in VR.
* Cyberspace fixes.  Gameplay in cyberspace is similar to Descent. You can steer with the right thumb stick and pressing **RT** will fire at the center of the screen.
* First person animation fixes.  The apartment intro arrest sequence, cryobeds, surgical beds, cyberspace terminals, and death sequences now play properly in VR.
* Cutscenes fixes.
* UI fixes.
  * Snap Turn is temporarily disabled when menus are active or keypads/puzzles are being used.
  * The camera will not move with your arm when the title menu is active.
  * The UI and HUD are attached to the right hand.
 
### PAK Mod (Optional)
* SystemShock-VRFixes_p.pak: Oculus Touch button prompts.

## Installation
1) Install a clean copy of System Shock Remake and delete any existing System Shock profiles from your UEVR global folder if installed.
2) Install the nightly build of UEVR (at least UEVR Nightly 940 or newer required) from [**HERE**](https://github.com/praydog/UEVR-nightly/releases).
3) Download the latest release of the System Shock UEVR plugin from [**HERE**](https://github.com/Ashok0/SystemShock-UEVR/releases). Click "Import Config" in the UEVR UI and navigate to "SystemReShock-Win64-Shipping.zip" and click on it and accept the DLL warning.
4) (Optional) Download the latest PAK mod from [**HERE**](https://github.com/Ashok0/SystemShock-UEVR/releases).  Copy the .pak file to \Steam\steamapps\common\System Shock Remake\SystemShock\Content\Paks
5) (Optional) For seated play, delete "17105528413843615245_props.json" from your UEVR global directory to disable physical crouching.  This will allow you to manually crouch by pressing **X**.
6) Launch System Shock and inject the game with UEVR!

## Controls
### General
* Move: LS
* Jump: A
* Crouch: X (Seated Mode Only)
* Sprint: LGrip

### Interaction & Combat
* Interact Mode: LT (Hold LT for interaction cursor)
* Interact: B
* Exit Interact: X
* Attack: RT
* Reload: B
* Mode Switch: Y
* Main Menu: System (Hold < 1s) OR Interact Mode + Y (Hold < 1s)
* MFD: System (Hold > 1s) OR Interact Mode + Y (Hold > 1s)
* Previous/Next Hotbar Item: Dpad (Right Thumbrest OR RGrip) + LS

### MFD
* Context menu: Y
* Select menu options: Y
* Scroll menu options: RS (Flick Up/Dn/Left/Right)
* Select item: LT
* Move item: RS
* Sort: B
* Analyze: RT
* Vaporize: A (Hold > 2s)
* Previous Tab: LGrip
* Next Tab: RGrip

### Cyberspace
* Move: LS
* Ascend: A
* Descend: X
* Roll Left: LB
* Roll Right: RB
* Pulser: RT
* I.C.E. Drill: LT
* Turbo Dash: LS (Dn)
* Recall: Y
* Decoy: B

## Known Issues
1) When you rotate your right motion controller in real life, weapons pivot slightly off center from your hand.  It's a subtle issue which isn't terribly gamebreaking.
2) Minor UI issues
* When interacting with keypads/circuit puzzles/vending machines, you may need to manually recenter the crosshair using the right stick.
* Moving around in your play area after interacting with keypads/circuit puzzles/vending machines can cause the crosshair to become misaligned.
* Inject the game during the opening splash screens. If you inject after the title screen loads, this can result in a bug where a mouse cursor is visible in your view.
3) If you have issues with motion control "jitter", you can minimize the issue by reducing your "Graphics" settings.
4) If you get "stuck" while crawling through the vents in Research Labs, you can get "unstuck" by jumping (A) and/or standing up and crouching again while holding the left stick forward.  

## FAQ
**Can I adjust settings for the game when using this mod?**
You can change select settings safely.  Do not change any of the following, however, as these settings are managed directly by the VR plugin.  Modifying the default settings related to crouching will break physical crouching.

*UEVR SETTINGS*
* VR_RoomscaleMovement
* UI_Y_Offset
* UI_Distance
* UI_Size
* VR_SnapTurn
* VR_AimMethod
* VR_DecoupledPitch
* VR_CameraForwardOffset
* VR_CameraRightOffset
* VR_CameraUpOffset

*IN-GAME SETTINGS*
* Toggle Crouch
* Crouch = X

**I pressed *X* and crouching is not working!**
With default settings, you should always crouch by crouching in real life.  If you press the crouch button (**X**), the player will technically crouch but this plugin prevents the crouch button from modifying the camera height as this would interfere with physical crouching.  If you wish to use the **X** button to crouch, you must delete "17105528413843615245_props.json" from your UEVR global directory.

**Pressing *LT* no longer activates Interact mode!**
If **LT** is not working for you, you may be in a crouched state.  Press **X** to toggle crouching off.  Interact mode is disabled when you are crouched as the cursor only works properly in VR at a standing height.  

**My game crashes to the desktop with a "LowLevelFatalError" message!**
Performing a clean install of System Shock will resolve the issue.

**My game crashes to the desktop when using the inventory menu!**
Uninstall prior installed System Shock mods from your Steam folder as incompatible mods may cause System Shock to crash to the desktop.

**My weapons are invisible!** This issue can occur when using an older flat saved game in VR.  Restarting the game with a clean saved game will resolve the issue.

**Is this mod still in development?**
The mod is currently in maintenance mode but I will be continuing to support the mod if any new bugs are found.  If you experience any issues with the mod, please send me a bug report so I can continue to improve the System Shock VR plugin!

## Recommended Mods
* [**Adjusted Respawn Limits**](https://www.nexusmods.com/systemshock2023/mods/93) - Reduce enemy respawning 
  * Extract the contents and copy "v1.2_Reinforcements_0.5x_P.pak" to *\Steam\steamapps\common\System Shock Remake\SystemShock\Content\Paks*
* [**Cyberspace Adjustments**](https://www.nexusmods.com/systemshock2023/mods/83) - Reduce the difficulty of cyberspace sections of the game 
* [**Classic Evil Shodan Portrait**](https://www.nexusmods.com/systemshock2023/mods/101) - Replace SHODAN's evil portrait with the classic ones from System Shock 1 or 2.

## Credits
Special thanks to Praydog, TimBurton, Markmon, and CJ117 for their support while developing this VR mod!

[**Feel free to buy me a coffee**](http://www.patreon.com/ashok0) :coffee:
