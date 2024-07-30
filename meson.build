project('snake', ['c', 'cpp'])

dir = meson.current_source_dir()
src_dir = dir / 'src'

src_dbg_module = [
    src_dir / 'dbg/debug.c',
    src_dir / 'dbg/debug.h',
]

src_core_module = [
    src_dir / 'core/core.h',
    src_dir / 'core/debug.h',
    src_dir / 'core/list.c',
    src_dir / 'core/list.h',
    src_dir / 'core/snake.c',
    src_dir / 'core/snake.h',
]

src_ctrl_module = [
    src_dir / 'ctrl/ctrl.h',
    src_dir / 'ctrl/kbd.c',
    src_dir / 'ctrl/kbd.h',
]

src_display_module = [
    src_dir / 'display/display.h',
    src_dir / 'display/icon.h',
    src_dir / 'display/tui.h',
    src_dir / 'display/tui.c',
]

src_modules = [
    src_dbg_module,
    src_core_module,
    src_ctrl_module,
    src_display_module,
]

# 添加 _DEBUG_ 宏
if get_option('buildtype') == 'debug'
    add_project_arguments('-D_DEBUG_', language: 'c')
    add_project_arguments('-D_DEBUG_', language: 'cpp')
    message('Add macro "_DEBUG_" for C & C++ compiler')
endif

# 宽字符版本 (具有 unicode 支持) 的 ncurses 库
dep_ncursor = dependency('ncursesw')

inc_dir = include_directories('src')
executable(
    'snake',
    src_modules + src_dir / 'main.c',
    include_directories: inc_dir,
    dependencies: dep_ncursor,
)

subdir('test')
