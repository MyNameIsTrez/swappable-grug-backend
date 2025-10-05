// This is a placeholder file for the real grug.c and grug.h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h> // TODO: REMOVE!

enum grug_runtime_error_type {
	GRUG_ON_FN_DIVISION_BY_ZERO,
	GRUG_ON_FN_STACK_OVERFLOW,
	GRUG_ON_FN_TIME_LIMIT_EXCEEDED,
	GRUG_ON_FN_OVERFLOW,
	GRUG_ON_FN_GAME_FN_ERROR,
};

typedef void (*grug_runtime_error_handler_t)(const char *reason, enum grug_runtime_error_type type, const char *on_fn_name, const char *on_fn_path);

typedef void (*grug_init_globals_fn_t)(void *globals, uint64_t id);

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

struct grug_value {
    enum type type;
    union {
        bool bool_;
        int32_t i32;
        float f32;
        const char* string;
        uint64_t id;
    } value;
};

struct on_function {
    const char* name;
    void* fn_ptr;
    struct grug_value (*function)(void* fn_ptr, void* globals, struct grug_value* args);
};

struct grug_file {
    const char* name;
    size_t globals_size;
    grug_init_globals_fn_t init_globals_fn;
    struct on_function* on_fns;
    size_t on_fns_size;
};

struct grug_mod_dir {
	const char *name;

	struct grug_mod_dir *dirs;
	size_t dirs_size;

	struct grug_file *files;
	size_t files_size;
};

struct grug_backend {
    struct grug_file *(*compile_file)(const char* path, const char* file_contents);
    void (*free_file)(struct grug_file *file);
};

static struct grug_backend *backend;

struct grug_mod_dir grug_mods;

bool grug_init(grug_runtime_error_handler_t handler, const char *mod_api_json_path, const char *mods_dir_path, uint64_t on_fn_time_limit_ms, struct grug_backend *backend_) {
    (void)handler;
    (void)mod_api_json_path;
    (void)mods_dir_path;
    (void)on_fn_time_limit_ms;

    backend = backend_;

    return false;
}

bool grug_regenerate_modified_mods(void) {
    // TODO: The real grug.c opens the file normally
    const char *file_contents = 
        "on_spawn(age_in_years: i32) {\n"
        "    i: i32 = 0\n"
        "    while i < age_in_years {\n"
        "        print_string(\"Woof!\")\n"
        "        i = i + 1\n"
        "    }\n"
        "}\n";

    struct grug_file *file = backend->compile_file("mods/animals/labrador-Dog.grug", file_contents);

    static struct grug_mod_dir dir = {0};
    dir.name = "animals";
    dir.files = file;
    dir.files_size = 1;

    grug_mods.name = "mods";
    grug_mods.dirs = &dir;
    grug_mods.dirs_size = 1;

    return false;
}
