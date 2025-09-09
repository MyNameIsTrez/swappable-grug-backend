try:
    import backend  # try to load user-installed backend first
except ImportError:
    from . import backend  # fallback to bundled backend

def hello():
    print("hello")
    backend.do_work()
