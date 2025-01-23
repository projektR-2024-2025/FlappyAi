#==========!!!MOLIM DA SE OVO NE DIRA!!!===========
cmake_minimum_required(VERSION 3.28)
project(SMFL)

set(CMAKE_CXX_STANDARD 17)

add_executable(SMFL
        main.cpp
        Agent.h
        CGP.cpp
        CGPIndividual.cpp
        NeuralNetwork.cpp
        NNlogic.cpp
        SelectionScreen.cpp
        Simulator.cpp
        Entity.h
        Entity.cpp
        Entity.cpp
        Gene.cpp
        mainFunctionsHeader.h
        Constants.h
        Population.h
        Population.cpp
        Parameters.cpp
        Controller.h
        gp_tonka/tonka.cpp
        gp_tonka/GA.cpp
        gp_tonka/FunctionBinaryTree.cpp
        #SFML/Node.h
        #SFML/Entity.cpp
        #SFML/Gene.cpp
        #SFML/Population.cpp
        #SFML/conioMac.h
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    target_include_directories(SMFL PRIVATE "/opt/homebrew/opt/libomp/include")
    target_compile_options(SMFL PRIVATE
            -Xpreprocessor -fopenmp
            -Wno-unused-command-line-argument
    )
    target_link_libraries(SMFL PRIVATE
            /opt/homebrew/opt/libomp/lib/libomp.dylib
    )
endif()

set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(SMFL PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

#==========!!!MOLIM DA SE OVO NE DIRA!!!===========














#
#cmake_minimum_required(VERSION 3.5.0)
#project(Flappy VERSION 0.1.0 LANGUAGES C CXX)
#
#find_package(OpenMP REQUIRED)
#add_executable(Flappy
#    CGP.cpp CGPIndividual.cpp NNlogic.cpp NeuralNetwork.cpp
#    Parameters.cpp SelectionScreen.cpp Simulator.cpp main.cpp
#    gp_tonka/FunctionBinaryTree.cpp gp_tonka/GA.cpp)
#
## Copy the config.txt file to the build directory
#configure_file(Map1.txt ${CMAKE_BINARY_DIR}/Map1.txt COPYONLY)
#configure_file(Map2.txt ${CMAKE_BINARY_DIR}/Map2.txt COPYONLY)
#configure_file(Map3.txt ${CMAKE_BINARY_DIR}/Map3.txt COPYONLY)
#configure_file(Map4.txt ${CMAKE_BINARY_DIR}/Map4.txt COPYONLY)
#configure_file(Map5.txt ${CMAKE_BINARY_DIR}/Map5.txt COPYONLY)
#
#target_link_libraries(Flappy -lsfml-graphics)
#target_link_libraries(Flappy -lsfml-system)
#target_link_libraries(Flappy -lsfml-window)
#target_link_libraries(Flappy -lgomp)
