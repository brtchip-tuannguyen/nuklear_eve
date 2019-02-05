/*
Nuklear EVE
Copyright (C) 2019  Bridgetek Pte Lte
Author: Jan Boon <jan.boon@kaetemi.be>
*/

/*
This C source file demonstrates the Nuklear EVE implementation.
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

#include "nuklear_eve.h"
// #include "demo/style.c"
#include "demo/calculator.c"
#include "demo/overview.c"
#include "demo/node_editor.c"

#include "Ft_Esd_Utility.h"

int main(void)
{
    nk_evefont *font;
    struct nk_context *ctx;

    int show_calculator = 0;
    int show_overview = 0;
    int show_node_editor = 0;

    /* Initialize Nuklear EVE */
    font = nk_evefont_create_rom(27);
    ctx = nk_eve_init(font);

    /* Main Loop */
    while (true)
    {
        /* Input */
        nk_eve_update();

        /* GUI */
        if (nk_begin(ctx, "Nuklear EVE", nk_rect(50, 50, 300, 400),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            enum
            {
                EASY,
                HARD
            };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 200, 1);
            nk_checkbox_label(ctx, "Show Calculator", &show_calculator);

            nk_layout_row_static(ctx, 30, 200, 1);
            nk_checkbox_label(ctx, "Show Overview", &show_overview);

            nk_layout_row_static(ctx, 30, 200, 1);
            nk_checkbox_label(ctx, "Show Node Editor", &show_node_editor);

            nk_layout_row_static(ctx, 30, 200, 1);
            if (nk_button_label(ctx, "Show Bridgetek Logo"))
                Esd_ShowLogo();

            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY))
                op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD))
                op = HARD;
            nk_layout_row_dynamic(ctx, 22, 1);
            nk_property_int(ctx, "Xyz:", 0, &property, 100, 10, 1);
        }
        nk_end(ctx);

        /* -------------- EXAMPLES ---------------- */
        if (show_calculator)
            calculator(ctx);
        if (show_overview)
            overview(ctx);
        if (show_node_editor)
            node_editor(ctx);
        /* ----------------------------------------- */

        /* Draw */
        nk_eve_render(nk_rgb(30, 30, 30));
    }

    /* Release Nuklear EVE */
    nk_eve_shutdown();

    return EXIT_SUCCESS;
}

/* end of file */