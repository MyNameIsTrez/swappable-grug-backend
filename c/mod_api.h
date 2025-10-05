#pragma once
#include "grug/grug.h"
#include <stdio.h>

// -----------------------------
// Game functions (direct call)
// -----------------------------
static inline void print_string(const char *str) {
    printf("[game fn] %s\n", str);
}

// -----------------------------
// Entity on_functions
// -----------------------------

// Regular function signature for compiler backends
static inline void on_spawn__Dog(int32_t age_in_years) {
    printf("[game] Dog::on_spawn called (age=%d)\n", age_in_years);

    // For interpreter backends, we simulate the trampoline
    grug_ast_t *ast = grug_get_ast_for("Dog", "on_spawn");
    if (!ast) {
        printf("[game] No AST found for Dog::on_spawn\n");
        return;
    }

    // Pack arguments for interpreter backend
    grug_value_t args[1];
    args[0].tag = GRUG_TAG_I32;
    args[0].as.i32 = age_in_years;

    grug_call_backend(ast, args, 1);
}
