#include "view.hxx"
#include <iostream>


namespace {
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;
using Transform = ge211::Transform;

int const fuel_thickness = 4;

Color const
        timer_color {0xFF, 0xFF, 0xFF};
}

static std::string const thrust_effect_filename {"ignition.mp3"};
static std::string const success_effect_filename {"success_sound.mp3"};
static std::string const fail_effect_filename {"fail-sound.wav"};
static std::string const warning_effect_filename {"warning.wav"};


///
/// Constructor
///
static ge211::Color const floor_color {100, 100, 100};
static ge211::Color const valid_Floor_color {0, 0, 250};


View::View(Model& model,
           ge211::Mixer& mixer
        )
        : model_(model),
        mixer_(mixer),
        fuel_bar_({1, fuel_thickness}, timer_color),
        floor_sprite({model.screen_dimensions_.width, 25}, floor_color),
        floor_A({100, 30}, valid_Floor_color),
        floor_B({50, 30}, valid_Floor_color),
        floor_C({150, 30}, valid_Floor_color),
        floor_D({50, 30}, valid_Floor_color),
        floor_E({100, 30}, valid_Floor_color),
        floor_F({50, 30}, valid_Floor_color),
        floor_G({150, 30}, valid_Floor_color)
{
    if (mixer_.is_enabled()) {
        load_audio_();
    }
    floor_vector.push_back(floor_A);
    floor_vector.push_back(floor_B);
    floor_vector.push_back(floor_C);
    floor_vector.push_back(floor_D);
    floor_vector.push_back(floor_E);
    floor_vector.push_back(floor_F);
    floor_vector.push_back(floor_G);
}

///
/// Member functions
///


void
View::draw(ge211::Sprite_set& sprites)
{
    sprites.add_sprite(floor_sprite, {0,
                                      model_.screen_dimensions_.height -
                                      30});
    for (size_t i = 0; i < model_.allFloors.size(); i++) {
        for (size_t j = 0; j < model_.validFloors.size(); j++) {
            if (model_.allFloors[i] == model_.validFloors[j]) {
                sprites.add_sprite(floor_vector[i], {model_.allFloors[i].x,
                                                     model_.screen_dimensions_
                                                           .height -
                                                     35}, 1);
            }
        }
    }

    sprites.add_sprite(twinkle, {200,550}, 0);
    sprites.add_sprite(star, {1200,500}, 0);
    sprites.add_sprite(star, {400,300}, 0);
    sprites.add_sprite(twinkle, {1000,200}, 0);


/*
    for (size_t i = 0; i < model_.rand_stars.size(); i++) {
        sprites.add_sprite(twinkle, {model_.rand_stars[i]}, 0);

    }
*/

    //states of playing
    bool start = (!model_.get_launch()) &&
                 (model_.landing_type() ==
                  Inertial_space_object::Landing_Type::none);
    bool crash = (!model_.get_launch()) &&
                 (model_.landing_type() !=
                  Inertial_space_object::Landing_Type::none);
    bool playing = (model_.get_launch()) && (model_.landing_type() ==
                                             Inertial_space_object::none);


    if (!model_.is_game_over()) {
        if (playing || start) {
            if (start) {
                sprites.add_sprite(click_reminder, {650, 250});
                draw_instructions_(sprites);
            }
            sprites.add_sprite(fuel_sprite, {100, 50});
            Space_ship space_ship = model_.space_ship();
            ge211::Posn<int> pos {0, 0};
            pos.x = (int) space_ship.position().x;
            pos.y = (int) space_ship.position().y;
            ge211::Transform
                    rotation =
                    ge211::Transform::rotation(space_ship.heading());
            rotation.set_scale(space_ship_scale_);
            pos.x
                    -=
                    space_ship_.dimensions().width * space_ship_scale_ /
                    2;
            pos.y
                    -=
                    space_ship_.dimensions().height *
                    space_ship_scale_ / 2;
            sprites.add_sprite(space_ship_, pos, 2, rotation);

            draw_fuel_(sprites);
            draw_score_(sprites, score_value);
            draw_velocity_(sprites, model_.get_velocity());
        } else if (crash) {
            draw_collision_results_(sprites, score_value);
        }
    } else { //game is over
            draw_game_over_(sprites, score_value);
        }
    }

void
View::play_thrust_effect() const
{
    mixer_.try_play_effect(thrust_sound_);
}

void
View::play_success_effect() const
{
    mixer_.try_play_effect(success_sound_);
}

void
View::play_fail_effect() const
{
    mixer_.try_play_effect(fail_sound_);
}

void
View::play_lowfuel_effect() const
{
    mixer_.try_play_effect(warning_sound_);
}

void
View::draw_fuel_(Sprite_set& sprites)
{
    double fraction = model_.get_fuel() / 10;
    auto posn = Posn(screen_dimensions).right_by(275).down_by
            (fuel_thickness +
            65);
    auto trans = Transform()
            .set_scale_x(fraction)
            .set_scale_y(fuel_thickness);
    sprites.add_sprite(fuel_bar_, posn, -1, trans);
}

void
View::draw_score_(Sprite_set& sprites,
                  int score)
{
    score = model_.get_score();
    sprites.add_sprite(score_text, {100,80});
    ge211::Text_sprite::Builder score_builder(sans30);
    score_builder.color(ge211::Color::white()) << score;
    score_.reconfigure(score_builder);
    auto posn = Posn(screen_dimensions).right_by(200).down_by
            (80);
    sprites.add_sprite(score_, posn, -1);
}

void
View::draw_instructions_(Sprite_set& sprites)
{
    ge211::Text_sprite::Builder instructions_builder(sans30);
    instructions_builder.color(ge211::Color::white());
    instructions_builder.message("Use the up arrow to thrust, and "
                                  "the side arrows to steer. "
                                  "Smaller landing spots earn"
                                  " higher scores. Only land on blue spaces, "
                                  "and land carefully!");
    instructions_builder.word_wrap(500);
    instructions_sprite_.reconfigure(instructions_builder);
    sprites.add_sprite(instructions_sprite_, {500, 300}, -1);
}

void
View::draw_velocity_(Sprite_set& sprites,
                        Inertial_space_object::Velocity v)
{
    sprites.add_sprite(vertical_text, {950, 50});
    sprites.add_sprite(horizontal_text, {950, 100});

    //vertical v sprite
    v = model_.get_velocity();
    ge211::Text_sprite::Builder vertical_builder(sans30);
    vertical_builder.color(ge211::Color::white()) << v.height;
    vertical_v_.reconfigure(vertical_builder);
    auto posn = Posn(screen_dimensions).right_by(1200).down_by
            (50);
    sprites.add_sprite(vertical_v_, posn, -1);


    //horizontal v sprite
    ge211::Text_sprite::Builder horizontal_builder(sans30);
    horizontal_builder.color(ge211::Color::white()) << v.width;
    horizontal_v_.reconfigure(horizontal_builder);
    auto pos = Posn(screen_dimensions).right_by(1250).down_by
            (100);
    sprites.add_sprite(horizontal_v_, pos, -1);
}



void
View::draw_game_over_(Sprite_set& sprites,
                      int score)
{
    sprites.add_sprite(over_sprite, {600, 300});
    sprites.add_sprite(final_score_sprite, {570, 375});
    score = model_.get_score();
    ge211::Text_sprite::Builder final_builder(sans30);
    final_builder.color(ge211::Color::white()) << score;
    score_.reconfigure(final_builder);
    auto posn = Posn(screen_dimensions).right_by(750).down_by
            (375);
    sprites.add_sprite(score_, posn, -1);
}

void
View::draw_collision_results_(ge211::Sprite_set& sprites,
                             int score)
{
        Inertial_space_object::Landing_Type landing = model_.landing_type();
        if (landing == Inertial_space_object::Landing_Type::success) {
            sprites.add_sprite(success_sprite, {600, 300});
        } else if (landing == Inertial_space_object::Landing_Type::fail) {
            sprites.add_sprite(fail_sprite, {600, 300});
        }

        sprites.add_sprite(score_text, {570, 375});
        score = model_.get_score();
        ge211::Text_sprite::Builder final_builder(sans30);
        final_builder.color(ge211::Color::white()) << score;
        score_.reconfigure(final_builder);
        auto posn = Posn(screen_dimensions).right_by(750).down_by
                (375);
        sprites.add_sprite(score_, posn, -1);
        sprites.add_sprite(start_reminder, {550, 430});
}

void
View::load_audio_()
{
    thrust_sound_.try_load(thrust_effect_filename, mixer_);
    success_sound_.try_load(success_effect_filename, mixer_);
    fail_sound_.try_load(fail_effect_filename, mixer_);
    warning_sound_.try_load(warning_effect_filename, mixer_);
}

