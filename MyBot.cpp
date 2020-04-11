#include "hlt/game.hpp"
#include "hlt/constants.hpp"
#include "hlt/log.hpp"

#include <random>
#include <ctime>
#include <sstream>

using namespace std;
using namespace hlt;

int main(int argc, char* argv[]) {
    unsigned int rng_seed;
    if (argc > 1) {
        rng_seed = static_cast<unsigned int>(stoul(argv[1]));
    } else {
        rng_seed = static_cast<unsigned int>(time(nullptr));
    }
    mt19937 rng(rng_seed);

    Game game;
    // At this point "game" variable is populated with initial map data.
    // This is a good place to do computationally expensive start-up pre-processing.
    // As soon as you call "ready" function below, the 2 second per turn timer will start.
    game.ready("MyCppBot");

    log::log("Successfully created bot! My Player ID is " + to_string(game.my_id) + ". Bot rng seed is " + to_string(rng_seed) + ".");
    vector<MapCell> scan(game.game_map->_scan());

    for (auto & i : scan) { // <=> for (int i = 0; i < scan.size(); ++i) {
        log::log("Map cell coord " + to_string(i.position.x) + ", " + to_string(i.position.y) + ", halite " + to_string(i.halite));
    }

    for (;;) {
        game.update_frame();
        shared_ptr<Player> me = game.me;
        unique_ptr<GameMap>& game_map = game.game_map;

        //Actions to realize per turn
        vector<Command> command_queue;

        for (const auto& ship_iterator : me->ships) {
            shared_ptr<Ship> ship = ship_iterator.second;
            if (ship->halite < 400) {
                command_queue.push_back(ship->move(game_map->naive_navigate(ship, scan[ship->id].position)));
            } else {
                command_queue.push_back(ship->move(game_map->naive_navigate(ship, me->shipyard->position)));
            }
        }

        //condition if the turn number is over 200 to create a shipyard
        if (
            game.turn_number <= 200 &&
            me->halite >= constants::SHIP_COST*2 &&
            !game_map->at(me->shipyard)->is_occupied())
        {
            command_queue.push_back(me->shipyard->spawn());
        }

        if (!game.end_turn(command_queue)) {
            break;
        }
    }

    return 0;
}
