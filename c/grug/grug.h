#pragma once
#include <stddef.h>
#include <stdint.h>

// Generic tagged value for interpreter calls
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

// Minimal AST (mock)
typedef struct grug_ast {
    const char *mod_name;
    const char *entity_name;
    const char *function_name;
} grug_ast_t;

// Symbol table entry exposed by backends
typedef struct grug_symbol {
    const char *name;
    void *fn; // pointer to function implementation
} grug_symbol_t;

// Backend descriptor (custom backends export a grug_backend_instance)
typedef struct grug_backend {
    const char *name;
    grug_symbol_t *symbols;
    size_t num_symbols;
    // Generic backend execution entrypoint (keeps demo simple)
    void (*execute)(const grug_ast_t *ast, grug_value_t *args, size_t argc);
} grug_backend_t;

/* bindings/frontend API (minimal) */
void grug_init(void);
void grug_load_mods(void);
grug_ast_t *grug_get_ast_for(const char *entity, const char *fn);
void grug_call_backend(const grug_ast_t *ast, grug_value_t *args, size_t argc);
void grug_load_custom_backend(const char *so_path);
void grug_set_backend(grug_backend_t *backend);
