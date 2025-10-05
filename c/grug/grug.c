#include "grug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

// -----------------------------
// Internal state
// -----------------------------
static grug_backend_t *current_backend = NULL;
static grug_ast_t loaded_ast = {
    .mod_name = "animals/labrador",
    .entity_name = "Dog",
    .function_name = "on_spawn"
};

// -----------------------------
// Default interpreter backend
// -----------------------------
static void interpreter_execute(const grug_ast_t *ast, grug_value_t *args, int argc) {
    printf("[backend: interpreter] Executing %s::%s\n",
           ast->entity_name, ast->function_name);

    // Expect single argument: i32 age_in_years
    if (argc == 1 && args[0].tag == GRUG_TAG_I32) {
        int age = args[0].as.i32;
        for (int i = 0; i < age; i++) {
            printf("Woof!\n");
        }
    }
}

grug_backend_t default_interpreter_backend = {
    .name = "default_interpreter",
    .execute = interpreter_execute
};

// -----------------------------
// Backend management
// -----------------------------
void grug_load_player_backend(const char *so_path) {
    void *handle = dlopen(so_path, RTLD_NOW);
    if (!handle) {
        printf("[bindings] No player backend found at %s, using default\n", so_path);
        return;
    }

    grug_backend_t *player_backend = dlsym(handle, "grug_backend_instance");
    if (player_backend) {
        printf("[bindings] Loaded player backend: %s\n", player_backend->name);
        current_backend = player_backend;
    } else {
        printf("[bindings] Missing grug_backend_instance symbol; using default\n");
    }
}

// -----------------------------
// Bindings layer
// -----------------------------
void grug_init(void) {
    printf("[bindings] Initializing grug...\n");
    current_backend = &default_interpreter_backend;
}

void grug_load_mods(void) {
    printf("[frontend] (mock) Loaded mod for entity '%s'\n", loaded_ast.entity_name);
}

grug_ast_t *grug_get_ast_for(const char *entity, const char *fn) {
    if (strcmp(entity, loaded_ast.entity_name) == 0 &&
        strcmp(fn, loaded_ast.function_name) == 0)
        return &loaded_ast;
    return NULL;
}

void grug_call_backend(const grug_ast_t *ast, grug_value_t *args, int argc) {
    if (!ast || !current_backend) {
        printf("[bindings] No backend or AST available.\n");
        return;
    }
    current_backend->execute(ast, args, argc);
}
