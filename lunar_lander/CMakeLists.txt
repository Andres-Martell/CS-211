cmake_minimum_required(VERSION 3.3)
project(lunar_lander CXX)
include(.cs211/cmake/CMakeLists.txt)
set(GAME_EXE lunar_lander)

set(MODEL_SRC
        src/space_object.cxx
        src/model.cxx)

add_program(lunar_lander
        src/lunarlauncher.cxx
        src/model.cxx
        src/view.cxx
        src/controller.cxx
        ${MODEL_SRC})
target_link_libraries(lunar_lander ge211)

target_link_libraries(${GAME_EXE} ge211)

add_test_program(model_test
        ${MODEL_SRC}
        test/model_test.cxx)
target_link_libraries(model_test ge211)


