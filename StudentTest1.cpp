#include "Board.hpp"
#include "doctest.h"

#include "Board.hpp"
#include "City.hpp"
#include "Color.hpp"
#include "Player.hpp"

#include "Dispatcher.hpp"
#include "FieldDoctor.hpp"
#include "GeneSplicer.hpp"
#include "Medic.hpp"
#include "OperationsExpert.hpp"
#include "Researcher.hpp"
#include "Scientist.hpp"
#include "Virologist.hpp"

using namespace pandemic;

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

const int number_of_tests = 10;
const int number_of_inner_test = 5;

void init_board(Board &board) {
    board.remove_cures();
    board[City::Kinshasa] = 3;      // put 3 yellow disease cubes in Kinshasa.
    board[City::Kinshasa] = 2;      // change number of disease cubes in Kinshasa to 2.
    board[City::MexicoCity] = 3;    // put 3 yellow disease cubes in MexicoCity
    board[City::HoChiMinhCity] = 1; // put 1 red disease cube in HoChiMinhCity
    board[City::Chicago] = 1;       // put 1 blue disease cube in Chicago
    board[City::Cairo] = 3;         //put 3 black disease cubes.
    board[City::Miami] = 5;
}

TEST_CASE("TEST player") {
    Board board;
    init_board(board);

    Player player{board, City::Kinshasa};
    CHECK(board[City::Kinshasa] == 2);
    CHECK(board[City::MexicoCity] == 3);
    CHECK_NOTHROW(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 1);
    CHECK_NOTHROW(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 0);
    CHECK_THROWS(player.treat(City::Kinshasa));
    CHECK(board[City::Kinshasa] == 0);

    CHECK_THROWS(player.drive(City::MexicoCity));
    CHECK_NOTHROW(player.take_card(City::MexicoCity));

    CHECK_THROWS(player.fly_charter(City::MexicoCity));
    CHECK_NOTHROW(player.take_card(City::Kinshasa).build());
    CHECK_THROWS(player.fly_charter(City::MexicoCity));
    CHECK_NOTHROW(player.fly_direct(City::MexicoCity).take_card(City::MexicoCity).build());
    CHECK(board[City::MexicoCity] == 3);
    CHECK_NOTHROW(player.treat(City::MexicoCity).treat(City::MexicoCity));
    CHECK(board[City::MexicoCity] == 1);
    CHECK_THROWS(player.treat(City::MexicoCity).treat(City::MexicoCity));
    CHECK(board[City::MexicoCity] == 0);

    // go to miami with is adjecent to mexicocity.
    CHECK_NOTHROW(player.drive(City::Miami));
    CHECK_NOTHROW(player.take_card(City::Miami).build().take_card(City::Santiago).take_card(City::Lima));
    CHECK_NOTHROW(player.take_card(City::Bogota).take_card(City::Kinshasa).take_card(City::LosAngeles));
    // we have 5 yellow cards so find a cure.
    CHECK_THROWS(player.discover_cure(Color::Black));
    CHECK_NOTHROW(player.discover_cure(Color::Yellow).discover_cure(Color::Yellow).discover_cure(Color::Yellow));
    CHECK_NOTHROW(player.take_card(City::Cairo).take_card(City::Algiers).take_card(City::Riyadh).take_card(City::Mumbai));
    CHECK_THROWS(player.discover_cure(Color::Black));
    CHECK_NOTHROW(player.take_card(City::Baghdad).discover_cure(Color::Black).drive(City::MexicoCity));

    CHECK_THROWS(player.treat(City::MexicoCity));
    board[City::MexicoCity] = 10;
    CHECK_NOTHROW(player.treat(City::MexicoCity));
    CHECK(board[City::MexicoCity] == 0);
}
