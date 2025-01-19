
cmake_minimum_required(VERSION 3.28)
project(FlappyAi)

set(CMAKE_CXX_STANDARD 17)


add_executable(FlappyAi
        SFML/main.cpp
        SFML/Agent.h
        SFML/CGP.cpp
        SFML/CGPIndividual.cpp
        SFML/NeuralNetwork.cpp
        SFML/NNlogic.cpp
        SFML/SelectionScreen.cpp
        SFML/Simulator.cpp
        SFML/Entity.h
        SFML/Entity.cpp
        SFML/Entity.cpp
        SFML/Gene.cpp
        SFML/mainFunctionsHeader.h
        SFML/Constants.h
        SFML/Population.h
        SFML/Population.cpp
        #SFML/Node.h
        #SFML/Entity.cpp
        #SFML/Gene.cpp
        #SFML/Population.cpp
        #SFML/conioMac.h
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    target_include_directories(FlappyAi PRIVATE "/opt/homebrew/opt/libomp/include")
    target_compile_options(FlappyAi PRIVATE
            -Xpreprocessor -fopenmp
            -Wno-unused-command-line-argument
    )
    target_link_libraries(FlappyAi PRIVATE
            /opt/homebrew/opt/libomp/lib/libomp.dylib
    )
endif()

set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(FlappyAi PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)












#cmake_minimum_required(VERSION 3.28)
#project(FlappyAi)
#
#set(CMAKE_CXX_STANDARD 17)
#
#
#set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
#
#set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
#set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
#
#target_include_directories(FlappyAi PRIVATE "/opt/homebrew/opt/libomp/include")
#target_compile_options(FlappyAi PRIVATE -fopenmp)
#target_link_libraries(FlappyAi PRIVATE "/opt/homebrew/opt/libomp/lib/libomp.dylib")
#
#
#find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
#
#add_executable(FlappyAi SFML/main.cpp
#        SFML/Agent.h
#        SFML/CGP.cpp
#        SFML/CGPIndividual.cpp
#        #SFML/main.cpp
#        SFML/NeuralNetwork.cpp
#        SFML/NNlogic.cpp
#        SFML/SelectionScreen.cpp
#        SFML/Simulator.cpp
#        SFML/Entity.cpp
#        SFML/Gene.cpp
#        SFML/Population.cpp
#        SFML/Simulator.cpp
#        SFML/conioMac.h
#)
#
#
#if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
#    target_include_directories(FlappyAi PRIVATE "/opt/homebrew/opt/libomp/include")
#    target_compile_options(FlappyAi PRIVATE -Xpreprocessor -fopenmp)
#    target_compile_options(FlappyAi PRIVATE -Wno-unused-command-line-argument)
#    target_link_libraries(FlappyAi PRIVATE "/opt/homebrew/opt/libomp/lib/libomp.dylib")
#endif()
#
#target_link_libraries(FlappyAi PRIVATE
#        sfml-graphics
#        sfml-window
#        sfml-system
#)
#
