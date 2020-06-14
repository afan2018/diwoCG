cmake_minimum_required (VERSION 3.7)

project(diwoCG)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

SET(EXECUTABLE_NAME main)

add_executable(${EXECUTABLE_NAME} main.cpp engine.cpp control.cpp screenshot.cpp obj_mesh.cpp geometries.cpp nurbs_mesh.cpp texture_ctrl.cpp light.cpp)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
IF(APPLE)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations" LINK_FLAGS "-framework OpenGL -framework GLUT")
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_SOURCE_DIR}/lib/libnfd.a)
ELSE()
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_SOURCE_DIR}/lib/nfd.a)
ENDIF()
