import ctypes
import os

# TODO: REMOVE
# class OnFunction(ctypes.Structure):
#     _fields_ = [
#         ("name", ctypes.c_char_p),
#         ("fn_ptr", ctypes.c_void_p),
#         ("function", ctypes.CFUNCTYPE(GrugItem, ctypes.c_void_p, ctypes.c_void_p, ctypes.POINTER(GrugItem)))
#     ]

# TODO: REMOVE
# class GrugFile(ctypes.Structure):
#     _fields_ = [
#         ("on_fns", ctypes.POINTER(OnFunction)),
#         ("on_fns_size", ctypes.c_size_t),
#         ("name", ctypes.c_char_p),
#         ("entity_type", ctypes.c_char_p),
#     ]


class GrugBackend(ctypes.Structure):
    _fields_ = [
        # ("object", ctypes.c_void_p), # TODO: Remove?
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


@ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p)
def compile_file(path, file_contents):
    os.exit(1)  # TODO: Implement


@ctypes.CFUNCTYPE(None, ctypes.c_void_p)
def free_file(file):
    os.exit(1)  # TODO: Implement


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
