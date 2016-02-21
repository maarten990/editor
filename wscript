APPNAME = 'editor'
VERSION = '0.1'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_c')

def configure(conf):
    conf.load('compiler_c')

    conf.check_cc(lib='termbox',
                  mandatory=True,
                  uselib_store='TERMBOX')

    conf.check_cfg(path='python3-config', args='--cflags --ldflags',
                   package='', uselib_store='PYTHON')

def build(bld):
    sources = bld.path.ant_glob('*.c')
    libs = ['TERMBOX', 'PYTHON']

    bld.program(source=sources, use=libs, target=APPNAME,
                cflags=['-O0', '-g', '-Wall', '-std=gnu99',
                '-fdiagnostics-color'])
