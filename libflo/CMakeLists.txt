project(libflo)

# Core: runtime & language
set(INCLUDE_CORE

    include/flo/lang/environment.h
    include/flo/lang/type.h
    include/flo/lang/value.h
    include/flo/lang/number.h
    include/flo/lang/string.h

    include/flo/bytecode/opcode.h
    include/flo/bytecode/chunk.h

    include/flo/runtime/runtime.h
    )

set(SOURCE_CORE
    source/environment.cpp
    source/value.cpp
    source/number.cpp
    source/string.cpp

    source/opcode.cpp
    source/chunk.cpp

    source/runtime.cpp
    )

# Dev: the compiler
set(INCLUDE_DEV
    include/flo/dev/source.h
    include/flo/dev/token.h
    include/flo/dev/keyword.h
    include/flo/dev/tokeniser.h
    include/flo/dev/ast.h
    include/flo/dev/parser.h
    include/flo/dev/bytecode_generator.h

    include/flo/dev/compiler.h
    include/flo/dev/compile_error.h

    include/flo/dev/ast_printer.h
    include/flo/dev/chunk_factory.h
    )

set(SOURCE_DEV
    source/source.cpp
    source/token.cpp
    source/keyword.cpp
    source/tokeniser.cpp
    source/ast.cpp
    source/parser.cpp
    source/bytecode_generator.cpp

    source/compiler.cpp
    source/compile_error.cpp

    source/ast_printer.cpp
    source/chunk_factory.cpp
    )


add_library(${PROJECT_NAME}
    ${INCLUDE_CORE} ${SOURCE_CORE}
    ${INCLUDE_DEV} ${SOURCE_DEV}
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(flo
    main.cpp
    )
target_link_libraries(flo ${PROJECT_NAME})



