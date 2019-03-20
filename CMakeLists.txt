cmake_minimum_required(VERSION 3.7.2)

project(libACPI C)


file(GLOB deps src/*.c src/*/*.c)
list(SORT deps)

add_library(libACPI EXCLUDE_FROM_ALL ${deps})


target_include_directories(libACPI PUBLIC include) 


target_link_libraries(libACPI Configuration  sel4 sel4muslcsys)

# warn about everything
add_compile_options(-Wall -Werror -W -Wextra)
