#pragma once
#include <stdint.h>

// -----------------------------
// Generic tagged value type
// -----------------------------
typedef enum {
    GRUG_TAG_I32,
    GRUG_TAG_STRING
} grug_tag_t;

typedef struct {
    grug_tag_t tag;
    union {
        int32_t i32;
        const char *string;
    } as;
} grug_value_t;

// Forward declarations
struct grug_ast;
struct grug_backend;

// -----------------------------
// AST structure (simplified)
// -----------------------------
typedef struct grug_ast {
    const char *mod_name;
    const char *entity_name;
    const char *function_name;
} grug_ast_t;

// -----------------------------
// Backend interface
// -----------------------------
typedef struct grug_backend {
    const char *name;
    void (*execute)(const grug_ast_t *ast, grug_value_t *args, int argc);
} grug_backend_t;

// -----------------------------
// Public API for bindings
// -----------------------------
void grug_init(void);
void grug_load_mods(void);
grug_ast_t *grug_get_ast_for(const char *entity, const char *fn);
void grug_call_backend(const grug_ast_t *ast, grug_value_t *args, int argc);
void grug_load_player_backend(const char *so_path);

// Global default backend instance
extern grug_backend_t default_interpreter_backend;
