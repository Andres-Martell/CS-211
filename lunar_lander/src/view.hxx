#pragma once
#include <algorithm>
#include "model.hxx"
#include<stdio.h>
#include <ge211.hxx>


// Controls how the game is shown to the user:
class View
{
public:
    ///
    /// Public member functions
    ///
    using Dims = ge211::Dims<int>;
    explicit View(Model& model,
                  ge211::Mixer& mixer
       );

    void draw(ge211::Sprite_set&);
    void play_thrust_effect() const;
    void play_success_effect() const;
    void play_fail_effect() const;
    void play_lowfuel_effect() const;
    void draw_collision_results_(ge211::Sprite_set& sprites,
                                 int score);

    ge211::Text_sprite score_;
    ge211::Text_sprite instructions_sprite_;
    ge211::Text_sprite vertical_v_;
    ge211::Text_sprite horizontal_v_;

private:
    ///
    /// Private helper functions
    ///
    using Posn = ge211::Posn<int>;
    void draw_fuel_(ge211::Sprite_set& sprites);
    void draw_score_(ge211::Sprite_set& sprites,
                     int score);
    void draw_instructions_(ge211::Sprite_set& sprites);
    void draw_velocity_(ge211::Sprite_set& sprites,
                        Inertial_space_object::Velocity);
    void draw_game_over_(ge211::Sprite_set& sprites,
                          int score);

    void load_audio_();
    // The view can look at the model but doesn't change it.

    Model& model_;
    ge211::Mixer& mixer_;

    // Sprites for tiles:

    ge211::Image_sprite space_ship_ {"lunar_lander.png"};
    ge211::Image_sprite star {"star.png"};
    ge211::Image_sprite twinkle {"twinkle.png"};




    ge211::Font sans30 {"sans.ttf", 30};
    ge211::Text_sprite fuel_sprite {"Fuel value: ",
                                    sans30};
    int score_value = model_.get_score();
    ge211::Rectangle_sprite const fuel_bar_;
    ge211::Text_sprite over_sprite {"Game over!",
                                    sans30};
    ge211::Text_sprite score_text {"Score: ", sans30};
    ge211::Text_sprite vertical_text {"Vertical velocity: ", sans30};
    ge211::Text_sprite horizontal_text {"Horizontal Velocity: ", sans30};
    ge211::Text_sprite success_sprite {"Successful landing!", sans30};
    ge211::Text_sprite fail_sprite{"Failed collision!", sans30};
    ge211::Text_sprite final_score_sprite {"Final Score: ",
                                    sans30};
    ge211::Text_sprite const click_reminder {"Click to start", sans30};

    ge211::Text_sprite const start_reminder {"Click to start next round",
                                             sans30};

    ge211::Dims<int> screen_dimensions;
    double space_ship_scale_ = .1;

    //potential floors
    ge211::Rectangle_sprite const floor_sprite;
    ge211::Rectangle_sprite const floor_A;
    ge211::Rectangle_sprite const floor_B;
    ge211::Rectangle_sprite const floor_C;
    ge211::Rectangle_sprite const floor_D;
    ge211::Rectangle_sprite const floor_E;
    ge211::Rectangle_sprite const floor_F;
    ge211::Rectangle_sprite const floor_G;

    std::vector<ge211::Rectangle_sprite> floor_vector;

    //sounds
    ge211::Sound_effect thrust_sound_;
    ge211::Sound_effect success_sound_;
    ge211::Sound_effect fail_sound_;
    ge211::Sound_effect warning_sound_;
};
