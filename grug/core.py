try:
    import backend  # try to load user-installed backend first
except ImportError:
    from . import backend  # fallback to bundled backend

IS_NATIVE = backend.is_native()
IS_SAME_LANGUAGE = backend.get_language() == "python"

def is_native():
    return IS_NATIVE

def is_same_language():
    return IS_SAME_LANGUAGE

def call(*args):
    print(f"args: {args}")
    if IS_NATIVE:
        pass # TODO: Finish
    else:
        backend.do_work() # TODO: Finish

# === Dynamically create wrapper functions ===
# TODO: Loop over mod_api.json as seen here:
#       https://github.com/MyNameIsTrez/grug/issues/62#issuecomment-3261895296
import sys
this_module = sys.modules[__name__]
if IS_SAME_LANGUAGE:
    setattr(this_module, "print_string", getattr(backend, "print_string"))
else:
    setattr(this_module, "print_string", call)
