# Donkey Kong – Console Arcade Engine
A robust C++ recreation of the classic Donkey Kong arcade game, featuring advanced game logic, ASCII-based graphics, and dynamic gameplay states.

## Key Technical Features
* **Advanced Enemy AI:** Implemented "Hunting Mode" for ghosts, featuring pathfinding logic to track and follow the player.
* **Dynamic Rendering Engine:** Supports both classic and color modes, including visual feedback such as blinking effects and custom UI elements (e.g., heart-based health system).
* **Robust Error Handling:** Integrated a file-validation system that gracefully handles invalid map files, ensuring game continuity by auto-correcting scores and transitioning to the next valid level.
* **Custom Physics & Traversal:** Designed direction-switching logic for barrel entities based on environment markers (`^`), simulating classic arcade mechanics.

## Technologies
* **Language:** C++
* **Environment:** Windows Console API / Cross-platform C++ logic
* **Architecture:** Object-Oriented Programming (OOP) with a focus on modular entity management
