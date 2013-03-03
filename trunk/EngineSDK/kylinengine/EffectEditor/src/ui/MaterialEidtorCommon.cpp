#include "MaterialEidtorCommon.h"

Ogre::String getFilteringName(Ogre::FilterOptions& option)
{
	Ogre::String name;
	if(option == Ogre::FO_NONE)
		name = "none";
	else if(option == Ogre::FO_POINT)
		name = "point";
	else if(option == Ogre::FO_LINEAR)
		name = "linear";
	else if(option == Ogre::FO_ANISOTROPIC)
		name = "anisotropic";
	return name;
}
Ogre::String getAddressingName(Ogre::TextureUnitState::TextureAddressingMode& mode)
{
	Ogre::String name;
	if(mode == Ogre::TextureUnitState::TAM_WRAP)
		name = "warp";
	else if(mode == Ogre::TextureUnitState::TAM_MIRROR)
		name = "mirror";
	else if(mode == Ogre::TextureUnitState::TAM_CLAMP)
		name = "clamp";
	else if(mode == Ogre::TextureUnitState::TAM_BORDER)
		name = "border";
	return name;
}
Ogre::String getLayerBlendName(Ogre::LayerBlendSource& blend)
{
	Ogre::String name;
	if(blend == Ogre::LBS_CURRENT)
		name = "current";
	else if(blend == Ogre::LBS_TEXTURE)
		name = "texture";
	else if(blend == Ogre::LBS_DIFFUSE)
		name = "diffuse";
	else if(blend == Ogre::LBS_SPECULAR)
		name = "specular";
	else if(blend == Ogre::LBS_MANUAL)
		name = "manual";
	return name;
}
Ogre::String getLayerBlendOperationName(Ogre::LayerBlendOperationEx& operation)
{
	Ogre::String name;
	if(operation == Ogre::LBX_SOURCE1)
		name = "source1";
	else if(operation == Ogre::LBX_SOURCE2)
		name = "source2";
	else if(operation == Ogre::LBX_MODULATE)
		name = "modulate";
	else if(operation == Ogre::LBX_MODULATE_X2)
		name = "modulate_x2";
	else if(operation == Ogre::LBX_MODULATE_X4)
		name = "modulate_x4";
	else if(operation == Ogre::LBX_ADD)
		name = "add";
	else if(operation == Ogre::LBX_ADD_SIGNED)
		name = "add_signed";
	else if(operation == Ogre::LBX_ADD_SMOOTH)
		name = "smooth";
	else if(operation == Ogre::LBX_SUBTRACT)
		name = "subtract";
	else if(operation == Ogre::LBX_BLEND_DIFFUSE_ALPHA)
		name = "blend_diffuse_alpha";
	else if(operation == Ogre::LBX_BLEND_TEXTURE_ALPHA)
		name = "blend_texture_alpha";
	else if(operation == Ogre::LBX_BLEND_CURRENT_ALPHA)
		name = "blend_current_alpha";
	else if(operation == Ogre::LBX_BLEND_MANUAL)
		name = "blend_manual";
	else if(operation == Ogre::LBX_DOTPRODUCT)
		name = "dotproduct";
	else if(operation == Ogre::LBX_BLEND_DIFFUSE_COLOUR)
		name = "blend_diffuse_color";
	return name;
}
Ogre::String getSceneBlendFactorName(Ogre::SceneBlendFactor& factor)
{
	Ogre::String name;
	if(factor == Ogre::SBF_ONE)
		name = "one";
	else if(factor == Ogre::SBF_ZERO)
		name = "zero";
	else if(factor == Ogre::SBF_DEST_COLOUR)
		name = "dest_color";
	else if(factor == Ogre::SBF_SOURCE_COLOUR)
		name = "source_color";
	else if(factor == Ogre::SBF_ONE_MINUS_DEST_COLOUR)
		name = "one_minus_dest_color";
	else if(factor == Ogre::SBF_ONE_MINUS_SOURCE_COLOUR)
		name = "one_minus_source_color";
	else if(factor == Ogre::SBF_DEST_ALPHA)
		name = "dest_alpha";
	else if(factor == Ogre::SBF_SOURCE_ALPHA)
		name = "source_alpha";
	else if(factor == Ogre::SBF_ONE_MINUS_DEST_ALPHA)
		name = "one_minus_dest_alpha";
	else if(factor == Ogre::SBF_ONE_MINUS_SOURCE_ALPHA)
		name = "one_minus_source_alpha";
	return name;
}
Ogre::String getSceneBlendName(Ogre::SceneBlendFactor SourceBlendFactor,
							   Ogre::SceneBlendFactor DestBlendFactor)
{
	Ogre::String blendName;

	if(SourceBlendFactor == Ogre::SBF_SOURCE_ALPHA && DestBlendFactor == Ogre::SBF_ONE_MINUS_SOURCE_ALPHA)
		blendName= "transparent_alpha";
	else if(SourceBlendFactor == Ogre::SBF_SOURCE_COLOUR && DestBlendFactor == Ogre::SBF_ONE_MINUS_SOURCE_COLOUR)
		blendName = "transparent_color";
	else if(SourceBlendFactor == Ogre::SBF_DEST_COLOUR && DestBlendFactor == Ogre::SBF_ZERO)
		blendName = "modulate";
	else if(SourceBlendFactor == Ogre::SBF_ONE && DestBlendFactor == Ogre::SBF_ONE)
		blendName = "add";
	else if(SourceBlendFactor == Ogre::SBF_ONE && DestBlendFactor == Ogre::SBF_ZERO)
		blendName = "replace";
	else 
		blendName = "null";

	return blendName;

}
Ogre::String getCompareFunctionName(Ogre::CompareFunction function)
{
	Ogre::String name;
	if(function == Ogre::CMPF_ALWAYS_FAIL)
		name = "always_fail";
	else if(function == Ogre::CMPF_ALWAYS_PASS)
		name = "always_pass";
	else if(function == Ogre::CMPF_LESS)
		name = "less";
	else if(function == Ogre::CMPF_LESS_EQUAL)
		name = "less_equal";
	else if(function == Ogre::CMPF_EQUAL)
		name = "equal";
	else if(function == Ogre::CMPF_NOT_EQUAL)
		name = "not_equal";
	else if(function == Ogre::CMPF_GREATER_EQUAL)
		name = "greater_equal";
	else if(function == Ogre::CMPF_GREATER)
		name = "greater";

	return name;
}
Ogre::String getManualCullingModeName(Ogre::ManualCullingMode maualCullingMode)
{
	Ogre::String name;
	if(maualCullingMode == Ogre::MANUAL_CULL_NONE)
		name = "none";
	else if(maualCullingMode == Ogre::MANUAL_CULL_BACK)
		name = "back";
	else if(maualCullingMode == Ogre::MANUAL_CULL_FRONT)
		name = "front";
	return name;
}

Ogre::String getCullingModeName(Ogre::CullingMode cullingMode)
{
	Ogre::String name;
	if(cullingMode == Ogre::CULL_NONE)
		name = "none";
	else if(cullingMode == Ogre::CULL_CLOCKWISE)
		name = "clockwise";
	else if(cullingMode == Ogre::CULL_ANTICLOCKWISE)
		name = "anticlockwise";
	return name;
}
Ogre::String getShadingModeName(Ogre::ShadeOptions shaderOpt)
{
	Ogre::String name;
	if(shaderOpt == Ogre::SO_FLAT)
		name = "flat";
	else if(shaderOpt == Ogre::SO_GOURAUD)
		name = "gouraud";
	else if(shaderOpt == Ogre::SO_PHONG)
		name = "phong";
	return name;
}
Ogre::String getPolygonModeName(Ogre::PolygonMode polygonMode)
{
	Ogre::String name;
	if(polygonMode == Ogre::PM_POINTS)
		name = "points";
	else if(polygonMode == Ogre::PM_WIREFRAME)
		name = "wireframe";
	else if(polygonMode == Ogre::PM_SOLID)
		name = "solid";
	return name;
}
Ogre::String getFogModeName(Ogre::FogMode fogMode)
{
	Ogre::String name;
	if(fogMode == Ogre::FOG_NONE)
		name = "none";
	else if(fogMode == Ogre::FOG_EXP)
		name = "exp";
	else if(fogMode == Ogre::FOG_EXP2)
		name = "exp2";
	else if(fogMode == Ogre::FOG_LINEAR)
		name = "linear";
	return name;
}

Ogre::String getLightTypeName(Ogre::Light::LightTypes  lightType)
{
	Ogre::String name;
	if(lightType == Ogre::Light::LT_POINT)
		name = "point";
	else if(lightType == Ogre::Light::LT_DIRECTIONAL)
		name = "directional";
	else if(lightType == Ogre::Light::LT_SPOTLIGHT)
		name = "spot";
	return name;
}
Ogre::String getEnvironmentTypeName(Ogre::TextureUnitState::EnvMapType envMapType)
{
	Ogre::String name;
	if(envMapType == Ogre::TextureUnitState::ENV_PLANAR)
		name = "planar";
	else if(envMapType == Ogre::TextureUnitState::ENV_CURVED)
		name = "curved";
	else if(envMapType == Ogre::TextureUnitState::ENV_REFLECTION)
		name = "reflection";
	else if(envMapType == Ogre::TextureUnitState::ENV_NORMAL)
		name = "normal";
	return name;

}

Ogre::String getTransformTypeName(Ogre::TextureUnitState::TextureTransformType transformType)
{
	Ogre::String name;

	if(transformType == Ogre::TextureUnitState::TT_TRANSLATE_U)
		name = "scroll_x";
	else if(transformType == Ogre::TextureUnitState::TT_TRANSLATE_V)
		name = "scroll_y";
	else if(transformType == Ogre::TextureUnitState::TT_ROTATE)
		name = "rotate";
	else if(transformType == Ogre::TextureUnitState::TT_SCALE_U)
		name = "scale_x";
	else if(transformType == Ogre::TextureUnitState::TT_SCALE_V)
		name = "scale_y";
	else
		name = "none";

	return name;

}
Ogre::String getWaveformTypeName(Ogre::WaveformType waveType)
{
	Ogre::String name;
	if (waveType = Ogre::WFT_SINE)
		name = "sine";
	else if (waveType = Ogre::WFT_TRIANGLE)
		name = "triangle";
	else if (waveType = Ogre::WFT_SQUARE)
		name = "square";
	else if (waveType = Ogre::WFT_SAWTOOTH)
		name = "sawtooth";
	else if (waveType = Ogre::WFT_INVERSE_SAWTOOTH)
		name = "inverse_sawtooth";
	else
		name = "none";
	return name;
}


Ogre::LayerBlendOperationEx convertBlendOpEx(const Ogre::String& param)
{
	Ogre::LayerBlendOperationEx blendOperationEx;
	if (param == "source1")
		blendOperationEx = Ogre::LBX_SOURCE1;
	else if (param == "source2")
		blendOperationEx = Ogre::LBX_SOURCE2;
	else if (param == "modulate")
		blendOperationEx = Ogre::LBX_MODULATE;
	else if (param == "modulate_x2")
		blendOperationEx = Ogre::LBX_MODULATE_X2;
	else if (param == "modulate_x4")
		blendOperationEx = Ogre::LBX_MODULATE_X4;
	else if (param == "add")
		blendOperationEx = Ogre::LBX_ADD;
	else if (param == "add_signed")
		blendOperationEx = Ogre::LBX_ADD_SIGNED;
	else if (param == "add_smooth")
		blendOperationEx = Ogre::LBX_ADD_SMOOTH;
	else if (param == "subtract")
		blendOperationEx = Ogre::LBX_SUBTRACT;
	else if (param == "blend_diffuse_colour")
		blendOperationEx = Ogre::LBX_BLEND_DIFFUSE_COLOUR;
	else if (param == "blend_diffuse_alpha")
		blendOperationEx = Ogre::LBX_BLEND_DIFFUSE_ALPHA;
	else if (param == "blend_texture_alpha")
		blendOperationEx = Ogre::LBX_BLEND_TEXTURE_ALPHA;
	else if (param == "blend_current_alpha")
		blendOperationEx = Ogre::LBX_BLEND_CURRENT_ALPHA;
	else if (param == "blend_manual")
		blendOperationEx = Ogre::LBX_BLEND_MANUAL;
	else if (param == "dotproduct")
		blendOperationEx = Ogre::LBX_DOTPRODUCT;
	return blendOperationEx;
}
Ogre::LayerBlendSource convertBlendSource(const Ogre::String& param)
{
	Ogre::LayerBlendSource blendSource;
	if (param == "src_current")
		blendSource = Ogre::LBS_CURRENT;
	else if (param == "src_texture")
		blendSource = Ogre::LBS_TEXTURE;
	else if (param == "src_diffuse")
		blendSource = Ogre::LBS_DIFFUSE;
	else if (param == "src_specular")
		blendSource = Ogre::LBS_SPECULAR;
	else if (param == "src_manual")
		blendSource = Ogre::LBS_MANUAL;
	return blendSource;

}