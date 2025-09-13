def is_native():
    return False

def get_language():
    # return "rust"
    return "python"

# TODO: Import this fn from a player-overridable `import game_functions`
def print_string(str):
    print(f"Same language: {str}")

# TODO: Should interpreters in non-systems programming languages be allowed?
#       If so, should games be able to choose them over the default
#       of picking the IS_SAME_LANGUAGE branch? If so, what should the API be?
def call_interpreter():
    print("Default backend doing work")
