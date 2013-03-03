/********************************************************************
filename:   SceneInfomationDialog.cpp

purpose:    ������ʾ���򿪵ĳ����Ļ�����Ϣ������δ�С��
*********************************************************************/

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "SceneInfomationDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include <wx/busyinfo.h>

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

#include "SceneInfomationDialog.h"

#include "SceneManipulator.h"

#include "Core/TerrainData.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyStaticEntityObject.h"
#include "Core/FairyModelObject.h"
#include "Core/FairyTerrainLiquidObject.h"
#include "Core/FairyLightObject.h"
#include "Core/FairyParticleSystemObject.h"
#include "Core/FairyEffectObject.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/SceneInfomation_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreMaterial.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreTextureUnitState.h>
#include <OgreCamera.h>
#include <OgreAxisAlignedBox.h>

IMPLEMENT_DYNAMIC_CLASS(SceneInfomationDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SceneInfomationDialog, wxDialog)

    EVT_INIT_DIALOG(SceneInfomationDialog::OnInitDialog)
    EVT_TREE_ITEM_ACTIVATED(ID_TREECTRL_TEXTURE_DETAIL_LIST, SceneInfomationDialog::OnResourceInfoDoubleClick)
    EVT_LISTBOX_DCLICK(ID_LISTBOX_RESOURCE_OWNNER, SceneInfomationDialog::OnResourceOwnerDoubleClick)

    EVT_BUTTON(ID_BUTTON_SAVE, SceneInfomationDialog::OnSaveInfo)
    EVT_BUTTON(ID_BUTTON_REFRESH, SceneInfomationDialog::OnRefresh)

    EVT_TREE_KEY_DOWN(ID_TREECTRL_TEXTURE_DETAIL_LIST, SceneInfomationDialog::OnTreeKeyDown)

END_EVENT_TABLE()

class ResourceItemData : public wxTreeItemData
{
public:

    ResourceItemData(SceneInfomationDialog::ResourceInfo* info) 
        : resourceInfo(info)
    {

    }

    SceneInfomationDialog::ResourceInfo* resourceInfo;
};

SceneInfomationDialog::~SceneInfomationDialog()
{
    _clearAllResourceInfos();
}

void
SceneInfomationDialog::_setResourceList(void)
{    
    _clearAllResourceInfos();

    // �������б��е����ݼ��뵽listbox��
    mResourceInfoTree->DeleteAllItems();
    wxTreeItemId resourceParentId = mResourceInfoTree->AddRoot(wxString("Resource List"));

    Fairy::SceneInfo* sceneInfo = mManipulator->getSceneInfo();

    // �������е�mesh
    Fairy::Scene::ObjectsByTypeRange meshes =
        sceneInfo->findObjectsByType(Fairy::StaticEntityObject::msType);

    for (Fairy::Scene::ObjectsByTypeIterator it = meshes.first; it != meshes.second; ++it)
    {
        const Fairy::ObjectPtr& object = *it;
        const Fairy::StaticEntityObject* meshObject = static_cast<Fairy::StaticEntityObject *>(object.get());
        
        // ��mesh���������б�
        _addResourceCount(meshObject->getMeshName(), "", meshObject->mTransform.position, meshObject->getName(), mModelInfoList);

        // ������������б�
        _addUndergroundItem(object, resourceParentId);

        Ogre::Entity* entity = meshObject->getEntity();

        for (unsigned int subEntityIndex = 0; subEntityIndex < entity->getNumSubEntities(); ++subEntityIndex)
        {
            Ogre::SubEntity* subEntity = entity->getSubEntity(subEntityIndex);

            Ogre::MaterialPtr mat = subEntity->getMaterial();

            for (unsigned short techIndex = 0; techIndex < mat->getNumTechniques(); ++techIndex)
            {
                Ogre::Technique* tech = mat->getTechnique(techIndex);

                for (unsigned short passIndex = 0; passIndex < tech->getNumPasses(); ++passIndex)
                {
                    Ogre::Pass* pass = tech->getPass(passIndex);

                    for (unsigned short texIndex = 0; texIndex < pass->getNumTextureUnitStates(); ++texIndex)
                    {
                        Ogre::TextureUnitState* texture = pass->getTextureUnitState(texIndex);

                        for (unsigned int frameIndex = 0; frameIndex < texture->getNumFrames(); ++frameIndex)
                        {
                            const Ogre::String& textureName = texture->getFrameTextureName(frameIndex);

                            // �����entity���õ���ͼ�����������б�
                            _addResourceCount(textureName, meshObject->getMeshName(), meshObject->mTransform.position, 
                                meshObject->getName(), mTextureInfoList);
                        }
                    }
                }
            }
        }
    }

    // �������е�model
    Fairy::Scene::ObjectsByTypeRange models =
        sceneInfo->findObjectsByType(Fairy::ModelObject::msType);

    for (Fairy::Scene::ObjectsByTypeIterator it = models.first; it != models.second; ++it)
    {
        const Fairy::ObjectPtr& object = *it;
        const Fairy::ModelObject* modelObject = static_cast<Fairy::ModelObject *>(object.get());

        // ��model���������������б�owner����Ϊ��
        _addResourceCount(modelObject->getModelName(), "", modelObject->mTransform.position, modelObject->getName(), mModelInfoList);

        // ������������б�
        _addUndergroundItem(object, resourceParentId);
    }

    // �������б�����
    _sort(mTextureInfoList);
    _sort(mModelInfoList);

    // �������е�particle��effect
    Fairy::Scene::ObjectsByTypeRange particles =
        sceneInfo->findObjectsByType(Fairy::ParticleSystemObject::msType);

    for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
    {
        const Fairy::ObjectPtr& object = *it;
        const Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

        // ��particle���������������б�owner����Ϊ��
        _addResourceCount(particleObject->getParticleSystemName(), "",
            particleObject->mTransform.position, particleObject->getName(), mEffectList);

        // ������������б�
        _addUndergroundItem(object, resourceParentId);
    }

    Fairy::Scene::ObjectsByTypeRange effects =
        sceneInfo->findObjectsByType(Fairy::EffectObject::msType);

    for (Fairy::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
    {
        const Fairy::ObjectPtr& object = *it;
        const Fairy::EffectObject* effectObject = static_cast<Fairy::EffectObject *>(object.get());

        // ��model���������������б�owner����Ϊ��
        _addResourceCount(effectObject->getEffectName(), "",
            effectObject->mTransform.position, effectObject->getName(), mEffectList);
        // ������������б�
        _addUndergroundItem(object, resourceParentId);
    }

    Fairy::Scene::ObjectsByTypeRange lights =
        sceneInfo->findObjectsByType(Fairy::LightObject::msType);

    for (Fairy::Scene::ObjectsByTypeIterator it = lights.first; it != lights.second; ++it)
    {
        const Fairy::ObjectPtr& object = *it;
        const Fairy::EffectObject* lightObject = static_cast<Fairy::EffectObject *>(object.get());
        // ������������б�
        _addUndergroundItem(object, resourceParentId);
    }

    // ��effect�б�����
    _sort(mEffectList);

    // �������ε����и��ӵ�����
    Fairy::TerrainData* terrainData = mManipulator->getTerrainData();

    int xsize = terrainData->getXSize();
    int zsize = terrainData->getZSize();

    int minx, maxx, minz, maxz;

    minx = maxx = minz = maxz = -1;

    bool bFirstGrid = false;

    for ( int x=0; x<xsize; ++x )
    {
        for( int z=0; z<zsize; ++z )
        {
            const Fairy::TerrainData::GridInfo &gridInfo = terrainData->getGridInfo(x,z);

            Ogre::String layerTexName;    // ���浱ǰ�����ĸ������õ��������������
            int id = 0;				// ���浱ǰ����ĳһ�������id

            // ��鵱ǰ��ĵ�0������
            id = gridInfo.layers[0].pixmapId;

            // ���idС��1��˵��������ӵ���һ�㻹û��Ϳ������ֱ������
            if (id<1)
                continue;

            Fairy::TerrainData::Pixmap pixmap = terrainData->mPixmaps[id-1];
            layerTexName = terrainData->mTextures[ pixmap.textureId ];

            // �����entity���õ���ͼ�����������б�
            _addResourceCount(layerTexName, "", terrainData->getGridPosition(x, z), "", mTerrainTextureList);            

            // ��鵱ǰ��ĵ�1������
            id = gridInfo.layers[1].pixmapId;
            if (id<1)
                continue;

            pixmap = terrainData->mPixmaps[id-1];

            layerTexName = terrainData->mTextures[ pixmap.textureId ];

            // �����entity���õ���ͼ�����������б�
            _addResourceCount(layerTexName, "", terrainData->getGridPosition(x, z), "", mTerrainTextureList);            
        }		
    }

    // ����
    _sort(mTerrainTextureList);

    mModelTextureParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Model Texture"));

    for (size_t i = 0; i < mTextureInfoList.size(); ++i)
    {
        Ogre::String textureInfo = mTextureInfoList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mTextureInfoList[i]->mResourceReferenceCount );

        wxString texName(textureInfo.c_str());
        mResourceInfoTree->AppendItem(mModelTextureParentId, texName, -1, -1, new ResourceItemData(mTextureInfoList[i]));
    }

    mModelParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Model"));

    for (size_t i = 0; i < mModelInfoList.size(); ++i)
    {
        Ogre::String modelInfo = mModelInfoList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mModelInfoList[i]->mResourceReferenceCount );

        wxString modelName(modelInfo.c_str());
        mResourceInfoTree->AppendItem(mModelParentId, modelName, -1, -1, new ResourceItemData(mModelInfoList[i]));
    }

    mTerrainTextureParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Terrain Texture"));

    for (size_t i = 0; i < mTerrainTextureList.size(); ++i)
    {
        Ogre::String modelInfo = mTerrainTextureList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mTerrainTextureList[i]->mResourceReferenceCount );

        wxString texName(modelInfo.c_str());
        mResourceInfoTree->AppendItem(mTerrainTextureParentId, texName, -1, -1, new ResourceItemData(mTerrainTextureList[i]));
    }

    mEffectParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Effect"));

    for (size_t i = 0; i < mEffectList.size(); ++i)
    {
        Ogre::String effectInfo = mEffectList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mEffectList[i]->mResourceReferenceCount );

        wxString effectName(effectInfo.c_str());
        mResourceInfoTree->AppendItem(mEffectParentId, effectName, -1, -1, new ResourceItemData(mEffectList[i]));
    }
}

void
SceneInfomationDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = SceneInfomation(this, true, true);

    mSceneInfoText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_SCENE_INFO), wxTextCtrl);
    mResourceInfoTree = wxDynamicCast(this->FindWindow(ID_TREECTRL_TEXTURE_DETAIL_LIST), wxTreeCtrl);
    mResourceOwnerListBox = wxDynamicCast(this->FindWindow(ID_LISTBOX_RESOURCE_OWNNER), wxListBox);

    _setSceneInfoText();
    _setResourceList();
}

void SceneInfomationDialog::_addResourceCount(const Ogre::String& resourceName, const Ogre::String& ownerName, const Ogre::Vector3& pos,
                       const Ogre::String& ownerObjectName, ResourceInfoList& resourceInfoList)
{
    ResourceInfoList::iterator i = resourceInfoList.begin();

    while ( i != resourceInfoList.end() )
    {
        if ( (*i)->mResourceName == resourceName )
        {
            ++ ( (*i)->mResourceReferenceCount );

            (*i)->mResourceOwnerInfos.push_back( ResourceOwnerInfo(ownerName, pos, ownerObjectName) );

            return;
        }

        ++i;
    }

    ResourceInfo* resource = new ResourceInfo(resourceName, 1);

    resource->mResourceOwnerInfos.push_back( ResourceOwnerInfo(ownerName, pos, ownerObjectName) );

    resourceInfoList.push_back(resource);
}

void SceneInfomationDialog::_clearResourceInfos(ResourceInfoList& resourceInfoList)
{
    ResourceInfoList::iterator i = resourceInfoList.begin();

    while ( i != resourceInfoList.end() )
    {
        delete *i;

        ++i;
    }

    resourceInfoList.clear();
}

SceneInfomationDialog::ResourceInfo* 
SceneInfomationDialog::_getResourceInfo(const Ogre::String& resourceName, const ResourceInfoList& resourceInfoList)
{
    ResourceInfoList::const_iterator i = resourceInfoList.begin();

    while ( i != resourceInfoList.end() )
    {
        if ( (*i)->mResourceName == resourceName )
        {
            return *i;
        }

        ++i;
    }

    return NULL;
}

void SceneInfomationDialog::_sort(ResourceInfoList& resourceInfoList)
{
    std::sort( resourceInfoList.begin(), resourceInfoList.end(), referenceLess() );
}

void SceneInfomationDialog::OnResourceOwnerDoubleClick( wxCommandEvent &event )
{
    ResourceOwnerInfo* ownerInfo = (ResourceOwnerInfo*)(event.GetClientData());

    if (ownerInfo)
    {
        const Fairy::ObjectPtr& object = mManipulator->getSceneInfo()->findObjectByName(ownerInfo->mOwnerObjectName);

        Ogre::Vector3 aabbHeight(0.0,0.0,0.0);

        if (object)
        {
            if(object->hasProperty("bounding box"))
            {
                const Ogre::AxisAlignedBox& aabb = Fairy::VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));

                aabbHeight = Ogre::Vector3(0,aabb.getMaximum().y,0);
            }

            mManipulator->selectObject(object);
        }

        _lookatObject(ownerInfo->mPosition + aabbHeight);
    }
}

void SceneInfomationDialog::OnResourceInfoDoubleClick(wxTreeEvent& e)
{
    mResourceOwnerListBox->Clear();

    wxTreeItemId itemId = e.GetItem();
    wxTreeItemId parentId = mResourceInfoTree->GetItemParent(itemId);

    if (parentId == mUndergroundParentId)
    {
        const Ogre::String& objectName(mResourceInfoTree->GetItemText(itemId).c_str());

        const Fairy::ObjectPtr& object = mManipulator->getSceneInfo()->findObjectByName(objectName);

        if (object)
        {
            mManipulator->selectObject(object);

            if (object->hasProperty("position"))
            {
                const Ogre::Vector3& pos = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));

                _lookatObject(pos);
            }
        }
    }
    else
    {
        ResourceItemData *resourceItemData = (ResourceItemData *)(mResourceInfoTree->GetItemData(itemId));

        if (resourceItemData)
        {
            ResourceInfo* resourceInfo = resourceItemData->resourceInfo;

            ResourceInfo* info = NULL;

            if (parentId == mModelTextureParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mTextureInfoList);
            else if (parentId == mModelParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mModelInfoList);
            else if (parentId == mTerrainTextureParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mTerrainTextureList);
            else if (parentId == mEffectParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mEffectList);

            if (info)
            {
                const ResourceOwnerInfos& ownerInfos = info->mResourceOwnerInfos;

                for (size_t i = 0; i < ownerInfos.size(); ++i)
                {
                    Ogre::String ownerInfoString = ownerInfos[i].mOwnerName + "   " + Ogre::StringConverter::toString(ownerInfos[i].mPosition);

                    wxString infoStr(ownerInfoString.c_str());
                    mResourceOwnerListBox->InsertItems(1, &infoStr, 0);
                    mResourceOwnerListBox->SetClientData(0, (void*)(&(ownerInfos[i])));
                }
            }
        }
    }    
}

void SceneInfomationDialog::OnSaveInfo( wxCommandEvent &event )
{
    Fairy::SceneInfo* sceneInfo = mManipulator->getSceneInfo();

    Ogre::String fileName = sceneInfo->mName + ".SceneInfo";
    std::ofstream of(fileName.c_str(), std::ios::binary | std::ios::out);

    of << "Scene Name : " << sceneInfo->mName << std::endl;

    of << "Model Texture Count :\t" << Ogre::StringConverter::toString(mTextureInfoList.size()) << std::endl;
    of << "Terrain Texture Count :\t" << Ogre::StringConverter::toString(mTerrainTextureList.size()) << std::endl;
    of << "Model Count :\t" << Ogre::StringConverter::toString(mModelInfoList.size()) << std::endl;

    size_t texTotalSize = 0;

    of << "\nModel Texture List :\t" << std::endl;

    for (size_t texIndex = 0; texIndex < mTextureInfoList.size(); ++texIndex)
    {
        Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(mTextureInfoList[texIndex]->mResourceName);

        if (texture.isNull())
            continue;

        texTotalSize += texture->getSize();

        of << Ogre::StringConverter::toString(texIndex) << '\t' << mTextureInfoList[texIndex]->mResourceName << '\t'
            << Ogre::StringConverter::toString(texture->getWidth()) << "x" << Ogre::StringConverter::toString(texture->getHeight())
            << '\t' << Ogre::StringConverter::toString(texture->getSize()) << " byte\t" << "Reference Count :\t"
            << Ogre::StringConverter::toString(mTextureInfoList[texIndex]->mResourceReferenceCount) << std::endl;

    }

    of << "\nTotal Model Texture Size :\t" << Ogre::StringConverter::toString(texTotalSize) << " byte" << std::endl;

    texTotalSize = 0;

    of << "\nTerrain Texture List :\t" << std::endl;

    for (size_t texIndex = 0; texIndex < mTerrainTextureList.size(); ++texIndex)
    {
        Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(mTerrainTextureList[texIndex]->mResourceName);

        if (texture.isNull())
            continue;

        texTotalSize += texture->getSize();

        of << Ogre::StringConverter::toString(texIndex) << '\t' << mTerrainTextureList[texIndex]->mResourceName << '\t'
            << Ogre::StringConverter::toString(texture->getWidth()) << "x" << Ogre::StringConverter::toString(texture->getHeight())
            << '\t' << Ogre::StringConverter::toString(texture->getSize()) << " byte\t" << "Reference Count :\t"
            << Ogre::StringConverter::toString(mTerrainTextureList[texIndex]->mResourceReferenceCount) << std::endl;
    }

    of << "\nTotal Terrain Texture Size :\t" << Ogre::StringConverter::toString(texTotalSize) << " byte" << std::endl;

    of.close();
}

void SceneInfomationDialog::OnRefresh( wxCommandEvent& event )
{
    _setSceneInfoText();
    _setResourceList();
}

void SceneInfomationDialog::OnTreeKeyDown(wxTreeEvent& event)
{
    switch (event.GetKeyCode())
    {
    case WXK_DELETE:
        {           
            wxTreeItemId itemId = mResourceInfoTree->GetSelection();

            if (itemId == mUndergroundParentId)
            {
                // ���ֶԻ������û�ѡ���Ƿ�����˲���
                wxString prompt = _("Do you want to delete these objects?");

                int res = wxMessageBox(prompt, _("Warning"),
                    wxYES_NO|wxICON_QUESTION,
                    this);

                switch (res)
                {
                default:   
                case wxNO:
                    return;

                case wxYES:
                    {
                        wxTreeItemIdValue cookie;
                        wxTreeItemId item = mResourceInfoTree->GetFirstChild(itemId, cookie);
                        while (item.IsOk())
                        {
                            wxString itemText = mResourceInfoTree->GetItemText(item);
                            mManipulator->removeObject(itemText.c_str());

                            item = mResourceInfoTree->GetNextChild(itemId, cookie);
                        }

                        mResourceInfoTree->Delete(itemId);

                        mUndergroundParentId = 0;
                    }
                }                
            }
            else
            {
                ResourceItemData *resourceItemData = (ResourceItemData *)(mResourceInfoTree->GetItemData(itemId));

                if (resourceItemData)
                {
                    ResourceInfo* resourceInfo = resourceItemData->resourceInfo;

                    ResourceInfo* info = NULL;

                    info = _getResourceInfo(resourceInfo->mResourceName, mModelInfoList);

                    if (info)
                    {
                        // ���ֶԻ������û�ѡ���Ƿ�����˲���
                        wxString prompt = _("Do you want to delete these objects?");

                        int res = wxMessageBox(prompt, _("Warning"),
                            wxYES_NO|wxICON_QUESTION,
                            this);

                        switch (res)
                        {
                        default:   
                        case wxNO:
                            return;

                        case wxYES:
                            {
                                const ResourceOwnerInfos& ownerInfos = info->mResourceOwnerInfos;

                                for (size_t i = 0; i < ownerInfos.size(); ++i)
                                {
                                    mManipulator->removeObject(ownerInfos[i].mOwnerObjectName);
                                }

                                mResourceInfoTree->Delete(itemId);
                            }
                        }
                    }
                }
            }            
        }
        break;
    }
}

size_t SceneInfomationDialog::_getObjectCountByType(const Ogre::String& type)
{
    size_t count = 0;

    Fairy::SceneInfo* sceneInfo = mManipulator->getSceneInfo();

    // �������е�mesh
    Fairy::Scene::ObjectsByTypeRange objects =
        sceneInfo->findObjectsByType(type);

    for (Fairy::Scene::ObjectsByTypeIterator it = objects.first; it != objects.second; ++it)
        ++count;

    return count;
}

void SceneInfomationDialog::_setSceneInfoText(void)
{
    Fairy::TerrainData* terrainData = mManipulator->getTerrainData();

    // terrain size
    wxString infoText = "Terrain Size   " + Ogre::StringConverter::toString(terrainData->mXSize) +
        " x " + Ogre::StringConverter::toString(terrainData->mZSize) + '\n';

    // texture count
    int texCount = 0;
    size_t memoryUsage = 0;

    Ogre::TextureManager::ResourceMapIterator it = Ogre::TextureManager::getSingleton().getResourceIterator();
    while (it.hasMoreElements())
    {
        Ogre::ResourceHandle handle = it.peekNextKey();
        Ogre::ResourcePtr res = it.getNext();
        if ( res->isLoaded() )
        {
            ++texCount;
            memoryUsage += res->getSize();
        }
    }

    infoText += "Loaded Texture Count   " + Ogre::StringConverter::toString(texCount) + '\n';
    infoText += "Loaded Texture Memory   " + Ogre::StringConverter::toString(memoryUsage) + " byte = " + 
        Ogre::StringConverter::toString(memoryUsage / (1024*1024)) + " mb\n";

    // object count
    infoText += "Static Entity Count " + Ogre::StringConverter::toString( _getObjectCountByType(Fairy::StaticEntityObject::msType) ) + '\n';
    infoText += "Model Count " + Ogre::StringConverter::toString( _getObjectCountByType(Fairy::ModelObject::msType) ) + '\n';
    infoText += "Liquid Count " + Ogre::StringConverter::toString( _getObjectCountByType(Fairy::TerrainLiquidObject::msType) ) + '\n';
    infoText += "Light Count " + Ogre::StringConverter::toString( _getObjectCountByType(Fairy::LightObject::msType) ) + '\n';

    mSceneInfoText->SetValue(infoText);
}

void SceneInfomationDialog::_clearAllResourceInfos(void)
{
    _clearResourceInfos(mTextureInfoList);
    _clearResourceInfos(mModelInfoList);
    _clearResourceInfos(mTerrainTextureList);
    _clearResourceInfos(mEffectList);

    mModelTextureParentId = 0;
    mModelParentId = 0;    
    mTerrainTextureParentId = 0;
    mEffectParentId = 0;
    mUndergroundParentId = 0;
}

void SceneInfomationDialog::_addUndergroundItem(const Fairy::ObjectPtr& object, wxTreeItemId rootId)
{
    if (false == mUndergroundParentId.IsOk())
        mUndergroundParentId = mResourceInfoTree->AppendItem(rootId, wxString("Underground"));

    Fairy::TerrainData* terrainData = mManipulator->getTerrainData();

    if (object->getType() == Fairy::StaticEntityObject::msType ||
        object->getType() == Fairy::ModelObject::msType)
    {
        // �����ģ��,�͵ü�������bounding box�ĸ߶�,����ȵ��θ߶ȵ�,�Ϳ����ж������ڵ�����
        if(object->hasProperty("bounding box"))
        {
            if (object->hasProperty("position"))
            {
                Ogre::Matrix4 transform = Ogre::Matrix4::IDENTITY;

                Ogre::Matrix3 rot3x3;
                if (object->hasProperty("orientation"))
                {
                    Ogre::Quaternion orientation = Fairy::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
                    orientation.ToRotationMatrix(rot3x3);
                }
                else
                {
                    rot3x3 = Ogre::Matrix3::IDENTITY;
                }

                Ogre::Matrix3 scale3x3;
                if (object->hasProperty("scale"))
                {
                    Ogre::Vector3 scale = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("scale"));
                    scale3x3 = Ogre::Matrix3::ZERO;
                    scale3x3[0][0] = scale.x;
                    scale3x3[1][1] = scale.y;
                    scale3x3[2][2] = scale.z;
                }
                else
                {
                    scale3x3 = Ogre::Matrix3::IDENTITY;
                }

                transform = rot3x3 * scale3x3;

                Ogre::Vector3 position = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));
                transform.setTrans(position);

                Ogre::AxisAlignedBox aabb = Fairy::VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));
                aabb.transformAffine(transform);

                // ȡ�ð˸����������
				//david-<<
                const Ogre::Vector3* corners = aabb.getAllCorners();
				//david->>

                unsigned short lowerCount = 0;

                for (unsigned short i=0; i<8; ++i)
                {
                    // ���ڶ������ĸ�����,�ж����ĸ�������ڵ��εĸ߶�,����ĸ��㶼�ȵ��ε�,�����ĸ��������Ҳ�ȵ��ε�,�Ҿ��ж���������ڵ�������
                    if ( i==1 || i==2 || i==4 || i==5 )
                    {
                        if ( corners[i].y < terrainData->getHeightAt(corners[i].x, corners[i].z) )
                            ++lowerCount;

                        if (lowerCount == 4)
                        {
                            if ( corners[i].y < terrainData->getHeightAt(position.x, position.z) )
                            {
                                wxString itemName(object->getName().c_str());
                                mResourceInfoTree->AppendItem(mUndergroundParentId, itemName, -1, -1);
                                break;
                            }
                        }
                    }
                }
				//david-<<
				//OGRE_FREE(corners,Ogre::MEMCATEGORY_SCENE_CONTROL);
				//david->>
            }        
        }
    }
    else if (object->getType() == Fairy::LightObject::msType)
    {
        const Fairy::LightObject* lightObject = static_cast<Fairy::LightObject *>(object.get());

        // �����û��λ����Ϣ,���ü����Ƿ��ڵ���
        if (lightObject->mLightType == Ogre::Light::LT_DIRECTIONAL)
            return;

        if (object->hasProperty("position"))
        {
            const Ogre::Vector3& pos = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));
            if ( (pos.y) < terrainData->getHeightAt(pos.x, pos.z) )
            {
                wxString itemName(object->getName().c_str());
                mResourceInfoTree->AppendItem(mUndergroundParentId, itemName, -1, -1);
            }
        }  
    }
}

void SceneInfomationDialog::_lookatObject(const Ogre::Vector3& pos)
{
    Ogre::Camera* camera = mManipulator->getCamera();
	if(camera->getProjectionType() == Ogre::PT_PERSPECTIVE)
	{
		Ogre::Vector3 position = pos;

		position.y += 2750.0f;

		Ogre::Vector3 direction = camera->getDirection();
		float v = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(89.9f));
		float s = Ogre::Math::Sqrt((direction.x*direction.x + direction.z*direction.z) / (1-v*v));
		direction.x /= s;
		direction.z /= s;
		direction.y = -v;
		camera->setDirection(direction);

		camera->setPosition(position);
	}
	else
	{
		Ogre::Vector3 position = pos;
		Ogre::Vector3 oldPos = camera->getPosition();

		camera->setPosition(position.x, oldPos.y, position.z);
	}
}