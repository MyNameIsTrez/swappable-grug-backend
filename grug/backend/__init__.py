import ctypes
import sys


class OnFunction(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("fn_ptr", ctypes.c_void_p),
        # (
        #     "function",
        #     ctypes.CFUNCTYPE(
        #         GrugItem, ctypes.c_void_p, ctypes.c_void_p, ctypes.POINTER(GrugItem)
        #     ),
        # ),
    ]


class GrugFile(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("on_fns", ctypes.POINTER(OnFunction)),
        ("on_fns_size", ctypes.c_size_t),
    ]


class GrugBackend(ctypes.Structure):
    _fields_ = [
        (
            "compile_file",
            ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p),
        ),
        ("free_file", ctypes.CFUNCTYPE(None, ctypes.c_void_p)),
    ]


def is_native():
    return False


def get_language():
    # return "rust"
    return "python"


@ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_int32)
def on_spawn(g, age_in_years):
    i = 0
    while i < age_in_years:
        print("Woof!")
        i += 1


compiled_file = None


@ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p)
def compile_file(path, file_contents):
    global compiled_file
    # TODO: Unhardcode
    casted = ctypes.cast(on_spawn, ctypes.c_void_p)
    # TODO: This OnFunction will dangle when compile_file() returns
    on_fn = OnFunction(b"on_spawn", casted)
    on_fns = ctypes.pointer(on_fn)
    compiled_file = GrugFile(
        b"labrador-Dog.grug",
        on_fns,
        1,
    )
    r = ctypes.byref(compiled_file)
    v = ctypes.cast(r, ctypes.c_void_p)
    return v.value


@ctypes.CFUNCTYPE(None, ctypes.c_void_p)
def free_file(file):
    sys.exit(1)  # TODO: Implement


def get_backend_struct():
    return GrugBackend(compile_file, free_file)


# TODO: Import this fn from a player-overridable `import game_functions`
def print_string(str):
    print(f"Same language: {str}")


# TODO: Should interpreters in non-systems programming languages be allowed?
#       If so, should games be able to choose them over the default
#       of picking the IS_SAME_LANGUAGE branch? If so, what should the API be?
def call_interpreter():
    print("Default backend doing work")
