# Helicopter_game_using_opengl
### **Helicopter Game (OpenGL) - Retro Style**  

This is a simple yet engaging 2D helicopter game made using OpenGL. The game follows a **Flappy Bird-style** obstacle-avoidance mechanic, where the player controls a helicopter and tries to navigate through incoming obstacles while avoiding crashes.  

## **🛠 Features**
- **Smooth physics-based movement** with gravity and lift mechanics.
- **Animated helicopter blades** for a realistic effect.
- **Dynamic obstacles** inspired by Flappy Bird.
- **Score tracking** based on successful obstacle passes.
- **Crash counter** to track failed attempts.
- **Game Over screen** with restart prompt.
- **Minimalist retro graphics** with a sky-blue background.


## **🎮 Gameplay**
- **Press SPACE** to make the helicopter go up.
- **Gravity pulls it down automatically** when no input is given.
- **Avoid hitting the obstacles** and the ground or ceiling.
- **Survive as long as possible** to increase your score.
- **Game Over occurs after too many crashes** (adjustable limit).


## **🔧 How It Works**
- The game world is **100x100 units**, with the helicopter starting in the middle.
- **Gravity (0.05) and lift force (1.2) create a natural flying effect.**
- **Obstacles move from right to left**, creating a side-scrolling experience.
- **Collision detection** resets the helicopter or ends the game if the crash limit is reached.


## **💻 How to Run**
1. Install **OpenGL/GLUT** if not already set up.  
2. Compile the game using:  
   g++ helicopter_game.cpp -o helicopter -lglut -lGLU -lGL
3. Run the executable:  
   ./helicopter
 
