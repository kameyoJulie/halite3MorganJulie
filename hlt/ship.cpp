#include "ship.hpp"
#include "input.hpp"

hlt::Ship::Ship(PlayerId player_id, EntityId ship_id, int x, int y, Halite halite) :
        Entity(player_id, ship_id, x, y),
        halite(halite)
{}

bool hlt::Ship::is_full() const {
    return halite >= constants::MAX_HALITE;
}

hlt::Command hlt::Ship::make_dropoff() const {
    return hlt::command::transform_ship_into_dropoff_site(id);
}

hlt::Command hlt::Ship::move(hlt::Direction direction) const {
    return hlt::command::move(id, direction);
}

hlt::Command hlt::Ship::stay_still() const {
    return hlt::command::move(id, Direction::STILL);
}

std::shared_ptr<hlt::Ship> hlt::Ship::_generate(hlt::PlayerId player_id) {
    hlt::EntityId ship_id;
    int x;
    int y;
    hlt::Halite halite;
    hlt::get_sstream() >> ship_id >> x >> y >> halite;

    return std::make_shared<hlt::Ship>(player_id, ship_id, x, y, halite);
}