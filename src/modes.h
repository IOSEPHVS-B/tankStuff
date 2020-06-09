

// Globals
#define GAME_WIDTH 360
#define GAME_HEIGHT 240
float scale;

// Useful functions
#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))
#define deg(a) (a * (3.1416 / 180))

// All game modes
#define MODE_NUM 1
typedef enum Mode {
    modePlay = 0
} Mode;

// Gamemode functions
typedef void (*initFunc) ();
typedef void (*updateFunc) (float);
typedef void (*drawFunc) ();
typedef void (*freeFunc) ();
typedef bool (*doneFunc) ();

typedef struct modeFunctions
{
    updateFunc update;
    drawFunc draw;
    freeFunc destroy;
    doneFunc done;
} modeFunctions;


modeFunctions* initGame();


Mode currentMode;
