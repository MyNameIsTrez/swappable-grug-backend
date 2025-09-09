import grug

# TODO: Write tests that assert all subpackages are fully overridable.
print(f"is_native: {grug.is_native()}")
print(f"is_same_language: {grug.is_same_language()}")

grug.print_string("hi")
