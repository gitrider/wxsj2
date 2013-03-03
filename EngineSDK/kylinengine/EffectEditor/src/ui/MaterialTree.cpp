

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/log.h"
#endif

#include "wx/colordlg.h"

#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"

#include "math.h"

#ifdef __WIN32__
// this is not supported by native control
#define NO_VARIABLE_HEIGHT
#endif

#include "commctrl.h"
#include "MaterialTree.h"
#include "MaterialEditor.h"
#include "MaterialEidtorCommon.h"
#include "EffectObjectPropertyEditor.h"
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"
#include "OgreParticleAffector.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreStringConverter.h"
#include "OgreBlendMode.h"
static HTREEITEM GetItemFromPoint(HWND hwndTV, int x, int y)
{
    TV_HITTESTINFO tvht;
    tvht.pt.x = x;
    tvht.pt.y = y;

    return (HTREEITEM)TreeView_HitTest(hwndTV, &tvht);
}
// Menu for toolbar of material editor
const static int feID_MENU_MATERIAL_DELEMENT_ELEMENT	= wxNewId();
const static int feID_MENU_MATERIAL_NEW_TECHNIQUE		= wxNewId();
const static int feID_MENU_MATERIAL_NEW_PASS			= wxNewId();
const static int feID_MENU_MATERIAL_NEW_TEXTUREUNIT	= wxNewId();

IMPLEMENT_DYNAMIC_CLASS(MaterialTree, wxTreeCtrl)

BEGIN_EVENT_TABLE(MaterialTree, wxTreeCtrl)
EVT_TREE_SEL_CHANGED(ID_MATERIAL_OBJECT_TREE,OnSelChanged)

EVT_TREE_ITEM_MENU(ID_MATERIAL_OBJECT_TREE, MaterialTree::OnItemMenu)

EVT_MENU(feID_MENU_MATERIAL_DELEMENT_ELEMENT, MaterialTree::OnMenu)
EVT_MENU(feID_MENU_MATERIAL_NEW_TECHNIQUE, MaterialTree::OnMenu)
EVT_MENU(feID_MENU_MATERIAL_NEW_PASS, MaterialTree::OnMenu)
EVT_MENU(feID_MENU_MATERIAL_NEW_TEXTUREUNIT, MaterialTree::OnMenu)

END_EVENT_TABLE()

int MaterialTree::OnCompareItems(const wxTreeItemId& item1,
							  const wxTreeItemId& item2)
{
	if ( m_reverseSort )
	{
		// just exchange 1st and 2nd items
		return wxTreeCtrl::OnCompareItems(item2, item1);
	}
	else
	{
		return wxTreeCtrl::OnCompareItems(item1, item2);
	}
}

void MaterialTree::GetItemsRecursively(const wxTreeItemId& idParent,
									 wxTreeItemIdValue cookie)
{
	wxTreeItemId id;

	if ( !cookie )
		id = GetFirstChild(idParent, cookie);
	else
		id = GetNextChild(idParent, cookie);

	if ( !id.IsOk() )
		return;

	wxString text = GetItemText(id);
	wxLogMessage(text);

	if (ItemHasChildren(id))
		GetItemsRecursively(id);

	GetItemsRecursively(idParent, cookie);
}

void MaterialTree::DoToggleIcon(const wxTreeItemId& item)
{

}

void MaterialTree::LogEvent(const wxChar *name, const wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	wxString text;
	if ( item.IsOk() )
		text << _T('"') << GetItemText(item).c_str() << _T('"');
	else
		text = _T("invalid item");
	wxLogMessage(wxT("%s(%s)"), name, text.c_str());
}

long MaterialTree::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if(GetHwnd())
    { 
       
        switch ( nMsg )
        {
        case WM_LBUTTONDOWN: 
            int x = GET_X_LPARAM(lParam),
                y = GET_Y_LPARAM(lParam);
            HTREEITEM htItem = GetItemFromPoint(GetHwnd(), x, y);

            if ( htItem)
            {
                if ( wParam & MK_CONTROL )
                { 
                    SetFocus();

                    // toggle selected state
                    wxTreeCtrl::ToggleItemSelection(htItem);

                    tagNMTREEVIEWW ntv;

                    NMHDR hdr;
                    hdr.hwndFrom = GetHwnd();
                    hdr.idFrom = GetId();
                    hdr.code = TVN_SELCHANGEDW;

                    TVITEMW itemNew;
                    if(IsSelected(htItem))
                        itemNew.hItem = HTREEITEM(htItem);
                    else
                        itemNew.hItem = NULL;

                    ntv.hdr = hdr;
                    ntv.itemNew = itemNew;

                    WXLPARAM rc;
                    wxTreeCtrl::MSWOnNotify(GetId(), (LPARAM)&ntv, &rc); 

                    ::SetFocus(GetHwnd());                  

                    return 0L;
                }
            }
        }



    }
    return wxTreeCtrl::MSWWindowProc(nMsg,wParam,lParam);

}
void MaterialTree::AddMaterialToTree(Ogre::MaterialPtr& pMaterial,Ogre::String name)
{
	int image = MaterialTree::TreeCtrlIcon_Material;
	wxTreeItemId rootId = AddRoot(name,
		image, image,
		new MaterialTreeItemData(wxT("material root")));

	int iPorpertyImage = MaterialTree::TreeCtrlIcon_MaterialProperty;

	int iTechniqueImage = MaterialTree::TreeCtrlIcon_Technique;
	int iPassImage = MaterialTree::TreeCtrlIcon_Pass;
	int iTextureUnitImage = MaterialTree::TreeCtrlIcon_TextureUnitState;

	wxString material_nameName = "name:";
	material_nameName.Append(pMaterial->getName().c_str());
	wxTreeItemId materialNameId = AppendItem(rootId,material_nameName,
		iPorpertyImage, iPorpertyImage,
		new MaterialTreeItemData(wxT("material item"),wxT("name"),pMaterial.get()));

	wxString receive_shadowsName = "receive_shadows:";
	bool receive = pMaterial->getReceiveShadows();
	receive_shadowsName.Append(receive ? wxT("on"):wxT("off"));
	wxTreeItemId receive_shadowsId = AppendItem(rootId,receive_shadowsName,
		iPorpertyImage, iPorpertyImage,
		new MaterialTreeItemData(wxT("material item"),wxT("receive_shadows"),pMaterial.get()));

	wxString transparency_casts_shadowsName = "transparency_casts_shadows:";
	bool casts = pMaterial->getTransparencyCastsShadows();
	transparency_casts_shadowsName.Append(casts ? wxT("on"):wxT("off"));
	wxTreeItemId transparency_casts_shadowsId = AppendItem(rootId,transparency_casts_shadowsName,
		iPorpertyImage, iPorpertyImage,
		new MaterialTreeItemData(wxT("material item"),wxT("transparency_casts_shadows"),pMaterial.get()));

	Ogre::Material::TechniqueIterator dti = pMaterial->getTechniqueIterator();
	while (dti.hasMoreElements())
	{
		Ogre::Technique* dt = dti.getNext();
		wxString techniqueName = "technique:";
		techniqueName.append(dt->getName().c_str());
		wxTreeItemId TechniqueId = AppendItem(rootId,techniqueName,
			iTechniqueImage, iTechniqueImage,
			new MaterialTreeItemData(wxT("technique"),wxT("technique"),dt));

		wxString technique_nameName = "name:";
		technique_nameName.Append(dt->getName());
		wxTreeItemId nameId = AppendItem(TechniqueId,technique_nameName,
			iPorpertyImage, iPorpertyImage,
			new MaterialTreeItemData(wxT("technique item"),wxT("name"),dt));

		wxString lod_indexName = "lod_index:";
		lod_indexName.Append(Ogre::StringConverter::toString(dt->getLodIndex()).c_str());
		wxTreeItemId lod_indexId = AppendItem(TechniqueId,lod_indexName,
			iPorpertyImage, iPorpertyImage,
			new MaterialTreeItemData(wxT("technique item"),wxT("lod_index"),dt));

		wxString schemeName = "scheme:";
		schemeName.Append(dt->getSchemeName().c_str());
		wxTreeItemId schemeId = AppendItem(TechniqueId,schemeName,
			iPorpertyImage, iPorpertyImage,
			new MaterialTreeItemData(wxT("technique item"),wxT("scheme"),dt));


		Ogre::Technique::PassIterator dpi = dt->getPassIterator();
		while (dpi.hasMoreElements())
		{
			Ogre::Pass* dp = dpi.getNext();
			wxString passName = "pass:";
			passName.append(dp->getName());
			wxTreeItemId PassId = AppendItem(TechniqueId,passName,
				iPassImage, iPassImage,
				new MaterialTreeItemData(wxT("pass"),wxT("pass"),dp));

			wxString pass_nameName = "name:";
			pass_nameName.Append(dp->getName().c_str());
			wxTreeItemId nameId = AppendItem(PassId,pass_nameName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("name"),dp));

			
			wxString ambientName = "ambient:";
			ambientName.Append(Ogre::StringConverter::toString(dp->getAmbient()).c_str());
			wxTreeItemId AmbientDiffuseId = AppendItem(PassId,ambientName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("ambient"),dp));

			wxString diffuseName = "diffuse:";
			diffuseName.Append(Ogre::StringConverter::toString(dp->getDiffuse()).c_str());
			wxTreeItemId PassDiffuseId = AppendItem(PassId,diffuseName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("diffuse"),dp));

			wxString specularName = "specular:";
			specularName.Append(Ogre::StringConverter::toString(dp->getSpecular()).c_str());
			wxTreeItemId PassSpecularId = AppendItem(PassId,specularName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("specular"),dp));

			wxString emissiveName = "emissive:";
			emissiveName.Append(Ogre::StringConverter::toString(dp->getSelfIllumination()).c_str());
			wxTreeItemId PassEmissiveId = AppendItem(PassId,emissiveName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("emissive"),dp));


			//混合模式
			Ogre::SceneBlendFactor SourceBlendFactor = dp->getSourceBlendFactor();
			Ogre::SceneBlendFactor DestBlendFactor = dp->getDestBlendFactor();


			//场景混合
			wxString SceneBlendName = "scene_blend:";
			wxString blendName = getSceneBlendName(SourceBlendFactor,DestBlendFactor);
			SceneBlendName.Append(blendName);

			wxTreeItemId PassSceneId = AppendItem(PassId,SceneBlendName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("scene_blend"),dp));


			//深度检测
			wxString depthCheckName = wxT("depth_check:");
			bool check = dp->getDepthCheckEnabled();
			depthCheckName.Append(check ? wxT("true"):wxT("false"));
			wxTreeItemId depthCheckId = AppendItem(PassId,depthCheckName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("depth_check"),dp));

			//深度写入
			wxString depthWriteName = wxT("depth_write:");
			bool write = dp->getDepthWriteEnabled();
			depthWriteName.Append(write ? wxT("true"):wxT("false"));
			wxTreeItemId depthWriteId = AppendItem(PassId,depthWriteName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("depth_write"),dp));

			//深度检测函数
			wxString depthFuncName = wxT("depth_func:");
			Ogre::CompareFunction function = dp->getDepthFunction();

			depthFuncName.Append(getCompareFunctionName(function).c_str());

			wxTreeItemId depthFuncId = AppendItem(PassId,depthFuncName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("depth_func"),dp));

			//alpha rejection
			/*wxString alphaRejectionName = wxT("alpha_rejection:");
			Ogre::CompareFunction alphaRejectfunction = dp->getAlphaRejectFunction();

			if(alphaRejectfunction == Ogre::CMPF_ALWAYS_FAIL)
				alphaRejectionName.Append(wxT("always_fail"));
			else if(alphaRejectfunction == Ogre::CMPF_ALWAYS_PASS)
				alphaRejectionName.Append(wxT("always_pass"));
			else if(alphaRejectfunction == Ogre::CMPF_LESS)
				alphaRejectionName.Append(wxT("less"));
			else if(alphaRejectfunction == Ogre::CMPF_LESS_EQUAL)
				alphaRejectionName.Append(wxT("less_equal"));
			else if(alphaRejectfunction == Ogre::CMPF_EQUAL)
				alphaRejectionName.Append(wxT("equal"));
			else if(alphaRejectfunction == Ogre::CMPF_NOT_EQUAL)
				alphaRejectionName.Append(wxT("not_equal"));
			else if(alphaRejectfunction == Ogre::CMPF_GREATER_EQUAL)
				alphaRejectionName.Append(wxT("greater_equal"));
			else if(alphaRejectfunction == Ogre::CMPF_GREATER)
				alphaRejectionName.Append(wxT("greater"));

			alphaRejectionName.Append(" ");
			alphaRejectionName.Append(Ogre::StringConverter::toString(dp->getAlphaRejectValue()));

			wxTreeItemId alphaRejectionId = AppendItem(PassId,alphaRejectionName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("alpha_rejection"),dp));*/

			//颜色写入
			wxString colorWriteName = wxT("colour_write:");
			bool color_write = dp->getColourWriteEnabled();
			colorWriteName.Append(color_write ? wxT("true"):wxT("false"));
			wxTreeItemId colorWriteId = AppendItem(PassId,colorWriteName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("colour_write"),dp));


			//硬件裁减
			wxString cullingModeName = wxT("cull_hardware:");
			Ogre::CullingMode cullingMode =  dp->getCullingMode();
			cullingModeName.Append(getCullingModeName(cullingMode).c_str());

			wxTreeItemId cullingModeId = AppendItem(PassId,cullingModeName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("cull_hardware"),dp));


			//软件裁减
			wxString MaualCullingModeName = wxT("cull_software:");
			Ogre::ManualCullingMode maualCullingMode =  dp->getManualCullingMode();
			MaualCullingModeName.Append(getManualCullingModeName(maualCullingMode).c_str());

			wxTreeItemId maualCullingModeId = AppendItem(PassId,MaualCullingModeName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("cull_software"),dp));

			//光照
			wxString lightName = wxT("lighting:");
			bool light = dp->getLightingEnabled();
			lightName.Append(light ? wxT("true"):wxT("false"));
			wxTreeItemId lightId = AppendItem(PassId,lightName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("lighting"),dp));

			//着色模式
			wxString shadingName = wxT("shading:");
			Ogre::ShadeOptions shaderOpt = dp->getShadingMode();
			shadingName.Append(getShadingModeName(shaderOpt).c_str());

			wxTreeItemId shaderId = AppendItem(PassId,shadingName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("shading"),dp));

			//模型渲染模式
			wxString polygonModeName = wxT("polygon_mode:");
			Ogre::PolygonMode polygonMode = dp->getPolygonMode();
			polygonModeName.Append(getPolygonModeName(polygonMode).c_str());
			wxTreeItemId polygonModeId = AppendItem(PassId,polygonModeName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("polygon_mode"),dp));


			//深度偏移
			wxString depthBiasName = wxT("depth_bias:");
			Ogre::Vector2 vDepthBias = Ogre::Vector2::ZERO;
			vDepthBias.x = dp->getDepthBiasConstant();
			vDepthBias.y = dp->getDepthBiasSlopeScale();
			depthBiasName.Append(Ogre::StringConverter::toString(vDepthBias));
			wxTreeItemId depthBiasId = AppendItem(PassId,depthBiasName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("depth_bias"),dp));

			//雾效覆盖
			wxString fogOverrideName = wxT("fog_override:");
			bool fogOverride = dp->getFogOverride();
			fogOverrideName.Append(fogOverride ? wxT("true"):wxT("false"));
			fogOverrideName.Append(wxT(" "));
			Ogre::FogMode fogMode = dp->getFogMode();
			fogOverrideName.Append(getFogModeName(fogMode).c_str());
			Ogre::ColourValue fogColor = dp->getFogColour();
			fogOverrideName.Append(wxT(" "));
			fogOverrideName.Append(Ogre::StringConverter::toString(fogColor).c_str());
			fogOverrideName.Append(wxT(" "));
			fogOverrideName.Append(Ogre::StringConverter::toString(dp->getFogDensity()).c_str());
			fogOverrideName.Append(wxT(" "));
			fogOverrideName.Append(Ogre::StringConverter::toString(dp->getFogStart()).c_str());
			fogOverrideName.Append(wxT(" "));
			fogOverrideName.Append(Ogre::StringConverter::toString(dp->getFogEnd()).c_str());

			wxTreeItemId fogOverrideId = AppendItem(PassId,fogOverrideName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("fog_override"),dp));


			////////////////////////////////////////////////////////////////////////
			//贴图单元
			Ogre::Pass::TextureUnitStateIterator tusi = dp->getTextureUnitStateIterator();
			while (tusi.hasMoreElements())
			{
				Ogre::TextureUnitState* tus = tusi.getNext();

				wxString textureUnitName = "texture_unit:";
				textureUnitName.append(tus->getName().c_str());
				wxTreeItemId TextureUnitId = AppendItem(PassId,textureUnitName,
					iTextureUnitImage, iTextureUnitImage,
					new MaterialTreeItemData(wxT("texture_unit"),wxT("texture_unit"),tus));
				//名称
				wxString textureUnit_nameName = wxT("name:");
				textureUnit_nameName.Append(tus->getName().c_str());
				wxTreeItemId textureId = AppendItem(TextureUnitId,textureUnit_nameName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("name"),tus));

				//贴图
				wxString textureName = wxT("texture:");
				textureName.Append(tus->getTextureName().c_str());
				wxTreeItemId textureUnit_nameId = AppendItem(TextureUnitId,textureName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("texture"),tus));
				//别名
				wxString textureAliasName = wxT("texture_alias:");
				textureAliasName.Append(tus->getTextureNameAlias().c_str());
				wxTreeItemId textureAliasId = AppendItem(TextureUnitId,textureAliasName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("texture_alias"),tus));

				//贴图
				wxString animTextureName = wxT("anim_texture:");
				int iFramsNum = tus->getNumFrames();
				if(iFramsNum > 1)
				{
					Ogre::String name = tus->getFrameTextureName(0);
					animTextureName.Append(name);
					animTextureName.Append(wxT(" "));
					animTextureName.Append(Ogre::StringConverter::toString(iFramsNum).c_str());
					animTextureName.Append(wxT(" "));
					animTextureName.Append(Ogre::StringConverter::toString(tus->getAnimationDuration()).c_str());

				}
				else
				{
					animTextureName.Append(wxT("off"));
				}
				wxTreeItemId animTextureId = AppendItem(TextureUnitId,animTextureName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("texture_unit item"),wxT("anim_texture "),tus));

				//贴图通道
				wxString coordSetName = wxT("tex_coord_set:");
				coordSetName.Append(Ogre::StringConverter::toString(tus->getTextureCoordSet()).c_str());
				wxTreeItemId coordSetId = AppendItem(TextureUnitId,coordSetName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("tex_coord_set"),tus));

				//贴图边缘颜色
				wxString borderColorName = wxT("tex_border_colour :");
				borderColorName.Append(Ogre::StringConverter::toString(tus->getTextureBorderColour()).c_str());
				wxTreeItemId borderColorId = AppendItem(TextureUnitId,borderColorName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("tex_border_colour"),tus));

				//贴图地址模式
				wxString addressMode_uName = wxT("tex_address_mode_u:");
				wxString addressMode_vName = wxT("tex_address_mode_v:");
				wxString addressMode_wName = wxT("tex_address_mode_w:");

				Ogre::TextureUnitState::UVWAddressingMode addressingMode = 
					tus->getTextureAddressingMode();
				addressMode_uName.Append(getAddressingName(addressingMode.u).c_str());
				addressMode_vName.Append(getAddressingName(addressingMode.v).c_str());
				addressMode_wName.Append(getAddressingName(addressingMode.w).c_str());


				wxTreeItemId addressMode_uId = AppendItem(TextureUnitId,addressMode_uName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("tex_address_mode_u"),tus));

				wxTreeItemId addressMode_vId = AppendItem(TextureUnitId,addressMode_vName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("tex_address_mode_v"),tus));

				wxTreeItemId addressMode_wId = AppendItem(TextureUnitId,addressMode_wName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("tex_address_mode_w"),tus));

				//贴图过滤
				wxString filtering_minName = wxT("filtering_min:");
				Ogre::FilterOptions minFiltering = tus->getTextureFiltering(Ogre::FT_MIN);
				filtering_minName.Append(getFilteringName(minFiltering).c_str());
				wxTreeItemId filtering_minId = AppendItem(TextureUnitId,filtering_minName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("filtering_min"),tus));

				wxString filtering_magName = wxT("filtering_mag:");
				Ogre::FilterOptions magFiltering = tus->getTextureFiltering(Ogre::FT_MAG);
				filtering_magName.Append(getFilteringName(magFiltering).c_str());
				wxTreeItemId filtering_magId = AppendItem(TextureUnitId,filtering_magName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("filtering_mag"),tus));

				wxString filtering_mipName = wxT("filtering_mip:");
				Ogre::FilterOptions mipFiltering = tus->getTextureFiltering(Ogre::FT_MIP);
				filtering_mipName.Append(getFilteringName(mipFiltering).c_str());
				wxTreeItemId filtering_mipId = AppendItem(TextureUnitId,filtering_mipName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("filtering_mip"),tus));

				wxString max_anisotropyName = wxT("max_anisotropy:");
				max_anisotropyName.Append(Ogre::StringConverter::toString(tus->getTextureAnisotropy()).c_str());
				wxTreeItemId anisotropyId = AppendItem(TextureUnitId,max_anisotropyName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("max_anisotropy"),tus));

				wxString mipmapsName = wxT("mipmaps:");
				mipmapsName.Append(Ogre::StringConverter::toString(tus->getNumMipmaps()).c_str());
				wxTreeItemId mipmapsId = AppendItem(TextureUnitId,mipmapsName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("mipmaps"),tus));

				//颜色操作模式
				Ogre::LayerBlendModeEx colourBlendMode = tus->getColourBlendMode();

				Ogre::LayerBlendOperationEx operation = colourBlendMode.operation;
				Ogre::LayerBlendSource source1 = colourBlendMode.source1;
				Ogre::LayerBlendSource source2 = colourBlendMode.source2;
				Ogre::ColourValue colourArg1 = colourBlendMode.colourArg1;
				Ogre::ColourValue colourArg2 = colourBlendMode.colourArg2;
				Ogre::Real factor = colourBlendMode.factor;

				Ogre::SceneBlendFactor colourBlendFallbackSrc = tus->getColourBlendFallbackSrc();
				Ogre::SceneBlendFactor colourBlendFallbackDest = tus->getColourBlendFallbackDest();

				wxString colour_opName = wxT("colour_op:");

				if(operation == Ogre::LBX_SOURCE1 && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
					&& colourBlendFallbackSrc == Ogre::SBF_ONE && colourBlendFallbackDest == Ogre::SBF_ZERO)
					colour_opName.Append(wxT("replace"));
				else if(operation == Ogre::LBX_ADD && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
					&& colourBlendFallbackSrc == Ogre::SBF_ONE && colourBlendFallbackDest == Ogre::SBF_ONE)
					colour_opName.Append(wxT("add"));
				else if(operation == Ogre::LBX_MODULATE && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
					&& colourBlendFallbackSrc == Ogre::SBF_DEST_COLOUR && colourBlendFallbackDest == Ogre::SBF_ZERO)
					colour_opName.Append(wxT("modulate"));
				else if(operation == Ogre::LBX_BLEND_TEXTURE_ALPHA && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
					&& colourBlendFallbackSrc == Ogre::SBF_SOURCE_ALPHA && colourBlendFallbackDest == Ogre::SBF_ONE_MINUS_SOURCE_ALPHA)
					colour_opName.Append(wxT("alpha_blend"));
				else
					colour_opName.Append(wxT("none"));

				wxTreeItemId colour_opId = AppendItem(TextureUnitId,colour_opName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("colour_op"),tus));

				/*
				wxString colour_op_exName = wxT("colour_op_ex:");
				colour_op_exName.Append(getLayerBlendOperationName(operation).c_str());
				colour_op_exName.Append(wxT(" "));
				colour_op_exName.Append(getLayerBlendName(source1).c_str());
				colour_op_exName.Append(wxT(" "));
				colour_op_exName.Append(getLayerBlendName(source2).c_str());
				colour_op_exName.Append(wxT("  "));
				colour_op_exName.Append(Ogre::StringConverter::toString(colourArg1).c_str());
				colour_op_exName.Append(wxT("  "));
				colour_op_exName.Append(Ogre::StringConverter::toString(colourArg2).c_str());
				colour_op_exName.Append(wxT(" "));
				colour_op_exName.Append(Ogre::StringConverter::toString(factor).c_str());

				wxTreeItemId colour_op_exId = AppendItem(TextureUnitId,colour_op_exName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("colour_op_ex"),tus));

				wxString fallbackName = wxT("colour_op_multipass_fallback :");
				fallbackName.Append(getSceneBlendFactorName(colourBlendFallbackSrc).c_str());
				fallbackName.Append(wxT(" "));
				fallbackName.Append(getSceneBlendFactorName(colourBlendFallbackDest).c_str());

				wxTreeItemId fallbackId = AppendItem(TextureUnitId,fallbackName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("colour_op_multipass_fallback "),tus));


				//alpha操作
				Ogre::LayerBlendModeEx alphaBlendMode = tus->getAlphaBlendMode();

				Ogre::LayerBlendOperationEx alphaOperation = alphaBlendMode.operation;
				Ogre::LayerBlendSource alphaSource1 = alphaBlendMode.source1;
				Ogre::LayerBlendSource alphaSource2 = alphaBlendMode.source2;
				Ogre::Real alphaArg1 = alphaBlendMode.alphaArg1;
				Ogre::Real alphaArg2 = alphaBlendMode.alphaArg2;
				Ogre::Real alphafactor = alphaBlendMode.factor;

				wxString alpha_op_exName = wxT("alpha_op_ex:");
				alpha_op_exName.Append(getLayerBlendOperationName(alphaOperation).c_str());
				alpha_op_exName.Append(wxT(" "));
				alpha_op_exName.Append(getLayerBlendName(alphaSource1).c_str());
				alpha_op_exName.Append(wxT(" "));
				alpha_op_exName.Append(getLayerBlendName(alphaSource2).c_str());
				alpha_op_exName.Append(wxT(" "));
				alpha_op_exName.Append(Ogre::StringConverter::toString(alphaArg1<0.01 || alphaArg1> -0.01 ? 0.0f : alphaArg1).c_str());
				alpha_op_exName.Append(wxT(" "));
				alpha_op_exName.Append(Ogre::StringConverter::toString(alphaArg2<0.01 || alphaArg1> -0.01 ? 0.0f : alphaArg2).c_str());
				alpha_op_exName.Append(wxT(" "));
				alpha_op_exName.Append(Ogre::StringConverter::toString(alphafactor<0.01 || alphaArg1> -0.01 ? 0.0f : alphafactor).c_str());

				wxTreeItemId alpha_op_exId = AppendItem(TextureUnitId,alpha_op_exName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("alpha_op_ex"),tus));*/


				//坐标偏移
				wxString scroll_uName = wxT("scroll_u:");
				scroll_uName.Append(Ogre::StringConverter::toString(tus->getTextureUScroll()).c_str());
				wxTreeItemId scroll_uId = AppendItem(TextureUnitId,scroll_uName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("scroll_u"),tus));

				wxString scroll_vName = wxT("scroll_v:");
				scroll_vName.Append(Ogre::StringConverter::toString(tus->getTextureVScroll()).c_str());
				wxTreeItemId scroll_vId = AppendItem(TextureUnitId,scroll_vName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("scroll_v"),tus));

				//旋转
				wxString rotateName = wxT("rotate:");
				rotateName.Append(Ogre::StringConverter::toString(tus->getTextureRotate().valueDegrees()).c_str());
				wxTreeItemId rotateId = AppendItem(TextureUnitId,rotateName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("rotate"),tus));


				wxString scale_uName = wxT("scale_u:");
				scale_uName.Append(Ogre::StringConverter::toString(tus->getTextureUScale()).c_str());
				wxTreeItemId scale_uId = AppendItem(TextureUnitId,scale_uName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("scale_u"),tus));

				wxString scale_vName = wxT("scale_v:");
				scale_vName.Append(Ogre::StringConverter::toString(tus->getTextureVScale()).c_str());
				wxTreeItemId scale_vId = AppendItem(TextureUnitId,scale_vName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("scale_v"),tus));

				//贴图
				Ogre::TextureUnitState::EffectMap effectMap = tus->getEffects();
				Ogre::TextureUnitState::EffectMap::iterator it;

				//环境贴图
				it = effectMap.find(Ogre::TextureUnitState::ET_ENVIRONMENT_MAP);
				int envMapType = -1;
				if (it != effectMap.end())
				{
					envMapType = it->second.subtype;
				}

				wxString env_mapName = wxT("env_map:");
				if(envMapType == -1)
				{
					env_mapName.Append(wxT("off"));
				}
				else
				{
					if(envMapType == Ogre::TextureUnitState::ENV_PLANAR)
						env_mapName.Append(wxT("planar"));
					else if(envMapType == Ogre::TextureUnitState::ENV_CURVED)
						env_mapName.Append(wxT("curved"));
					else if(envMapType == Ogre::TextureUnitState::ENV_REFLECTION)
						env_mapName.Append(wxT("reflection"));
					else if(envMapType == Ogre::TextureUnitState::ENV_NORMAL)
						env_mapName.Append(wxT("normal"));
				}
				wxTreeItemId env_mapId = AppendItem(TextureUnitId,env_mapName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("env_map"),tus));

				//滚动动画
				float fUSpeed = 0.0f;
				float fVSpeed = 0.0f;
				it = effectMap.find(Ogre::TextureUnitState::ET_UVSCROLL);
				if (it != effectMap.end())
					fUSpeed = fVSpeed = it->second.arg1;

				it = effectMap.find(Ogre::TextureUnitState::ET_USCROLL);
				if (it != effectMap.end())
				{
					fUSpeed = it->second.arg1;
				}

				it = effectMap.find(Ogre::TextureUnitState::ET_VSCROLL);
				if (it != effectMap.end())
				{
					fVSpeed = it->second.arg1;
				}
				wxString scrollAnim_Name = wxT("scroll_anim:");
				Ogre::Vector3 vSpeed = Ogre::Vector3(fUSpeed,fVSpeed,0);
				scrollAnim_Name.Append(Ogre::StringConverter::toString(vSpeed).c_str());
				wxTreeItemId scrollAnim_uId = AppendItem(TextureUnitId,scrollAnim_Name,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("scroll_anim"),tus));


				//旋转动画
				float fRotateSpeed = 0.0f;
				it = effectMap.find(Ogre::TextureUnitState::ET_ROTATE);
				if (it != effectMap.end())
				{
					fRotateSpeed = it->second.arg1;
				}

				wxString rotate_animName = wxT("rotate_anim:");
				rotate_animName.Append(Ogre::StringConverter::toString(fRotateSpeed).c_str());
				wxTreeItemId rotate_animId = AppendItem(TextureUnitId,rotate_animName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("rotate_anim"),tus));

				//wave_xform动画
				wxString wave_xformName = wxT("wave_xform:");
				it = effectMap.find(Ogre::TextureUnitState::ET_TRANSFORM);
				if (it != effectMap.end())
				{
					int ttype = it->second.subtype;
					if(ttype == Ogre::TextureUnitState::TT_TRANSLATE_U)
						wave_xformName.Append(wxT("scroll_x"));
					else if(ttype == Ogre::TextureUnitState::TT_TRANSLATE_V)
						wave_xformName.Append(wxT("scroll_y"));
					else if(ttype == Ogre::TextureUnitState::TT_ROTATE)
						wave_xformName.Append(wxT("rotate"));
					else if(ttype == Ogre::TextureUnitState::TT_SCALE_U)
						wave_xformName.Append(wxT("scale_x"));
					else if(ttype == Ogre::TextureUnitState::TT_SCALE_V)
						wave_xformName.Append(wxT("scale_y"));
					else
						wave_xformName.Append(wxT("none"));

					wave_xformName.Append(wxT(" "));
					int waveType = it->second.waveType;

					if (waveType = Ogre::WFT_SINE)
						wave_xformName.Append(wxT("sine"));
					else if (waveType = Ogre::WFT_TRIANGLE)
						wave_xformName.Append(wxT("triangle"));
					else if (waveType = Ogre::WFT_SQUARE)
						wave_xformName.Append(wxT("square"));
					else if (waveType = Ogre::WFT_SAWTOOTH)
						wave_xformName.Append(wxT("sawtooth"));
					else if (waveType = Ogre::WFT_INVERSE_SAWTOOTH)
						wave_xformName.Append(wxT("inverse_sawtooth"));
					else
						wave_xformName.Append(wxT("none"));

					wave_xformName.Append(wxT(" "));

					wave_xformName.Append(Ogre::StringConverter::toString( it->second.base).c_str());
					wave_xformName.Append(wxT(" "));
					wave_xformName.Append(Ogre::StringConverter::toString( it->second.frequency).c_str());
					wave_xformName.Append(wxT(" "));
					wave_xformName.Append(Ogre::StringConverter::toString( it->second.phase).c_str());
					wave_xformName.Append(wxT(" "));
					wave_xformName.Append(Ogre::StringConverter::toString( it->second.amplitude).c_str());

				}
				else
				{
					wave_xformName.Append(wxT("off"));
				}
				wxTreeItemId wave_xformId = AppendItem(TextureUnitId,wave_xformName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("wave_xform"),tus));

				//贴图转换矩阵
				wxString transformName = wxT("transform:");
				transformName.Append(Ogre::StringConverter::toString(tus->getTextureTransform()).c_str());
				wxTreeItemId transformId = AppendItem(TextureUnitId,transformName,
					iPorpertyImage, iPorpertyImage,
					new MaterialTreeItemData(wxT("texture_unit item"),wxT("transform"),tus));


			}

			//最大灯光数
			wxString maxLightName = wxT("max_lights:");
			maxLightName.Append(Ogre::StringConverter::toString(dp->getMaxSimultaneousLights()).c_str());
			wxTreeItemId maxLightId = AppendItem(PassId,maxLightName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("max_lights"),dp));

			/*wxString iterationName = wxT("iteration:");
			iterationName.Append(Ogre::StringConverter::toString(dp->getPassIterationCount()).c_str());
			wxTreeItemId iterationId = AppendItem(PassId,iterationName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("iteration"),dp));

			//
			wxString onlyLightTypeName = wxT("only_light_type:");
			Ogre::Light::LightTypes  lightType = dp->getOnlyLightType();
			onlyLightTypeName.Append(getLightTypeName(lightType).c_str());
			wxTreeItemId conlyLightTypeId = AppendItem(PassId,onlyLightTypeName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("only_light_type"),dp));*/

			//点尺寸
			wxString pointSizeName = wxT("point_size:");
			pointSizeName.Append(Ogre::StringConverter::toString(dp->getPointSize()).c_str());
			wxTreeItemId pointSizeId = AppendItem(PassId,pointSizeName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("point_size"),dp));


			wxString pointSpritesName = wxT("point_sprites:");
			bool sprites = dp->getPointSpritesEnabled();
			pointSpritesName.Append(sprites ? wxT("true"):wxT("false"));
			wxTreeItemId pointSpritesId = AppendItem(PassId,pointSpritesName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("point_sprites"),dp));


			//点尺寸退化
			wxString pointSizeAttenName = wxT("point_size_attenuation:");
			pointSizeAttenName.Append(dp->isPointAttenuationEnabled() ? wxT("on") : wxT("off"));
			pointSizeAttenName.Append(wxT(" "));
			pointSizeAttenName.Append(Ogre::StringConverter::toString(dp->getPointAttenuationConstant()).c_str());
			pointSizeAttenName.Append(wxT(" "));
			pointSizeAttenName.Append(Ogre::StringConverter::toString(dp->getPointAttenuationLinear()).c_str());
			pointSizeAttenName.Append(wxT(" "));
			pointSizeAttenName.Append(Ogre::StringConverter::toString(dp->getPointAttenuationQuadratic()).c_str());
			wxTreeItemId pointSizeAttenId = AppendItem(PassId,pointSizeAttenName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("point_size_attenuation"),dp));


			//最小点尺寸
			wxString pointMinName = wxT("point_size_min:");
			pointMinName.Append(Ogre::StringConverter::toString(dp->getPointMinSize()).c_str());
			wxTreeItemId pointMinId = AppendItem(PassId,pointMinName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("point_size_min"),dp));

			//最大点尺寸
			wxString pointMaxName = wxT("point_size_max:");
			pointMaxName.Append(Ogre::StringConverter::toString(dp->getPointMaxSize()).c_str());
			wxTreeItemId pointMaxId = AppendItem(PassId,pointMaxName,
				iPorpertyImage, iPorpertyImage,
				new MaterialTreeItemData(wxT("pass item"),wxT("point_size_max"),dp));

			Expand(PassId);
		}
		Expand(TechniqueId);
	}
	Expand(rootId);
	mSelectTextureUnit = NULL;
	mSelectTechnique = NULL;
	mSelectPass = NULL;

}
void MaterialTree::SelecteTextureUnitItem(Ogre::TextureUnitState* texture)
{
	if(!texture)
		return;
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(itemId)); 
		if(pItemData->GetDesc() == "technique")
		{
			wxTreeItemId childId = GetFirstChild(itemId,idValue);
			while(childId.IsOk())
			{
				MaterialTreeItemData* pChildData = dynamic_cast<MaterialTreeItemData*>(GetItemData(childId)); 
				if(pChildData->GetDesc() == "pass")
				{
					wxTreeItemId subId = GetFirstChild(childId,idValue);
					while(subId.IsOk())
					{
						MaterialTreeItemData* pSubData = dynamic_cast<MaterialTreeItemData*>(GetItemData(subId)); 
						if(texture == (Ogre::TextureUnitState*)pSubData->GetData())
						{
							SelectItem(subId);
							Expand(subId);
							mSelectTextureUnit = texture;
							mSelectPass = NULL;
							mSelectTechnique = NULL;
							break;
						}
						subId = GetNextSibling(subId);

					}
				}
				childId = GetNextSibling(childId);

			}
			
		}
		itemId = GetNextSibling(itemId);
	}

}
void MaterialTree::SelectePassItem(Ogre::Pass* pass)
{
	if(!pass)
		return;
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(itemId)); 
		if(pItemData->GetDesc() == "technique")
		{
			wxTreeItemId childId = GetFirstChild(itemId,idValue);
			while(childId.IsOk())
			{
				MaterialTreeItemData* pChildData = dynamic_cast<MaterialTreeItemData*>(GetItemData(childId)); 
				if(pChildData->GetDesc() == "pass")
				{
					if(pass == (Ogre::Pass*)pChildData->GetData())
					{
						SelectItem(childId);
						Expand(childId);
						mSelectTextureUnit = NULL;
						mSelectPass = pass;
						mSelectTechnique = NULL;
						break;
					}
				}
				childId = GetNextSibling(childId);

			}

		}
		itemId = GetNextSibling(itemId);
	}
}
void MaterialTree::SelecteTechniqueItem(Ogre::Technique* technique)
{
	if(!technique)
		return;
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(itemId)); 
		if(pItemData->GetDesc() == "technique")
		{
			if(technique == (Ogre::Technique*)pItemData->GetData())
			{
				SelectItem(itemId);
				Expand(itemId);
				mSelectTextureUnit = NULL;
				mSelectPass = NULL;
				mSelectTechnique = technique;
				break;
			}
			

		}
		itemId = GetNextSibling(itemId);
	}

}
void MaterialTree::refrashSelected()
{
	wxTreeItemId selectedId = this->GetSelection();
	if(selectedId)
	{
		this->SelectItem(selectedId);
	}
}
void MaterialTree::refreshItem(const wxString& Desc,const wxString& propertyName,
							   const wxString& propertyValue,void* data)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId sub_subId;
	wxTreeItemId subId;
	wxTreeItemId childId;

	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(itemId)); 
		if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
		{
			this->SetItemText(itemId,propertyName + ":" + propertyValue);
			return;
		}
		childId = GetFirstChild(itemId,idValue);
		while(childId.IsOk())
		{

			MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(childId)); 
			if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
			{
				this->SetItemText(childId,propertyName + ":" + propertyValue);
				return;
			}
			subId = GetFirstChild(childId,idValue);
			while(subId.IsOk())
			{

				MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(subId)); 
				if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
				{
					this->SetItemText(subId,propertyName + ":" + propertyValue);
					return;
				}
				sub_subId = GetFirstChild(subId,idValue);
				while(sub_subId.IsOk())
				{
					MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(sub_subId)); 
					if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
					{
						this->SetItemText(sub_subId,propertyName + ":" + propertyValue);
						return;
					}
					sub_subId = GetNextSibling(sub_subId);
				}
				subId = GetNextSibling(subId);
			}
			childId = GetNextSibling(childId);
		}
		itemId = GetNextSibling(itemId);
	}
}
void MaterialTree::CreateImageList(int size)
{
	if ( size == -1 )
	{
		SetImageList(NULL);
		return;
	}
	if ( size == 0 )
		size = m_imageSize;
	else
		m_imageSize = size;

	// Make an image list containing small icons
	wxImageList *images = new wxImageList(size, size, true);

	// should correspond to TreeCtrlIcon_xxx enum
	wxBusyCursor wait;
	wxBitmap bitmaps[12];
	bitmaps[0] = wxBITMAP(MATERIAL);
	bitmaps[1] = wxBITMAP(TECHNIQUE);
	bitmaps[2] = wxBITMAP(PASS);
	bitmaps[3] = wxBITMAP(PARTICLEPROPERTY);
	bitmaps[4] = wxBITMAP(TEXTURE);
	bitmaps[5] = wxBITMAP(PARTICLEITEM);
	bitmaps[6] = wxBITMAP(MATERIAL);
	bitmaps[7] = wxBITMAP(PARTICLE);
	bitmaps[8] = wxBITMAP(PARTICLE);
	bitmaps[9] = wxBITMAP(PARTICLE);
	bitmaps[10] = wxBITMAP(PARTICLE);
	bitmaps[11] = wxBITMAP(PARTICLE);

	int sizeOrig = bitmaps[0].GetWidth();
	for ( size_t i = 0; i < WXSIZEOF(bitmaps); i++ )
	{
		if ( size == sizeOrig )
		{
			images->Add(bitmaps[i]);
		}
		else
		{
			images->Add(wxBitmap(bitmaps[i].ConvertToImage().Rescale(size, size)));
		}
	}

	AssignImageList(images);

}

#define TREE_EVENT_HANDLER(name)                                 \
	void MaterialTree::name(wxTreeEvent& event)                        \
{                                                                \
	LogEvent(_T(#name), event);                                  \
	SetLastItem(wxTreeItemId());                                 \
	event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnBeginRDrag)
TREE_EVENT_HANDLER(OnDeleteItem)
TREE_EVENT_HANDLER(OnGetInfo)
TREE_EVENT_HANDLER(OnSetInfo)
TREE_EVENT_HANDLER(OnItemExpanded)
TREE_EVENT_HANDLER(OnItemExpanding)
TREE_EVENT_HANDLER(OnItemCollapsed)
TREE_EVENT_HANDLER(OnSelChanging)


#undef TREE_EVENT_HANDLER

void LogKeyEvent(const wxChar *name, const wxKeyEvent& event)
{
	wxString key;
	long keycode = event.GetKeyCode();
	{
		switch ( keycode )
		{
		case WXK_BACK: key = wxT("BACK"); break;
		case WXK_TAB: key = wxT("TAB"); break;
		case WXK_RETURN: key = wxT("RETURN"); break;
		case WXK_ESCAPE: key = wxT("ESCAPE"); break;
		case WXK_SPACE: key = wxT("SPACE"); break;
		case WXK_DELETE: key = wxT("DELETE"); break;
		case WXK_START: key = wxT("START"); break;
		case WXK_LBUTTON: key = wxT("LBUTTON"); break;
		case WXK_RBUTTON: key = wxT("RBUTTON"); break;
		case WXK_CANCEL: key = wxT("CANCEL"); break;
		case WXK_MBUTTON: key = wxT("MBUTTON"); break;
		case WXK_CLEAR: key = wxT("CLEAR"); break;
		case WXK_SHIFT: key = wxT("SHIFT"); break;
		case WXK_ALT: key = wxT("ALT"); break;
		case WXK_CONTROL: key = wxT("CONTROL"); break;
		case WXK_MENU: key = wxT("MENU"); break;
		case WXK_PAUSE: key = wxT("PAUSE"); break;
		case WXK_CAPITAL: key = wxT("CAPITAL"); break;
		case WXK_END: key = wxT("END"); break;
		case WXK_HOME: key = wxT("HOME"); break;
		case WXK_LEFT: key = wxT("LEFT"); break;
		case WXK_UP: key = wxT("UP"); break;
		case WXK_RIGHT: key = wxT("RIGHT"); break;
		case WXK_DOWN: key = wxT("DOWN"); break;
		case WXK_SELECT: key = wxT("SELECT"); break;
		case WXK_PRINT: key = wxT("PRINT"); break;
		case WXK_EXECUTE: key = wxT("EXECUTE"); break;
		case WXK_SNAPSHOT: key = wxT("SNAPSHOT"); break;
		case WXK_INSERT: key = wxT("INSERT"); break;
		case WXK_HELP: key = wxT("HELP"); break;
		case WXK_NUMPAD0: key = wxT("NUMPAD0"); break;
		case WXK_NUMPAD1: key = wxT("NUMPAD1"); break;
		case WXK_NUMPAD2: key = wxT("NUMPAD2"); break;
		case WXK_NUMPAD3: key = wxT("NUMPAD3"); break;
		case WXK_NUMPAD4: key = wxT("NUMPAD4"); break;
		case WXK_NUMPAD5: key = wxT("NUMPAD5"); break;
		case WXK_NUMPAD6: key = wxT("NUMPAD6"); break;
		case WXK_NUMPAD7: key = wxT("NUMPAD7"); break;
		case WXK_NUMPAD8: key = wxT("NUMPAD8"); break;
		case WXK_NUMPAD9: key = wxT("NUMPAD9"); break;
		case WXK_MULTIPLY: key = wxT("MULTIPLY"); break;
		case WXK_ADD: key = wxT("ADD"); break;
		case WXK_SEPARATOR: key = wxT("SEPARATOR"); break;
		case WXK_SUBTRACT: key = wxT("SUBTRACT"); break;
		case WXK_DECIMAL: key = wxT("DECIMAL"); break;
		case WXK_DIVIDE: key = wxT("DIVIDE"); break;
		case WXK_F1: key = wxT("F1"); break;
		case WXK_F2: key = wxT("F2"); break;
		case WXK_F3: key = wxT("F3"); break;
		case WXK_F4: key = wxT("F4"); break;
		case WXK_F5: key = wxT("F5"); break;
		case WXK_F6: key = wxT("F6"); break;
		case WXK_F7: key = wxT("F7"); break;
		case WXK_F8: key = wxT("F8"); break;
		case WXK_F9: key = wxT("F9"); break;
		case WXK_F10: key = wxT("F10"); break;
		case WXK_F11: key = wxT("F11"); break;
		case WXK_F12: key = wxT("F12"); break;
		case WXK_F13: key = wxT("F13"); break;
		case WXK_F14: key = wxT("F14"); break;
		case WXK_F15: key = wxT("F15"); break;
		case WXK_F16: key = wxT("F16"); break;
		case WXK_F17: key = wxT("F17"); break;
		case WXK_F18: key = wxT("F18"); break;
		case WXK_F19: key = wxT("F19"); break;
		case WXK_F20: key = wxT("F20"); break;
		case WXK_F21: key = wxT("F21"); break;
		case WXK_F22: key = wxT("F22"); break;
		case WXK_F23: key = wxT("F23"); break;
		case WXK_F24: key = wxT("F24"); break;
		case WXK_NUMLOCK: key = wxT("NUMLOCK"); break;
		case WXK_SCROLL: key = wxT("SCROLL"); break;
		case WXK_PAGEUP: key = wxT("PAGEUP"); break;
		case WXK_PAGEDOWN: key = wxT("PAGEDOWN"); break;
		case WXK_NUMPAD_SPACE: key = wxT("NUMPAD_SPACE"); break;
		case WXK_NUMPAD_TAB: key = wxT("NUMPAD_TAB"); break;
		case WXK_NUMPAD_ENTER: key = wxT("NUMPAD_ENTER"); break;
		case WXK_NUMPAD_F1: key = wxT("NUMPAD_F1"); break;
		case WXK_NUMPAD_F2: key = wxT("NUMPAD_F2"); break;
		case WXK_NUMPAD_F3: key = wxT("NUMPAD_F3"); break;
		case WXK_NUMPAD_F4: key = wxT("NUMPAD_F4"); break;
		case WXK_NUMPAD_HOME: key = wxT("NUMPAD_HOME"); break;
		case WXK_NUMPAD_LEFT: key = wxT("NUMPAD_LEFT"); break;
		case WXK_NUMPAD_UP: key = wxT("NUMPAD_UP"); break;
		case WXK_NUMPAD_RIGHT: key = wxT("NUMPAD_RIGHT"); break;
		case WXK_NUMPAD_DOWN: key = wxT("NUMPAD_DOWN"); break;
		case WXK_NUMPAD_PAGEUP: key = wxT("NUMPAD_PAGEUP"); break;
		case WXK_NUMPAD_PAGEDOWN: key = wxT("NUMPAD_PAGEDOWN"); break;
		case WXK_NUMPAD_END: key = wxT("NUMPAD_END"); break;
		case WXK_NUMPAD_BEGIN: key = wxT("NUMPAD_BEGIN"); break;
		case WXK_NUMPAD_INSERT: key = wxT("NUMPAD_INSERT"); break;
		case WXK_NUMPAD_DELETE: key = wxT("NUMPAD_DELETE"); break;
		case WXK_NUMPAD_EQUAL: key = wxT("NUMPAD_EQUAL"); break;
		case WXK_NUMPAD_MULTIPLY: key = wxT("NUMPAD_MULTIPLY"); break;
		case WXK_NUMPAD_ADD: key = wxT("NUMPAD_ADD"); break;
		case WXK_NUMPAD_SEPARATOR: key = wxT("NUMPAD_SEPARATOR"); break;
		case WXK_NUMPAD_SUBTRACT: key = wxT("NUMPAD_SUBTRACT"); break;
		case WXK_NUMPAD_DECIMAL: key = wxT("NUMPAD_DECIMAL"); break;

		default:
			{
				if ( keycode < 128 && wxIsprint((int)keycode) )
					key.Printf(wxT("'%c'"), (char)keycode);
				else if ( keycode > 0 && keycode < 27 )
					key.Printf(_("Ctrl-%c"), wxT('A') + keycode - 1);
				else
					key.Printf(wxT("unknown (%ld)"), keycode);
			}
		}
	}

	wxLogMessage( wxT("%s event: %s (flags = %c%c%c%c)"),
		name,
		key.c_str(),
		event.ControlDown() ? wxT('C') : wxT('-'),
		event.AltDown() ? wxT('A') : wxT('-'),
		event.ShiftDown() ? wxT('S') : wxT('-'),
		event.MetaDown() ? wxT('M') : wxT('-'));
}

void MaterialTree::OnTreeKeyDown(wxTreeEvent& event)
{
	LogKeyEvent(wxT("Tree key down "), event.GetKeyEvent());

	event.Skip();
}

void MaterialTree::OnBeginDrag(wxTreeEvent& event)
{
	// need to explicitly allow drag
	if ( event.GetItem() != GetRootItem() )
	{
		m_draggedItem = event.GetItem();

		wxPoint clientpt = event.GetPoint();
		wxPoint screenpt = ClientToScreen(clientpt);

		wxLogMessage(wxT("OnBeginDrag: started dragging %s at screen coords (%i,%i)"),
			GetItemText(m_draggedItem).c_str(),
			screenpt.x, screenpt.y);

		event.Allow();
	}
	else
	{
		wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
	}
}

void MaterialTree::OnEndDrag(wxTreeEvent& event)
{
	wxTreeItemId itemSrc = m_draggedItem,
		itemDst = event.GetItem();
	m_draggedItem = (wxTreeItemId)0l;

	// where to copy the item?
	if ( itemDst.IsOk() && !ItemHasChildren(itemDst) )
	{
		// copy to the parent then
		itemDst = GetItemParent(itemDst);
	}

	if ( !itemDst.IsOk() )
	{
		wxLogMessage(wxT("OnEndDrag: can't drop here."));

		return;
	}

	wxString text = GetItemText(itemSrc);
	wxLogMessage(wxT("OnEndDrag: '%s' copied to '%s'."),
		text.c_str(), GetItemText(itemDst).c_str());

	// just do append here - we could also insert it just before/after the item
	// on which it was dropped, but this requires slightly more work... we also
	// completely ignore the client data and icon of the old item but could
	// copy them as well.
	//
	// Finally, we only copy one item here but we might copy the entire tree if
	// we were dragging a folder.
	//int image = wxGetApp().ShowImages() ? TreeCtrlIcon_File : -1;
	//AppendItem(itemDst, text, image);
}

void MaterialTree::OnBeginLabelEdit(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnBeginLabelEdit"));

	// for testing, prevent this item's label editing
	wxTreeItemId itemId = event.GetItem();
	if ( IsTestItem(itemId) )
	{
		wxMessageBox(wxT("You can't edit this item."));

		event.Veto();
	}
	else if ( itemId == GetRootItem() )
	{
		// test that it is possible to change the text of the item being edited
		SetItemText(itemId, _T("Editing root item"));
	}
}

void MaterialTree::OnEndLabelEdit(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnEndLabelEdit"));

	// don't allow anything except letters in the labels
	if ( !event.GetLabel().IsWord() )
	{
		wxMessageBox(wxT("The new label should be a single word."));

		event.Veto();
	}
}

void MaterialTree::OnItemCollapsing(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnItemCollapsing"));

	// for testing, prevent the user from collapsing the first child folder
	wxTreeItemId itemId = event.GetItem();
}

void MaterialTree::OnItemActivated(wxTreeEvent& event)
{
	// show some info about this item
	wxTreeItemId itemId = event.GetItem();
	MaterialTreeItemData *item = (MaterialTreeItemData *)GetItemData(itemId);

	if ( item != NULL )
	{
		item->ShowInfo(this);
	}

	wxLogMessage(wxT("OnItemActivated"));
}

void MaterialTree::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	
	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);

	ShowMenu(itemId, clientpt);
	event.Skip();
}

void MaterialTree::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint pt = event.GetPosition();

	wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);
}

void MaterialTree::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	MaterialTreeItemData *item = id.IsOk() ? (MaterialTreeItemData *)GetItemData(id): NULL;
	if (item == NULL)
	{
		return;
	}

	if (item->GetDesc() == "material root")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_MATERIAL_NEW_TECHNIQUE, wxT("添加技术"));	
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "technique")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_MATERIAL_NEW_PASS, wxT("添加通道"));	
		menu.Append(feID_MENU_MATERIAL_DELEMENT_ELEMENT, wxT("删除"));	
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "pass")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_MATERIAL_NEW_TEXTUREUNIT, wxT("添加纹理单元"));	
		menu.Append(feID_MENU_MATERIAL_DELEMENT_ELEMENT, wxT("删除"));	
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "texture_unit")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_MATERIAL_DELEMENT_ELEMENT, wxT("删除"));	
		PopupMenu(&menu, pt);
	}
}

void MaterialTree::OnMenu(wxCommandEvent & event)
{
	int id = event.GetId();
	MaterialEditor* parent = static_cast <MaterialEditor*>(GetParent());
	if(parent == NULL)
	{
		return;
	}

	if (id == feID_MENU_MATERIAL_DELEMENT_ELEMENT)
	{
		parent->OnNotifyToolBarCtrl(MaterialEditor::TC_DELETE_ElEMENT);
	}
	else if (id == feID_MENU_MATERIAL_NEW_TECHNIQUE)
	{
		parent->OnNotifyToolBarCtrl(MaterialEditor::TC_NEW_TECHNIQUE);
	}
	else if (id == feID_MENU_MATERIAL_NEW_PASS)
	{
		parent->OnNotifyToolBarCtrl(MaterialEditor::TC_NEW_PASS);
	}
	else if (id == feID_MENU_MATERIAL_NEW_TEXTUREUNIT)
	{
		parent->OnNotifyToolBarCtrl(MaterialEditor::TC_NEW_TEXTUREUNIT);
	}
}

void MaterialTree::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	MaterialTreeItemData *item = itemId.IsOk() ? (MaterialTreeItemData *)GetItemData(itemId)
		: NULL;

	wxLogMessage(wxT("Item \"%s\" right clicked"), item ? item->GetDesc()
		: _T(""));

	event.Skip();
}

void MaterialTree::OnRMouseDown(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button down"));

	event.Skip();
}

void MaterialTree::OnRMouseUp(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button up"));

	event.Skip();
}

void MaterialTree::OnRMouseDClick(wxMouseEvent& event)
{
	wxTreeItemId id = HitTest(event.GetPosition());
	if ( !id )
		wxLogMessage(wxT("No item under mouse"));
	else
	{
		MaterialTreeItemData *item = (MaterialTreeItemData *)GetItemData(id);
		if ( item )
			wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
	}

	event.Skip();
}

static inline const wxChar *Bool2String(bool b)
{
	return b ? wxT("") : wxT("not ");
}
void MaterialTreeItemData::ShowInfo(wxTreeCtrl *tree)
{
	wxLogMessage(wxT("Item '%s': %sselected, %sexpanded, %sbold,\n")
		wxT("%u children (%u immediately under this item)."),
		m_desc.c_str(),
		Bool2String(tree->IsSelected(GetId())),
		Bool2String(tree->IsExpanded(GetId())),
		Bool2String(tree->IsBold(GetId())),
		unsigned(tree->GetChildrenCount(GetId())),
		unsigned(tree->GetChildrenCount(GetId(), false)));
}

void MaterialTree::OnSelChanged(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	if ( !id )
	{
		wxLogMessage(wxT("No item under mouse"));
	}

	MaterialTreeItemData* pItemData = dynamic_cast<MaterialTreeItemData*>(GetItemData(id)); 
	if(pItemData)
	{

		MaterialEditor* pEditor = dynamic_cast<MaterialEditor*>(m_parent);
		EffectObjectPropertyEditor * pPropertyEditor = pEditor->GetParentFrame()->GetEffectObjectProperty();
		

		if(pItemData->GetDesc() == "material root" || 
		pItemData->GetDesc() == "material item")
		{
			pPropertyEditor->InitMaterialProperty();
		}
		else if(pItemData->GetDesc() == "technique" || 
			pItemData->GetDesc() == "technique item" )
		{
			Ogre::Technique* pTechnique = (Ogre::Technique*)pItemData->GetData();
			pPropertyEditor->InitMaterialTechnique(pTechnique);
			mSelectTechnique = pTechnique;
			mSelectPass = NULL;
			mSelectTextureUnit = NULL;
		}
		else if(pItemData->GetDesc() == "pass" ||
			pItemData->GetDesc() == "pass item")
		{
			Ogre::Pass* pPass = (Ogre::Pass*)pItemData->GetData();
			pPropertyEditor->InitMaterialPass(pPass);
			mSelectTechnique = NULL;
			mSelectPass = pPass;
			mSelectTextureUnit = NULL;
		}
		else if(pItemData->GetDesc() == "texture_unit" ||
			pItemData->GetDesc() == "texture_unit item")
		{
			Ogre::TextureUnitState* textureUnit = (Ogre::TextureUnitState*)pItemData->GetData();
			pPropertyEditor->InitMaterialTextureUnit(textureUnit);
			mSelectTextureUnit = textureUnit;
			mSelectPass = NULL;
			mSelectTechnique = NULL;
		}
	}
	event.Skip();
}