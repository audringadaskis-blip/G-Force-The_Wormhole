cmake_minimum_required(VERSION 3.16)
project(G-Force-The_Wormhole LANGUAGES CXX)

# C++17 is required for SFML 3 features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML installed on your system
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Add ALL source files (TP2 Requirement: Multiple files)
add_executable(Wormhole 
    main.cpp
    Player.cpp
    design.cpp
    mechanics.cpp
    Enemy.cpp
)

# Link SFML libraries
target_link_libraries(Wormhole PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Output executable to build folder
set_target_properties(Wormhole PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)