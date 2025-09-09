try:
    import backend  # try to load user-installed backend first
except ImportError:
    from . import backend  # fallback to bundled backend

is_native_ = backend.is_native()

def is_native():
    return is_native_

def call(*args):
    print(f"args: {args}")
    backend.do_work()

# === Dynamically create wrapper functions ===
# TODO: Loop over mod_api.json as seen here:
#       https://github.com/MyNameIsTrez/grug/issues/62#issuecomment-3261895296
import sys
this_module = sys.modules[__name__]
setattr(this_module, "print_string", call)
