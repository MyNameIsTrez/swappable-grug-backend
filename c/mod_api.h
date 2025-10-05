#pragma once
#include "grug/grug.h"
#include <stdio.h>

/* --- Game-level functions --- */
static inline void print_string(const char *s) {
    printf("[game fn] %s\n", s);
}

/* --- Global function pointers for entity callbacks --- */
/* These are the pointers the game calls. Generated from mod_api.json. */
extern void (*on_spawn_Dog)(int32_t age_in_years);

/* --- Interpreter trampoline (packs args into grug_value_t and calls backend) --- */
static inline void on_spawn__Dog_trampoline(int32_t age_in_years) {
    grug_ast_t *ast = grug_get_ast_for("Dog", "on_spawn");
    if (!ast) {
        printf("[trampoline] No AST found for Dog::on_spawn\n");
        return;
    }
    grug_value_t args[1];
    args[0].tag = GRUG_TAG_I32;
    args[0].as.i32 = age_in_years;
    grug_call_backend(ast, args, 1);
}

/* --- Hooks used by grug.c to set the pointers --- */
/* Implementations live in mod_api.c (generated) */
void mod_api_set_function(const char *name, void *fn);
void mod_api_register_trampolines(void);
