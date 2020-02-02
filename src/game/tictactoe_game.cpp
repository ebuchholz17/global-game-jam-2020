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
    setRNGSeed((unsigned int)((unsigned long long)memory)); // TODO(ebuchholz): seed with time?

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

    tictactoeGame->state = TICTACTOE_GAME_STATE_TIC_TAC_TOE;
    //tictactoeGame->state = TICTACTOE_GAME_STATE_BATTLE;

    tictactoe_state *ttt = &tictactoeGame->tttState;
    ttt->phase = TICTACTOE_PHASE_CHOOSING_CELL;
    ttt->oTurn = randomFloat() > 0.5f;
    ttt->selectedCell = 4;

    character_state *oPlayerState = &tictactoeGame->oPlayerState;
    oPlayerState->grounded = true;
    oPlayerState->x = 300.0f;
    oPlayerState->y = 180.0f;
    oPlayerState->animState.animationData = DATA_KEY_HITBOX_STANDING_WALK_0;
    oPlayerState->facing = CHARACTER_FACING_RIGHT;
    oPlayerState->hitPoints = 100;

    character_state *xPlayerState = &tictactoeGame->xPlayerState;
    xPlayerState->grounded = true;
    xPlayerState->x = 468.0f;
    xPlayerState->y = 180.0f;
    xPlayerState->animState.animationData = DATA_KEY_HITBOX_STANDING_WALK_0;
    xPlayerState->facing = CHARACTER_FACING_LEFT;
    xPlayerState->hitPoints = 100;

    tictactoeGame->cameraX = 384.0f;
    tictactoeGame->battleState = BATTLE_STATE_NORMAL;
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

        tttInput->right = input->controllers[0].dPadRight.down || input->controllers[0].leftStickRight.down || input->dKey.down;
        tttInput->rightJustPressed = input->controllers[0].dPadRight.justPressed || input->controllers[0].leftStickRight.justPressed || input->dKey.justPressed;

        tttInput->up = input->controllers[0].dPadUp.down || input->controllers[0].leftStickUp.down || input->wKey.down;
        tttInput->upJustPressed = input->controllers[0].dPadUp.justPressed || input->controllers[0].leftStickUp.justPressed || input->wKey.justPressed;

        tttInput->down = input->controllers[0].dPadDown.down || input->controllers[0].leftStickDown.down || input->sKey.down;
        tttInput->downJustPressed = input->controllers[0].dPadDown.justPressed || input->controllers[0].leftStickDown.justPressed || input->sKey.justPressed;

        tttInput->punch = input->controllers[0].aButton.down  || input->fKey.down;
        tttInput->punchJustPressed = input->controllers[0].aButton.justPressed  || input->fKey.justPressed;
    }
    else {
        tttInput->left = input->controllers[1].dPadLeft.down || input->controllers[1].leftStickLeft.down || input->hKey.down;
        tttInput->leftJustPressed = input->controllers[1].dPadLeft.justPressed || input->controllers[1].leftStickLeft.justPressed || input->hKey.justPressed;

        tttInput->right = input->controllers[1].dPadRight.down || input->controllers[1].leftStickRight.down || input->kKey.down;
        tttInput->rightJustPressed = input->controllers[1].dPadRight.justPressed || input->controllers[1].leftStickRight.justPressed || input->kKey.justPressed;

        tttInput->up = input->controllers[1].dPadUp.down || input->controllers[1].leftStickUp.down || input->uKey.down;
        tttInput->upJustPressed = input->controllers[1].dPadUp.justPressed || input->controllers[1].leftStickUp.justPressed || input->uKey.justPressed;

        tttInput->down = input->controllers[1].dPadDown.down || input->controllers[1].leftStickDown.down || input->jKey.down;
        tttInput->downJustPressed = input->controllers[1].dPadDown.justPressed || input->controllers[1].leftStickDown.justPressed || input->jKey.justPressed;

        tttInput->punch = input->controllers[1].aButton.down  || input->gKey.down;
        tttInput->punchJustPressed = input->controllers[1].aButton.justPressed  || input->gKey.justPressed;
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
                character->attackHit = false;
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
        case ACTION_STATE_HITSTUN: {
            animation = getFrameName(DATA_KEY_HITBOX_STANDING_HIT_O, playerIndex);
            if (character->facing == CHARACTER_FACING_LEFT) {
                character->x += KNOCKBACK * DELTA_TIME;
            }
            else {
                character->x -= KNOCKBACK * DELTA_TIME;
            }
            character->timer++;
            if (character->timer >= 10) {
                character->state = ACTION_STATE_FREE;
            }
        } break;
        case ACTION_STATE_BLOCKSTUN: {
            animation = getFrameName(DATA_KEY_HITBOX_STANDING_BLOCK_O, playerIndex);
            character->timer++;
            if (character->timer >= 5) {
                character->state = ACTION_STATE_FREE;
            }
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
        //drawHitBoxes(currentFrame, spriteList, assets, (float)-currentFrame->xOffset, (float)-currentFrame->yOffset);
        popSpriteMatrix(spriteList);
        popSpriteMatrix(spriteList);
    }
    else {
        addSprite(character->x + currentFrame->xOffset, character->y + currentFrame->yOffset, assets, ATLAS_KEY_GAME, currentFrame->frameKey, spriteList, 0.0f, 1.0f);
        //drawHitBoxes(currentFrame, spriteList, assets, character->x, character->y);
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
                //drawBox(spriteList, 0.0f, 0.0f, newHurtbox, assets,    "hitbox_frame_data", 0, 0);
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
                //drawBox(spriteList, 0.0f, 0.0f, newHurtbox, assets,    "hitbox_frame_data", 0, 0);
                *oHit = true;
            }
        }
    }
}

void onPlayerHit (character_state *character, int playerIndex, tictactoe_game *ttg) {
    character->state = ACTION_STATE_HITSTUN;
    character->timer = 0;
    character->hitPoints -= 20;
    if (character->hitPoints <= 0) {
        character->hitPoints = 0;
    }
}

void onAttackBlocked (character_state *character, int playerIndex, tictactoe_game *ttg) {
    character->state = ACTION_STATE_BLOCKSTUN;
    character->timer = 0;
}

vector2 cellPos (int i) {
    int col = i % 3;
    int row = i / 3;
    return Vector2(117.0f + 70.0f * col, 50.0f + 65.0f * row);
}


int checkIfOpponentInAdjacentCell (tictactoe_state *tttState, int cellNum) {
    ttt_cell *cell = &tttState->board[cellNum];
    int cellValue = cell->value;
    int opponentValue = cellValue == 1 ? 2 : 1;

    int row = cellNum / 3;
    int col = cellNum % 3;

    // TODO(ebuchholz): allow multiple opponents
    if (row > 0) {
        int oppCellNum = (row-1) * 3 + col;
        ttt_cell *neighborCell = &tttState->board[oppCellNum];
        if (neighborCell->value == opponentValue) {
            return oppCellNum;
        }
    }
    if (row < 2) {
        int oppCellNum = (row+1) * 3 + col;
        ttt_cell *neighborCell = &tttState->board[oppCellNum];
        if (neighborCell->value == opponentValue) {
            return oppCellNum;
        }
    }
    if (col > 0) {
        int oppCellNum = row * 3 + col - 1;
        ttt_cell *neighborCell = &tttState->board[oppCellNum];
        if (neighborCell->value == opponentValue) {
            return oppCellNum;
        }
    }
    if (col < 2) {
        int oppCellNum = row * 3 + col + 1;
        ttt_cell *neighborCell = &tttState->board[oppCellNum];
        if (neighborCell->value == opponentValue) {
            return oppCellNum;
        }
    }
    return -1;
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
            tictactoe_state *tttState = &tictactoeGame->tttState;

            reticule_info reticuleInfo = {};
            status_text_info statusTextInfo = {};

            switch (tttState->phase) {
                case TICTACTOE_PHASE_CHOOSING_CELL: {
                    tictactoe_input tttInput;
                    int playerIndex = tttState->oTurn? 0 : 1;
                    getInputForPlayer(input, &tttInput, playerIndex);

                    if (tttInput.upJustPressed) {
                        int newSelectedCell = tttState->selectedCell - 3;
                        if (newSelectedCell < 0) {
                            newSelectedCell += 9;
                        }
                        tttState->selectedCell = newSelectedCell;
                    }
                    if (tttInput.downJustPressed) {
                        int newSelectedCell = tttState->selectedCell + 3;
                        if (newSelectedCell >= 9) {
                            newSelectedCell -= 9;
                        }
                        tttState->selectedCell = newSelectedCell;
                    }
                    if (tttInput.leftJustPressed) {
                        int newSelectedCell = tttState->selectedCell - 1;
                        if (newSelectedCell < 0) {
                            newSelectedCell += 9;
                        }
                        tttState->selectedCell = newSelectedCell;
                    }
                    if (tttInput.rightJustPressed) {
                        int newSelectedCell = tttState->selectedCell + 1;
                        if (newSelectedCell >= 9) {
                            newSelectedCell -= 9;
                        }
                        tttState->selectedCell = newSelectedCell;
                    }

                    if (tttInput.punchJustPressed) {

                        ttt_cell *cell = &tttState->board[tttState->selectedCell];
                        if (cell->value == 0) {
                            cell->value = playerIndex + 1;
                            cell->health = 100;
                            tttState->oTurn = !tttState->oTurn;
                            int oppCell = checkIfOpponentInAdjacentCell(tttState, tttState->selectedCell);
                            // go to battle
                            if (oppCell != -1) {
                                tictactoeGame->state = TICTACTOE_GAME_STATE_BATTLE;
                                tictactoeGame->battleState = BATTLE_STATE_STARTED;
                                if (playerIndex == 0) {
                                    tictactoeGame->battleResult.oNum = tttState->selectedCell;
                                    tictactoeGame->battleResult.xNum = oppCell;

                                    ttt_cell *oCell = &tttState->board[tttState->selectedCell];
                                    tictactoeGame->battleResult.oHealth = oCell->health;

                                    ttt_cell *xCell = &tttState->board[oppCell];
                                    tictactoeGame->battleResult.xHealth = xCell->health;
                                }
                                else {
                                    tictactoeGame->battleResult.oNum = oppCell;
                                    tictactoeGame->battleResult.xNum = tttState->selectedCell;
                                    
                                    ttt_cell *oCell = &tttState->board[oppCell];
                                    tictactoeGame->battleResult.oHealth = oCell->health;

                                    ttt_cell *xCell = &tttState->board[tttState->selectedCell];
                                    tictactoeGame->battleResult.xHealth = xCell->health;
                                }
                            }
                            tttState->selectedCell = 4;
                        }
                    }

                    statusTextInfo.draw = true;
                    reticuleInfo.draw = true;
                    if (tttState->oTurn) {
                        reticuleInfo.key = "blue_reticule";
                        statusTextInfo.text = "P1's turn";
                    }
                    else {
                        reticuleInfo.key = "red_reticule";
                        statusTextInfo.text = "P2's turn";
                    }
                    reticuleInfo.cellNum = tttState->selectedCell;

                } break;
                case TICTACTOE_PHASE_BATTLE_OVER: {
                    battle_result *result = &tictactoeGame->battleResult;
                    int victimCell = result->oWin ? result->xNum : result->oNum;
                    int otherCell = result->oWin ? result->oNum : result->xNum;
                    int otherCellHealth = result->oWin ? result->oHealth : result->xHealth;

                    ttt_cell *cell = &tttState->board[victimCell];
                    cell->value = 0;
                    cell = &tttState->board[otherCell];
                    cell->health = otherCellHealth;
                    
                    tttState->phase = TICTACTOE_PHASE_CHOOSING_CELL;
                } break;
            }
            
            addSprite(0.0f, 0.0f, assets, ATLAS_KEY_GAME, "tictactoe_backing", spriteList);
            addSprite(10.0f, 5.0f, assets, ATLAS_KEY_GAME, "ui", spriteList);

            for (int i = 0; i < 9; ++i) {
                vector2 pos = cellPos(i);
                ttt_cell *cell = &tttState->board[i];
                if (cell->value != 0) {

                    if (cell->value == 1) {
                        addSprite(pos.x, pos.y, assets, ATLAS_KEY_GAME, "o", spriteList, 0.5f, 0.5f);
                    }
                    else {
                        addSprite(pos.x, pos.y, assets, ATLAS_KEY_GAME, "x", spriteList, 0.5f, 0.5f);
                    }
                }
            }

            if (reticuleInfo.draw) {
                vector2 pos = cellPos(reticuleInfo.cellNum);
                addSprite(pos.x, pos.y- 9.0f, assets, ATLAS_KEY_GAME, reticuleInfo.key, spriteList, 0.5f, 0.5f);
            }

            if (statusTextInfo.draw) {
                addText(150, 5, statusTextInfo.text, assets, TEXTURE_KEY_FONT_2, spriteList);
            }

        } break;
        case TICTACTOE_GAME_STATE_BATTLE: {
            character_state *oPlayerState = &tictactoeGame->oPlayerState;
            character_state *xPlayerState = &tictactoeGame->xPlayerState;
            switch (tictactoeGame->battleState) {
                case BATTLE_STATE_STARTED: {
                    oPlayerState->grounded = true;
                    oPlayerState->x = 300.0f;
                    oPlayerState->y = 180.0f;
                    oPlayerState->animState.animationData = DATA_KEY_HITBOX_STANDING_WALK_0;
                    oPlayerState->facing = CHARACTER_FACING_RIGHT;
                    oPlayerState->hitPoints = tictactoeGame->battleResult.oHealth;
                    oPlayerState->state = ACTION_STATE_FREE;
                    oPlayerState->blocking = false;

                    xPlayerState->grounded = true;
                    xPlayerState->x = 468.0f;
                    xPlayerState->y = 180.0f;
                    xPlayerState->animState.animationData = DATA_KEY_HITBOX_STANDING_WALK_0;
                    xPlayerState->facing = CHARACTER_FACING_LEFT;
                    xPlayerState->hitPoints = tictactoeGame->battleResult.xHealth;
                    xPlayerState->state = ACTION_STATE_FREE;
                    xPlayerState->blocking = false;

                    tictactoeGame->cameraX = 384.0f;
                    tictactoeGame->battleState = BATTLE_STATE_NORMAL;
                } break;
                case BATTLE_STATE_NORMAL: {
                    updateCharacter(oPlayerState, input, 0, tictactoeGame);
                    updateCharacter(xPlayerState, input, 1, tictactoeGame);

                    float minCharX = oPlayerState->x < xPlayerState->x ? oPlayerState->x : xPlayerState-> x;
                    float maxCharX = oPlayerState->x > xPlayerState->x ? oPlayerState->x : xPlayerState-> x;

                    bool minCharOutOfCam = false;
                    bool maxCharOutOfCam = false;
                    if (maxCharX - tictactoeGame->cameraX > 100.0f) {
                        maxCharOutOfCam = true;
                    }
                    if (minCharX - tictactoeGame->cameraX < -100.0f) {
                        minCharOutOfCam = true;
                    }

                    if (minCharOutOfCam && maxCharOutOfCam) {
                        // do nothing
                    }
                    else if (minCharOutOfCam) {
                        if (maxCharX - minCharX > 200.0f) {
                            float cameraX = tictactoeGame->cameraX;
                            float offset = (cameraX + 100.0f) - maxCharX;
                            tictactoeGame->cameraX -= offset;
                        }
                        else {
                            tictactoeGame->cameraX = tictactoeGame->cameraX + (minCharX- (tictactoeGame->cameraX- 100.0f));
                        }
                    }
                    else if (maxCharOutOfCam) {
                        if (maxCharX - minCharX > 200.0f) {
                            float cameraX = tictactoeGame->cameraX;
                            float offset = minCharX - (cameraX - 100.0f);
                            tictactoeGame->cameraX += offset;
                        }
                        else {
                            tictactoeGame->cameraX = tictactoeGame->cameraX + (maxCharX- (tictactoeGame->cameraX+100.0f));
                        }
                    }
                    if (tictactoeGame->cameraX < 192.0f) {
                        tictactoeGame->cameraX = 192.0f;
                    }
                    if (tictactoeGame->cameraX > 575.0f) {
                        tictactoeGame->cameraX = 575.0f;
                    }

                    if (oPlayerState->x > tictactoeGame->cameraX + 170.0f) {
                        oPlayerState->x = tictactoeGame->cameraX + 170.0f;
                    }
                    if (oPlayerState->x < tictactoeGame->cameraX - 170.0f) {
                        oPlayerState->x = tictactoeGame->cameraX - 170.0f;
                    }
                    if (xPlayerState->x > tictactoeGame->cameraX + 170.0f) {
                        xPlayerState->x = tictactoeGame->cameraX + 170.0f;
                    }
                    if (xPlayerState->x < tictactoeGame->cameraX - 170.0f) {
                        xPlayerState->x = tictactoeGame->cameraX - 170.0f;
                    }

                    bool oHit = false;
                    bool xHit = false;
                    checkHitBoxIntersections(oPlayerState, xPlayerState, tictactoeGame, spriteList, assets, &oHit, &xHit);

                    bool gameOver = false;
                    bool oIsWinner = false;
                    if (oHit && !xPlayerState->attackHit) {
                        xPlayerState->attackHit = true;
                        if (oPlayerState->blocking) {
                            onAttackBlocked(oPlayerState, 0, tictactoeGame);
                        }
                        else{
                            onPlayerHit(oPlayerState, 0, tictactoeGame);
                            if (oPlayerState->hitPoints == 0) {
                                gameOver = true;
                                oIsWinner = false;
                            }
                        }
                    }
                    if (xHit && !oPlayerState->attackHit) {
                        oPlayerState->attackHit = true;
                        if (xPlayerState->blocking) {
                            onAttackBlocked(xPlayerState, 1, tictactoeGame);
                        }
                        else{
                            onPlayerHit(xPlayerState, 1, tictactoeGame);
                            if (xPlayerState->hitPoints == 0) {
                                gameOver = true;
                                oIsWinner = true;
                            }
                        }
                    }

                    if (gameOver) {
                        tictactoeGame->battleState = BATTLE_STATE_OVER;
                        tictactoeGame->oWonFight = oIsWinner;
                        tictactoeGame->timer = 0.0f;
                        tictactoeGame->vel = 0.0f;
                    }
                } break;
                case BATTLE_STATE_OVER: {
                    character_state *loser;
                    int index;
                    if (tictactoeGame->oWonFight) {
                        loser = xPlayerState;
                        index = 1;
                    }
                    else {
                        loser = oPlayerState;
                        index = 0;
                    }

                    data_key animation = getFrameName(DATA_KEY_HITBOX_STANDING_HIT_O, index);
                    if (animation != loser->animState.animationData) {
                        startCharacterAnimState(loser, animation);
                    }
                    else {
                        updateCharacterAnimState(loser, tictactoeGame);
                    }

                    tictactoeGame->vel += 800.0f * DELTA_TIME;
                    loser->y += tictactoeGame->vel * DELTA_TIME;
                    tictactoeGame->timer += DELTA_TIME;
                    if (tictactoeGame->timer >= 1.0f) {
                        tictactoeGame->state = TICTACTOE_GAME_STATE_TIC_TAC_TOE;

                        tictactoeGame->battleResult.oWin = tictactoeGame->oWonFight;
                        tictactoeGame->battleResult.oHealth = oPlayerState->hitPoints;
                        tictactoeGame->battleResult.xHealth = xPlayerState->hitPoints;
                        tictactoeGame->tttState.phase = TICTACTOE_PHASE_BATTLE_OVER;
                    }
                } break;
            }
            pushSpriteTransform(spriteList, Vector2(-(tictactoeGame->cameraX - 192.0f), 0.0f));
            addSprite(0.0f, 0.0f, assets, ATLAS_KEY_GAME, "arena", spriteList);

            drawCharacter(oPlayerState, input, tictactoeGame, spriteList, assets);
            drawCharacter(xPlayerState, input, tictactoeGame, spriteList, assets);

            //addSprite(oPlayerState->x, oPlayerState->y, assets, ATLAS_KEY_HITBOX_EDITOR,   "hitbox_frame_data", spriteList);
            //addSprite(xPlayerState->x, xPlayerState->y, assets, ATLAS_KEY_HITBOX_EDITOR,  "hitbox_frame_data", spriteList);
            popSpriteMatrix(spriteList);

            // draw hitpoints
            addSprite(10.0f, 10.0f, assets, ATLAS_KEY_GAME, "o", spriteList, 0.0f, 0.0f, 0.5f);

            addSprite(374.0f, 10.0f, assets, ATLAS_KEY_GAME, "x", spriteList, 1.0f, 0.0f, 0.5f);

            matrix3x3 posMatrix = translationMatrix(40.0f, 10.0f);
            pushSpriteMatrix(posMatrix, spriteList);
            matrix3x3 scaleTransform = scaleMatrix3x3((1.0f / 10.0f) * ((float)oPlayerState->hitPoints), 1.0f);
            pushSpriteMatrix(scaleTransform, spriteList);
            addSprite(0.0f, 0.0f, assets, ATLAS_KEY_GAME, "hp_segment", spriteList, 0.0f, 0.0f);
            popSpriteMatrix(spriteList);
            popSpriteMatrix(spriteList);

            posMatrix = translationMatrix(344.0f, 10.0f);
            pushSpriteMatrix(posMatrix, spriteList);
            scaleTransform = scaleMatrix3x3(-(1.0f / 10.0f) * ((float)xPlayerState->hitPoints), 1.0f);
            pushSpriteMatrix(scaleTransform, spriteList);
            addSprite(0.0f, 0.0f, assets, ATLAS_KEY_GAME, "hp_segment", spriteList, 0.0f, 0.0f);
            popSpriteMatrix(spriteList);
            popSpriteMatrix(spriteList);
        } break;
    }
}
