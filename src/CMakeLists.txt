set(supergl_SRCS
    Camera.cpp
    GameTime.cpp
    GLWindow.cpp
    Input.cpp
    Light.cpp
    Material.cpp
    Mesh.cpp
    MeshFile.cpp
    MeshRenderer.cpp
    ModuleEntry.cpp
    ShaderProgram.cpp
    Texture2D.cpp
    Transform.cpp
    Engine.cpp
    Shaders.cpp
    BasicEffect.cpp
    EffectLibrary.cpp
    Vector2.cpp
    Vector3.cpp
    Vector4.cpp
    Vectors.cpp
    )
    

if(WIN32)
    set(supergl_SRCS
        ${supergl_SRCS}
        Camera.h
        Common.h
        Effect.h
        GameTime.h
        GLWindow.h
        Input.h
        Light.h
        Material.h
        Mesh.h
        MeshFile.h
        MeshEffect.h
        MeshRenderer.h
        ShaderProgram.h
        Texture2D.h
        Transform.h
        Engine.h
        Shaders.h
        BasicEffect.h
        EffectLibrary.h
        Vector2.h
        Vector3.h
        Vector4.h
        Vectors.h
        )
endif(WIN32)


add_library(supergl SHARED ${supergl_SRCS})
target_link_libraries(supergl ${supergl_LIBRARIES})

if(UNIX)
    set_property(TARGET supergl PROPERTY PREFIX "")
    install(TARGETS supergl DESTINATION usr/lib/python3.4)
endif(UNIX)

if(WIN32)
    set_property(TARGET supergl PROPERTY SUFFIX ".pyd")
endif(WIN32)


