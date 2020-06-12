# diwoCG

## Build

With `MinGW`:

```bash
cmake . -G "MinGW Makefiles"
mingw32-make
main.exe
```

## Usage

- Click and drag to rotate view
- `WASD` to navigate, `Z` for down, `C` for up
- Hover object (you will see a white frame), and hit `Space` to select it, or hit `Space` without selecting anything to cancel selection
- Hold `G` while selection to move the object around (keep it unchanged in your view)
- Hold `P` and drag to take screenshot (of a section of the screen)
- FPS is written to console output, once per second
