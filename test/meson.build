dep_gtest = dependency('gtest', required: true)
dep_gtest_main = dependency('gtest_main', required: true)

message('Add macro "_TEST_" for C & C++ compiler')
test_core = executable(
    'test_core',
    src_dbg_module + src_core_module + 'test_core.cpp',
    include_directories: inc_dir,
    dependencies: [dep_gtest, dep_gtest_main],
    c_args: ['-D_TEST_'],
    cpp_args: ['-D_TEST_'],
)

test('test_core', test_core)
