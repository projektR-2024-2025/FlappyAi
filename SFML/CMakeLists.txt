cmake_minimum_required(VERSION 3.5.0)
project(Flappy VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenMP REQUIRED)

add_executable(Flappy 
    cgp_andrija/CGP1.cpp cgp_andrija/CGP1Individual.cpp NNlogic.cpp NeuralNetwork.cpp
    Parameters.cpp SelectionScreen.cpp Simulator.cpp main.cpp
    gp_tonka/FunctionBinaryTree.cpp gp_tonka/GA.cpp gp_tonka/TreeParser.cpp)

# Copy the config.txt file to the build directory
configure_file(Map1.txt ${CMAKE_BINARY_DIR}/Map1.txt COPYONLY)
configure_file(Map2.txt ${CMAKE_BINARY_DIR}/Map2.txt COPYONLY)
configure_file(Map3.txt ${CMAKE_BINARY_DIR}/Map3.txt COPYONLY)
configure_file(Map4.txt ${CMAKE_BINARY_DIR}/Map4.txt COPYONLY)
configure_file(Map5.txt ${CMAKE_BINARY_DIR}/Map5.txt COPYONLY)

target_link_libraries(Flappy -lsfml-graphics)
target_link_libraries(Flappy -lsfml-system)
target_link_libraries(Flappy -lsfml-window)
target_link_libraries(Flappy -lgomp)
