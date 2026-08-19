#include <genesis.h>

#include "main.h"
#include "res_gfx.h"
#include "res_snd.h"


static Sprite *player;

// posição
static s16 playerX = 144;
static s16 playerY = 96;

// velocidade de movimentação do personagem
s16 velocidade = 2;

// velocidade de movimentação do sprite do personagem
u16 velocidadeAnimacao = 2;

static void loadPlayer(void);


int main(bool hardReset)
{
    (void) hardReset;

    // Desabilita interrupções durante a configuração do VDP
    SYS_disableInts();

    // Carrega o background
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

    // Carrega o personagem
    loadPlayer();

    // Reabilita interrupções
    SYS_enableInts();


    while (TRUE)
    {
        // Lê o controle 1
        u16 button = JOY_readJoypad(JOY_1);

        // Indica se o personagem está se movimentando
        bool moving = FALSE;


        // ==========================================
        // MOVIMENTO HORIZONTAL
        // ==========================================

        if (button & BUTTON_LEFT)
        {
            playerX -= velocidade;
            moving = TRUE;

            SPR_setHFlip(player, TRUE);
        }

        if (button & BUTTON_RIGHT)
        {
            playerX += velocidade;
            moving = TRUE;
            
            SPR_setHFlip(player, FALSE);
        }


        // ==========================================
        // MOVIMENTO VERTICAL
        // ==========================================

        if (button & BUTTON_UP)
        {
            playerY -= velocidade;
            moving = TRUE;
        }

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


        // Atualiza o sistema de sprites
        SPR_update();


        // Aguarda o próximo frame
        SYS_doVBlankProcess();
    }


    return 0;
}



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