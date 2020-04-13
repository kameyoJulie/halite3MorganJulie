// ship manager

#pragma once

#include "entity.hpp"
#include "constants.hpp"
#include "command.hpp"

#include <memory>

namespace hlt {
    struct Ship : Entity {
        Halite halite;

        Ship(PlayerId player_id, EntityId ship_id, int x, int y, Halite halite);

        bool is_full() const;

        Command make_dropoff() const;
        Command move(Direction direction) const;
        Command stay_still() const;

        static std::shared_ptr<Ship> _generate(PlayerId player_id);
    };
}
