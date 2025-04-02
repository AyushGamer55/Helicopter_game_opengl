#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>

// Game Constants
const float WORLD_WIDTH = 100.0f;
const float WORLD_HEIGHT = 100.0f;
const float GRAVITY = 0.05f;
const float LIFT_FORCE = 1.2f;
const float MAX_SPEED = 2.0f;
const int MAX_CRASHES = 50;
const float OBSTACLE_WIDTH = 5.0f;

// Game State
struct GameState {
    float helicopterY = 50.0f;
    float velocity = 0.0f;
    int score = 0;
    int crashes = 0;
    float bladeRotation = 0.0f;
    bool isGameOver = false;
    float obstacleSpeed = 0.5f;
};

struct Obstacle {
    float x, y;
};

GameState game;
std::vector<Obstacle> obstacles;

// Function to Draw Text
void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

// Initialize Game
void initGame() {
    game.helicopterY = 50.0f;
    game.velocity = 0.0f;
    game.score = 0;
    game.crashes = 0;
    game.bladeRotation = 0.0f;
    game.isGameOver = false;
    obstacles.clear();

    // Generate Obstacles
    for (int i = 0; i < 5; i++) {
        obstacles.push_back({WORLD_WIDTH + i * 30.0f, static_cast<float>(rand() % 60 + 20)});
    }
}

// Draw Helicopter
void drawHelicopter() {
    glPushMatrix();
    glTranslatef(20.0f, game.helicopterY, 0.0f);

    // Body (Green)
    glColor3f(0.0f, 0.6f, 0.0f);
    glRectf(-5, -2, 5, 2);

    // Cockpit (Black)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(5, 2);
    glVertex2f(5, -2);
    glVertex2f(7, 0);
    glEnd();

    // Tail (Green)
    glColor3f(0.0f, 0.6f, 0.0f);
    glRectf(-10, -1, -5, 1);

    // Landing Skids (Gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(-4, -3); glVertex2f(4, -3);
    glVertex2f(-6, -3); glVertex2f(-4, -2);
    glVertex2f(6, -3); glVertex2f(4, -2);
    glEnd();

    // Main Rotor (Animated)
    glPushMatrix();
    glTranslatef(0, 2, 0);
    glRotatef(game.bladeRotation, 0.0f, 0.0f, 1.0f);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
        glVertex2f(-7.0f, 0.0f); glVertex2f(7.0f, 0.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

// Draw Obstacles (Flappy Bird Style)
void drawObstacles() {
    for (const auto& obstacle : obstacles) {
        glColor3f(0.8f, 0.8f, 0.0f);  // Yellow color
        glRectf(obstacle.x, 0.0f, obstacle.x + OBSTACLE_WIDTH, obstacle.y - 10.0f); // Upper part of obstacle
        glRectf(obstacle.x, obstacle.y + 10.0f, obstacle.x + OBSTACLE_WIDTH, WORLD_HEIGHT); // Lower part of obstacle
    }
}

// Draw HUD (Including Score)
void drawHUD() {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(5.0f, WORLD_HEIGHT - 5.0f, "Score: " + std::to_string(game.score));
    drawText(5.0f, WORLD_HEIGHT - 10.0f, "Crashes: " + std::to_string(game.crashes));

    if (game.isGameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(WORLD_WIDTH / 2 - 20, WORLD_HEIGHT / 2, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(WORLD_WIDTH / 2 - 15, WORLD_HEIGHT / 2 - 5, "Final Score: " + std::to_string(game.score));
        drawText(WORLD_WIDTH / 2 - 20, WORLD_HEIGHT / 2 - 10, "Press 'F' to Restart");
    }
}

// Collision Check
void checkCollision() {
    for (auto& obstacle : obstacles) {
        if (20.0f + 5.0f > obstacle.x && 20.0f - 5.0f < obstacle.x + OBSTACLE_WIDTH) {
            if (game.helicopterY < obstacle.y - 10.0f || game.helicopterY > obstacle.y + 10.0f) {
                game.crashes++;
                if (game.crashes >= MAX_CRASHES) {
                    game.isGameOver = true;
                } else {
                    game.helicopterY = 50.0f;
                    game.velocity = 0.0f;
                }
            }
        }
    }
}

// Obstacle Movement
void updateObstacles() {
    for (auto& obstacle : obstacles) {
        obstacle.x -= game.obstacleSpeed;

        // Check if the helicopter passes the obstacle successfully
        if (obstacle.x < 20.0f && obstacle.x + OBSTACLE_WIDTH > 20.0f) {
            game.score++;
        }

        // Reset the obstacle position when it goes off-screen
        if (obstacle.x < -OBSTACLE_WIDTH) {
            obstacle.x = WORLD_WIDTH;
            obstacle.y = static_cast<float>(rand() % 60 + 20);
        }
    }
}

// Physics Update
void updatePhysics() {
    if (game.isGameOver) return;

    game.velocity += GRAVITY;
    if (game.velocity > MAX_SPEED) game.velocity = MAX_SPEED;

    game.helicopterY += game.velocity;

    if (game.helicopterY < 0.0f || game.helicopterY > WORLD_HEIGHT) {
        game.crashes++;
        if (game.crashes >= MAX_CRASHES) {
            game.isGameOver = true;
        } else {
            game.helicopterY = 50.0f;
            game.velocity = 0.0f;
        }
    }

    game.bladeRotation += 10.0f;
    checkCollision();
}

// Keyboard Input
void keyboard(unsigned char key, int x, int y) {
    if (game.isGameOver && tolower(key) == 'f') {
        initGame();
        return;
    }
    if (!game.isGameOver && tolower(key) == ' ') {
        game.velocity = -LIFT_FORCE;
    }
}

// Main Update Loop
void update(int value) {
    if (!game.isGameOver) {
        updatePhysics();
        updateObstacles();  // Move obstacles
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);  // Sky Blue background

    drawHelicopter();
    drawObstacles(); // Add this to render obstacles
    drawHUD();

    glutSwapBuffers();
}

// Window Resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WORLD_WIDTH, 0.0, WORLD_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

// Main Function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Helicopter Game - Retro Style");

    srand(static_cast<unsigned>(time(nullptr)));
    initGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
