def can_build(platform):
    return platform == "psp"

def configure(env):
    # FIXME:XXX: broken :/
    #env.Append(CCFLAGS=['-DUSE_QUAD_VECTORS'])
    pass