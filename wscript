APPNAME = 'editor'
VERSION = '0.1'

top = '.'
out = 'build'

def options(opt):
    opt.recurse('lib')
    opt.load('compiler_c')

def configure(conf):
    conf.recurse('lib')

    conf.load('compiler_c')

    conf.check_cc(lib='termbox',
                  mandatory=True,
                  uselib_store='TERMBOX')

    conf.check_cfg(path='python3-config', args='--cflags --ldflags',
                   package='', uselib_store='PYTHON')

def build(bld):
    bld.recurse('lib')

    sources = bld.path.ant_glob('*.c')
    libs = ['TERMBOX', 'PYTHON', 'libeditor']

    bld.program(source=sources, use=libs, target=APPNAME,
                cflags=['-O0', '-g', '-Wall', '-std=gnu99',
                        '-fdiagnostics-color'])
