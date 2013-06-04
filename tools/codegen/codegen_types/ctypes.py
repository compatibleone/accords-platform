
def from_platform_type(type_name):
    if (type_name == "string"):
        return "char *"
    if (type_name == "int"):
        return "int"
    raise(ValueError('Unexpected type {0}'.format(type_name)))