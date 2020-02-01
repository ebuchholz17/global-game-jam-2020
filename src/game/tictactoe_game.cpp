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
    oPlayerState->facing = CHARACTER_FACING_RIGHT;

    character_state *xPlayerState = &tictactoeGame->xPlayerState;
    xPlayerState->grounded = true;
    xPlayerState->x = 468.0f;
    xPlayerState->y = 180.0f;
    xPlayerState->animState.animationData = DATA_KEY_HITBOX_STANDING_WALK_0;
    xPlayerState->facing = CHARACTER_FACING_LEFT;

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
        tttInput->left = input->controllers[1].dPadLeft.down || input->controllers[1].leftStickLeft.down || input->hKey.down;
        tttInput->leftJustPressed = input->controllers[1].dPadLeft.justPressed || input->controllers[1].leftStickLeft.justPressed || input->hKey.justPressed;
        tttInput->right = input->controllers[1].dPadRight.down || input->controllers[1].leftStickRight.down || input->jKey.down;
        tttInput->rightJustPressed = input->controllers[1].dPadRight.justPressed || input->controllers[1].leftStickRight.justPressed || input->jKey.justPressed;
        tttInput->punch = input->controllers[1].aButton.down  || input->kKey.down;
        tttInput->punchJustPressed = input->controllers[1].aButton.justPressed  || input->kKey.justPressed;
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

data_key getFrameName (data_key key, int playerIndex) {
    if (playerIndex == 0) {
        return key;
    }
    else {
        switch (key) {
        default:
        case DATA_KEY_HITBOX_STANDING_BLOCK_O:
            return DATA_KEY_HITBOX_STANDING_BLOCK_X;
        case DATA_KEY_HITBOX_STANDING_BLOCK_WALK_O:
            return DATA_KEY_HITBOX_STANDING_BLOCK_WALK_X;
        case DATA_KEY_HITBOX_STANDING_HIT_O:
            return  DATA_KEY_HITBOX_STANDING_HIT_X;
        case DATA_KEY_HITBOX_STANDING_IDLE_0:
            return  DATA_KEY_HITBOX_STANDING_IDLE_X;
        case DATA_KEY_HITBOX_STANDING_PUNCH_O:
            return  DATA_KEY_HITBOX_STANDING_PUNCH_X;
        case DATA_KEY_HITBOX_STANDING_WALK_0:
            return  DATA_KEY_HITBOX_STANDING_WALK_X;
        }
    }
}

void updateCharacter (character_state *character, game_input *input, int playerIndex, tictactoe_game *ttg) {
    tictactoe_input tttInput = {};
    getInputForPlayer(input, &tttInput, playerIndex);

    data_key animation = getFrameName(DATA_KEY_HITBOX_STANDING_IDLE_0, playerIndex);
    switch (character->state) {
        case ACTION_STATE_FREE: {
            animation = getFrameName(DATA_KEY_HITBOX_STANDING_IDLE_0, playerIndex);
            character->blocking = false;
            if (tttInput.punchJustPressed) {
                character->state = ACTION_STATE_ATTACKING;
                animation = getFrameName(DATA_KEY_HITBOX_STANDING_PUNCH_O, playerIndex);
            }
            else {
                if (tttInput.left) {
                    character->x -= WALK_SPEED * DELTA_TIME;
                    if (character->facing == CHARACTER_FACING_RIGHT) {
                        character->blocking = true;
                        animation = getFrameName(DATA_KEY_HITBOX_STANDING_BLOCK_WALK_O, playerIndex);
                    }
                    else {
                        animation = getFrameName(DATA_KEY_HITBOX_STANDING_WALK_0, playerIndex);
                    }
                }
                if (tttInput.right) {
                    character->x += WALK_SPEED * DELTA_TIME;
                    if (character->facing == CHARACTER_FACING_LEFT) {
                        character->blocking = true;
                        animation = getFrameName(DATA_KEY_HITBOX_STANDING_BLOCK_WALK_O, playerIndex);
                    }
                    else {
                        animation = getFrameName(DATA_KEY_HITBOX_STANDING_WALK_0, playerIndex);
                    }
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

    if (character->facing == CHARACTER_FACING_LEFT) {
        matrix3x3 posMatrix = translationMatrix(character->x - currentFrame->xOffset, character->y - currentFrame->yOffset);
        pushSpriteMatrix(posMatrix, spriteList);
        matrix3x3 scaleTransform = scaleMatrix3x3(-1.0f, 1.0f);
        pushSpriteMatrix(scaleTransform, spriteList);

        addSprite(0.0f, 0.0f, assets, ATLAS_KEY_GAME, currentFrame->frameKey, spriteList, 0.0f, 1.0f);
        drawHitBoxes(currentFrame, spriteList, assets, (float)-currentFrame->xOffset, (float)-currentFrame->yOffset);
        popSpriteMatrix(spriteList);
        popSpriteMatrix(spriteList);
    }
    else {
        addSprite(character->x + currentFrame->xOffset, character->y + currentFrame->yOffset, assets, ATLAS_KEY_GAME, currentFrame->frameKey, spriteList, 0.0f, 1.0f);
        drawHitBoxes(currentFrame, spriteList, assets, character->x, character->y);
    }
} 

void checkHitBoxIntersections (character_state *oPlayerState, character_state *xPlayerState, 
                               tictactoe_game *tttG, sprite_list* spriteList, 
                               game_assets *assets, bool *oHit, bool *xHit) 
{
    animation_state *oPlayerAnimState = &oPlayerState->animState;
    character_animation_data *oAnimData = &tttG->animations[oPlayerAnimState->animationData];
    character_frame_data *oCurrentFrame = &oAnimData->frames[oPlayerAnimState->currentFrame];

    animation_state *xPlayerAnimState = &xPlayerState->animState;
    character_animation_data *xAnimData = &tttG->animations[xPlayerAnimState->animationData];
    character_frame_data *xCurrentFrame = &xAnimData->frames[xPlayerAnimState->currentFrame];

    matrix3x3 oTransform;
    if (oPlayerState->facing == CHARACTER_FACING_RIGHT) {
        oTransform = translationMatrix(oPlayerState->x, oPlayerState->y);
    }
    else {
        matrix3x3 translation = translationMatrix(oPlayerState->x, oPlayerState->y);
        matrix3x3 scaling = scaleMatrix3x3(-1.0f, 1.0f); 
        matrix3x3 translation2 = translationMatrix((float)-oCurrentFrame->xOffset, (float)-oCurrentFrame->yOffset);
        oTransform = translation * scaling;
    }
    matrix3x3 xTransform;
    if (xPlayerState->facing == CHARACTER_FACING_RIGHT) {
        xTransform = translationMatrix(xPlayerState->x, xPlayerState->y);
    }
    else {
        matrix3x3 translation = translationMatrix(xPlayerState->x, xPlayerState->y);
        matrix3x3 scaling = scaleMatrix3x3(-1.0f, 1.0f); 
        matrix3x3 translation2 = translationMatrix((float)-xCurrentFrame->xOffset, (float)-xCurrentFrame->yOffset);
        xTransform = translation * scaling;
    }
    // p1 hitboxes against p2 hitboxes
    for (int i = 0; i < oCurrentFrame->numHitboxes; ++i) {
        rectangle hitbox = oCurrentFrame->hitboxes[i];
        vector2 min = oTransform * hitbox.min;
        vector2 max = oTransform * hitbox.max;

        rectangle newHitbox;
        newHitbox.min.x = min.x < max.x ? min.x : max.x;
        newHitbox.max.x = min.x > max.x ? min.x : max.x;
        newHitbox.min.y = min.y < max.y ? min.y : max.y;
        newHitbox.max.y = min.y > max.y ? min.y : max.y;
        for (int j = 0; j < xCurrentFrame->numHurtboxes; ++j) {
            rectangle hurtbox = xCurrentFrame->hurtboxes[j];
            rectangle newHurtbox;
            vector2 hurtMin = xTransform * hurtbox.min;
            vector2 hurtMax = xTransform * hurtbox.max;
            newHurtbox.min.x = hurtMin.x < hurtMax.x ? hurtMin.x : hurtMax.x;
            newHurtbox.max.x = hurtMin.x > hurtMax.x ? hurtMin.x : hurtMax.x;
            newHurtbox.min.y = hurtMin.y < hurtMax.y ? hurtMin.y : hurtMax.y;
            newHurtbox.max.y = hurtMin.y > hurtMax.y ? hurtMin.y : hurtMax.y;

            if (rectangleIntersection(newHitbox, newHurtbox)) {
                drawBox(spriteList, 0.0f, 0.0f, newHurtbox, assets,    "hitbox_frame_data", 0, 0);
                *xHit = true;
            }
        }
    }
    for (int i = 0; i < xCurrentFrame->numHitboxes; ++i) {
        rectangle hitbox = xCurrentFrame->hitboxes[i];
        vector2 min = xTransform * hitbox.min;
        vector2 max = xTransform * hitbox.max;

        rectangle newHitbox;
        newHitbox.min.x = min.x < max.x ? min.x : max.x;
        newHitbox.max.x = min.x > max.x ? min.x : max.x;
        newHitbox.min.y = min.y < max.y ? min.y : max.y;
        newHitbox.max.y = min.y > max.y ? min.y : max.y;
        for (int j = 0; j < oCurrentFrame->numHurtboxes; ++j) {
            rectangle hurtbox = oCurrentFrame->hurtboxes[j];
            rectangle newHurtbox;
            vector2 hurtMin = oTransform * hurtbox.min;
            vector2 hurtMax = oTransform * hurtbox.max;
            newHurtbox.min.x = hurtMin.x < hurtMax.x ? hurtMin.x : hurtMax.x;
            newHurtbox.max.x = hurtMin.x > hurtMax.x ? hurtMin.x : hurtMax.x;
            newHurtbox.min.y = hurtMin.y < hurtMax.y ? hurtMin.y : hurtMax.y;
            newHurtbox.max.y = hurtMin.y > hurtMax.y ? hurtMin.y : hurtMax.y;

            if (rectangleIntersection(newHitbox, newHurtbox)) {
                drawBox(spriteList, 0.0f, 0.0f, newHurtbox, assets,    "hitbox_frame_data", 0, 0);
                *oHit = true;
            }
        }
    }
}

void onPlayerHit (character_state *character, int playerIndex, tictactoe_game *ttg) {

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
            character_state *xPlayerState = &tictactoeGame->xPlayerState;
            updateCharacter(oPlayerState, input, 0, tictactoeGame);
            updateCharacter(xPlayerState, input, 1, tictactoeGame);
            drawCharacter(oPlayerState, input, tictactoeGame, spriteList, assets);
            drawCharacter(xPlayerState, input, tictactoeGame, spriteList, assets);

            bool oHit = false;
            bool xHit = false;
            checkHitBoxIntersections(oPlayerState, xPlayerState, tictactoeGame, spriteList, assets, &oHit, &xHit);

            if (oHit) {
                onPlayerHit(oPlayerState, 0, tictactoeGame);
            }
            if (xHit) {
                onPlayerHit(xPlayerState, 1, tictactoeGame);
            }
            //addSprite(oPlayerState->x, oPlayerState->y, assets, ATLAS_KEY_HITBOX_EDITOR,   "hitbox_frame_data", spriteList);
            //addSprite(xPlayerState->x, xPlayerState->y, assets, ATLAS_KEY_HITBOX_EDITOR,  "hitbox_frame_data", spriteList);
            popSpriteMatrix(spriteList);

        } break;
    }
}
