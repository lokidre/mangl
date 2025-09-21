# Mangl TODO
---

## URGENT

## BUGS
- Packet: not working with vector in vector containing objects
- Android: pause the music if the app goes to background

## TEST

### Multiplayer
- Test for corrent disconnect if the peer is connected but didn't send anything
- Test for disconnect if peer sent invalid discovery info
- Test for disconnect if peer didn't send packet after some time (ping or packet)
- Test for disconnect if peer sent invalid packets


## Development

## Features
- Implement App::crash() functionality. And allow user to tap 5 times to the top right corner to generate test crash

- Android: Bluetooth. For Android 12 do not request user permission to enable bluetooth
  but just show alert requesting the user to enable bluetooth. requesting intent crashes

= iOS: use xcodebuild -showBuildSettings, extract all the necessary variables and pass them
  to Pods/FirebaseCrashlytics/run
  Write scripts that automatically generates it
  
- Linux: read ./config/user-dirs.dirs for Documents directory

- Dispatch:
  - Implement "syncMain". Stop the execution. Run the task in main thread. Continue

- Implement M::garble(key), M::ungarble(), M::saveGarbledFile(), M::loadGarbledFile();
- Implement GarbledAsset class. Encrypted yaml file.

- Implement Vulkan for Windows/Android
- for mangl implement something like "value_or" where "or" is some dummy prim. akin std::optional


### System
- Add ability to generate pattern for the prims. Have some callback to be called on rendergrid
- Native GUI. Rework.
- Implement timers:
  timestamp, atomicTimestamp, activeTimestamp (when the app wasn't paused) atomicActiveTimestamp

### Audio
- Music: Implement the music system in such a way that if the user disabled it in the settings then 
  the music subsystem doesn't even get initialized. To save resources.
  Implement some sort of "pending load" and actually initialize when the user requests first play.

- Implement same with sounds effects.

- OBOE sounds library for Android


### Scene
- Think about implementing generalized "Prim" or "Widget" scene element
- Implement native prompt dialogs (especially for Windows as it doesn't have native one, write some dialog code)
- Investigate packing for Prim and optimize for 64 bit architecutre. Minimize the size. Pack bools, etc.
  * maybe pack all the flags into one uint_t, save memory

### Widgets
- Make all widgets update pending changes on timer. Set the flag "pending update".
- Have some mechanism to handle widget creation from within widget callbacks.
  Schedule the tasks or something.
- Implement "contentsScale" flag for all widgets. During animations this can be set and all the contents will be scaled.

- PanelWidget: Implement. Works similar to dialog, but without default title and buttons.
  Maybe derive dialog from panel in the future. Panels are for endgame screens, etc.
- SelectPopup: Create
- PathWidget: Implement - composition of Bezier sprites
- Scroll Widget: 
  - Add scroll indicators for not paged scroll. 
  - Rework paged indicators.
  - Implement horizontal direction.
- Label:
  - Implement create native widget for Label widget
- Checkbox: 
  - add label


### Effects
- ChainEffect: Implement. The empty effect will just trigger end. Can be used as timer
- newDisappearEffect


## Scripts


### Resources
- Have automatically windows .rc files generated for app Icon.
  - Have app Icon automatically generated from png image


## Samples
- Add ./mangl/config file and provide signing identity for sample projects


## Ideas

- Implement Swift like wrappers. For example LabelOpt which will contain Label * ,
  which if 0 will ignore all the commands and return nullptr, false, 0 on any request and self reference if pointer is requested.
  This could be implemented by having the widget to point to some dummy widget which does nothing.

- Particle. Add particle type besides Prim type. Maybe Prim can point to particles array.
  Particles are lightweight prims. They can share the same texture. Have own color, rect.

## Tools
- Texture Maker:
  - BUG. DFS method causes small problem for chessdash
  - Detect duplicate textures
  - Add proper depenendecies for the texture fonts.

