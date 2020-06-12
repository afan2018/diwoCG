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
- Hover object (you will see a white frame), and hold `G` to move it around, that is, it will be kept still in your view
- `Space` to zoom hovered object to view
- Hold `P` and drag to take screenshot (of a section of the screen)
- FPS is written to console output, once per second
