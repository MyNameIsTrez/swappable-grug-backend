#include "grug/grug.h"
#include "mod_api.h"
#include <stdio.h>

int main(void) {
    printf("[game] Starting game...\n");

    grug_init();
    grug_load_player_backend("./player_backend.so");
    grug_load_mods();

    printf("[game] Spawning Dog (age=3)\n");
    on_spawn__Dog(3);

    printf("[game] Done.\n");
    return 0;
}
