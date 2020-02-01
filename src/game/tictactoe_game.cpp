#include "tictactoe_game.h"

static void drawBox (sprite_list *spriteList, float originX, float originY, rectangle box, game_assets *assets, char *boxKey, int additionalOffsetX, int additionalOffsetY) {
    pushSpriteTransform(spriteList, Vector2((float)additionalOffsetX, (float)additionalOffsetY));
    pushSpriteTransform(spriteList, Vector2(originX, originY));
    pushSpriteTransform(spriteList, box.min);
    vector2 rectDims = box.max - box.min;
    vector2 rectScale = rectDims * (1.0f / 10.0f);
    matrix3x3 boxTransform = scaleMatrix3x3(rectScale.x, rectScale.y);

    pushSpriteMatrix(boxTransform, spriteList);

    addSprite(0.0f, 0.0f, assets, ATLAS_KEY_HITBOX_EDITOR, boxKey, spriteList, 0.0f, 0.0f, 1.0f, 0.0f, 0.3f);

    popSpriteMatrix(spriteList);
    popSpriteMatrix(spriteList);
    popSpriteMatrix(spriteList);
    popSpriteMatrix(spriteList);
}

void initTicTacToeGame (memory_arena *memory, game_assets *assets, tictactoe_game* tictactoeGame) {
    setRNGSeed(0); // TODO(ebuchholz): seed with time?

    *tictactoeGame = {};

    data_asset *hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_BLOCK_O];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_BLOCK_O]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_BLOCK_WALK_O];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_BLOCK_WALK_O]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_BLOCK_WALK_X];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_BLOCK_WALK_X]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_BLOCK_X];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_BLOCK_X]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_HIT_O];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_HIT_O]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_HIT_X];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_HIT_X]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_IDLE_0];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_IDLE_0]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_IDLE_X];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_IDLE_X]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_PUNCH_O];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_PUNCH_O]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_PUNCH_X];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_PUNCH_X]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_WALK_0];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_WALK_0]);
    tictactoeGame->numAnimationDatas++;

    hitboxData = assets->dataAssets[DATA_KEY_HITBOX_STANDING_WALK_X];
    loadHitboxData(assets, hitboxData->data, &tictactoeGame->animations[DATA_KEY_HITBOX_STANDING_WALK_X]);
    tictactoeGame->numAnimationDatas++;

    assert(tictactoeGame->numAnimationDatas <= MAX_NUM_FRAME_DATA);

    tictactoeGame->state = TICTACTOE_GAME_STATE_BATTLE;

    character_state *oPlayerState = &tictactoeGame->oPlayerState;
    oPlayerState->grounded = true;
    oPlayerState->x = 300.0f;
    oPlayerState->y = 180.0f;
    oPlayerState->animState.animationData = DATA_KEY_HITBOX_STANDING_WALK_0;

    tictactoeGame->cameraX = 384.0f;
}



void drawHitBoxes (character_frame_data *currentFrame, sprite_list *spriteList, game_assets *assets, float x, float y) {
    for (int i = 0; i < currentFrame->numHitboxes; ++i) {
        rectangle box = currentFrame->hitboxes[i];
        char *boxKey = "hitbox_frame_data";
        drawBox(spriteList, x, y, box, assets, boxKey, 0, 0);
    }
    for (int i = 0; i < currentFrame->numHurtboxes; ++i) {
        rectangle box = currentFrame->hurtboxes[i];
        char *boxKey = "hurtbox_frame_data";
        drawBox(spriteList, x, y, box, assets, boxKey, 0, 0);
    }
}

void getInputForPlayer (game_input *input, tictactoe_input *tttInput, int playerIndex) {
    if (playerIndex == 0) {
        tttInput->left = input->controllers[0].dPadLeft.down || input->controllers[0].leftStickLeft.down || input->aKey.down;
        tttInput->leftJustPressed = input->controllers[0].dPadLeft.justPressed || input->controllers[0].leftStickLeft.justPressed || input->aKey.justPressed;
        tttInput->right = input->controllers[0].dPadRight.down || input->controllers[0].leftStickRight.down || input->sKey.down;
        tttInput->rightJustPressed = input->controllers[0].dPadRight.justPressed || input->controllers[0].leftStickRight.justPressed || input->sKey.justPressed;
        tttInput->punch = input->controllers[0].aButton.down  || input->dKey.down;
        tttInput->punchJustPressed = input->controllers[0].aButton.justPressed  || input->dKey.justPressed;
    }
    else {

    }
}

void startCharacterAnimState (character_state *character, data_key dataKey) {
    animation_state *playerAnimState = &character->animState;
    playerAnimState->currentFrameStep = 0;
    playerAnimState->currentFrame = 0;
    playerAnimState->animationData = dataKey;
}

bool updateCharacterAnimState (character_state *character, tictactoe_game *tictactoeGame) {
    animation_state *playerAnimState = &character->animState;
    character_animation_data *animData = &tictactoeGame->animations[playerAnimState->animationData];
    character_frame_data *currentFrame = &animData->frames[playerAnimState->currentFrame];
    //addSprite(character->x + currentFrame->xOffset, character->y + currentFrame->yOffset, assets, ATLAS_KEY_GAME, currentFrame->frameKey, spriteList, 0.0f, 1.0f);

    //drawHitBoxes(currentFrame, spriteList, assets, character->x, character->y);

    bool animationComplete = false;
    ++playerAnimState->currentFrameStep;
    if (playerAnimState->currentFrameStep >= currentFrame->duration) {
        playerAnimState->currentFrameStep = 0;
        ++playerAnimState->currentFrame;
        if (playerAnimState->currentFrame == animData->numFrames) {
            playerAnimState->currentFrame = 0;
            animationComplete = true;
        }
    }
    return animationComplete;
}

void updateCharacter (character_state *character, game_input *input, int playerIndex, tictactoe_game *ttg) {
    tictactoe_input tttInput;
    getInputForPlayer(input, &tttInput, playerIndex);

    data_key animation = DATA_KEY_HITBOX_STANDING_IDLE_0;
    switch (character->state) {
        case ACTION_STATE_FREE: {
            animation = DATA_KEY_HITBOX_STANDING_IDLE_0;
            if (tttInput.punchJustPressed) {
                character->state = ACTION_STATE_ATTACKING;
                animation = DATA_KEY_HITBOX_STANDING_PUNCH_O;
            }
            else {
                bool walking = false;
                if (tttInput.left) {
                    walking = true;
                    character->x -= WALK_SPEED * DELTA_TIME;
                }
                if (tttInput.right) {
                    walking = true;
                    character->x += WALK_SPEED * DELTA_TIME;
                }
                if (walking) {
                    animation = DATA_KEY_HITBOX_STANDING_WALK_0;
                }
            }
        } break;
        case ACTION_STATE_ATTACKING: {
            // locked in attack animation
            animation = character->animState.animationData;
        } break;
    }

    if (animation != character->animState.animationData) {
        startCharacterAnimState(character, animation);
    }
    else {
        bool animComplete = updateCharacterAnimState(character, ttg);
        if (character->state == ACTION_STATE_ATTACKING && animComplete) {
            character->state = ACTION_STATE_FREE;
        }
    }

} 

void drawCharacter (character_state *character, game_input *input, tictactoe_game *tictactoeGame, sprite_list *spriteList, game_assets *assets) {
    animation_state *oPlayerAnimState = &character->animState;
    character_animation_data *animData = &tictactoeGame->animations[oPlayerAnimState->animationData];
    character_frame_data *currentFrame = &animData->frames[oPlayerAnimState->currentFrame];
    addSprite(character->x + currentFrame->xOffset, character->y + currentFrame->yOffset, assets, ATLAS_KEY_GAME, currentFrame->frameKey, spriteList, 0.0f, 1.0f);

    //drawHitBoxes(currentFrame, spriteList, assets, character->x, character->y);
} 

void updateTicTacToeGame (memory_arena *memory, memory_arena *tempMemory, 
                          game_assets *assets, game_input *input, sprite_list *spriteList, tictactoe_game *tictactoeGame)
{
    //static int frame = 0;
    //static int frameStep = 0;

    // memory for dynamically created strings
    memory_arena stringMemory = {};
    stringMemory.capacity = 512 * 1024;
    stringMemory.base = allocateMemorySize(tempMemory, stringMemory.capacity);

    switch (tictactoeGame->state) {
        case TICTACTOE_GAME_STATE_TIC_TAC_TOE: {

        } break;
        case TICTACTOE_GAME_STATE_BATTLE: {
            pushSpriteTransform(spriteList, Vector2(-(tictactoeGame->cameraX - 192.0f), 0.0f));
            addSprite(0.0f, 0.0f, assets, ATLAS_KEY_GAME, "arena", spriteList);

            character_state *oPlayerState = &tictactoeGame->oPlayerState;
            updateCharacter(oPlayerState, input, 0, tictactoeGame);
            drawCharacter(oPlayerState, input, tictactoeGame, spriteList, assets);
            popSpriteMatrix(spriteList);
        } break;
    }
}
