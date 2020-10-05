#include "circuit.hpp"

#include <cstdio>
#include <tuple>
#include <queue>

#include "game.hpp"
#include "types.hpp"

sf::Texture Circuit::textureAtlas {};
std::vector<Circuit::TileType> Circuit::tileTypes {};

Circuit::Circuit(Game& _game) : game { _game }
{}

bool Circuit::loadFromFile(const std::string& path) {
  FILE* file;

#ifdef _MSC_VER
  auto err = fopen_s(&file, path.c_str(), "r");
  if (!file) {

    #pragma warning(push)
    #pragma warning(disable:4996)
    printf("Could not load circuit: %s. Error: %s\n", path.c_str(), strerror(err));
    #pragma warning(pop)

    return false;
  }
#else
  file = fopen(path.c_str(), "r");
  if (!file) {
    printf("Could not load circuit: %s\n", path.c_str());
    return false;
  }
#endif

  defer(fclose(file));

  size_t minTime;
  fscanf_s(file, "%zu", &minTime);
  lapTimeLimit = static_cast<float>(minTime);


  size_t h, w;
  fscanf_s(file, "%zu %zu", &h, &w);

  std::vector<std::vector<Tile>> newTiles;

  newTiles.resize(h);
  for (size_t i = 0; i < h; i++)
    newTiles[i].resize(w);

  bool initialFound = false;
  for (size_t i = 0; i < h; i++) {
    size_t type;
    for (size_t j = 0; j < w; j++) {
      fscanf_s(file, "%zu", &type);

      if (feof(file)) {
        printf("Missing data from circuit: %s\n", path.c_str());
        return false;
      }

      newTiles[i][j].type = type;

      if (type == 1) { // initial
        if (initialFound) {
          printf("Multiple initial positions at : %s\n", path.c_str());
          return false;
        }
        initialFound = true;

        initialPosY = i;
        initialPosX = j;
      }
    }
  }

  tiles = std::move(newTiles);

  // Add checkpoints and walls by iterating in circuit graph
  walls.clear();
  checkpoints.clear();

  std::vector<std::vector<std::vector<u8>>> bfsVis (
    tiles.size(),
    std::vector<std::vector<u8>>(
      tiles[0].size(),
      std::vector<u8>(
        16,
        0
      )
    )
  );

  std::queue<std::tuple<int, int, TileType::Direction>> bfsQueue;
  bfsQueue.emplace(
    static_cast<int>(initialPosX),
    static_cast<int>(initialPosY)-1,
    TileType::DIR_DOWN
  );

  while (!bfsQueue.empty()) {
    auto [pos_x, pos_y, enter] = bfsQueue.front();
    bfsQueue.pop();

    const auto type = tiles[pos_y][pos_x].type;
    const auto& tileType = tileTypes[type];
    //printf("%d %d %u: ", pos_x, pos_y, enter);

    const sf::Vector2f shift = sf::Vector2f { static_cast<float>(pos_x), static_cast<float>(pos_y) } *
                               static_cast<float>(CIRCUIT_TILE_SIZE);

    // Add walls
    for (const auto& wall : tileType.walls)
      walls.push_back({ wall[0] + shift, wall[1] + shift });

    // Add checkpoint
    if (tileType.hasCheckpoint) {
      auto& checkpoint = tileType.checkpoint;
      checkpoints.push_back({ checkpoint[0] + shift, checkpoint[1] + shift });
    }

    auto tryMove =
      [enter, &tileType, &bfsVis, &bfsQueue, this]
      (int new_x, int new_y, TileType::Direction exit_dir) {

        auto flipDir = [](u8 dir) {
          return ((dir & 0x5) << 1) | ((dir & 0xa) >> 1);
        };

        auto validPos = [this](int x, int y) {
          return !(x < 0 or y < 0 or x >= (int) tiles[0].size() or y >= (int) tiles.size());
        };

        if (tileType.exits[enter] & exit_dir) {
          const auto new_enter = static_cast<TileType::Direction>(flipDir(static_cast<u8>(exit_dir)));
          if (validPos(new_x, new_y) and
              !bfsVis[new_y][new_x][new_enter]) {

            //printf("(%d %d %u) ", new_x, new_y, new_enter);
            bfsQueue.emplace(new_x, new_y, new_enter);

            u8 all_enters = static_cast<u8>(new_enter | tileTypes[tiles[new_y][new_x].type].exits[new_enter]);
            if (all_enters & 1) bfsVis[new_y][new_x][1] = 1;
            if (all_enters & 2) bfsVis[new_y][new_x][2] = 1;
            if (all_enters & 4) bfsVis[new_y][new_x][4] = 1;
            if (all_enters & 8) bfsVis[new_y][new_x][8] = 1;
          }
        }
      }
    ;

    tryMove(pos_x, pos_y-1, TileType::DIR_UP);
    tryMove(pos_x, pos_y+1, TileType::DIR_DOWN);
    tryMove(pos_x+1, pos_y, TileType::DIR_RIGHT);
    tryMove(pos_x-1, pos_y, TileType::DIR_LEFT);

    //printf("\n");
  }

  return true;
}

void Circuit::setLapTimeLimit(float timeLimit) {
  lapTimeLimit = timeLimit;
}

void Circuit::update(float) {
  if (currentCheckpoint >= checkpoints.size())
    return;

  if (carIntersectsLine(game.car, checkpoints[currentCheckpoint])) {
    currentCheckpoint++;
    if(currentCheckpoint == 1) {
      for (auto& ghost : game.newGhosts) {
        ghost.activateRendering();
        game.ghosts.push_back(std::move(ghost));
      }
      game.newGhosts.clear();
    }

    if (currentCheckpoint == checkpoints.size()) {
      game.completeLap();
    }
  }
}

void Circuit::render(const sf::View& view) const {
  game.window.setView(view);

  // Todo(naum): save as a texture
  for (size_t i = 0; i < tiles.size(); i++) {
    const auto& row = tiles[i];
    for (size_t j = 0; j < row.size(); j++) {
      const auto& tileType = row[j].type;

      auto& tile = tileTypes[tileType];
      tile.sprite.setPosition(
        static_cast<float>(j * CIRCUIT_TILE_SIZE),
        static_cast<float>(i * CIRCUIT_TILE_SIZE)
      );

      game.window.draw(tile.sprite);
    }
  }

  // render checkpoints
  for (size_t i = 0; i < checkpoints.size(); i++) {
    const auto& checkpoint = checkpoints[i];

    sf::Color color { 0, 0, 0, 192 };
    if (currentCheckpoint >  i) color = sf::Color::Green;
    if (currentCheckpoint == i) color = sf::Color::Blue;

    sf::Vertex verts[] = {
      { checkpoint[0], color },
      { checkpoint[1], color }
    };

    game.window.draw(verts, 2, sf::Lines);
  }

  /*
  // DEBUG
  // render walls
  for (auto wall : walls) {
    sf::Vertex verts[] = { { wall[0], sf::Color::White }, { wall[1], sf::Color::White } };
    game.window.draw(verts, 2, sf::Lines);
  }

  // Render grid
  sf::Vertex verts[2];
  for (size_t i = 0; i <= tiles.size(); i++) {
    const auto y = static_cast<float>(i * CIRCUIT_TILE_SIZE);
    verts[0] = sf::Vertex({ 0.0f, y });
    verts[1] = sf::Vertex({ static_cast<float>(CIRCUIT_TILE_SIZE * tiles[0].size()), y });
    game.window.draw(verts, 2, sf::Lines);
  }

  for (size_t i = 0; i <= tiles[0].size(); i++) {
    const auto x = static_cast<float>(i * CIRCUIT_TILE_SIZE);
    verts[0] = sf::Vertex({ x, 0.0f });
    verts[1] = sf::Vertex({ x, static_cast<float>(CIRCUIT_TILE_SIZE * tiles.size()) });
    game.window.draw(verts, 2, sf::Lines);
  }

  // Render atlas
  // Todo(naum): save as a texture
  for (size_t i = 0; i < tileTypes.size(); i++) {
    auto& tile = tileTypes[i];
    tile.sprite.setPosition(
      static_cast<float>((i%10) * CIRCUIT_TILE_SIZE),
      static_cast<float>((i/10) * CIRCUIT_TILE_SIZE)
    );

    game.window.draw(tile.sprite);
  }
  */
}


void Circuit::resetCheckpoints() {
  currentCheckpoint = 0;
}

void Circuit::startRace() {
  game.car.setPosition(
    {
      static_cast<float>(CIRCUIT_TILE_SIZE * (initialPosX + 0.5f)),
      static_cast<float>(CIRCUIT_TILE_SIZE * (initialPosY + 0.8f)),
    }
  );
  resetCheckpoints();
}

void Circuit::loadAtlas() {
  if(textureAtlas.loadFromFile("assets/gfx/circuit-atlas-border.png")) {
    textureAtlas.setSmooth(true);
  }

  const auto textureSize = textureAtlas.getSize();
  const auto width  = textureSize.x / (CIRCUIT_TILE_SIZE+2);
  const auto height = textureSize.y / (CIRCUIT_TILE_SIZE+2);

  // Todo(naum): add all this info to a text file and load
  tileTypes.resize(width * height);
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      tileTypes[y * width + x].setTilePosition(y, x);
      // Todo(naum): get walls
      // Todo(naum): get checkpoint
      // Todo(naum): get enter/exit directions
    }
  }

  // Read atlas map
  FILE* file;

#ifdef _MSC_VER
  auto err = fopen_s(&file, "assets/circuits/atlas.map", "r");
  if (!file) {

    #pragma warning(push)
    #pragma warning(disable:4996)
    printf("Could not load atlas map. Error: %s\n", strerror(err));
    #pragma warning(pop)

    return;
  }
#else
  file = fopen("assets/circuits/atlas.map", "r");
  if (!file) {
    printf("Could not load atlas map\n");
    return;
  }
#endif

  defer(fclose(file));

  /*
  auto printTileType = [] (size_t type) {
    const auto& t = tileTypes[type];
    printf("type: %zu\n", type);
    printf("%d %d %d %d\n", t.exits[1], t.exits[2], t.exits[4], t.exits[8]);
    printf("wallcount %zu\n", t.walls.size());
    for (const auto& wall : t.walls)
      printf("wall: %.0f %.0f %.0f %.0f\n",
               wall[0].x, wall[0].y,
               wall[1].x, wall[1].y);
    printf("has checkpoint %d\n", static_cast<int>(t.hasCheckpoint));
    printf("checkpoint: %.0f %.0f %.0f %.0f\n",
             t.checkpoint[0].x, t.checkpoint[0].y,
             t.checkpoint[1].x, t.checkpoint[1].y);
    printf("\n");
  };
  */

  // Read originals
  while (true) {
    size_t type;
    fscanf_s(file, " [%zu]", &type);
    if (feof(file))
      break;

    if (type >= width * height) {
      printf("Invalid tile type in atlas map: %zu\n", type);
      continue;
    }

    auto& tileType = tileTypes[type];

    // Check if it's clone or merge
    if (size_t origType, flip; fscanf_s(file, " c %zu %zu", &origType, &flip) == 2)
      continue;

    if (size_t typeA, typeB; fscanf_s(file, " m %zu %zu", &typeA, &typeB) == 2)
      continue;

    // Not clone nor merge, so get everything

    // Get exits
    size_t upExits, downExits, rightExits, leftExits;
    fscanf_s(file,
             "%zu %zu %zu %zu",
             &upExits,
             &downExits,
             &rightExits,
             &leftExits);


    tileType.exits[1] = static_cast<u8>(upExits);
    tileType.exits[2] = static_cast<u8>(downExits);
    tileType.exits[4] = static_cast<u8>(rightExits);
    tileType.exits[8] = static_cast<u8>(leftExits);

    // Get walls
    std::vector<sf::Line> walls;
    size_t wallCount;
    fscanf_s(file, "%zu", &wallCount);


    for (size_t i = 0; i < wallCount; i++) {
      size_t a_x, a_y, b_x, b_y;
      fscanf_s(file,
               "%zu %zu %zu %zu",
               &a_x, &a_y,
               &b_x, &b_y);

      walls.push_back(
        {{
          { static_cast<float>(a_x), static_cast<float>(a_y) },
          { static_cast<float>(b_x), static_cast<float>(b_y) }
        }}
      );
    }

    tileType.walls = walls;

    // Get checkpoint
    size_t hasCheckpoint;
    fscanf_s(file, "%zu", &hasCheckpoint);
    tileType.hasCheckpoint = static_cast<bool>(hasCheckpoint);


    if (hasCheckpoint) {
      size_t a_x, a_y, b_x, b_y;
      fscanf_s(file,
               "%zu %zu %zu %zu",
               &a_x, &a_y,
               &b_x, &b_y);


      tileType.checkpoint =
        {{
          { static_cast<float>(a_x), static_cast<float>(a_y) },
          { static_cast<float>(b_x), static_cast<float>(b_y) }
        }}
      ;
    }
  }

  // Read clones
  rewind(file);
  while (true) {
    size_t type;

    while (fgetc(file) != '[' and !feof(file)) ;

    fscanf_s(file, "%zu]", &type);
    if (feof(file))
      break;

    auto& tileType = tileTypes[type];

    // Check if it's a clone
    size_t origType, flip;
    if (fscanf_s(file, " c %zu %zu", &origType, &flip) != 2)
      continue;

    // It's a clone!!!
    auto flipDir = [flip](u8 dir) {
      if (flip & 1)
        dir = ((dir & 0x4) << 1) | ((dir & 0x8) >> 1) | (dir & 0x3);
      if (flip & 2)
        dir = (dir & 0xc) | ((dir & 0x1) << 1) | ((dir & 0x2) >> 1);
      return dir;
    };

    auto flipExits = [flip](u8 exits) -> u8 {
      if (flip & 1)
        exits = ((exits & 0x4) << 1) | ((exits & 0x8) >> 1) | (exits & 0x3);
      if (flip & 2)
        exits = (exits & 0xc) | ((exits & 0x1) << 1) | ((exits & 0x2) >> 1);
      return exits;
    };

    auto flipLine = [flip](sf::Line line) {
      if (flip & 1) {
        line[0].x = 108.0f - line[0].x; line[1].x = 108.0f - line[1].x;
        std::swap(line[0], line[1]);
      }

      if (flip & 2) {
        line[0].y = 108.0f - line[0].y; line[1].y = 108.0f - line[1].y;
        std::swap(line[0], line[1]);
      }

      return line;
    };

    auto orig = tileTypes[origType];

    tileType.exits[flipDir(1)] = flipExits(orig.exits[1]);
    tileType.exits[flipDir(2)] = flipExits(orig.exits[2]);
    tileType.exits[flipDir(4)] = flipExits(orig.exits[4]);
    tileType.exits[flipDir(8)] = flipExits(orig.exits[8]);

    for (auto wall : orig.walls)
      tileType.walls.push_back(flipLine(wall));

    tileType.hasCheckpoint = orig.hasCheckpoint;
    if (tileType.hasCheckpoint)
      tileType.checkpoint = flipLine(orig.checkpoint);
  }

  // Read merges
  rewind(file);
  while (true) {
    size_t type;

    while (fgetc(file) != '[' and !feof(file)) ;

    fscanf_s(file, "%zu]", &type);
    if (feof(file))
      break;

    auto& tileType = tileTypes[type];
    // Check if it's a merge
    size_t typeA, typeB; 
    if (fscanf_s(file, " m %zu %zu", &typeA, &typeB) != 2)
      continue;

    const auto& tileTypeA = tileTypes[typeA];
    const auto& tileTypeB = tileTypes[typeB];

    tileType.exits[1] = tileTypeA.exits[1] | tileTypeB.exits[1];
    tileType.exits[2] = tileTypeA.exits[2] | tileTypeB.exits[2];
    tileType.exits[4] = tileTypeA.exits[4] | tileTypeB.exits[4];
    tileType.exits[8] = tileTypeA.exits[8] | tileTypeB.exits[8];

    // Maybe use back inserter
    for (auto wall : tileTypeA.walls)
      tileType.walls.push_back(wall);
    for (auto wall : tileTypeB.walls)
      tileType.walls.push_back(wall);

    // We cant merge checkpoints since it can break
    // the checkpoint order
    tileType.hasCheckpoint = false;
  }
}
