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

- [ ] Physics
  - [x] Car movement
  - [ ] Collisions
    - [x] Car-Walls
    - [ ] Car-Shadows
- [ ] Circuit
  - [x] Walls
  - [x] Checkpoints
  - [ ] Expansion
- [ ] Particle system
  - [x] Smoke/dirt
  - [ ] Tire tracks
- [ ] Graphics
  - [ ] Car
  - [ ] Circuits
- [ ] Sounds
  - [ ] Car engine
  - [ ] Car break
  - [ ] Collision
- [ ] Shadows
  - [ ] Save movement
  - [ ] Replay

# Extra ideas

- Realistic car movement
- Drift
- Nitro
- Rollback: go back in time x seconds
- Power ups? attacks?
- Car destruction
- Track grass
- Multiplayer battleroyale
