#include <stdlib.h>
#include <raylib.h>
#include "../modes.h"
#include "play.h"

static modeFunctions* modeFuncs;

// Initialize everything here
modeFunctions* initGame()
{
    // Initialize all the callback functions
    modeFuncs = malloc(sizeof(modeFunctions));
    modeFuncs->update = update;
    modeFuncs->draw = draw;
    //modeFuncs->destroy = destroy;
    //modeFuncs->done = done;

    // Initialize other stuff
    camera.position = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_CUSTOM);

    tankModel = LoadModel("art/models/tank.obj");
    tankTexture = LoadTexture("art/textures/tank.png");
    SetMaterialTexture(&tankModel.materials[0], MAP_DIFFUSE, tankTexture);

    groundModel = LoadModel("art/models/ground.obj");
    groundTexture = LoadTexture("art/textures/ground.png");
    SetMaterialTexture(&groundModel.materials[0], MAP_DIFFUSE, groundTexture);

    mainShader = LoadShader("shaders/jitter.vs", "shaders/fog.fs");

    int fogColorLoc = GetShaderLocation(mainShader, "fogColor");
    int fogDensityLoc = GetShaderLocation(mainShader, "fogDensity");
    Vector4 skyColor = (Vector4){SKY_COLOR.r/255.0f, SKY_COLOR.g/255.0f, SKY_COLOR.b/255.0f, 1.0f};
    SetShaderValueV(mainShader, fogColorLoc, &skyColor, UNIFORM_VEC4, 1);
    SetShaderValue(mainShader, fogDensityLoc, &fogDensity, UNIFORM_FLOAT);

    tankModel.materials[0].shader = mainShader;
    groundModel.materials[0].shader = mainShader;

    flagsTexture = LoadTexture("art/textures/flags.png");

    player.position = (Vector3){0.0f, 0.02f, 0.0f};
    player.velocity = (Vector2){0.0f, 0.0f};
    player.rotation = 0.0f;
    player.acceleration = 1.0f;
    player.rotationSpeed = 8.0f;
    player.friction = 16.0f;
    player.model = tankModel;

    muzzleTexture = LoadTexture("art/textures/particles.png");
    muzzleFrame = 0; // When at 0, it's paused

    bulletModel = LoadModel("art/models/bullet.obj");
    bulletTexture = LoadTexture("art/textures/bullet.png");
    SetMaterialTexture(&bulletModel.materials[0], MAP_DIFFUSE, bulletTexture);
    bulletModel.materials[0].shader = mainShader;


    // Return is a keyword which
    // makes the function return
    // something, dumbass
    return modeFuncs;
}

static void update(float dt)
{
    movePlayer(dt);
    movePlayerCamera(dt);
    playerShoot();
    updateShootParticles(dt);
}

static void draw()
{
    UpdateCamera(&camera);
    BeginMode3D(camera);
    BeginShaderMode(mainShader);

    ClearBackground(SKY_COLOR);
    DrawModel(groundModel, (Vector3){0.0f, 0.0f, 0.0f}, 64.0f, WHITE);
    DrawModelEx(
        player.model, 
        player.position, 
        (Vector3){0.0f, 1.0f, 0.0f}, 
        player.rotation, 
        (Vector3){0.3f, 0.3f, 0.3f}, 
        WHITE);

    if (muzzleFrame != 0)
    {
        DrawBillboardRec(
        camera, 
        muzzleTexture, 
        (Rectangle){muzzleFrame * 64 - 64, 0, 64, 64},
        (Vector3){
            player.position.x + sin(deg(player.rotation)), 
            player.position.y + 0.7f, 
            player.position.z + cos(deg(player.rotation))}, 
        0.8f, 
        WHITE);
    }

    for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i] != NULL)
    {
        DrawModelEx(
            bulletModel,
            (Vector3){bullets[i]->position.x, bullets[i]->height, bullets[i]->position.y},
            (Vector3){0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3){0.2f, 0.2f, 0.2f},
            WHITE
        );
    }

    /* DrawBillboardRec(
        camera, 
        flagsTexture, 
        (Rectangle){0, 0, 16, 32},
        (Vector3){player.position.x, player.position.y + 1.0f, player.position.z}, 
        0.3f, 
        WHITE); */

    EndShaderMode();
    EndMode3D();
}


static void movePlayer(float dt)
{
    if (IsKeyDown(KEY_W)) {
        player.velocity.x += sin(deg(player.rotation)) * player.acceleration * dt;
        player.velocity.y += cos(deg(player.rotation)) * player.acceleration * dt;
    }
    if (IsKeyDown(KEY_S)) {
        player.velocity.x -= sin(deg(player.rotation)) * player.acceleration * dt;
        player.velocity.y -= cos(deg(player.rotation)) * player.acceleration * dt;
    }
    if (IsKeyDown(KEY_A)) {
        player.rotation += player.rotationSpeed * dt * 10;
    }
    if (IsKeyDown(KEY_D)) {
        player.rotation -= player.rotationSpeed * dt * 10;
    }

    player.position.x += player.velocity.x;
    player.position.z += player.velocity.y;

    player.velocity.x -= player.velocity.x * player.friction * dt;
    player.velocity.y -= player.velocity.y * player.friction * dt;
}

static void movePlayerCamera(float dt)
{
    float camMoveSpeed = 8.0f;
    float camRotSpeed = 8.0f;
    if (IsKeyDown(KEY_E)){
        camera.fovy += (24.0f - camera.fovy) * dt * 8;
    }
    else {
        camera.fovy += (60.0f - camera.fovy) * dt * 8;
    }
    Vector3 newCamPos = (Vector3){
        player.position.x - sin(deg(player.rotation)) * 2,
        player.position.y + 1.5f,
        player.position.z - cos(deg(player.rotation)) * 2
    };
    camera.position.x += (newCamPos.x - camera.position.x) * dt * camMoveSpeed;
    camera.position.y += (newCamPos.y - camera.position.y) * dt * camMoveSpeed;
    camera.position.z += (newCamPos.z - camera.position.z) * dt * camMoveSpeed;

    camera.target.x += (player.position.x - camera.target.x) * dt * camRotSpeed;
    camera.target.y += (player.position.y + 1.5f - camera.target.y) * dt * camRotSpeed;
    camera.target.z += (player.position.z - camera.target.z) * dt * camRotSpeed;

    /* camera.position.x -= sin(deg(player.rotation));
    camera.position.z -= cos(deg(player.rotation));

    camera.target = player.position;
    camera.position.x -= sin(deg(player.rotation));
    camera.position.z -= cos(deg(player.rotation));
    camera.target.y += 1.0f; */
}

static void playerShoot()
{  
    if (IsKeyPressed(KEY_SPACE))
    {
        muzzleFrame = 1;

        for (int i = 0; i < MAX_BULLETS; i++) 
        //if (bullets[i] != NULL)
        {
            printf("nyukkuh");
            bullets[i] = malloc(sizeof(Bullet));
            bullets[i] -> position.x = player.position.x + sin(deg(player.rotation)) / 4;
            bullets[i] -> position.y = player.position.z + cos(deg(player.rotation)) / 4;
            bullets[i] -> velocity = 24.0f;
            bullets[i] -> rotation = player.rotation;
            bullets[i] -> height = 0.5f;
    
            break;
        }
    }
}

static void updateShootParticles(float dt)
{
    if (muzzleFrame != 0)
    {
        muzzleTimer += dt;
        if (muzzleTimer >= muzzleFrameSpeed) {
            muzzleTimer -= muzzleFrameSpeed;
            muzzleFrame += 1;
            if (muzzleFrame >= muzzleFrameNum) {
                muzzleFrame = 0;
            }
        }
    }
    else {
        muzzleTimer = 0;
    }

    for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i] != NULL)
    {
        bullets[i] -> height -= dt * 1;
        bullets[i] -> position.x += sin(deg( bullets[i]->rotation )) * bullets[i]->velocity * dt;
        bullets[i] -> position.y += cos(deg( bullets[i]->rotation )) * bullets[i]->velocity * dt;
    }
}
