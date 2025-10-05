try:
    import backend  # Try to load user-installed backend first
except ImportError:
    from . import backend  # Fallback to bundled backend

import ctypes
import os
import sys
from dataclasses import dataclass, field
from typing import Any, Callable, List, Union

# Type alias for Python or C init function
# InitGlobalsFn = Union[
#     Callable[[object, int], None],  # Python backend
#     ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint64),  # C backend
# ]
InitGlobalsFn = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint64)


class Globals:
    def __init__(self, size: int):
        self._array = (ctypes.c_byte * size)()

    @property
    def c_array(self):
        return self._array


# TODO:
# Let this be dynamically generated somehow, based on mod_api.json
# The problem though is that grug_dll.grug_regenerate_modified_mods() doesn't tell us
# what to cast, when a new grug file is added.
class DogOnFns(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        # ("spawn", ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_int32)),
        # TODO: The bindings should wrap all on fns of a file into a dict, where fn names are keys
        # TODO: The bindings should also cast fn_ptr to the right type, based on mod_api.json
        ("fn_ptr", ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_int32)),
    ]


class File(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("globals_size", ctypes.c_size_t),
        ("init_globals_fn", InitGlobalsFn),
        ("on_fns", ctypes.POINTER(DogOnFns)),
        ("on_fns_size", ctypes.c_size_t),
    ]

    def create_entity(self, entity_id: int):
        g = Globals(self.globals_size)
        ref = ctypes.byref(g.c_array)
        self.init_globals_fn(ref, entity_id)
        return g


class Dir(ctypes.Structure):
    pass  # Forward declare so we can self-reference


Dir._fields_ = [
    ("name", ctypes.c_char_p),
    ("dirs", ctypes.POINTER(Dir)),
    ("dirs_size", ctypes.c_size_t),
    ("files", ctypes.POINTER(File)),
    ("files_size", ctypes.c_size_t),
]


IS_NATIVE = backend.is_native()
IS_SAME_LANGUAGE = backend.get_language() == "python"

# If the backend is also in Python, then its get_backend_struct
# won't be a CFUNCTYPE, but just a regular Python function.
if not IS_SAME_LANGUAGE:
    backend.get_backend_struct.restype = ctypes.c_void_p
backend_struct = backend.get_backend_struct()

# RTLD_GLOBAL here allows mods to access grug_runtime_error_type from grug.c
# TODO: Let this open a dll on Windows, and a dylib on Mac
grug_dll = ctypes.PyDLL("./grug/grug.so", os.RTLD_GLOBAL)

grug_dll.grug_init.restype = ctypes.c_bool
grug_dll.grug_init.argtypes = (
    ctypes.c_void_p,  # grug_runtime_error_handler_t handler
    ctypes.c_char_p,  # const char *mod_api_json_path
    ctypes.c_char_p,  # const char *mods_dir_path
    ctypes.c_uint64,  # uint64_t on_fn_time_limit_ms
    # TODO: Let this file define GrugBackend, instead of backend.GrugBackend.
    ctypes.POINTER(backend.GrugBackend),  # struct grug_backend *backend
)

grug_dll.grug_regenerate_modified_mods.restype = ctypes.c_bool


def init(runtime_error_handler, mod_api_json_path, mods_dir_path, on_fn_time_limit_ms):
    global runtime_error_handler_

    runtime_error_handler_ = runtime_error_handler

    if grug_dll.grug_init(
        cfunc_runtime_error_handler,
        mod_api_json_path.encode(),
        mods_dir_path.encode(),
        on_fn_time_limit_ms,
        ctypes.pointer(backend_struct),
    ):
        sys.exit(1)  # TODO: Implement


@ctypes.CFUNCTYPE(None, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p)
def cfunc_runtime_error_handler(reason, type, on_fn_name, on_fn_path):
    runtime_error_handler_(
        reason.decode(), type, on_fn_name.decode(), on_fn_path.decode()
    )


def regenerate_modified_mods():
    if grug_dll.grug_regenerate_modified_mods():
        sys.exit(1)  # TODO: Implement


def is_native():
    return IS_NATIVE


def is_same_language():
    return IS_SAME_LANGUAGE


def register_game_fn(game_fn_name, game_fn):
    pass  # TODO: Implement


def get_mods():
    return Dir.in_dll(grug_dll, "grug_mods")


def call(*args):
    print(f"args: {args}")
    if IS_NATIVE:
        sys.exit(1)  # TODO: Finish
    else:
        backend.call_interpreter()  # TODO: Finish


# Dynamically create wrapper functions
# TODO: Loop over mod_api.json, as seen here: https://github.com/MyNameIsTrez/grug/issues/62#issuecomment-3261895296
import sys

this_module = sys.modules[__name__]
if IS_SAME_LANGUAGE:
    setattr(this_module, "print_string", getattr(backend, "print_string"))
else:
    setattr(this_module, "print_string", call)
