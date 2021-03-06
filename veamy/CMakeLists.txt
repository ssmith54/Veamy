set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(libveamy STATIC ${SRCS} ${HEADERS})

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(libveamy PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Depend on a library that we defined in the top-level file
target_link_libraries(libveamy libdelynoi libutilities)

#add_custom_target(libveamy_target ALL
#        COMMAND ${CMAKE_AR} rc libveamy_target.a $<TARGET_FILE:libutilities> $<TARGET_FILE:libpoly> $<TARGET_FILE:libvem>)

#add_library(libveamy STATIC IMPORTED)
#set_target_properties(libveamy PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libveamy_target.a)

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS libveamy EXPORT VeamyConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION include)  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
install(EXPORT VeamyConfig DESTINATION share/VeamyConfig/cmake)
export(TARGETS libveamy FILE VeamyConfig.cmake)