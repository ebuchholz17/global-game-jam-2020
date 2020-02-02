#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

#define MAX_NUM_FRAME_DATA 20
#define WALK_SPEED 100.0f
#define KNOCKBACK 200.0f

enum tictactoe_game_state{
    TICTACTOE_GAME_STATE_TIC_TAC_TOE,
    TICTACTOE_GAME_STATE_BATTLE
};

struct animation_state {
    data_key animationData;
    int currentFrame;
    int currentFrameStep;
};

enum action_state {
    ACTION_STATE_FREE,
    ACTION_STATE_ATTACKING,
    ACTION_STATE_HITSTUN,
    ACTION_STATE_BLOCKSTUN,
};

enum character_facing {
    CHARACTER_FACING_LEFT,
    CHARACTER_FACING_RIGHT
};

struct character_state {
    animation_state animState;
    action_state state;
    float x;
    float y;

    bool grounded;
    bool blocking;
    character_facing facing;
    int timer;

    int hitPoints;
    bool attackHit;
};

struct tictactoe_input {
    bool left;
    bool leftJustPressed;
    bool right;
    bool rightJustPressed;
    bool up;
    bool upJustPressed;
    bool down;
    bool downJustPressed;
    bool punch;
    bool punchJustPressed;
};

enum battle_state {
    BATTLE_STATE_NORMAL,
    BATTLE_STATE_OVER,
    BATTLE_STATE_STARTED
};

enum tictactoe_phase {
    TICTACTOE_PHASE_CHOOSING_CELL,
    TICTACTOE_PHASE_BATTLE_OVER
};

struct ttt_cell {
    int value;
    bool defeated;
    int health;
};

struct tictactoe_state {
    bool oTurn;
    tictactoe_phase phase;

    int selectedCell;
    ttt_cell board[9];

    bool oHasBackup;
    bool xHasBackup;
};

struct battle_result {
    int oNum;
    int xNum;

    bool oWin;
    int oHealth;
    int xHealth;
};

struct reticule_info {
    bool draw;
    char *key;
    int cellNum;
};

struct status_text_info {
    bool draw;
    char *text;
};

struct tictactoe_game {
    tictactoe_state tttState;

    character_animation_data animations[MAX_NUM_FRAME_DATA];
    int numAnimationDatas;

    tictactoe_game_state state;

    character_state oPlayerState;
    character_state xPlayerState;

    battle_state battleState;
    battle_result battleResult;

    float cameraX;
    bool oWonFight;
    float timer;
    float vel;
};

#endif
