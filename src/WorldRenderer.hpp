#pragma once

#include "IBL.hpp"
#include "TransientResources.hpp"
#include "TiledLighting.hpp"
#include "ShadowRenderer.hpp"

#include "Passes/GBufferPass.hpp"
#include "Passes/DeferredLightingPass.hpp"
#include "Passes/SkyboxPass.hpp"
#include "Passes/WeightedBlendedPass.hpp"
#include "Passes/TransparencyCompositionPass.hpp"
#include "Passes/WireframePass.hpp"
#include "Passes/BrightPass.hpp"
#include "Passes/SSAO.hpp"
#include "Passes/SSR.hpp"
#include "Passes/TonemapPass.hpp"
#include "Passes/FXAA.hpp"
#include "Passes/GammaCorrectionPass.hpp"
#include "Passes/Vignette.hpp"
#include "Passes/Blur.hpp"

struct LightProbe {
  Texture diffuse, specular;
};

enum class OutputMode : uint32_t {
  Depth = 0,
  Emissive,
  BaseColor,
  Normal,
  Metallic,
  Roughness,
  AmbientOcclusion,

  SSAO,
  BrightColor,
  Reflections,

  Accum,
  Reveal,

  LightHeatmap,

  HDR,
  FinalImage,
};
enum RenderFeature_ : uint32_t {
  RenderFeature_None = 0,

  RenderFeature_Shadows = 1 << 1,
  RenderFeature_SSAO = 1 << 2,
  RenderFeature_SSR = 1 << 3,
  RenderFeature_Bloom = 1 << 4,
  RenderFeature_FXAA = 1 << 5,
  RenderFeature_Vignette = 1 << 6,

  RenderFeature_Default = RenderFeature_Shadows | RenderFeature_SSAO |
                          RenderFeature_Bloom | RenderFeature_FXAA |
                          RenderFeature_Vignette,

  RenderFeature_All = RenderFeature_Default | RenderFeature_SSR,
};

enum DebugFlag_ : uint32_t {
  DebugFlag_None = 0,
  DebugFlag_Wireframe = 1 << 1,
  DebugFlag_CascadeSplits = 1 << 2,
};

struct RenderSettings {
  OutputMode outputMode{OutputMode::FinalImage};
  uint32_t renderFeatures{RenderFeature_Default};
  struct {
    float threshold{1.2f};
    int32_t numPasses{5};
    float blurScale{1.4f};
  } bloom;
  Tonemap tonemap{Tonemap::ACES};
  uint32_t debugFlags{0u};
};

class WorldRenderer {
public:
  explicit WorldRenderer(RenderContext &);
  ~WorldRenderer();

  void setSkybox(Texture &cubemap);

  void drawFrame(const RenderSettings &, Extent2D resolution,
                 const PerspectiveCamera &, std::span<const Light>,
                 std::span<const Renderable>, float deltaTime);

private:
  void _setupPipelines();

  struct FrameInfo {
    float deltaTime;
    Extent2D resolution;
    const PerspectiveCamera &camera;
    uint32_t features;
  };
  void _uploadFrameBlock(FrameGraph &, FrameGraphBlackboard &,
                         const FrameInfo &);

  [[nodiscard]] FrameGraphResource
  _addColor(FrameGraph &, FrameGraphResource target, FrameGraphResource source);
  void _present(FrameGraph &, FrameGraphBlackboard &, OutputMode);

private:
  RenderContext &m_renderContext;

  float m_time{0.0f};

  IBL m_ibl;
  Texture m_brdf;

  Texture *m_skybox{nullptr};
  LightProbe m_globalLightProbe;

  // ---

  TransientResources m_transientResources;

  TiledLighting m_tiledLighting;
  ShadowRenderer m_shadowRenderer;

  GBufferPass m_gBufferPass;
  DeferredLightingPass m_deferredLightingPass;

  SkyboxPass m_skyboxPass;

  WeightedBlendedPass m_weightedBlendedPass;
  TransparencyCompositionPass m_transparencyCompositionPass;

  WireframePass m_wireframePass;

  BrightPass m_brightPass;
  SSAO m_ssao;
  SSR m_ssr;

  TonemapPass m_tonemapPass;
  FXAA m_fxaa;
  GammaCorrectionPass m_gammaCorrectionPass;
  VignettePass m_vignettePass;

  Blur m_blur;

  GraphicsPipeline m_blitPipeline;
  GraphicsPipeline m_additiveBlitPipeline;
};
