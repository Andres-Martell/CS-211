#include"space_object.hxx"
#include <cmath>


static double
degrees_to_radian(double degrees)
{
    return acos(-1) * degrees / 180;
}

Space_object::Space_object(Position position)
        : top_left_(position)
{
}

bool
Space_ship::check_collision(Floor currfloor, Space_ship ship)
{
    if(ship.top_left_.y+ship.size() > currfloor.y &&
    (ship.top_left_.x> currfloor.x &&
    ship.top_left_.x + ship.size() < currfloor.x + currfloor.width)){
        return true;
    }
    return false;
}

bool
Space_ship::check_landing(Floor currfloor, Space_ship ship)
{
    if(check_collision(currfloor, ship) ){
        if (sqrt(
                pow(ship.v_.height,2)+pow(ship.v_.width,2)
                ) < 20 &&
                (deg_< 20 || deg_ > 340 )
                ){
            landing_type_ = Landing_Type::success;
            return true;
        }
    }
    landing_type_ = Landing_Type::fail;
    return false;
}

Space_object::Position
Space_object::position() const
{
    return top_left_;
}

Control&
Space_ship::control()
{
    return control_;
}

Inertial_space_object::Inertial_space_object(
        Position position,
        Velocity velocity,

        Angular_velocity angular_velocity,
        Fuel_Value fuel_value)
        : Space_object(position),
          ddeg_(angular_velocity),
          dv_(velocity),
          fuel_(fuel_value)
{
}

Space_object::Angle
Space_object::heading() const
{
    return deg_;
}

void
Space_ship::integrate(double dt)
{
    v_.height += 6 * dt;
    if (control_.right && (deg_ < 91 || deg_ > 269 )) {
        deg_ += heading_change * dt;
        if(deg_ >=91 && deg_ < 269 ){
            deg_ = 90;
        }
    }
    if (control_.left && (deg_ < 91 || deg_ > 269 )) {
        deg_ -= heading_change * dt;
        if(deg_ <= 269 && deg_ > 91){
            deg_ = 270;
        }
    }
    if (deg_ < 0) { deg_ += 360; }
    if (deg_ > 360) { deg_ -= 360; }
    if (control_.thrust) {
        v_
                +=
                {sin(degrees_to_radian(deg_)) * velocity_change * dt,
                 -cos(degrees_to_radian(deg_)) * velocity_change * dt};
    }
    Inertial_space_object::integrate(dt);
    if (top_left_.x < 0){
        top_left_.x = screen_dimensions_.width - size();
    }
    if (top_left_.x + size()  > screen_dimensions_.width){
        top_left_.x = 0;
    }
    if (top_left_.y < 2){
        top_left_.y = 2;
        v_.height = 0;
    }
}

int
Inertial_space_object::get_fuel(){
    return fuel_;
}

void
Inertial_space_object::subtract_fuel_value(int fuelLoss)
{
    fuel_ -= fuelLoss;
}
void
Inertial_space_object::add_fuel_value(int fuelGain)
{
    fuel_ += fuelGain;
}

int
Inertial_space_object::get_score(){
    return score_;
}

void
Inertial_space_object::change_score_value(int scoreAddition){
    score_ += scoreAddition;
}

Inertial_space_object::Landing_Type
Inertial_space_object::get_landing()
{
    return landing_type_;
}

void
Inertial_space_object::reset_landing_type(){
    landing_type_ = Landing_Type::none;
}

void
Inertial_space_object::integrate(double dt)
{
    top_left_.x += v_.width * dt;
    top_left_.y += v_.height * dt;
    v_ += dv_ * dt;
    deg_ += ddeg_ * dt;
}

double
Space_ship::size()
{
    return 15;
}
