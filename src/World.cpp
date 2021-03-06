#include "World.h"

#include "Renderer.h"

bool is_in_screen(const Camera2D& cam, const Rectangle& rect) {
    return rect.x - cam.target.x + cam.offset.x < -rect.width ||
           rect.x - cam.target.x + cam.offset.x > WIDTH ||
           rect.y - cam.target.y + cam.offset.y < -rect.height ||
           rect.y - cam.target.y + cam.offset.y > HEIGHT;
}

World::World() {
    tileset = LoadTexture("assets/Tileset.png");
    set_width = tileset.width / TILE_PIXEL_SIZE;
    set_height = tileset.height / TILE_PIXEL_SIZE;
}

World::World(std::string tileset_file, std::string level_file) {
    tileset = LoadTexture(tileset_file.c_str());
    set_width = tileset.width / TILE_PIXEL_SIZE;
    set_height = tileset.height / TILE_PIXEL_SIZE;

    load_level(level_file);
}

World::~World() { UnloadTexture(tileset); }

void World::update(float delta) {}

void World::render() {
    for (int y = 0; y < WORLD_SIZE; y++) {
        for (int x = 0; x < WORLD_SIZE; x++) {
            Vector2 worldPos{(float)x * TILE_SIZE, (float)y * TILE_SIZE};

            if (Renderer::is_in_screen(
                    Rectangle{worldPos.x, worldPos.y, TILE_SIZE, TILE_SIZE}))
                continue;

            int32_t id = level[y][x].id;

            Rectangle bbox{worldPos.x, worldPos.y, TILE_SIZE, TILE_SIZE};

            Renderer::render_tile(bbox, id);
        }
    }
}

bool is_equal(Color a, Color b) {
    return a.a == b.a && a.r == b.r && a.g == b.g && a.b == b.b;
}

void World::load_level(std::string filename) {
    Image data = LoadImage(filename.c_str());
    Color* colors = LoadImageColors(data);

    for (int y = 0; y < data.height; y++) {
        for (int x = 0; x < data.width; x++) {
            Color c = colors[y * data.width + x];

            uint32_t tile = 256 ^ 3 * c.a + 256 ^ 2 * c.r + 256 ^ 1 * c.g + c.b;

            // Example
            if (is_equal(c, RED)) {
                level[y][x] = {tile, FOREGROUND};
            }
        }
    }
}
