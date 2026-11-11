# Compositor validation

These checks are pending; automated tests do not establish visual correctness in
a running KWin session. Use a nested/disposable session first.

1. Confirm Static Blur DX appears separately in Desktop Effects and its KCM opens.
   Enable only this blur effect. Reopen settings and confirm saved source/path.
2. Use a distinctive image with labeled quadrants. Place a translucent Konsole
   over a moving video and then other windows. Custom source must remain fixed.
   Move/resize/maximize/restore the window; sample locations must remain screen
   anchored, without stale stripes or inverted imagery.
3. Test requested blur and forced matching, decorations, menus/docks when enabled,
   rounded corners, noise, opacity, color adjustments, both refraction modes and
   transitions between all three source modes.
4. Move windows between and across outputs at 100%, 125%, 150% and 200% scaling,
   including an output left/above the primary (negative coordinates), portrait
   rotation, resolution changes, hotplug and unplug. Look for seams, black/stale
   patches and leaking corners. The image fills each monitor independently.
5. Check SDR/HDR outputs and changes in output color settings. Watch that source
   conversion is refreshed and image brightness/orientation remain correct.
6. In Wallpaper mode, change wallpaper, recreate/restart Plasma desktop, switch
   activities and hotplug outputs. Confirm fresh desktop capture and recovery
   while no desktop surface exists.
7. Choose missing and corrupt files, replace an image then Apply, and simulate
   GL allocation failure under a debugger. Confirm no crash, no uninitialized
   texture draw and no substitution of live background pixels into blur.
8. Watch `kwin_effect_static_blur_dx.*` logging and GPU/frame timing. Static source
   must not decode/upload on every frame. Disable/unload/re-enable repeatedly to
   check resource cleanup. Confirm the original DX effect still works afterward.
