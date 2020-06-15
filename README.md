# diwoCG

## Build

### MinGW

```bash
cmake . -G "MinGW Makefiles"
mingw32-make
main.exe
```

Note that the program uses [`nativefiledialog`](https://github.com/mlabbe/nativefiledialog), a C library that enables cross-platform file selection with native dialogs. The building process is, due to its cross-platform yet binary nature, somehow messy, taking advantage of prebuilt statically-linked libraries, located in the [`lib/`](lib/) directory.

These libraries are prebuilt with instructions from the `nfd` repo, as well as various heuristics and trail-and-error, so it's literally impossible to provide an automatic build script to cover all possible platforms and build toolchains.

Currently, the supported platforms are `MacOS / clang`, `Windows / MinGW` and `Windows / Visual Studio`. If you're building this project under an unlisted platform, you'll have to figure out the build instructions yourself.

So, back to building process.

[`lib/nfd.lib`](lib/nfd.lib) is build with `MinGW` [as instructed](https://github.com/mlabbe/nativefiledialog/blob/master/docs/build.md), however the original repo uses [`sprintf_s`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sprintf-s-sprintf-s-l-swprintf-s-swprintf-s-l?view=vs-2019), which is an Windows-only function, and internally uses `BufferOverflow.lib` from Windows SDK.

`MinGW`, as a port of the GNU toolchain to Windows, contains a set of modified version of Windows header files as well as static libraries that work well on most cases, but not *ALL* cases. As of `nfd`, `sprintf_s` requires `BufferOverflow.lib` which is not shipped along with `MinGW`, so we have to get it out of Windows SDK manually.

The problem here is that the libraries in Windows SDK depends on other libraries, like `ntdll.lib`, which `MinGW` already has its own version, so if you try to link `BufferOverflow.lib`, the `-lntdll` option will instruct `g++` to look for the symbols in `libntdll.a`, a file provided by `MinGW`. However, `BufferOverflow.lib` expects `ntdll.lib` from Windows SDK, which is not compatible with `libntdll.a` - and then came the infamous `undefined reference`.

Now you might want to replace `libntdll.a` with `ntdll.lib`, but even that won't work, because `ntdll.lib` - clearly compiled with the MSVC compiler - calls `__aullshr`, which is an internal function available in MSVC only. You'll get another `undefined reference`, and this time there's nothing to replace.

So our ultimate solution is to modify `nfd` source and change `sprintf_s` to `snprintf`, thus eliminating usage of `BufferOverflow.lib` and further problems. There's one problem left: `undefined reference to _imp__SHCreateItemFromParsingName@16`, which is [located in `shell32.lib`](https://docs.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-shcreateitemfromparsingname), but not in the `MinGW` `libshell32.a`. So we removed `libshell32.a` and replaced it with `shell32.lib`, then everything worked.

Ugly as it might show, it's already the simplest solution we can find, and hopefully it'll work for you. If not, well, have fun dealing with compilers!

## Usage

- Click and drag to rotate view
- `WASD` to navigate, `Z` for down, `C` for up
- Hover object (you will see a white frame), and hit `Space` to select it, or hit `Space` without selecting anything to cancel selection
- Hold `G` while selection to move the object around (keep it unchanged in your view)
- Hold `F` while selection to orbit an orbit (`G` will not work under this case)
- Hold `P` and drag to take screenshot (of a section of the screen)
- Hit `R` while selection to zoom to fit
- FPS is written to console output, once per second
- Hit `K` (globally) to enable/disable lighting
- Hit `L` while selecting a light source (which will appear as a bright white sphere) to enable/disable the light
- Use arrow keys and `,` `.` for moving a selected object
- Use `U` `I` `O` to rotate a selected object around x, y and z axis
- Hit `T` while selection to open a dialog and choose an image as texture. Hit again to clear it.
