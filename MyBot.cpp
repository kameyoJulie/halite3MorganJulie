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
        Direction destination;

        for (const auto& ship_iterator : me->ships) {
            shared_ptr<Ship> ship = ship_iterator.second;

            //If halite in cargo < 400 => harvest else go back to the shipyard
            if (ship->halite < 400 /*&& moveNextCell(destination)==true*/) {
                command_queue.push_back(ship->move(game_map->naive_navigate(ship, scan[ship->id].position,  me->shipyard->position)));
            }
            else /*if (moveNextCell(destination)==true)*/{
                command_queue.push_back(ship->move(game_map->naive_navigate(ship, me->shipyard->position,  me->shipyard->position)));
            }
        }

        //condition if the turn number is over 200 & total halite > 2000 & shipyard not occuped
        // to create a new ship
        if (game.turn_number <= 200 &&
            me->halite >= constants::SHIP_COST*2 &&
            !game_map->at(me->shipyard)->is_occupied()) {
        // if (me->ships.empty()) {
            command_queue.push_back(me->shipyard->spawn());
        }

        if (!game.end_turn(command_queue)) {
            break;
        }
    }

    return 0;
}
/*
bool moveNextCell(Position p_destination) {
    float haliteNeeded = game_map[p_destination].halite - (game_map[p_destination].halite * (10 / 100)); //We need to make sure that the next cell we move in have 10% Halite min
    //game_map->at(ship)->halite < constants::MAX_HALITE / 10

    bool moveable = false;

    if (game_map[p_destination]->halite > haliteNeeded && at(p_destination)->!is_occupied()) {
        moveable = true;
    } 
    else { 
        moveable = false
    }

    /*
    //scan the 4 cardinals directions
    haliteN = MapCell.position.directional_offset(Direction.North)->halite;
    haliteS = MapCell.position.directional_offset(Direction.South)->halite;
    haliteE = MapCell.position.directional_offset(Direction.East)->halite;
    haliteW = MapCell.position.directional_offset(Direction.West)->halite;

    std::array<int, 4> nextHalite = { haliteN,haliteS,haliteE,haliteW };
    std::array<bool, 4> canAccessNextCell = { false,false,false,false };

    //check if the next cell we want to access have the min amount of Halite
    for (int i = 0; i < 4; ++i) {
        if (nextHalite[i] > haliteNeeded) {
            canAccessNextCell[i] = true;
        }
    }
    */
//}

/*
Position optiNextDestination(shared_ptr<Ship> ship) {

    Position l_optiDestination;

    //condition if there is high hallite cell in the next 10 cells then make it the destination
    if (true) {
        return l_optiDestination;
    }

    //calculate Distance and use moveNextCell to go to destination

    return l_optiDestination;
}*/


void updatePosOpponent() {

}