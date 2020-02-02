#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

#define MAX_NUM_FRAME_DATA 20
#define WALK_SPEED 100.0f
#define KNOCKBACK 200.0f

enum tictactoe_game_state{
    TICTACTOE_GAME_STATE_TITLE,
    TICTACTOE_GAME_STATE_TIC_TAC_TOE,
    TICTACTOE_GAME_STATE_BATTLE,
    TICTACTOE_GAME_STATE_GAME_END
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
    bool kick;
    bool kickJustPressed;
};

enum battle_state {
    BATTLE_STATE_NORMAL,
    BATTLE_STATE_ZOOM,
    BATTLE_STATE_OVER,
    BATTLE_STATE_REPAIR,
    BATTLE_STATE_STARTED
};

enum tictactoe_phase {
    TICTACTOE_PHASE_CHOOSING_CELL,
    TICTACTOE_PHASE_BATTLE_OVER,
    TICTACTOE_PHASE_ZOOM,
    TICTACTOE_PHASE_FADE_CELLS,
    TICTACTOE_PHASE_DECIDE_REINFORCEMENT,
    TICTACTOE_PHASE_CHOOSE_REINFORCEMENT,
    TICTACTOE_PHASE_WIN_PAUSE
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

    vector2 zoomTarget;
    float zoomScale;
    float zoomTimer;
    float zoomT;
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
    float xOffset;
};

struct hand_draw_info {
    bool draw;
    vector2 pos;
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
    float zoomT;
    float vel;

    bool oNeighbors[9];
    bool xNeighbors[9];
    bool destroyedCells[9];
    bool blinkingCells[9];
    bool blinkOn;

    bool oWinsWholeGame;

    bool repairPlayer;
    int targetRepairPlayer;
    int targetRepairHealth;
};

#endif
