#ifndef __Material_Editor_Common_H__
#define __Material_Editor_Common_H__
#include "Ogre.h"

Ogre::String getFilteringName(Ogre::FilterOptions& option);
Ogre::String getAddressingName(Ogre::TextureUnitState::TextureAddressingMode& mode);
Ogre::String getLayerBlendName(Ogre::LayerBlendSource& blend);
Ogre::String getLayerBlendOperationName(Ogre::LayerBlendOperationEx& operation);
Ogre::String getSceneBlendFactorName(Ogre::SceneBlendFactor& factor);
Ogre::String getSceneBlendName(Ogre::SceneBlendFactor SourceBlendFactor,
							   Ogre::SceneBlendFactor DestBlendFactor);
Ogre::String getCompareFunctionName(Ogre::CompareFunction function);
Ogre::String getManualCullingModeName(Ogre::ManualCullingMode maualCullingMode);
Ogre::String getCullingModeName(Ogre::CullingMode cullingMode);
Ogre::String getShadingModeName(Ogre::ShadeOptions shaderOpt);
Ogre::String getPolygonModeName(Ogre::PolygonMode polygonMode);
Ogre::String getFogModeName(Ogre::FogMode fogMode);
Ogre::String getLightTypeName(Ogre::Light::LightTypes  lightType);
Ogre::String getEnvironmentTypeName(Ogre::TextureUnitState::EnvMapType envMapType);
Ogre::String getTransformTypeName(Ogre::TextureUnitState::TextureTransformType transformType);
Ogre::String getWaveformTypeName(Ogre::WaveformType waveType);


Ogre::LayerBlendOperationEx convertBlendOpEx(const Ogre::String& param);
Ogre::LayerBlendSource convertBlendSource(const Ogre::String& param);

#endif