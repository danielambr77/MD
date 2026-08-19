#include <genesis.h>

#include "main.h"
#include "res_gfx.h"
#include "res_snd.h"


static Sprite *player;

// posição
static s16 playerX = 144;
static s16 playerY = 96;

// velocidade do jogador
s16 velocidade = 1;

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

        if (moving)
        {
            /*
             * O donut possui 8 frames:
             *
             * 0 = parado
             * 1
             * 2
             * 3
             * 4
             * 5
             * 6
             * 7
             *
             * Enquanto estiver andando,
             * alternamos entre os frames 1-7.
             */

            u16 frame = 1 + ((vtimer >> velocidadeAnimacao) % 7);

            SPR_setFrame(
                player,
                frame
            );
        }
        else
        {
            // Parado = frame 0
            SPR_setFrame(
                player,
                0
            );
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