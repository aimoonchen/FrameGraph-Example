#include "App.hpp"
#include "GLFW/glfw3.h"
#include "Math.hpp"
#include "LightUtility.hpp"

#include "TextureLoader.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "spdlog/spdlog.h"

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

#include <random>
#include <array>

namespace {

const std::filesystem::path kAssetsDir{"./assets/"};

ImGuiKey remapKey(int keycode) {
  switch (keycode) {
  case GLFW_KEY_TAB:
    return ImGuiKey_Tab;
  case GLFW_KEY_LEFT:
    return ImGuiKey_LeftArrow;
  case GLFW_KEY_RIGHT:
    return ImGuiKey_RightArrow;
  case GLFW_KEY_UP:
    return ImGuiKey_UpArrow;
  case GLFW_KEY_DOWN:
    return ImGuiKey_DownArrow;
  case GLFW_KEY_PAGE_UP:
    return ImGuiKey_PageUp;
  case GLFW_KEY_PAGE_DOWN:
    return ImGuiKey_PageDown;
  case GLFW_KEY_HOME:
    return ImGuiKey_Home;
  case GLFW_KEY_END:
    return ImGuiKey_End;
  case GLFW_KEY_INSERT:
    return ImGuiKey_Insert;
  case GLFW_KEY_DELETE:
    return ImGuiKey_Delete;
  case GLFW_KEY_BACKSPACE:
    return ImGuiKey_Backspace;
  case GLFW_KEY_SPACE:
    return ImGuiKey_Space;
  case GLFW_KEY_ENTER:
    return ImGuiKey_Enter;
  case GLFW_KEY_ESCAPE:
    return ImGuiKey_Escape;
  case GLFW_KEY_APOSTROPHE:
    return ImGuiKey_Apostrophe;
  case GLFW_KEY_COMMA:
    return ImGuiKey_Comma;
  case GLFW_KEY_MINUS:
    return ImGuiKey_Minus;
  case GLFW_KEY_PERIOD:
    return ImGuiKey_Period;
  case GLFW_KEY_SLASH:
    return ImGuiKey_Slash;
  case GLFW_KEY_SEMICOLON:
    return ImGuiKey_Semicolon;
  case GLFW_KEY_EQUAL:
    return ImGuiKey_Equal;
  case GLFW_KEY_LEFT_BRACKET:
    return ImGuiKey_LeftBracket;
  case GLFW_KEY_BACKSLASH:
    return ImGuiKey_Backslash;
  case GLFW_KEY_RIGHT_BRACKET:
    return ImGuiKey_RightBracket;
  case GLFW_KEY_GRAVE_ACCENT:
    return ImGuiKey_GraveAccent;
  case GLFW_KEY_CAPS_LOCK:
    return ImGuiKey_CapsLock;
  case GLFW_KEY_SCROLL_LOCK:
    return ImGuiKey_ScrollLock;
  case GLFW_KEY_NUM_LOCK:
    return ImGuiKey_NumLock;
  case GLFW_KEY_PRINT_SCREEN:
    return ImGuiKey_PrintScreen;
  case GLFW_KEY_PAUSE:
    return ImGuiKey_Pause;
  case GLFW_KEY_KP_0:
    return ImGuiKey_Keypad0;
  case GLFW_KEY_KP_1:
    return ImGuiKey_Keypad1;
  case GLFW_KEY_KP_2:
    return ImGuiKey_Keypad2;
  case GLFW_KEY_KP_3:
    return ImGuiKey_Keypad3;
  case GLFW_KEY_KP_4:
    return ImGuiKey_Keypad4;
  case GLFW_KEY_KP_5:
    return ImGuiKey_Keypad5;
  case GLFW_KEY_KP_6:
    return ImGuiKey_Keypad6;
  case GLFW_KEY_KP_7:
    return ImGuiKey_Keypad7;
  case GLFW_KEY_KP_8:
    return ImGuiKey_Keypad8;
  case GLFW_KEY_KP_9:
    return ImGuiKey_Keypad9;
  case GLFW_KEY_KP_DECIMAL:
    return ImGuiKey_KeypadDecimal;
  case GLFW_KEY_KP_DIVIDE:
    return ImGuiKey_KeypadDivide;
  case GLFW_KEY_KP_MULTIPLY:
    return ImGuiKey_KeypadMultiply;
  case GLFW_KEY_KP_SUBTRACT:
    return ImGuiKey_KeypadSubtract;
  case GLFW_KEY_KP_ADD:
    return ImGuiKey_KeypadAdd;
  case GLFW_KEY_KP_ENTER:
    return ImGuiKey_KeypadEnter;
  case GLFW_KEY_KP_EQUAL:
    return ImGuiKey_KeypadEqual;
  case GLFW_KEY_LEFT_SHIFT:
    return ImGuiKey_LeftShift;
  case GLFW_KEY_LEFT_CONTROL:
    return ImGuiKey_LeftCtrl;
  case GLFW_KEY_LEFT_ALT:
    return ImGuiKey_LeftAlt;
  case GLFW_KEY_LEFT_SUPER:
    return ImGuiKey_LeftSuper;
  case GLFW_KEY_RIGHT_SHIFT:
    return ImGuiKey_RightShift;
  case GLFW_KEY_RIGHT_CONTROL:
    return ImGuiKey_RightCtrl;
  case GLFW_KEY_RIGHT_ALT:
    return ImGuiKey_RightAlt;
  case GLFW_KEY_RIGHT_SUPER:
    return ImGuiKey_RightSuper;
  case GLFW_KEY_MENU:
    return ImGuiKey_Menu;
  case GLFW_KEY_0:
    return ImGuiKey_0;
  case GLFW_KEY_1:
    return ImGuiKey_1;
  case GLFW_KEY_2:
    return ImGuiKey_2;
  case GLFW_KEY_3:
    return ImGuiKey_3;
  case GLFW_KEY_4:
    return ImGuiKey_4;
  case GLFW_KEY_5:
    return ImGuiKey_5;
  case GLFW_KEY_6:
    return ImGuiKey_6;
  case GLFW_KEY_7:
    return ImGuiKey_7;
  case GLFW_KEY_8:
    return ImGuiKey_8;
  case GLFW_KEY_9:
    return ImGuiKey_9;
  case GLFW_KEY_A:
    return ImGuiKey_A;
  case GLFW_KEY_B:
    return ImGuiKey_B;
  case GLFW_KEY_C:
    return ImGuiKey_C;
  case GLFW_KEY_D:
    return ImGuiKey_D;
  case GLFW_KEY_E:
    return ImGuiKey_E;
  case GLFW_KEY_F:
    return ImGuiKey_F;
  case GLFW_KEY_G:
    return ImGuiKey_G;
  case GLFW_KEY_H:
    return ImGuiKey_H;
  case GLFW_KEY_I:
    return ImGuiKey_I;
  case GLFW_KEY_J:
    return ImGuiKey_J;
  case GLFW_KEY_K:
    return ImGuiKey_K;
  case GLFW_KEY_L:
    return ImGuiKey_L;
  case GLFW_KEY_M:
    return ImGuiKey_M;
  case GLFW_KEY_N:
    return ImGuiKey_N;
  case GLFW_KEY_O:
    return ImGuiKey_O;
  case GLFW_KEY_P:
    return ImGuiKey_P;
  case GLFW_KEY_Q:
    return ImGuiKey_Q;
  case GLFW_KEY_R:
    return ImGuiKey_R;
  case GLFW_KEY_S:
    return ImGuiKey_S;
  case GLFW_KEY_T:
    return ImGuiKey_T;
  case GLFW_KEY_U:
    return ImGuiKey_U;
  case GLFW_KEY_V:
    return ImGuiKey_V;
  case GLFW_KEY_W:
    return ImGuiKey_W;
  case GLFW_KEY_X:
    return ImGuiKey_X;
  case GLFW_KEY_Y:
    return ImGuiKey_Y;
  case GLFW_KEY_Z:
    return ImGuiKey_Z;
  case GLFW_KEY_F1:
    return ImGuiKey_F1;
  case GLFW_KEY_F2:
    return ImGuiKey_F2;
  case GLFW_KEY_F3:
    return ImGuiKey_F3;
  case GLFW_KEY_F4:
    return ImGuiKey_F4;
  case GLFW_KEY_F5:
    return ImGuiKey_F5;
  case GLFW_KEY_F6:
    return ImGuiKey_F6;
  case GLFW_KEY_F7:
    return ImGuiKey_F7;
  case GLFW_KEY_F8:
    return ImGuiKey_F8;
  case GLFW_KEY_F9:
    return ImGuiKey_F9;
  case GLFW_KEY_F10:
    return ImGuiKey_F10;
  case GLFW_KEY_F11:
    return ImGuiKey_F11;
  case GLFW_KEY_F12:
    return ImGuiKey_F12;
  case GLFW_KEY_F13:
    return ImGuiKey_F13;
  case GLFW_KEY_F14:
    return ImGuiKey_F14;
  case GLFW_KEY_F15:
    return ImGuiKey_F15;
  case GLFW_KEY_F16:
    return ImGuiKey_F16;
  case GLFW_KEY_F17:
    return ImGuiKey_F17;
  case GLFW_KEY_F18:
    return ImGuiKey_F18;
  case GLFW_KEY_F19:
    return ImGuiKey_F19;
  case GLFW_KEY_F20:
    return ImGuiKey_F20;
  case GLFW_KEY_F21:
    return ImGuiKey_F21;
  case GLFW_KEY_F22:
    return ImGuiKey_F22;
  case GLFW_KEY_F23:
    return ImGuiKey_F23;
  case GLFW_KEY_F24:
    return ImGuiKey_F24;
  }
  return ImGuiKey_None;
}

void showMetricsOverlay() {
  const auto windowFlags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

  const ImVec2 pad{10.0f, 10.0f};
  const auto *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos + pad, ImGuiCond_Always);

  ImGui::SetNextWindowBgAlpha(0.35f);
  if (ImGui::Begin("MetricsOverlay", nullptr, windowFlags)) {
    const auto frameRate = ImGui::GetIO().Framerate;
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / frameRate, frameRate);
  }
  ImGui::End();
}
void renderSettingsWidget(RenderSettings &settings) {
  if (ImGui::Begin("RenderSettings")) {
    ImGui::Combo("OutputMode",
                 reinterpret_cast<int32_t *>(&settings.outputMode),
                 "Depth\0Emissive\0BaseColor\0Normal\0Metallic\0Roughness\0"
                 "AmbientOcclusion\0SSAO\0BrightColor\0Reflections\0Accum\0"
                 "Reveal\0LightHeatmap\0HDR\0FinalImage");

    ImGui::Text("Features:");
    ImGui::CheckboxFlags("Shadows", &settings.renderFeatures,
                         RenderFeature_Shadows);
    ImGui::CheckboxFlags("GlobalIllumination", &settings.renderFeatures,
                         RenderFeature_GI);
    if (settings.renderFeatures & RenderFeature_GI) {
      ImGui::SliderInt("NumPropagations##GI",
                       &settings.globalIllumination.numPropagations, 1, 12);
    }

    ImGui::CheckboxFlags("IBL", &settings.renderFeatures, RenderFeature_IBL);

    ImGui::CheckboxFlags("SSAO", &settings.renderFeatures, RenderFeature_SSAO);
    ImGui::CheckboxFlags("SSR", &settings.renderFeatures, RenderFeature_SSR);

    ImGui::Separator();

    ImGui::CheckboxFlags("Bloom", &settings.renderFeatures,
                         RenderFeature_Bloom);
    if (settings.renderFeatures & RenderFeature_Bloom) {
      ImGui::SliderFloat("Radius", &settings.bloom.radius, 0.001f, 1.0f);
      ImGui::SliderFloat("Strength", &settings.bloom.strength, 0.001f, 1.0f);
    }

    ImGui::Separator();

    ImGui::CheckboxFlags("FXAA", &settings.renderFeatures, RenderFeature_FXAA);

    ImGui::CheckboxFlags("Vignette", &settings.renderFeatures,
                         RenderFeature_Vignette);

    ImGui::Separator();

    ImGui::Combo("Tonemap", reinterpret_cast<int32_t *>(&settings.tonemap),
                 "Clamp\0ACES\0Filmic\0Reinhard\0Uncharted\0");

    ImGui::Separator();

    ImGui::Text("DebugFlags:");
    ImGui::CheckboxFlags("Wireframe", &settings.debugFlags,
                         DebugFlag_Wireframe);
    if (settings.renderFeatures & RenderFeature_Shadows) {
      ImGui::CheckboxFlags("Visualize Cascade Splits", &settings.debugFlags,
                           DebugFlag_CascadeSplits);
    }
    if (settings.renderFeatures & RenderFeature_GI) {
      ImGui::CheckboxFlags("VPL", &settings.debugFlags, DebugFlag_VPL);
      ImGui::CheckboxFlags("Radiance", &settings.debugFlags,
                           DebugFlag_RadianceOnly);
    }
  }
  ImGui::End();
}

void cameraController(PerspectiveCamera &camera, const ImVec2 &mouseDelta,
                      bool mouseButtons[]) {
  constexpr auto LMB = 0, RMB = 1;
  constexpr auto kMouseSensitivity = 0.12f;
  constexpr auto kMoveSensitivity = kMouseSensitivity * 0.2f;

  auto position = camera.getPosition();
  auto yaw = camera.getYaw();
  auto pitch = camera.getPitch();

  if (mouseButtons[LMB] && mouseButtons[RMB]) {
    // Zoom on LMB + RMB
    position -= camera.getForward() * (mouseDelta.x * kMoveSensitivity);
    position += camera.getForward() * (mouseDelta.y * kMoveSensitivity);
  } else {
    if (mouseButtons[LMB]) {
      // Rotate on LMB
      yaw += mouseDelta.x * kMouseSensitivity;
      pitch -= mouseDelta.y * kMouseSensitivity;

      if (pitch > 89.f) pitch = 89.f;
      if (pitch < -89.f) pitch = -89.f;
    } else if (mouseButtons[RMB]) {
      // Pan on RMB
      position -= camera.getRight() * (mouseDelta.x * kMoveSensitivity);
      position += camera.getUp() * (mouseDelta.y * kMoveSensitivity);
    }
  }
  camera.setPosition(position).setYaw(yaw).setPitch(pitch).update();
}

[[nodiscard]] auto getRandomOf(std::default_random_engine &gen,
                               auto container) {
  std::uniform_int_distribution<std::size_t> d{0u, container.size() - 1u};
  return *std::next(container.begin(), d(gen));
}

} // namespace

//
// BaseApp class:
//

BaseApp::BaseApp() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}
BaseApp::~BaseApp() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

//
// App class:
//

App::App(const Config &config) {
  m_window = glfwCreateWindow(config.width, config.height,
                              config.caption.data(), nullptr, nullptr);

  glfwSetWindowUserPointer(m_window, this);
  glfwSetWindowFocusCallback(m_window, _onFocus);
  glfwSetCursorEnterCallback(m_window, _onCursorEnter);
  glfwSetMouseButtonCallback(m_window, _onMouseButton);
  glfwSetScrollCallback(m_window, _onMouseScroll);
  glfwSetKeyCallback(m_window, _onKey);
  glfwSetCharCallback(m_window, _onChar);

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(config.verticalSync ? 1 : 0);

  m_renderContext = std::make_unique<RenderContext>();
  TracyGpuContext;

  m_renderer = std::make_unique<WorldRenderer>(*m_renderContext);
  m_basicShapes = std::make_unique<BasicShapes>(*m_renderContext);
  m_cubemapConverter = std::make_unique<CubemapConverter>(*m_renderContext);

  m_textureCache = std::make_unique<TextureCache>(*m_renderContext);
  m_materialCache = std::make_unique<MaterialCache>(*m_textureCache);
  m_meshCache = std::make_unique<MeshCache>(*m_renderContext, *m_textureCache);

  _setupUi();
  _setupScene();
}
App::~App() {
  m_uiRenderer.reset();
  ImGui::DestroyContext();

  m_renderContext->destroy(m_skybox);
}

void App::run() {
  using namespace std::chrono_literals;
  using clock = std::chrono::high_resolution_clock;

  constexpr fsec kTargetFrameTime{16ms};
  fsec deltaTime{kTargetFrameTime};

  auto &io = ImGui::GetIO();
  ImVec2 lastMousePos{0.0f, 0.0f};

  while (!glfwWindowShouldClose(m_window)) {
    const auto beginTicks = clock::now();
    glfwPollEvents();
    _processInput();

    io.DeltaTime = deltaTime.count();
    const auto swapchainExtent = _getSwapchainExtent();
    io.DisplaySize = ImVec2{static_cast<float>(swapchainExtent.width),
                            static_cast<float>(swapchainExtent.height)};
    const auto mousePosition = _getMousePosition();
    io.MousePos = ImVec2{mousePosition.x, mousePosition.y};

    for (int32_t i{0}; i < IM_ARRAYSIZE(io.MouseDown); ++i) {
      io.MouseDown[i] =
        m_mouseJustPressed[i] || glfwGetMouseButton(m_window, i) != 0;
      m_mouseJustPressed[i] = false;
    }

    ImGui::NewFrame();

    m_camera.setPerspective(
      60.0f, static_cast<float>(swapchainExtent.width) / swapchainExtent.height,
      0.1f, max3(m_sceneAABB.getExtent()));

    if (!io.WantCaptureMouse)
      cameraController(m_camera, io.MousePos - lastMousePos, io.MouseDown);
    lastMousePos = io.MousePos;

    _update(deltaTime);

    showMetricsOverlay();
    renderSettingsWidget(m_renderSettings);

    m_renderer->drawFrame(m_renderSettings, swapchainExtent, m_sceneAABB,
                          m_camera, m_lights, m_renderables, deltaTime.count());

    ImGui::Render();
    m_uiRenderer->draw(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
    TracyGpuCollect;

    deltaTime = clock::now() - beginTicks;
    if (deltaTime > 1s) deltaTime = kTargetFrameTime;

    FrameMark;
  }
}

void App::_setupUi() {
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();
  auto &io = ImGui::GetIO();

  io.ConfigWindowsMoveFromTitleBarOnly = true;
#if 0
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
  io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
#endif
  io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  io.Fonts->AddFontDefault();

  m_uiRenderer = std::make_unique<ImGuiRenderer>(*m_renderContext, *io.Fonts);
}

void App::_setupScene() {
  auto equirectangular =
    loadTexture(kAssetsDir / "newport_loft.hdr", *m_renderContext);
  m_skybox = m_cubemapConverter->equirectangularToCubemap(*equirectangular);
  m_renderContext->destroy(*equirectangular);
  m_renderer->setSkybox(m_skybox);

#if 0
  m_camera.setPosition({10.0f, 8.0f, -10.0f}).setPitch(-25.0f).setYaw(135.0f);

  const auto loadMaterial = [&cache =
                               *m_materialCache](const std::string_view name) {
    return cache.load(kAssetsDir /
                      std::format("materials/{0}/{0}.material", name));
  };

  auto stoneBlockWallMaterial = loadMaterial("stone_block_wall");
  _addRenderable(m_basicShapes->getPlane(),
                 glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, -1.5f, 0.0f}),
                 *stoneBlockWallMaterial, MaterialFlag_ReceiveShadow);

  // clang-format off
  const std::vector<std::string> materialNames{
#if 1
    "stringy_marble",
    "streaky_metal",
    "gold_scuffed",
    "rusted_iron",
    "warped_sheet_metal",
    "frosted_glass",
#endif
    "floor_tiles",
  };
  // clang-format on

  std::vector<std::reference_wrapper<const Material>> materials;
  std::transform(
    materialNames.cbegin(), materialNames.cend(),
    std ::back_inserter(materials),
    [loadMaterial](const std::string_view name) -> const Material & {
      return *loadMaterial(name);
    });

  _createTower({3, 3, 3}, 2.5f, {0.0f, -2.5f, 0.0f}, materials);
  _spawnPointLights(10, 10, 2.0f);
#else
  m_camera.setPosition({42.6f, 28.0f, -7.4f}).setPitch(-25.0f).setYaw(170.0f);

  auto sponza = m_meshCache->load(kAssetsDir / "meshes/Sponza/Sponza.gltf");
  const glm::mat4 scale = glm::scale(glm::mat4{1.0f}, glm::vec3{5.0f});
  m_sceneAABB = sponza->aabb.transform(scale);
  _addRenderable(*sponza, scale, std::nullopt);
#endif

  _createSun();
}

void App::_addRenderable(
  const Mesh &mesh, const glm::mat4 &m,
  std::optional<std::reference_wrapper<const Material>> materialOverride,
  uint8_t flags) {

  auto id = 0;
  for (auto &[_, material] : mesh.subMeshes) {
    m_renderables.push_back(Renderable{
      .mesh = mesh,
      .subMeshIndex = id++,
      .material = materialOverride ? materialOverride->get() : *material,
      .flags = flags,
      .modelMatrix = m,
      .aabb = mesh.aabb.transform(m),
    });
  }
}

void App::_createTower(
  const glm::ivec3 &dimensions, float spacing, const glm::vec3 &startPosition,
  std::span<std::reference_wrapper<const Material>> materials) {
  std::random_device rd{};
  std::default_random_engine gen{rd()};

  const auto &mesh = m_basicShapes->getSphere();

  for (int32_t row{0}; row < dimensions.y; ++row)
    for (int32_t col{0}; col < dimensions.x; ++col)
      for (int32_t z{0}; z < dimensions.z; ++z) {
        const glm::vec3 localPosition{(col - (dimensions.x / 2)) * spacing,
                                      row * spacing + spacing,
                                      (z - (dimensions.z / 2)) * spacing};
        auto material = getRandomOf(gen, materials);
        _addRenderable(
          mesh, glm::translate(glm::mat4{1.0}, startPosition + localPosition),
          material);
      }
}

void App::_createSun() {
  m_lights.push_back(Light{
    .type = LightType::Directional,
    .visible = true,
    //.direction = glm::vec3{-0.875632942f, 0.478866994f, 0.0628755689},
    .direction = glm::vec3{-0.551486731f, -0.827472687f, 0.105599940f},
    .color = glm::vec3{1.0f},
    .intensity = 10.0f,
  });
}
void App::_spawnPointLights(uint16_t width, uint16_t depth, float step) {
  std::uniform_real_distribution<float> dist{0.0f, 1.0f};
  std::random_device rd{};
  std::default_random_engine gen{rd()};

  for (int32_t x{-width}; x <= width; x += step) {
    for (int32_t z{-depth}; z <= depth; z += step) {
      const glm::vec3 position{x, -0.5f, z};
      const glm::vec3 color{dist(gen), dist(gen), dist(gen)};
      const auto range = calculateLightRadius(color);
      m_lights.emplace_back(Light{
        .type = LightType::Point,
        .position = position,
        .color = color,
        .intensity = range * 2.0f,
        .range = range,
        .castsShadow = false,
      });
    }
  }
}

void App::_update(fsec dt) {
  if (ImGui::Begin("Scene")) {
    const auto extent = m_sceneAABB.getExtent();
    ImGui::Text("Extent: [%.2f, %.2f, %.2f]", extent.x, extent.y, extent.z);

    if (ImGui::CollapsingHeader("Camera")) {
      const auto &position = m_camera.getPosition();
      ImGui::Text("Position = [%.2f, %.2f, %.2f]", position.x, position.y,
                  position.z);
      const auto &forward = m_camera.getForward();
      ImGui::Text("Direction = [%.2f, %.2f, %.2f]", forward.x, forward.y,
                  forward.z);
    }

    if (auto *light = m_lights.empty() ? nullptr : &m_lights.front(); light) {
      if (ImGui::CollapsingHeader("Sun")) {
        if (glm::vec4 color{light->color, light->intensity};
            ImGui::ColorEdit4("Color", glm::value_ptr(color))) {
          light->color = glm::vec3{color};
          light->intensity = color.a;
        }

        if (ImGui::Button("Pilot")) {
          light->direction = m_camera.getForward();
        }
        ImGui::SameLine();
        static bool animate{false};
        if (ImGui::Button("Animate")) animate = !animate;

        if (animate) {
          static float speed = 15.0f;
          ImGui::SliderFloat("deg/s", &speed, 0.01f, 90.0f);
          auto &direction = light->direction;
          direction = glm::rotateY(direction, glm::radians(speed) * dt.count());
        }
      }
    }
  }
  ImGui::End();
}
void App::_processInput() {
  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(m_window, true);
}

Extent2D App::_getSwapchainExtent() const {
  int32_t w, h;
  glfwGetFramebufferSize(m_window, &w, &h);
  return {static_cast<uint32_t>(w), static_cast<uint32_t>(h)};
}
glm::vec2 App::_getMousePosition() const {
  double x, y;
  glfwGetCursorPos(m_window, &x, &y);
  return {static_cast<float>(x), static_cast<float>(y)};
}

void App::_onMouseButton(GLFWwindow *window, int button, int action, int mods) {
  auto app = static_cast<App *>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS && button >= 0 &&
      button < IM_ARRAYSIZE(m_mouseJustPressed))
    app->m_mouseJustPressed[button] = true;
}
void App::_onMouseScroll(GLFWwindow *window, double xoffset, double yoffset) {
  auto &io = ImGui::GetIO();
  io.MouseWheelH += static_cast<float>(xoffset);
  io.MouseWheel += static_cast<float>(yoffset);
}
void App::_onKey(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  auto &io = ImGui::GetIO();
  io.SetKeyEventNativeData(remapKey(key), scancode, scancode);
}
void App::_onFocus(GLFWwindow *window, int focused) {
  auto &io = ImGui::GetIO();
  io.AddFocusEvent(focused != 0);
}
void App::_onCursorEnter(GLFWwindow *window, int entered) {}
void App::_onChar(GLFWwindow *window, unsigned int c) {
  auto &io = ImGui::GetIO();
  io.AddInputCharacter(c);
}
