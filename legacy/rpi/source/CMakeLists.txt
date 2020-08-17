# Automatically create a list of source files
file(GLOB SourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Automatically create a list of header files
file(GLOB HeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(AllFiles "${SourceFiles};${HeaderFiles}")
source_group("All Files" FILES ${AllFiles})

# Build target
add_executable(BitMatrix ${AllFiles})
set_target_properties(BitMatrix PROPERTIES DEBUG_POSTFIX "d" CXX_STANDARD 14) 

target_include_directories(BitMatrix BEFORE PRIVATE
    ${PROJECT_SOURCE_DIR}/source
)

target_precompile_headers(BitMatrix
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Canvas.h
)

target_link_libraries(BitMatrix
    wiringPi
    pthread
    X11
)
