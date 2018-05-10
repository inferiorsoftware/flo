project(libflo)

# Core: runtime & language
set(INCLUDE_CORE
    
    )

set(SOURCE_CORE
    
    )

# Dev: the compiler
set(INCLUDE_DEV
    include/flo/dev/source.h
    include/flo/dev/token.h

    include/flo/dev/tokeniser.h

    include/flo/dev/compile_error.h
    )

set(SOURCE_DEV
    source/source.cpp
    source/token.cpp

    source/tokeniser.cpp

    source/compile_error.cpp
    )


add_library(${PROJECT_NAME}
    ${INCLUDE_CORE} ${SOURCE_CORE}
    ${INCLUDE_DEV} ${SOURCE_DEV}
    )


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
