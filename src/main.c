#include <genesis.h>

#include "main.h"
#include "res_gfx.h"
#include "res_snd.h"

#define PLAYER_IDLE 0
#define PLAYER_WALK 1
#define PLAYER_RUN 2


static Sprite *player;

// posição
static s16 playerX = 144;
static s16 playerY = 96;

// velocidade de movimento do jogador
s16 velocidade = 2;

// Contador usado para aceleração
u16 contadorVelocidade = 0;

// velocidade máxima andando
s16 velocidadeNormal = 2;

// velocidade máxima correndo
s16 velocidadeCorrendo = 4;

// estado da animação do sprite do jogador
u16 playerState = PLAYER_IDLE;

// velocidade de animação do sprite do jogador
u16 velocidadeAnimacao = 3;

static void loadPlayer(void);


int main(bool hardReset)
{
    (void) hardReset;

    // Desabilita interrupções durante a configuração do VDP
    SYS_disableInts();


    // ==========================================
    // CARREGA O BACKGROUND
    // ==========================================

    VDP_drawImageEx(
        BG_B,
        &img_starfield,
        TILE_ATTR_FULL(
            PAL1,
            FALSE,
            FALSE,
            FALSE,
            TILE_USER_INDEX
        ),
        0,
        0,
        TRUE,
        FALSE
    );


    // ==========================================
    // CARREGA O PERSONAGEM
    // ==========================================

    loadPlayer();


    // Reabilita interrupções
    SYS_enableInts();


    // ==========================================
    // LOOP PRINCIPAL
    // ==========================================

    while (TRUE)
    {
        // Lê o controle 1
        u16 button = JOY_readJoypad(JOY_1);

        // Velocidade de movimento do personagem
        playerState = PLAYER_IDLE;

        if (button & BUTTON_A){
            playerState = PLAYER_RUN;

            contadorVelocidade++;

            if (contadorVelocidade >= 10){
                contadorVelocidade = 0;

                if (velocidade < 4){
                    velocidade++;
                }
            }
        }else{
            contadorVelocidade++;

            if (contadorVelocidade >= 10){
                contadorVelocidade = 0;

                if (velocidade > 2){
                    velocidade--;
                }
            }
        }

        // Indica se o personagem está se movimentando
        bool moving = FALSE;


        // ==========================================
        // MOVIMENTO PARA A ESQUERDA
        // ==========================================

        if (button & BUTTON_LEFT)
        {
            playerX -= velocidade;
            moving = TRUE;

            // Espelha o sprite horizontalmente
            SPR_setHFlip(
                player,
                TRUE
            );
        }


        // ==========================================
        // MOVIMENTO PARA A DIREITA
        // ==========================================

        if (button & BUTTON_RIGHT)
        {
            playerX += velocidade;
            moving = TRUE;

            // Volta o sprite para a posição normal
            SPR_setHFlip(
                player,
                FALSE
            );
        }


        // ==========================================
        // MOVIMENTO PARA CIMA
        // ==========================================

        if (button & BUTTON_UP)
        {
            playerY -= velocidade;
            moving = TRUE;
        }


        // ==========================================
        // MOVIMENTO PARA BAIXO
        // ==========================================

        if (button & BUTTON_DOWN)
        {
            playerY += velocidade;
            moving = TRUE;
        }

        if(moving){
            if (button & BUTTON_A){
                playerState = PLAYER_RUN;
            }else{
                playerState = PLAYER_WALK;
            }
        }else{
            playerState = PLAYER_IDLE;
        }

        // ==========================================
        // LIMITES DA TELA
        // ==========================================

        if (playerX < 0)
            playerX = 0;

        if (playerX > 288)
            playerX = 288;

        if (playerY < 0)
            playerY = 0;

        if (playerY > 194)
            playerY = 194;


        // ==========================================
        // ATUALIZA POSIÇÃO
        // ==========================================

        SPR_setPosition(
            player,
            playerX,
            playerY
        );


        // ==========================================
        // ANIMAÇÃO
        // ==========================================

        switch (playerState){

            // ==========================================
            // PARADO
            // ==========================================
            case PLAYER_IDLE:

                SPR_setFrame(
                    player,
                    0
                );

            break;

            // ==========================================
            // ANDANDO
            // ==========================================

            case PLAYER_WALK:{
                u16 frame = 1 + ((vtimer >> 3) % 7);

                SPR_setFrame(
                    player,
                    frame
                );

            break;
            }


            // ==========================================
            // CORRENDO
            // ==========================================

            case PLAYER_RUN: {
                // % 8 - Sprite 256 x 64
                // 0 parado, 1-7 caminhando, 8-15 correndo.
                u16 frame = 1 + ((vtimer >> 2) % 8); 

                SPR_setFrame(
                    player,
                    frame
                );

            break;
            }
        }

        // ==========================================
        // ATUALIZA O SISTEMA DE SPRITES
        // ==========================================

        SPR_update();


        // ==========================================
        // AGUARDA O PRÓXIMO FRAME
        // ==========================================

        SYS_doVBlankProcess();
    }


    return 0;
}


// ==================================================
// CARREGA O JOGADOR
// ==================================================

static void loadPlayer(void)
{
    // Inicializa o sistema de sprites
    SPR_init();


    // Cria o donut
    player = SPR_addSprite(
        &spr_donut,
        playerX,
        playerY,
        TILE_ATTR_FULL(
            PAL2,
            TRUE,
            FALSE,
            FALSE,
            0
        )
    );


    // Carrega a paleta do donut
    PAL_setPalette(
        PAL2,
        spr_donut.palette->data,
        CPU
    );


    // Primeira atualização dos sprites
    SPR_update();
}