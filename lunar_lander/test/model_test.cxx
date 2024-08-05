#include "model.hxx"
#include <catch.hxx>
#include <ge211.hxx>


TEST_CASE("spaceship starts with the proper velocity and acceleration,"
          "lands successfully, and resets.")
          //Case should test that the spaceship starts without moving and
          // with launch false both before the player starts, and upon
          // starting the game (when
          // the player actually clicks to start), that it has a 0 vertical
          // velocity, const horizontal v, and an acceleration equal to our
          // game's gravity, then test that it can land
          // successfully on a valid landing spot, and that a new round
          // begins with the spaceship properly reset
          {
              ge211::Dims<int> screen_dimensions {1400, 768};
              Model m(screen_dimensions);

              //initial spaceship starting values
              CHECK(m.get_launch() == false);
              CHECK(m.get_velocity().height == 0);
              CHECK(m.get_velocity().width == 10.0);

              //launching the spaceship (as if player clicked to start)
              m.change_launch(true);

              CHECK(m.get_velocity().height == 0);
              CHECK(m.get_velocity().width == 10);
              m.update(1);
              //moves the game state forward by one ft, meaning the spaceship
              // should have fallen according to gravity

              // our acceleration is = to v_.height += 6 * ft;
              //so spaceship should have a v of 6 now if gravity works
              CHECK(m.get_velocity().height == 6);
              CHECK(m.get_velocity().width == 10);

              Floor test_floor = m.validFloors[0];
              int posx = test_floor.x + 5; //making it 5 pixels in front of
              // the top left of the floor
              int posy = test_floor.y + m.size() - 1; //one pixel away from
              // landing
              m.set_pos(posx, posy);
              m.update(1);
              //should've successfully collided and landing type should be
              // successful

              CHECK(m.landing_type() ==
                    Inertial_space_object::success);

              m.update(1);
              CHECK(m.get_launch() == false);
              CHECK(m.get_velocity().height == 0);
              CHECK(m.get_velocity().width == 10.0);
}

TEST_CASE("spaceship has no fuel and crashes, causing game over")
{

    ge211::Dims<int> screen_dimensions {1400, 768};
    Model m(screen_dimensions);

    //initial spaceship starting values
    CHECK(m.get_launch() == false);
    CHECK(m.get_velocity().height == 0);
    CHECK(m.get_velocity().width == 10);

    m.change_launch(true);
    m.lose_fuel(900);
    m.update(50);

    CHECK(m.is_game_over() == true);


}

TEST_CASE("spaceship crashes with little fuel left, causing game over due to "
          "fuel subtraction from failed landing")
{

    ge211::Dims<int> screen_dimensions {1400, 768};
    Model m(screen_dimensions);

    //initial spaceship starting values
    CHECK(m.get_launch() == false);
    CHECK(m.get_velocity().height == 0);
    CHECK(m.get_velocity().width == 10);

    m.change_launch(true);
    m.lose_fuel(870);
    m.update(50);
    //should crash and subtract 100 from fuel value (currently at 30), so
    // should end game

    CHECK(m.is_game_over());

}

TEST_CASE("when landing == success, spaceship gets fuel boost"){
    ge211::Dims<int> screen_dimensions {1400, 768};
    Model m(screen_dimensions);

    m.change_launch(true);
    m.lose_fuel(400);
    Floor test_floor = m.validFloors[0];
    int posx = test_floor.x + 5; //making it 5 pixels in front of
    // the top left of the floor
    int posy = test_floor.y + m.size() - 1; //one pixel away from
    // landing
    m.set_pos(posx, posy);
    int before_fuel = m.get_fuel();
    m.update(1);
    int after_fuel = m.get_fuel();

    CHECK(m.landing_type() == Inertial_space_object::Landing_Type::success);
    CHECK(before_fuel < after_fuel);
}

TEST_CASE("when thrusters are on, spaceship uses fuel")
{
    ge211::Dims<int> screen_dimensions {1400, 768};
    Model m(screen_dimensions);

    //initial spaceship starting values
    CHECK(m.get_launch() == false);
    CHECK(m.get_velocity().height == 0);
    CHECK(m.get_velocity().width == 10.0);

    //launching the spaceship (as if player clicked to start)
    m.change_launch(true);

    CHECK(m.get_velocity().height == 0);
    CHECK(m.get_velocity().width == 10);
    m.update(1);

    //moves the game state forward by one ft, meaning the spaceship should
    // have fallen according to gravity

    // our acceleration is = to v_.height += 6 * ft;
    //so spaceship should have a v of 6 now if gravity works
    CHECK(m.get_velocity().height == 6);
    CHECK(m.get_velocity().width == 10);

    int before_fuel = m.get_fuel();
    m.thrust(true);
    m.update(1);
    int after_fuel = m.get_fuel();

    //uses one unit of fuel
    CHECK(before_fuel == after_fuel + 1);
}

