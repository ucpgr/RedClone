# Tile Assets (first pass)
- Tilesheets: `assets/tiles/*.jpg`
- Metadata: `assets/tiles/*.lua` (same basename as sheet)
- Metadata returns a Lua table with `sheet`, `tile_width`, `tile_height`, and `tiles`.

Metadata tile entry required fields:
- `name`, `terrain`, `kind`
- `x`, `y`, `w`, `h` (source rectangle in the tilesheet)
- `height` for `flat`
- `low_height`, `high_height`, and `direction` for `ramp`

Example:
```lua
return {
  sheet = "temperate.jpg",
  tile_width = 64,
  tile_height = 32,
  tiles = {
    { name = "grass_flat", terrain = "grass", kind = "flat", height = 0, x = 0, y = 0, w = 64, h = 32 },
    { name = "grass_ramp_north", terrain = "grass", kind = "ramp", direction = "north", low_height = 0, high_height = 1, x = 64, y = 0, w = 64, h = 32 }
  }
}
```

Naming:
- Flat: `<terrain>_flat`
- Ramps: `<terrain>_ramp_<direction>`
- Ramp corners: `<terrain>_ramp_corner_<corner>`
- Future: `<terrain>_edge_<direction>`, `<terrain>_cliff_<direction>`

Directions:
`north`, `east`, `south`, `west`, `northeast`, `southeast`, `southwest`, `northwest`.

Connector rules:
- Edge pairs: N↔S, E↔W.
- Connection currently requires equal edge height and matching terrain.
- Primitive map rendering remains fallback if metadata/assets are unavailable.
