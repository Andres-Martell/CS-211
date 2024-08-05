#pragma once

#include<memory>
#include <ge211.hxx>
#include<iostream>
#include<stdio.h>



struct Control
{
    bool left = false;
    bool right = false;
    bool thrust = false;

};

using Floor = ge211::Rect<int>;

class Space_object
{
public:
    using Dimensions = ge211::Dims<double>;
    using Position = ge211::Posn<double>;
    using Angle = double; // in ° counterclockwise from N


    Space_object(Position);

    Position position() const;

    Angle heading() const;

    virtual double size() = 0;

    // Just updates the position (no collision checking).
    virtual void integrate(double dt) = 0;

    // Facing direction, may not match velocity
    virtual ~Space_object() = default;

     Position top_left_;
     Angle deg_ = 0.0;
};


class Inertial_space_object : public Space_object
{
public:
    using Velocity = ge211::Dims<double>;
    using Acceleration = ge211::Dims<double>;
    using Angular_velocity = double;
    using Fuel_Value = int;
    using Score_Value = int;
    enum Landing_Type {none, success, fail};

    Inertial_space_object(
            Position,
            Velocity = {0, 0.0},
            Angular_velocity = 0.0,
            Fuel_Value = 900);

    void integrate(double dt) override;
    int get_fuel();
    void subtract_fuel_value(int fuelLoss);
    void add_fuel_value(int fuelGain);
    int get_score();
    void change_score_value(int scoreAddition);
    Landing_Type get_landing();
    void reset_landing_type();
    Velocity v_ = {10, 0};
    Landing_Type landing_type_ = Landing_Type::none;


private:
    Angular_velocity ddeg_ = 0;
    Acceleration dv_ {0.0, 0};
    Fuel_Value fuel_ = 900;
    Score_Value score_ = 0;
};


class Space_ship : public Inertial_space_object
{
public:

    explicit Space_ship(Dimensions screen_dimensions)
            : Inertial_space_object({screen_dimensions.width / 4,
                                     screen_dimensions.height / 5}),
                                     //this starting in top left corner
              screen_dimensions_(screen_dimensions)
    { }

    // Facing direction, may not match velocity
    Control& control();

    static bool check_collision(Floor floor, Space_ship ship);
    bool
    check_landing(Floor mainfloor, Space_ship ship);
    void integrate(double dt) override;
    double size() override;


private:
    double const heading_change = 180;
    double const velocity_change = 50;
    Control control_ {false, false, false};
    Dimensions screen_dimensions_;
};




