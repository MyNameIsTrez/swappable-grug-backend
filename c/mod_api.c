#include "mod_api.h"
#include <string.h>

/* define the game-visible function pointer */
void (*on_spawn_Dog)(int32_t age_in_years) = NULL;

/* set a named function pointer (generated: check all known names) */
void mod_api_set_function(const char *name, void *fn) {
    if (strcmp(name, "on_spawn_Dog") == 0) {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpedantic"
        on_spawn_Dog = fn;
        #pragma GCC diagnostic pop
        return;
    }
    /* unknown symbol: ignore or log in real system */
    printf("[mod_api] Unknown symbol registration: %s\n", name);
}

/* install interpreter trampolines as the default binding for each known function */
void mod_api_register_trampolines(void) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    mod_api_set_function("on_spawn_Dog", on_spawn__Dog_trampoline);
    #pragma GCC diagnostic pop
}
