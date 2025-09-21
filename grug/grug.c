// This is a placeholder file for the real grug.c and grug.h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum grug_runtime_error_type {
	GRUG_ON_FN_DIVISION_BY_ZERO,
	GRUG_ON_FN_STACK_OVERFLOW,
	GRUG_ON_FN_TIME_LIMIT_EXCEEDED,
	GRUG_ON_FN_OVERFLOW,
	GRUG_ON_FN_GAME_FN_ERROR,
};

typedef void (*grug_runtime_error_handler_t)(const char *reason, enum grug_runtime_error_type type, const char *on_fn_name, const char *on_fn_path);

enum type {
	type_void,
	type_bool,
	type_i32,
	type_f32,
	type_string,
	type_id,
	type_resource,
	type_entity,
};

typedef struct {
    enum type type;
    union {
        bool bool_;
        int32_t i32;
        float f32;
        char* string;
        uint64_t id;
    } value;
} grug_value;

typedef struct {
    char* name;
    void* fn_ptr;
    grug_value (*function)(void* fn_ptr, void* globals, grug_value* args);
} on_function;

struct grug_file {
    // A hashmap may be faster, but for simplicity sake a list is nicer to work with
    // A game that desires speed can build their own hashmap
    on_function* on_functions;
    size_t num_on_functions;
    // TODO: Rest effectively matches current grug_file type
    char* name;
    char* entity_type;
    // TODO: ...etc
};

struct grug_backend {
    void* object;
    // Grug detects file changes, then has the backend compiles it via this function pointer
    struct grug_file (*compile_file)(char* path, char* file_contents);
    // This lets the backend know we don't need this file anymore
    void (*free_file)(struct grug_file file);
};

static struct grug_backend *backend;

struct grug_mod_dir grug_mods;

bool grug_init(grug_runtime_error_handler_t handler, const char *mod_api_json_path, const char *mods_dir_path, uint64_t on_fn_time_limit_ms, struct grug_backend *backend_) {
    (void)handler;
    (void)mod_api_json_path;
    (void)mods_dir_path;
    (void)on_fn_time_limit_ms;
    (void)backend_;
    return false;
}

bool grug_regenerate_modified_mods(void) {
    backend->compile_file();
    
    // TODO: Translate to C
    // file = File("labrador-Dog.grug", 8)
    // mod = Dir("animals", files=[file])
    // global mods
    // mods = Dir("animals", dirs=[mod])

    return false;
}
