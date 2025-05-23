# Vulkan 2D Side Scroller

## Overview
Vulkan 2D Side Scroller is a simple 2D side-scrolling game built using the Vulkan API. The project serves as a foundation for understanding game development with Vulkan, focusing on rendering, game state management, and entity handling.

## Project Structure
The project is organized into the following directories:

- **src/**: Contains the source code for the game.
  - **main.cpp**: Entry point of the application.
  - **game/**: Contains the Game class for managing game state and logic.
    - **Game.cpp**: Implementation of the Game class.
    - **Game.h**: Declaration of the Game class.
  - **renderer/**: Contains the VulkanRenderer class for handling rendering tasks.
    - **VulkanRenderer.cpp**: Implementation of the VulkanRenderer class.
    - **VulkanRenderer.h**: Declaration of the VulkanRenderer class.
  - **entities/**: Contains classes for game entities like Player and Enemy.
    - **Player.cpp**: Implementation of the Player class.
    - **Player.h**: Declaration of the Player class.
    - **Enemy.cpp**: Implementation of the Enemy class.
    - **Enemy.h**: Declaration of the Enemy class.
  - **utils/**: Contains utility functions and helpers.
    - **Utils.cpp**: Implementation of utility functions.
    - **Utils.h**: Declaration of utility functions.

- **include/**: Directory for external headers needed for Vulkan or other libraries.

- **CMakeLists.txt**: Configuration file for CMake, specifying project settings and dependencies.

## Setup Instructions
1. **Prerequisites**: Ensure you have the Vulkan SDK installed on your system.
2. **Clone the Repository**: Clone this repository to your local machine.
3. **Build the Project**:
   - Navigate to the project directory.
   - Create a build directory: `mkdir build && cd build`
   - Run CMake: `cmake ..`
   - Build the project: `cmake --build .`
4. **Run the Game**: Execute the generated binary to start the game.

## Features
- Basic player movement and jumping mechanics.
- Simple enemy AI behavior.
- Vulkan-based rendering for high performance.

## Future Improvements
- Add more complex game mechanics and levels.
- Implement additional features such as sound and music.
- Optimize rendering and game logic for better performance.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.