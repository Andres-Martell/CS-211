#pragma once
#include "space_object.hxx"
#include<memory>
#include<algorithm>
#include<iostream>
#include <ge211.hxx>



class Model
{
public:

    ///
    /// Constructors
    ///
    // Uses the dimensions of the board
    Model(ge211::Dims<int> screen_dimensions);

    ///
    /// Public member functions
    ///


    Space_ship space_ship();
    std::vector<Floor> randomize_floors(std::vector<Floor> validFloors);
    void update(double ft);
    void turn_right(bool state);
    void turn_left(bool state);
    int get_fuel();
    int get_score();
    Inertial_space_object::Velocity get_velocity();
    void change_score(int scoreAddition);
    void thrust(bool state);
    void lose_fuel(int fuelLoss);
    void add_fuel(int fuelGain);
    bool is_game_over();
    bool get_thrust();
    void launch();
    void change_launch(bool state);
    bool get_launch();
    Inertial_space_object::Landing_Type landing_type();
    void reset_landing_type();
    void reset();
    Space_object::Position set_pos(int x, int y);
    double size();



    ge211::Dims<int> screen_dimensions_;

    std::vector<Floor> allFloors;
    std::vector<Floor> validFloors;
    Floor mainFloor;
    Floor floor_A ;
    Floor floor_B;
    Floor floor_C;
    Floor floor_D;
    Floor floor_E;
    Floor floor_F;
    Floor floor_G;


private:

    ///
    /// Private member variables
    ///

    Space_ship space_ship_;
    bool game_over_;
    bool launch_;



    // Random sources


};
