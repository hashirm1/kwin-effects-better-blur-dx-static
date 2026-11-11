# Static Blur DX

A separate KWin effect derived from Better Blur DX 2.6.0, adding a fixed custom
image as the blur source. Initial target: Fedora 44, **KWin Wayland 6.8.2**.

The Performance tab's **Blur Source Mode** offers:

- **Live background** (default): regular Better Blur DX behavior.
- **Desktop wallpaper [experimental]**: render the Plasma desktop without other
  windows. Desktop icons and widgets can appear in this source.
- **Custom image**: choose a local image, then Apply. It fills each output with a
  centered aspect-preserving crop. Moving a window reveals another portion of
  that screen-anchored image; windows underneath never become the blur source.

Existing DX window matching, requested blur, decorations, rounding, noise,
brightness/contrast/saturation and refraction remain available. Applications still
need transparency to show the effect. Window opacity retains its DX behavior.
Transparent pixels in the image become black. Embedded image profiles are
converted to sRGB and then to the render target's color description.
Missing/unreadable images skip the blur draw, with a diagnostic in the KWin log.
Apply reloads the file; there is no file watcher. X11 and older KWin versions are
not supported by this initial fork.

## Build and test

Install the Fedora development packages for CMake, GCC C++, extra-cmake-modules,
Qt6 (base, declarative and tools), KF6 Config, ColorScheme, ConfigWidgets,
CoreAddons, I18n, KCMUtils, WidgetsAddons and WindowSystem, KDecoration3, libepoxy,
X11/XCB, and **kwin-devel matching the running kwin package**. The local build was
verified with kwin/kwin-devel 6.8.2-3.fc44.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build -j4
ctest --test-dir build --output-on-failure
```

The shader test uses a separate surfaceless EGL software context. If EGL is
unavailable it reports a skipped test. It compiles all active shader pairs and
reads pixels back through the actual custom-image conversion helper.

## Install and use

```sh
sudo cmake --install build
```

Log out and back in if KWin has already scanned its effect plugins. Open System
Settings → Desktop Effects. Disable **Blur**, **Better Blur**, and **Better Blur DX**,
then enable **Static Blur DX**. Open its settings, select Custom image on the
Performance tab, choose the file, and Apply. Only one blur effect should be active.

This installs `static_blur_dx.so` and `kwin_static_blur_dx_config.so` alongside DX;
it uses `Effect-static-blur-dx` in `kwinrc`. Existing DX settings are not imported.
The effect starts disabled and Live background remains the default source.

To uninstall, disable Static Blur DX first, then remove the two installed files
listed in `build/install_manifest.txt` (the plugin and KCM). Re-enable your previous
blur effect. Rebuild after KWin upgrades; the current CMake version guard must be
updated only after checking the new KWin API.

## Validation status

Build, image decoding/cropping, configuration isolation/persistence, shader
compilation and sRGB GPU conversion/orientation are covered locally. Installation
has been staged without changing the running desktop. Actual compositor loading,
window interactions, HDR, mixed-scale/rotated multi-monitor output and hotplug
still require the manual checks in [docs/validation.md](docs/validation.md).

See [PROVENANCE.md](PROVENANCE.md) for source details and limitations. GPL-2.0-or-later;
original KDE and Better Blur/DX authorship is retained in source files.
