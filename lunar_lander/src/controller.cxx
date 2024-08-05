#include "controller.hxx"
#include <fstream>
#include <stdexcept>
#include <iostream>

///
/// Constructors
///

Controller::Controller(ge211::Dims<int> screen_dimensions)
        : model_(screen_dimensions),
          view_(model_,
                mixer()),
          screen_dimensions_(screen_dimensions)
{
}

///
/// Public member functions
///

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return screen_dimensions_;
}

void
Controller::on_key_up(ge211::Key key)
{
    if (key == ge211::Key::left()) {
        model_.turn_left(false);
    }
    if (key == ge211::Key::right()) {
        model_.turn_right(false);
    }
    if (key == ge211::Key::up()) {
        model_.thrust(false);
    }

    if (key.code() == 'q') {
        exit(0);
    }

}

void
Controller::on_key_down(ge211::Key key)
{
    if (key == ge211::Key::left()) {
        model_.turn_left(true);
    }
    if (key == ge211::Key::right()) {
        model_.turn_right(true);
    }
    if (key == ge211::Key::up() && model_.get_fuel() > 0) {
        model_.thrust(true);
     //   view_.play_thrust_effect();
        }
      //  model_.change_fuel(50);
}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    //model_.launch_

    model_.change_launch(true);

}

void
Controller::on_frame(double dt)
{
  //  double animation_time = 15;
    model_.update(dt);
    //while(!model_.landing_type() ==
    // Inertial_space_object::Landing_Type::none){
    if(!model_.is_game_over()) {

        play_low_fuel_sound();
    }
   // }
    play_fail_sound();
    play_success_sound();
    play_thrust_sound();

}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites);
}




void
Controller::play_thrust_sound()
{
    if(model_.get_thrust()){
        view_.play_thrust_effect();
    }
}

void
Controller::play_success_sound()
{
    Inertial_space_object::Landing_Type collision_type = model_.landing_type();
    if(collision_type == Inertial_space_object::Landing_Type::success){
        view_.play_success_effect();
    }
}

void
Controller::play_fail_sound()
{
    Inertial_space_object::Landing_Type collision_type = model_.landing_type();
    if(collision_type == Inertial_space_object::Landing_Type::fail){
       view_.play_fail_effect();
     //  view_.play_lowfuel_effect();
    }
}

void
Controller::play_low_fuel_sound(){
  //  std::cout << "model game over?" << model_.is_game_over() << std::endl;
  // std::cout << "model get fuel: " << model_.get_fuel() << std::endl;
    if(!model_.is_game_over() && (model_.get_fuel() < 120)){
       // view_.play_success_effect();
        view_.play_lowfuel_effect();
        if(model_.get_thrust()){
            view_.play_thrust_effect();
        }
    }
}

/*
void
Controller::new_round_screen(ge211::Sprite_set& sprites, double duration){
    int score = model_.get_score();
    Inertial_space_object::Landing_Type collision_type = model_.landing_type();
    if(collision_type != Inertial_space_object::Landing_Type::none){
        view_.draw_collision_results_(sprites, score);
    }
}*/

