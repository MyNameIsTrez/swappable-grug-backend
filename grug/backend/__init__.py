def is_native():
    return False

def get_language():
    # return "rust"
    return "python"

# TODO: Import this fn from a player-overridable `import game_functions`
def print_string(str):
    print(f"Same language: {str}")

def do_work():
    print("Default backend doing work")
