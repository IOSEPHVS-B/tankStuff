#include <raylib.h>

// Gameplay things

static Color SKY_COLOR = (Color){240, 216, 157, 255};

Camera3D camera = { 0 };

Model tankModel;
Texture2D tankTexture;

Model groundModel;
Texture2D groundTexture;

Shader mainShader;
float fogDensity = 0.8f;

Texture2D flagsTexture;

static void update(float);
static void draw();
static void movePlayer(float);
static void movePlayerCamera(float);
static void playerShoot();
static void updateShootParticles(float);


// Player things

struct Player {
    Vector3 position;
    Vector2 velocity;
    float acceleration;
    float maxSpeed;
    float rotation;
    float rotationSpeed;
    float friction;
    Model model;
} player;

int muzzleFrame;
float muzzleTimer;
float muzzleFrameSpeed = 0.075f;
int muzzleFrameNum = 6;
Texture2D muzzleTexture;


#define MAX_BULLETS 16

typedef struct Bullet {
    Vector2 position;
    float velocity;
    float rotation;
    float height;
} Bullet;

Bullet *bullets[MAX_BULLETS] = {NULL};

Model bulletModel;
Texture2D bulletTexture;