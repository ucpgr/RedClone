# Tile Assets

## Layout
- Tile metadata lives in `assets/tiles/*.lua`.
- Each metadata file references a JPG tilesheet via the `sheet` field.
- The `sheet` path is resolved relative to its Lua metadata file.

## Metadata format
```lua
return {
  sheet = 'temperate.jpg',
  tile_width = 64,
  tile_height = 32,
  tiles = {
    { name='grass_raised_067', terrain='grass', kind='flat', height=0, x=0, y=0, w=64, h=32 }
  }
}
```

Validation rules:
- `tiles` must be non-empty.
- `name`, `terrain`, and `sheet` must be non-empty.
- Tile names must be unique per metadata file.
- `x`/`y` must be non-negative.
- `w`/`h` must be positive.

## Current default tile
The test landscape currently renders every terrain tile using `grass_raised_067`.

## Fallback behaviour
If metadata or JPG loading fails, the game continues running and falls back to primitive diamond tile rendering.
Selection highlight is always rendered as a diamond outline on top.

## Future work
- Per-terrain tile selection.
- Height-aware map generation and tile picking.
