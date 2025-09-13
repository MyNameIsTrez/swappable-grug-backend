try:
    import backend  # try to load user-installed backend first
except ImportError:
    from . import backend  # fallback to bundled backend

import ctypes
from dataclasses import dataclass, field
from typing import Any, Callable, List, Union

# Type alias for Python or C init function
InitGlobalsFn = Union[
    Callable[[object, int], None],  # Python backend
    ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint64),  # C backend
]


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


class Globals:
    """Opaque handle for a C-side globals array.

    Python code should not create or access the array directly.
    Only pass it back to library functions.
    """

    __slots__ = ("_array",)  # minimal memory footprint

    def __init__(self, size: int):
        # allocate the ctypes array internally
        self._array = (ctypes.c_byte * size)()

    @property
    def _as_ctypes(self):
        """Internal: return the underlying ctypes array for library calls."""
        return self._array


IS_NATIVE = backend.is_native()
IS_SAME_LANGUAGE = backend.get_language() == "python"


def is_native():
    return IS_NATIVE


def is_same_language():
    return IS_SAME_LANGUAGE


def register_game_fn(game_fn):
    pass  # TODO: Implement


# TODO: Unhardcode
file = File("labrador-Dog.grug", 8)
mod = Dir("animals", files=[file])
mods = Dir("animals", dirs=[mod])


def get_mods():
    return mods


def call(*args):
    print(f"args: {args}")
    if IS_NATIVE:
        pass  # TODO: Finish
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
