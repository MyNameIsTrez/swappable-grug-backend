#include "grug.h"
#include "../mod_api.h"   /* include generated mod API header */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

/* mock AST loaded from file */
static grug_ast_t loaded_ast = {
    .mod_name = "animals/labrador",
    .entity_name = "Dog",
    .function_name = "on_spawn"
};

/* backend state */
static grug_backend_t *current_backend = NULL;

/* default interpreter backend implementation (keeps demo tiny) */
static void interpreter_execute(const grug_ast_t *ast, grug_value_t *args, int argc) {
    printf("[backend: interpreter] Executing %s::%s\n",
           ast->entity_name, ast->function_name);
    if (argc == 1 && args[0].tag == GRUG_TAG_I32) {
        int age = args[0].as.i32;
        for (int i = 0; i < age; ++i) {
            /* interpreter-level effect; in a real interpreter you'd call print_string AST node */
            print_string("Woof!");
        }
    }
}

/* default backend object (no exported native symbols) */
static grug_backend_t default_interpreter_backend = {
    .name = "default_interpreter",
    .symbols = NULL,
    .num_symbols = 0,
    .execute = interpreter_execute
};

void grug_set_backend(grug_backend_t *backend) {
    current_backend = backend;
}

/* Load player's backend .so and, if found, register its provided symbols via mod_api_set_function().
   Note: grug.c does not hardcode function names — it simply iterates the backend's exported table. */
void grug_load_player_backend(const char *so_path) {
    void *handle = dlopen(so_path, RTLD_NOW);
    if (!handle) {
        printf("[bindings] No player backend found at %s — staying with interpreter\n", so_path);
        grug_set_backend(&default_interpreter_backend);
        return;
    }

    grug_backend_t *player_backend = dlsym(handle, "grug_backend_instance");
    if (!player_backend) {
        printf("[bindings] player backend missing symbol grug_backend_instance — using interpreter\n");
        grug_set_backend(&default_interpreter_backend);
        return;
    }

    /* first, install trampolines (default interpreter behavior) so we have sensible defaults */
    mod_api_register_trampolines();

    /* next, override with any native symbols the backend provides */
    if (player_backend->symbols && player_backend->num_symbols > 0) {
        for (int i = 0; i < player_backend->num_symbols; ++i) {
            const char *name = player_backend->symbols[i].name;
            void *fn = player_backend->symbols[i].fn;
            /* delegate actual per-name binding to mod_api.c (generated) */
            mod_api_set_function(name, fn);
            printf("[bindings] Backend override: %s -> %p\n", name, fn);
        }
    }

    grug_set_backend(player_backend);
    printf("[bindings] Using backend %s\n", player_backend->name);
}

/* initialization & minimal frontend simulation */
void grug_init(void) {
    printf("[bindings] Initializing grug...\n");
    /* set trampolines as default bindings */
    mod_api_register_trampolines();
    /* default backend */
    grug_set_backend(&default_interpreter_backend);
}

void grug_load_mods(void) {
    printf("[frontend] (mock) loaded mod %s for entity %s\n", loaded_ast.mod_name, loaded_ast.entity_name);
}

grug_ast_t *grug_get_ast_for(const char *entity, const char *fn) {
    if (strcmp(entity, loaded_ast.entity_name) == 0 && strcmp(fn, loaded_ast.function_name) == 0) {
        return &loaded_ast;
    }
    return NULL;
}

void grug_call_backend(const grug_ast_t *ast, grug_value_t *args, int argc) {
    if (!ast || !current_backend || !current_backend->execute) {
        printf("[bindings] no backend/ast available\n");
        return;
    }
    current_backend->execute(ast, args, argc);
}
