add_library(Catch catchmain.cpp)
target_include_directories(Catch PUBLIC YSB3D)

foreach(
  i
  planetest
  triangletest
  findneartritest
  triintersecttest
  prepastetest
  pastetest
  removeoverlaptest
  GCStest
  triangulationtest
  Yinsettest
  locatetest
  reFacBoundarytest)
  set(TAR "${i}.exe")
  add_executable(${TAR} ${i}.cpp)
  target_link_libraries(${TAR} PUBLIC Catch)
  target_link_libraries(${TAR} PUBLIC Catch Boost::timer --static)
  add_test(
    NAME ${TAR}
    COMMAND ${TAR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
endforeach(i)

foreach(i YSjoin YSmeet)
  set(TAR "${i}.exe")
  add_executable(${TAR} ${i}.cpp)
  target_link_libraries(${TAR} PUBLIC Catch Boost::timer --static)
  target_compile_definitions(${TAR} PUBLIC _precise10)
  add_test(
    NAME ${TAR}
    COMMAND ${TAR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
endforeach(i)
