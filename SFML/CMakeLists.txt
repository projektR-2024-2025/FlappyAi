if(APPLE)
    # Mac configuration
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
            SFML/Parameters.cpp
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
elseif(WIN32)
	# Windows configuration
    cmake_minimum_required(VERSION 3.5.0)
	project(FlappyAI VERSION 0.1.0 LANGUAGES C CXX)

	set(SFML_STATIC_LIBRARIES TRUE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -fopenmp")

	find_package(OpenMP REQUIRED)
	find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

	add_executable(FlappyAI WIN32
		cgp_andrija/CGP1.cpp cgp_andrija/CGP1Individual.cpp nn/NNlogic.cpp nn/NeuralNetwork.cpp
		Parameters.cpp SelectionScreen.cpp Simulator.cpp main.cpp
		gp_tonka/FunctionBinaryTree.cpp gp_tonka/GA.cpp gp_tonka/TreeParser.cpp)

	# Copy the config.txt file to the build directory
	configure_file(Map1.txt ${CMAKE_BINARY_DIR}/Map1.txt COPYONLY)
	configure_file(Map2.txt ${CMAKE_BINARY_DIR}/Map2.txt COPYONLY)
	configure_file(Map3.txt ${CMAKE_BINARY_DIR}/Map3.txt COPYONLY)
	configure_file(Map4.txt ${CMAKE_BINARY_DIR}/Map4.txt COPYONLY)
	configure_file(Map5.txt ${CMAKE_BINARY_DIR}/Map5.txt COPYONLY)
	configure_file(CGP_best.txt ${CMAKE_BINARY_DIR}/CGP_best.txt COPYONLY)
	configure_file(best_gp.txt ${CMAKE_BINARY_DIR}/best_gp.txt COPYONLY)
	configure_file(best_neural_network.txt ${CMAKE_BINARY_DIR}/best_neural_network.txt COPYONLY)
	configure_file(config.conf ${CMAKE_BINARY_DIR}/config.conf COPYONLY)

	target_link_libraries(FlappyAI sfml-graphics)
	target_link_libraries(FlappyAI "${OPENMP_CXX_FLAGS}")
else()
    # Linux configuration
    cmake_minimum_required(VERSION 3.5.0)
    project(Flappy VERSION 0.1.0 LANGUAGES C CXX)

    find_package(OpenMP REQUIRED)

    add_executable(Flappy 
        cgp_andrija/CGP1.cpp 
        cgp_andrija/CGP1Individual.cpp 
        nn/NNlogic.cpp 
        nn/NeuralNetwork.cpp
        Parameters.cpp 
        SelectionScreen.cpp 
        Simulator.cpp 
        main.cpp
        gp_tonka/FunctionBinaryTree.cpp 
        gp_tonka/GA.cpp 
        gp_tonka/TreeParser.cpp
    )

    configure_file(Map1.txt ${CMAKE_BINARY_DIR}/Map1.txt COPYONLY)
    configure_file(Map2.txt ${CMAKE_BINARY_DIR}/Map2.txt COPYONLY)
    configure_file(Map3.txt ${CMAKE_BINARY_DIR}/Map3.txt COPYONLY)
    configure_file(Map4.txt ${CMAKE_BINARY_DIR}/Map4.txt COPYONLY)
    configure_file(Map5.txt ${CMAKE_BINARY_DIR}/Map5.txt COPYONLY)
    configure_file(CGP_best.txt ${CMAKE_BINARY_DIR}/CGP_best.txt COPYONLY)
    configure_file(best_gp.txt ${CMAKE_BINARY_DIR}/best_gp.txt COPYONLY)
    configure_file(best_neural_network.txt ${CMAKE_BINARY_DIR}/best_neural_network.txt COPYONLY)
    configure_file(config.conf ${CMAKE_BINARY_DIR}/config.conf COPYONLY)

    target_link_libraries(Flappy -lsfml-graphics)
    target_link_libraries(Flappy -lsfml-system)
    target_link_libraries(Flappy -lsfml-window)
    target_link_libraries(Flappy -lgomp)
endif()