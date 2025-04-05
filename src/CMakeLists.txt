set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INC_DIR ${PROJECT_SOURCE_DIR}/include/ply)

# Add sources
file(GLOB AUDIO_SRC
    ${SRC_DIR}/audio/*.cpp
    ${INC_DIR}/audio/*.h
    ${INC_DIR}/audio/*.inl)
file(GLOB CORE_SRC
    ${SRC_DIR}/core/*.cpp
    ${INC_DIR}/core/*.h
    ${INC_DIR}/core/*.inl)
file(GLOB ECS_SRC
    ${SRC_DIR}/ecs/*.cpp
    ${INC_DIR}/ecs/*.h
    ${INC_DIR}/ecs/*.inl)
file(GLOB ENGINE_SRC
    ${SRC_DIR}/engine/*.cpp
    ${INC_DIR}/engine/*.h
    ${INC_DIR}/engine/*.inl)
file(GLOB GRAPHICS_SRC
    ${SRC_DIR}/graphics/*.cpp
    ${INC_DIR}/graphics/*.h
    ${INC_DIR}/graphics/*.inl)
file(GLOB MATH_SRC
    ${SRC_DIR}/math/*.cpp
    ${INC_DIR}/math/*.h
    ${INC_DIR}/math/*.inl)
file(GLOB NETWORK_SRC
    ${SRC_DIR}/network/*.cpp
    ${INC_DIR}/network/*.h
    ${INC_DIR}/network/*.inl)
file(GLOB PHYSICS_SRC
    ${SRC_DIR}/physics/*.cpp
    ${INC_DIR}/physics/*.h
    ${INC_DIR}/physics/*.inl)
file(GLOB UI_SRC
    ${SRC_DIR}/ui/*.cpp
    ${INC_DIR}/ui/*.h
    ${INC_DIR}/ui/*.inl)

source_group("Audio" FILES ${AUDIO_SRC})
source_group("Core" FILES ${CORE_SRC})
source_group("ECS" FILES ${ECS_SRC})
source_group("Engine" FILES ${ENGINE_SRC})
source_group("Graphics" FILES ${GRAPHICS_SRC})
source_group("Math" FILES ${MATH_SRC})
source_group("Network" FILES ${NETWORK_SRC})
source_group("Physics" FILES ${PHYSICS_SRC})
source_group("UI" FILES ${UI_SRC})
    
# Build library
add_library(polygine STATIC
    ${AUDIO_SRC}
    ${CORE_SRC}
    ${ECS_SRC}
    ${ENGINE_SRC}
    ${GRAPHICS_SRC}
    ${MATH_SRC}
    ${NETWORK_SRC}
    ${PHYSICS_SRC}
    ${UI_SRC})
    
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)