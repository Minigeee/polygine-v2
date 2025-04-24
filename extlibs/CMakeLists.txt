# Install dependencies
include(../cmake/CPM.cmake)
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cache/cpm")

# ---

# Loguru
CPMAddPackage(
  NAME loguru
  GITHUB_REPOSITORY emilk/loguru
  VERSION 2.1.0
  OPTIONS
    "LOGURU_WITH_STREAMS ON"
    "LOGURU_INSTALL OFF"
)
if(loguru_ADDED)
  add_library(loguru STATIC ${loguru_SOURCE_DIR}/loguru.cpp)
  target_include_directories(loguru PUBLIC ${loguru_SOURCE_DIR})
  target_compile_definitions(loguru PUBLIC LOGURU_WITH_STREAMS=1)
  if(WIN32)
    target_link_libraries(loguru PUBLIC dbghelp)
  else()
    find_package(Threads REQUIRED)
    target_link_libraries(loguru PUBLIC Threads::Threads dl)
  endif()
endif()

target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${loguru_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(polygine PUBLIC loguru)

# ---

# Hashmap
CPMAddPackage("gh:Tessil/hopscotch-map#master")
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${hopscotch-map_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# ---

# glm
CPMAddPackage("gh:g-truc/glm#master")
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${glm_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

# sdl
CPMAddPackage("gh:libsdl-org/SDL#main")
target_link_libraries(polygine PUBLIC SDL3::SDL3)

# ---

# Vulkan
CPMAddPackage("gh:KhronosGroup/Vulkan-Headers#main")
target_include_directories(polygine PRIVATE
    ${Vulkan-Headers_SOURCE_DIR}/include
)
set(Vulkan_INCLUDE_DIRS ${Vulkan-Headers_SOURCE_DIR}/include)
set(Vulkan_INCLUDE_DIR ${Vulkan-Headers_SOURCE_DIR}/include)

# Diligent
CPMAddPackage("gh:diligentgraphics/DiligentCore#master")
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${DiligentCore_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(polygine PUBLIC
    Diligent-GraphicsEngineOpenGL-shared
    Diligent-GraphicsEngineVk-shared)

# ---

# stb
CPMAddPackage("gh:nothings/stb#master")
target_include_directories(polygine PRIVATE ${stb_SOURCE_DIR})
