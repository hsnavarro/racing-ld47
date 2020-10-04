# atlas.map

Stores all information to be able to generate the circuit from file:
- Possible exits from each entry (bitmask: u 1, d 2, r 4, l 8)
- Walls (wall from point A -> point B only blocks cars in the "right" semiplane)
- Checkpoint

Walls and checkpoints values must be between 0 and 108

## Original tile

```
[tile_type]
up_exits_mask down_exits_mask right_exits_mask left_exits_mask
wall_count
wall_a_x wall_a_y wall_b_x wall_b_y
...
has_checkpoint
checkpoint_a_x checkpoint_a_y checkpoint_b_x checkpoint_b_y
```

## Clone tile

Flip mask: h 1, v 2

```
[tile_type]
c original_type flip_mask
```

## Merge tile

Merge tiles don't have checkpoint. There would be way extra work to properly
treat the checkpoint order if we allowed checkpoints on merges.

```
[tile_type]
m type_a type_b
```
