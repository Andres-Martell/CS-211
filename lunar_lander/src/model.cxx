#include"model.hxx"
#include <iostream>


///
/// Constructors
///
Model::Model(ge211::Dims<int> screen_dimensions)
        : screen_dimensions_(screen_dimensions),
          allFloors(),
          validFloors(),
          mainFloor(0,screen_dimensions.height-30,screen_dimensions.width,
                    25),
          floor_A(20, screen_dimensions.height-35,100, 30 ),
          floor_B(300, screen_dimensions.height-35,50, 30 ),
          floor_C(500, screen_dimensions.height-35,150, 30),
          floor_D(750, screen_dimensions.height-35,50, 30 ),
          floor_E(900, screen_dimensions.height-35,100, 30 ),
          floor_F(1100, screen_dimensions.height-35,50, 30 ),
          floor_G(1200, screen_dimensions.height-35,150, 30),
          space_ship_(screen_dimensions_.into<double>()),
          game_over_(false),
          launch_(false)
{
    allFloors.push_back(floor_A);
    allFloors.push_back(floor_B);
    allFloors.push_back(floor_C);
    allFloors.push_back(floor_D);
    allFloors.push_back(floor_E);
    allFloors.push_back(floor_F);
    allFloors.push_back(floor_G);
    validFloors = randomize_floors(allFloors);
}



///
/// Public member functions
///
void
Model::update(double ft)
{
    if(!game_over_) {
        if (launch_) {
            if (landing_type() != Inertial_space_object::Landing_Type::none) {
                reset_landing_type();
            }
            if (space_ship_.control().thrust) {
                if (space_ship_.get_fuel() >= 1) {
                    lose_fuel(1);
                }
            }
            space_ship_.integrate(ft);

            for (size_t i = 0; i < validFloors.size(); i++) {
                if (space_ship_.check_collision(validFloors[i],
                                                space_ship_)) {
                    if (space_ship_.check_landing(validFloors[i],
                                                  space_ship_)) {
                        add_fuel(100);
                        space_ship_.landing_type_ =
                            Inertial_space_object::Landing_Type::success;
                        launch_ = false;
                        double score_multiplier = 1.0/
                                (allFloors[i].width / 50.0) * 300.0;
                        change_score((200 + score_multiplier));
                        reset();
                    } else {
                    //fuel gets taken away
                    lose_fuel(100);
                        if (space_ship_.get_fuel() >= 1) {
                            space_ship_.landing_type_ =
                                    Inertial_space_object::Landing_Type::fail;
                            reset();
                        } else {
                        space_ship_.landing_type_ =
                                Inertial_space_object::Landing_Type::none;
                        game_over_ = true;
                        }
                    }
                }
            }

            if (space_ship_.check_collision(mainFloor, space_ship_)) {
                    //fuel gets taken away
                    lose_fuel(50);
                    if (space_ship_.get_fuel() >= 1) {
                        space_ship_.landing_type_ =
                                Inertial_space_object::Landing_Type::fail;
                        reset();
                    } else {
                        space_ship_.landing_type_ =
                                Inertial_space_object::Landing_Type::none;
                        game_over_ = true;
                    }
                }
            }
        } else {
            launch();
        }
}


std::vector<Floor>
Model::randomize_floors(std::vector<Floor> allFloors_){

 std::vector<Floor> valFloors;

    for (size_t i = 0; i < allFloors_.size(); i++) {
        int chance = (rand() % 10);

        if(chance > 4) {
            valFloors.push_back(allFloors_[i]);
        }
    }

    if(valFloors.empty()){
        valFloors = randomize_floors(allFloors_);
    }
    return valFloors;
}


void
Model::turn_right(bool state)
{
    space_ship_.control().right = state;
}

void
Model::turn_left(bool state)
{
    space_ship_.control().left = state;
}

int
Model::get_fuel(){
   return space_ship_.get_fuel();
}

int
Model::get_score(){
    return space_ship_.get_score();
}

Inertial_space_object::Velocity
Model::get_velocity(){
    return space_ship_.v_;
}

void
Model::change_score(int scoreAddition){
    space_ship_.change_score_value(scoreAddition);
}

void
Model::thrust(bool state)
{
    space_ship_.control().thrust = state;
 //   space_ship_.change_fuel_value(50);
}

void
Model::lose_fuel(int fuelLoss){
    space_ship_.subtract_fuel_value(fuelLoss);
}

void
Model::add_fuel(int fuelGain){
    space_ship_.add_fuel_value(fuelGain);
}

bool
Model::is_game_over()
{
    return game_over_;
}

bool
Model::get_thrust()
{
    return space_ship_.control().thrust;
}

void
Model::launch()
{
   if (!launch_){
       if(space_ship_.get_fuel() > 0) {
           space_ship_.v_ = {10, 0};
           space_ship_.deg_ = 0;
       }
   }
}

void
Model::change_launch(bool state)
{
    launch_ = state;
}

bool
Model::get_launch()
{
    return launch_;
}

Inertial_space_object::Landing_Type
Model::landing_type(){
    return space_ship_.get_landing();
}

void
Model::reset_landing_type()
{
    space_ship_.reset_landing_type();
}

void
Model::reset(){
    launch_ = false;
    launch();
    validFloors = randomize_floors(allFloors);

    space_ship_.top_left_.x = screen_dimensions_.width / 4;
    space_ship_.top_left_.y = screen_dimensions_.height / 5;
  //  space_ship_.landing_type_ = Inertial_space_object::Landing_Type::none;
}

Space_object::Position
Model::set_pos(int x, int y){
    space_ship_.top_left_.x = x;
    space_ship_.top_left_.y = y;
    return space_ship_.top_left_;
}

double
Model::size()
{
    return space_ship_.size();
}


Space_ship
Model::space_ship()
{
    return space_ship_;
}





///
/// Private member functions
///
