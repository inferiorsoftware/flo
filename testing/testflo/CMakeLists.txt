file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testflo ${SRCS})

TARGET_LINK_LIBRARIES(testflo
    libflo
    libgtest
    libgmock
)

add_test(NAME testflo
COMMAND testflo)