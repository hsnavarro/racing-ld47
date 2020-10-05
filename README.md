## Racing - Ludum Dare 47

A race game for Ludum Dare 47

# Idea

Time-trial racing game. You must complete the lap in the time limit, if you
didn't you just loop until you can.

## Laps

As soon as you complete the lap, the track expands (keep the same as previous
with an extra section) and you have to complete the new, improved track.
Also, a shadow appears.

The track expands 10 times (we may change this), and you have to complete all to
win.

## Shadows

A shadow appears doing the same movement you did on you successful lap
(a shadow). All these shadows have collision with your car.

Shadows appear when you cross the first checkpoint after a successful lap, to
avoid an instant collision.

When a shadow completes a task it vanishes and is recreated (this avoids the
problem of interpolating the end and beginning of the lap, since they can begin
and end in different positions)

# Todo

- [x] Physics
  - [x] Car movement
  - [x] Collisions
    - [x] Car-Walls
    - [x] Car-Shadows (Naum)

- [x] Circuit
  - [x] Walls
  - [x] Checkpoints
  - [x] Expansion

- [x] Particle system
  - [x] Smoke/dirt
  - [x] Tire tracks

- [x] Shadows (Navarro)
  - [x] Save movement
  - [x] Replay

- [x] Camera
  - [x] Follow player car

- [ ] Graphics
  - [ ] Car
  - [ ] Circuits

- [ ] Sounds
  - [ ] Car engine
  - [ ] Car break
  - [ ] Collision

# Extra ideas

- [x] Minimap
- [x] Drift
  - [x] Burst?

- Nitro
- Rollback: go back in time x seconds
- Car destruction
- Multiplayer battleroyale
- x Power ups? attacks?
- x Track grass
- x Realistic car movement
