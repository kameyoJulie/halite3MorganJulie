#include <mshtml.h>
#include "game_map.hpp"
#include "input.hpp"

hlt::MapCell *hlt::GameMap::at(const hlt::Position &position) {
    Position normalized = normalize(position);
    return &cells[normalized.y][normalized.x];
}

hlt::MapCell *hlt::GameMap::at(const hlt::Entity &entity) {
    return at(entity.position);
}

hlt::MapCell *hlt::GameMap::at(const hlt::Entity *entity) {
    return at(entity->position);
}

hlt::MapCell *hlt::GameMap::at(const shared_ptr<Entity> &entity) {
    return at(entity->position);
}

int hlt::GameMap::calculate_distance(const hlt::Position &source, const hlt::Position &target) {
    const auto& normalized_source = normalize(source);
    const auto& normalized_target = normalize(target);

    const int dx = std::abs(normalized_source.x - normalized_target.x);
    const int dy = std::abs(normalized_source.y - normalized_target.y);

    const int toroidal_dx = min(dx, width - dx);
    const int toroidal_dy = min(dy, height - dy);

    return toroidal_dx + toroidal_dy;
}

hlt::Position hlt::GameMap::normalize(const hlt::Position &position) {
    const int x = ((position.x % width) + width) % width;
    const int y = ((position.y % height) + height) % height;
    return { x, y };
}

vector<hlt::Direction> hlt::GameMap::get_unsafe_moves(const hlt::Position &source, const hlt::Position &destination) {
    const auto& normalized_source = normalize(source);
    const auto& normalized_destination = normalize(destination);

    const int dx = std::abs(normalized_source.x - normalized_destination.x);
    const int dy = std::abs(normalized_source.y - normalized_destination.y);
    const int wrapped_dx = width - dx;
    const int wrapped_dy = height - dy;

    std::vector<Direction> possible_moves;

    if (normalized_source.x < normalized_destination.x) {
        possible_moves.push_back(dx > wrapped_dx ? Direction::WEST : Direction::EAST);
    } else if (normalized_source.x > normalized_destination.x) {
        possible_moves.push_back(dx < wrapped_dx ? Direction::WEST : Direction::EAST);
    }

    if (normalized_source.y < normalized_destination.y) {
        possible_moves.push_back(dy > wrapped_dy ? Direction::NORTH : Direction::SOUTH);
    } else if (normalized_source.y > normalized_destination.y) {
        possible_moves.push_back(dy < wrapped_dy ? Direction::NORTH : Direction::SOUTH);
    }

    return possible_moves;
}

hlt::Direction hlt::GameMap::naive_navigate(shared_ptr<Ship> ship, const hlt::Position &destination, const hlt::Position &shipyard) {
    // get_unsafe_moves normalizes for us
    for (auto direction : get_unsafe_moves(ship->position, destination)) {
        Position target_pos = ship->position.directional_offset(direction);
        if (!at(target_pos)->is_occupied()) {
            at(target_pos)->mark_unsafe(ship);
            if (at(destination)->halite > 50 || destination == shipyard) {
                return direction;
            }
        }
    }

    return Direction::STILL;
}

void hlt::GameMap::_update() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x].ship.reset();
        }
    }

    int update_count;
    hlt::get_sstream() >> update_count;

    for (int i = 0; i < update_count; ++i) {
        int x;
        int y;
        int halite;
        hlt::get_sstream() >> x >> y >> halite;
        cells[y][x].halite = halite;
    }
}

std::unique_ptr<hlt::GameMap> hlt::GameMap::_generate() {
    std::unique_ptr<hlt::GameMap> map = std::make_unique<GameMap>();

    hlt::get_sstream() >> map->width >> map->height;

    map->cells.resize((size_t)map->height);
    for (int y = 0; y < map->height; ++y) {
        auto in = hlt::get_sstream();

        map->cells[y].reserve((size_t)map->width);
        for (int x = 0; x < map->width; ++x) {
            hlt::Halite halite;
            in >> halite;

            map->cells[y].push_back(MapCell(x, y, halite));
        }
    }

    return map;
}

std::vector<hlt::MapCell> hlt::GameMap::_scan() {
    std::vector<MapCell> highHaliteCells;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (cells[x][y].halite >= 500) {
                highHaliteCells.push_back(cells[x][y]);
            }
        }
    }
    return highHaliteCells;
}
