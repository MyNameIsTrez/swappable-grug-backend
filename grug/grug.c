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
        char* string;
        uint64_t id;
    } value;
};

struct on_function {
    char* name;
    void* fn_ptr;
    struct grug_value (*function)(void* fn_ptr, void* globals, struct grug_value* args);
};

struct grug_file {
    char* name;
    struct on_function* on_fns;
    size_t on_fns_size;
};

struct grug_mod_dir {
	const char *name;

	struct grug_mod_dir *dirs;
	size_t dirs_size;
	// size_t _dirs_capacity;

	struct grug_file *files;
	size_t files_size;
	// size_t _files_capacity;

	// bool _seen;
};

struct grug_backend {
    struct grug_file *(*compile_file)(const char* path, const char* file_contents);
    void (*free_file)(struct grug_file *file);
};

static struct grug_backend *backend;

// TODO: Unhardcode the directory and file layout
struct grug_mod_dir grug_mods = {
    .name = "mods",
    .dirs = &(struct grug_mod_dir){
        .name = "animals",
        .files = &(struct grug_file){
            .name = "labrador-Dog.grug",
            .on_fns = NULL,
            .on_fns_size = 1,
        },
        .files_size = 1,
    },
    .dirs_size = 1,
};

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

    fprintf(stderr, "before compile_file()\n");

    // struct grug_file *file = backend->compile_file("mods/animals/labrador-Dog.grug", file_contents);
    // (void)file; // TODO: REMOVE!

    backend->compile_file("mods/animals/labrador-Dog.grug", file_contents);

    // TODO: REMOVE!
    // fprintf(stderr, "file->name: %s\n", file->name);

    // grug_mods.dirs[0].files[0].on_fns = file->on_fns;

    return false;
}
