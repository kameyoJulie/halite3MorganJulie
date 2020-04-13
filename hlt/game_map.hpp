//game map controller used to locate all entities

#pragma once

#include "types.hpp"
#include "map_cell.hpp"

#include <vector>
using namespace std;
namespace hlt {
    class GameMap {
    private:
        int width;
        int height;
        std::vector<std::vector<MapCell>> cells;

    public:
        MapCell* at(const Position& position);
        MapCell* at(const Entity& entity);
        MapCell* at(const Entity* entity);
        MapCell* at(const std::shared_ptr<Entity>& entity);

        int calculate_distance(const Position& source, const Position& target);

        Position normalize(const Position& position);

        std::vector<Direction> get_unsafe_moves(const Position& source, const Position& destination);

        Direction naive_navigate(std::shared_ptr<Ship> ship, const Position& destination);

        void _update();
        static std::unique_ptr<GameMap> _generate();
        std::vector<hlt::MapCell> _scan();
    };
}
