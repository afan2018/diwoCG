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
- Hit `R` while selection to zoom to fit
- FPS is written to console output, once per second
- Hit `K` (globally) to enable/disable lighting
- Hit `L` while selecting a light source (which will appear as a bright white sphere) to enable/disable the light
- Use arrow keys and ',' '.' for moving a selected object
- Use 'U' 'I' 'O' to rotate a selected object around x, y and z axis
- Be careful! The scene will be totally black if you disable all lights!
