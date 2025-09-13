import grug


def print_string(str):
    print(str)


grug.register_game_fn(print_string)

mods = grug.get_mods()
animals_mod = mods.dirs[0]
labrador_file = animals_mod.files[0]

# This creates an entity, and initializes its globals
# The 0 we pass stands for entity ID 0
entity_globals = labrador_file.create_entity(0)

# This labrador is 3 years old
labrador_file.on_fns.spawn(entity_globals, 3)

# TODO: Write tests that assert all subpackages are fully overridable.
print(f"is_native: {grug.is_native()}")
print(f"is_same_language: {grug.is_same_language()}")
