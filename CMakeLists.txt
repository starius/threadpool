cmake_minimum_required(VERSION 2.6)


add_definitions( "-Wall -Wextra" )
add_definitions( "-std=c++0x" )


################################################################################
## Libraries ###################################################################

find_package( Qt4
    COMPONENTS
        QtCore
    REQUIRED
)
include( ${QT_USE_FILE} )


################################################################################
## test ########################################################################

add_executable( test
    test.cpp
)

target_link_libraries( test
    ${QT_LIBRARIES}
)
