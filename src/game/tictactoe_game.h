#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

#define MAX_NUM_FRAME_DATA 20
#define WALK_SPEED 100.0f

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
    ACTION_STATE_ATTACKING
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
};

struct tictactoe_input {
    bool left;
    bool leftJustPressed;
    bool right;
    bool rightJustPressed;
    bool punch;
    bool punchJustPressed;
};

struct tictactoe_game {
    character_animation_data animations[MAX_NUM_FRAME_DATA];
    int numAnimationDatas;

    tictactoe_game_state state;

    character_state oPlayerState;
    character_state xPlayerState;

    float cameraX;
};

#endif
