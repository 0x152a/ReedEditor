add_requires("fltk")
target("demo")
    set_kind("binary")
    add_includedirs("src")
    add_files("src/*.cpp")
    add_includedirs("src/res")
    add_files("src/res/*.cpp")
    add_files("src/frontend/*.cpp")
    add_packages("fltk")
    set_rundir("env")
    set_languages("c++17")


