# YAREN — Yet Another Rendering Engine

A minimal OpenGL rendering engine built from scratch in C++ to understand how GPUs draw things on screen.

No game engine. No high-level abstraction. Just raw OpenGL, shaders, and buffer management.

## What it does

- Renders solid-colored shapes and textured quads with translation/transformation support
- Compiles and binds GLSL shaders at runtime
- Manages GPU memory through vertex buffers, index buffers, and vertex array objects
- Loads textures from disk (PNG, JPG) via `stb_image` and binds them to shader samplers
- Provides a real-time ImGui overlay to tweak rendering parameters without recompiling

## Architecture

```
src/
├── Application.cpp       # Entry point — menu-driven test harness
├── Renderer.h/cpp        # Draw call orchestration, clear, error handling
├── Shader.h/cpp          # Shader loading, compilation, uniform management
├── Texture.h/cpp         # Image loading (stb_image), texture binding
├── VertexBuffer.h/cpp    # GPU vertex buffer allocation and data upload
├── IndexBuffer.h/cpp     # GPU index buffer for indexed drawing
├── VertexArray.h/cpp     # VAO — binds vertex buffers to attribute layouts
├── VertexLayout.h/cpp    # Defines per-vertex data structure (position, UV, etc.)
├── tests/
│   ├── Test.h            # Base test interface
│   └── TestClearColor.*  # Screen clear color picker (ImGui-driven)
├── vendor/
│   ├── glm/              # Vector/matrix math (glm)
│   ├── imgui/            # Debug UI overlay (Dear ImGui)
│   └── stb_image/        # Single-header image loader
res/
├── shaders/
│   └── Basic.shader      # Vertex + fragment shader (GLSL)
└── textures/             # Texture assets
```

## Design decisions

**Why no engine abstraction?** The goal was to understand what happens between a draw call and pixels on screen — not to build a usable engine. Every class maps directly to an OpenGL concept.

**Why ImGui?** Lets you change uniforms, clear colors, and transformation parameters at runtime without touching code. Makes the feedback loop instant.

**Why separate VertexLayout from VertexBuffer?** Decoupling the data (what bytes are in the buffer) from the interpretation (which bytes are position vs. UV vs. color) makes it easy to reuse buffers with different shaders.

## Build

Requires Visual Studio (Windows). Open `YAREN.sln` and build.

Dependencies (`src/vendor/`) are committed — no package manager needed.

## Tech

- **C++** (97.4%) / **GLSL** (2.6%)
- OpenGL for rendering
- Dear ImGui for debug UI
- glm for math
- stb_image for texture loading


# Solid colors

<!---![clear_color_1](https://user-images.githubusercontent.com/52310272/146238711-d3d285d0-97c1-4923-85f6-cb2239567e20.png)-->
![clear_color_2](https://user-images.githubusercontent.com/52310272/146238720-9dc817c0-ccc0-4101-9949-6a1054058157.png)
![clear_color_4](https://user-images.githubusercontent.com/52310272/146238726-9309d417-cc1d-4244-bd2d-5e348bebd177.png)
![clear_color_3](https://user-images.githubusercontent.com/52310272/146238722-afe8b5fe-b2c1-41ca-be0d-b63d34922296.png)

# Texture rendering

![texture_translation](https://user-images.githubusercontent.com/52310272/146238791-6d30546f-1b6d-44b4-8471-93dd1a8cadda.gif)
