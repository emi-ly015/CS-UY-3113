# Introduction to Game Programming (C++ / raylib)

This collection showcases a series of C++ game development projects built with raylib, progressing from simple 2D animation to fully interactive, multi-system gameplay. Each project focuses on a different layer of game design.

The work emphasizes building reusable systems and understanding how real-time applications manage state and interaction. The final project integrates these concepts into a more complete gameplay experience with multiple levels and player-driven progression.

## Overview

The repository is divided into multiple projects, each focusing on a specific aspect of game development:

- **Project 1**  
  Focuses on movement, rotation, and scaling to build a dynamic scene.

- **Project 2**  
  Builds interactive gameplay through a Pong-inspired game with collision handling

- **Project 3**  
  Explores gravity, velocity, and resource-based mechanics through a Lunar-lander-inspired game

- **Project 4**  
  Introduces structured game architecture thought a multi-scene platformer with camera tracking, levels, and AI

- **Final Project**  
  Farming simulator combining gameplay systems and world interaction


## Technical Themes

Several core game development concepts are explored throughout these projects:

- **Real-time game loops** for continuous input, update, and rendering cycles  
- **Collision detection and response handling**  
- **Scene and state management** for transitioning between game phases  
- **Camera systems** for tracking and framing gameplay  
- **Audio integration** for feedback and immersion  
- **Shader usage** for visual effects and rendering enhancements  
- **Frame-based and fixed timestep updates** for consistent gameplay behavior  
- **Sprite rendering and texture atlas animation**  
- **Entity systems** for reusable and modular game objects  


## Project Breakdown

### Project 1: Draw a Simple 2D Scene

`Project1` focuses on building a dynamic visual scene using simple cartoon character images. This project is centered on understanding how transformations and timing create motion and visual depth.

**What it covers:**
- Orbital motion using trigonometric calculations  
- Continuous rotation and scaling effects  
- Combining multiple sprites into a layered scene  


Run locally:

```bash
cd projects/project_1
make
make run
```

### Project 2: Pong 

`Project2` shifts from animation to gameplay by implementing a Pong-inspired game featuring two capybaras as paddles and an orange as the ball. This project marks the transition from simple visual scenes to interactive gameplay.

**What it covers:**
- Player-controlled movement and boundary constraints  
- Collision detection between paddles and the ball  
- Ball movement and direction updates based on impact  
- Score tracking and win conditions  
- Game-over and restart states  


Run locally:

```bash
cd projects/project_2
make
make run
```

### Project 3: Lunar Lander Simulation

`Project3` introduces physics-based gameplay through a rocket landing simulation. The player controls a rocket that must land safely on designated platforms while managing movement and resources.


**What it covers:**
- Gravity and thrust-driven movement  
- Velocity updates and damping effects  
- Fuel management as a gameplay constraint  
- Fixed timestep updates for consistent physics  
- Collision-based landing validation  
- Moving platforms and environmental challenges  

Run locally:

```bash
cd projects/project_3
make
make run
```

### Project 4: Rise of The AI

`Project4` builds on earlier work by introducing a more structured game design through a simple platformer. The project incorporates multiple systems working together, including player mechanics and basic AI behavior.

**What it covers:**
- Scene-based architecture for handling multiple levels  
- Tile map rendering for structured environments  
- Player movement, jumping, and interaction  
- Basic enemy AI (wander and follow behaviors)  
- Camera tracking to follow the player  
- Game state transitions (win/lose conditions)  
- Audio integration for music and sound effects 

Run locally:

```bash
cd projects/project_4
make
make run
```

### Project 5: Farming Simulator

`Project5` is a multi-level farming simulator inspired by games like :contentReference[oaicite:0]{index=0} and :contentReference[oaicite:1]{index=1}. It expands on earlier projects by combining multiple gameplay systems into a progression-based experience. The game is structured around three levels, each introducing new crops, objectives, and increasing complexity. Players plant, grow, and harvest crops, interact with animals, collect resources, and complete missions to advance. In the future, I plan to add different shaders, more 

**What it covers:**
- Multi-level scene system with progression and transitions  
- Tile-based world rendering using a custom map system 
- Inventory system with item tracking and selection  
- Crop lifecycle system (planting, watering, growth, harvesting)  
- Resource collection (wood, fruit, crops)  
- Shop system with trading and item availability  
- Mission-based objectives and win/lose conditions  
- Camera tracking tied to player movement  
- Animated entities with directional movement and sprite atlases  
- AI behaviors including wandering, following, and flying entities
- UI systems for inventory, missions, and player feedback  
- Scene transitions including menu, win, and lose states 

Run locally:

```bash
cd final_project
make
make run
```

## How To Build

### Prerequisites

To run these projects locally, you will need:

- `g++`
- `make`
- [`raylib`](https://www.raylib.com/)
- `pkg-config`

The provided makefiles are compatible with:

- macOS
- Linux / WSL
- Windows (via MinGW) 

**macOS setup:**
```bash
brew install raylib pkg-config
```

**On Ubuntu / Debian**

```bash
sudo apt update
sudo apt install build-essential libraylib-dev pkg-config
```

### Clone the Repository

```bash
git clone https://github.com/emi-ly015/<CS-UY-3113>.git
cd CS-UY-3113
```

Navigate to a project folder and run:

```bash
cd projects/project_4 
make
make run
```

For the final project:

```bash
cd final_project
make
make run
```

Clean Build Files:

```bash
make clean
```


## Additional Details

- Asset paths are referenced locally within the code, so moving files or folders may require updating those paths
- Each project is built independently rather than as part of a single unified engine
- Some directories may include compiled outputs (e.g., `raylib_app`); rebuild using `make` if necessary 
