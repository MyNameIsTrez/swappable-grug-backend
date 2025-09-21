try:
    import backend  # Try to load user-installed backend first
except ImportError:
    from . import backend  # Fallback to bundled backend

import ctypes
import os
from dataclasses import dataclass, field
from typing import Any, Callable, List, Union

# Type alias for Python or C init function
InitGlobalsFn = Union[
    Callable[[object, int], None],  # Python backend
    ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint64),  # C backend
]


class Globals:
    """Opaque handle for a C-side globals array.

    Python code should not create or access the array directly.
    Only pass it back to library functions.
    """

    __slots__ = ("_array",)  # Minimize memory footprint

    def __init__(self, size: int):
        # Allocate the ctypes array internally
        self._array = (ctypes.c_byte * size)()

    @property
    def _as_ctypes(self):
        """Internal: return the underlying ctypes array for library calls."""
        return self._array


@dataclass
class File:
    name: str
    globals_size: int
    init_globals_fn: InitGlobalsFn
    on_fns: Any

    def create_entity(self, entity_id: int):
        g = Globals(self.globals_size)
        self.init_globals_fn(g, entity_id)
        return g


@dataclass
class Dir:
    name: str
    files: List[File] = field(default_factory=list)
    dirs: List["Dir"] = field(default_factory=list)


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


def init(runtime_error_handler, mod_api_json_path, mods_dir_path, on_fn_time_limit_ms):
    global runtime_error_handler_

    runtime_error_handler_ = runtime_error_handler

    grug_dll.grug_init.restype = ctypes.c_bool
    if grug_dll.grug_init(
        cfunc_runtime_error_handler,
        mod_api_json_path.encode(),
        mods_dir_path.encode(),
        on_fn_time_limit_ms,
        backend_struct,
    ):
        os.exit(1)  # TODO: Implement


@ctypes.CFUNCTYPE(None, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p)
def cfunc_runtime_error_handler(reason, type, on_fn_name, on_fn_path):
    runtime_error_handler_(
        reason.decode(), type, on_fn_name.decode(), on_fn_path.decode()
    )


def regenerate_modified_mods():
    grug_dll.grug_regenerate_modified_mods.restype = ctypes.c_bool
    if grug_dll.grug_regenerate_modified_mods():
        os.exit(1)  # TODO: Implement


def is_native():
    return IS_NATIVE


def is_same_language():
    return IS_SAME_LANGUAGE


def register_game_fn(game_fn_name, game_fn):
    os.exit(1)  # TODO: Implement


def get_mods():
    # TODO: Do I have to cast this to class Dir?
    return grug_dll.mods


def call(*args):
    print(f"args: {args}")
    if IS_NATIVE:
        os.exit(1)  # TODO: Finish
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
