// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "BrushSelector.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "BrushSelector.h"


// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>
#include <OgreImage.h>

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
#include "Core/Terrain.h"
#include "Core/TerrainData.h"

#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"
#include "TerrainPaintInfoContainer.h"

#include "res/BrushSelector_wdr.h"
#include <OgreTextureManager.h>
#include "Core/FairyResources.h"
// ----------------------------------------------------------------------------
// BrushSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

//static const int feID_PIXMAP_LIST = wxNewId();

BEGIN_EVENT_TABLE(BrushSelector, wxPanel)
    EVT_UPDATE_UI(feID_PIXMAP_LIST, BrushSelector::OnUpdateControls)

	EVT_CHECKBOX(feID_FLIP_HORIZONTAL, BrushSelector::OnCheck) 
	EVT_CHECKBOX(feID_FLIP_VERTICAL, BrushSelector::OnCheck) 
	EVT_CHECKBOX(feID_ROTATE_RIGHT_ANGLE, BrushSelector::OnCheck) 
	EVT_CHECKBOX(feID_RANDOM, BrushSelector::OnCheck) 
    EVT_CHECKBOX(feID_MIRROR_DIAGONAL, BrushSelector::OnCheck)
    EVT_CHECKBOX(feID_ENABLE_PIXMAP_PREVIEW, BrushSelector::OnCheck) 

	EVT_BUTTON(feID_RELOAD_PIXMAP_LIST, BrushSelector::OnReloadTextureList)
	EVT_TREE_SEL_CHANGED(feID_PIXMAP_LIST, BrushSelector::OnPixmapListSelected)
    EVT_TREE_ITEM_RIGHT_CLICK(feID_PIXMAP_LIST, BrushSelector::OnRightButtonUp)

    EVT_MENU(feID_PIXMAP_FULLTERRAINPAINT, BrushSelector::OnFullTerrainPaint)
    EVT_MENU(feID_PIXMAP_PAINTINONEGRID, BrushSelector::OnPaintInOneGrid)

END_EVENT_TABLE()

void
BrushSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mSplitter = NULL;
    mBrushesTree = NULL;
	mBrushesPreview = NULL;

	mFlipHorizontal = NULL;
	mFlipVertical = NULL;
	mRotateRightAngle = NULL;
	mRandom = NULL;
    mMirrorDiagonal = NULL;

	mEnablePreview = NULL;

	mCurrentOrientationType = 0;
	mCurrentFileName = "";

    // ����Ԥ��ͼ�Ĵ�С
    mPreviewImageWidth = 64;
    mPreviewImageHeight = 64;
	// ��ʼ��image������Ϊ��ɫ
	mCurrentPreviewImage.Create( mPreviewImageWidth, mPreviewImageHeight, true );

    // ��úڰ�ͼ
    mBlackWhitePreviewImage = CellGridBitmaps(1).ConvertToImage();

    mResourceGroup = resourceGroup;

    mPaintInOneGrid = false;
    mIsRandomOrientation = false;

	mCatalogMap.clear();
}

BrushSelector::~BrushSelector()
{
    if (mBrushesTree)
    {
        mBrushesTree->Freeze();
        mBrushesTree->DeleteAllItems();
        mBrushesTree->Thaw();
    }
}

void
BrushSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
BrushSelector::Create(wxWindow *parent, wxWindowID winid,
                      const wxPoint& pos, const wxSize& size,
                      long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

	InitPixmapSelector(this, true, true);

	/// ��ȡ���ؼ�������
	mBrushesTree = wxDynamicCast(this->FindWindow(feID_PIXMAP_LIST), wxTreeCtrl);
	mBrushesPreview = wxDynamicCast(this->FindWindow(feID_PIXMAP_PREVIEW), wxStaticBitmap);
	mFlipHorizontal = wxDynamicCast(this->FindWindow(feID_FLIP_HORIZONTAL), wxCheckBox);
	mFlipVertical = wxDynamicCast(this->FindWindow(feID_FLIP_VERTICAL), wxCheckBox);
	mRotateRightAngle = wxDynamicCast(this->FindWindow(feID_ROTATE_RIGHT_ANGLE), wxCheckBox);
	mRandom = wxDynamicCast(this->FindWindow(feID_RANDOM), wxCheckBox);
    mMirrorDiagonal = wxDynamicCast(this->FindWindow(feID_MIRROR_DIAGONAL), wxCheckBox);

	mEnablePreview = wxDynamicCast(this->FindWindow(feID_ENABLE_PIXMAP_PREVIEW), wxCheckBox);

    ReloadTextureList();

    // ��ʼ��ʱ����ѡ��ˢ�ķ���ѡ��
    mFlipHorizontal->Enable(false);
    mFlipVertical->Enable(false);
    mRotateRightAngle->Enable(false);
    mMirrorDiagonal->Enable(false);
    mRandom->Enable(false);
    mEnablePreview->Enable(false);

    // ����ͼ�úڰ�ͼ
    mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);

    return true;
}

void
BrushSelector::ReloadTextureList(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    ReloadTextureList();
}

void
BrushSelector::ReloadTextureList(void)
{
	CatalogMap catalogMap;
	mCatalogMap.clear();

    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mBrushesTree->Freeze();

    mBrushesTree->DeleteAllItems();

	wxTreeItemId rootId = mBrushesTree->AddRoot(/*_("Brushes")*/wxT("��ˢ�б�"));
 
    // ���½��������ļ�
    GetSceneManipulator()->reloadPaintInfo();

    const Fairy::TerrainPaintInfoContainer *paintInfoContainer = GetSceneManipulator()->getTerrainPaintInfoContainer();

    assert (paintInfoContainer);

    const Fairy::TextureInfoMap &textureInfoMap = paintInfoContainer->getTextureInfoMap();

	OwnerTexs ownerTextures;

	Fairy::TextureInfoMap::const_iterator it = textureInfoMap.begin();	

    // ������ˢ����
    while ( it != textureInfoMap.end() )
    {
        const Fairy::TextureInfos &textureInfos = it->second;
        
        // �����û�ˢ�µ���������
        for ( size_t i=0; i<textureInfos.size(); ++i )
        {
            // ȡ�������Ĵ����������
            Ogre::String ownerTexName = textureInfos[i].ownerTextureName;

			// ��¼�����������
			ownerTextures.insert(OwnerTexs::value_type(ownerTexName, ownerTexName));

            // ��������������а�����/��˵��������һ���ļ����е�
            size_t pos = ownerTexName.find_last_of('/');

            // ���ļ�����
            if (pos != Ogre::String::npos)
            {
                // ��ȥ�������ļ�����ʣ��·����
                ownerTexName.erase(pos+1);
                // ���ϻ�ˢ������
                ownerTexName.append(textureInfos[i].brushName);
                wxTreeItemId id = mBrushesTree->AppendItem( GetParentId(ownerTexName, catalogMap), wxT(textureInfos[i].textureName) );
				
				mCatalogMap.insert(CatalogMap::value_type( textureInfos[i].textureName, id ));
            }
            else
            {
                Ogre::String brushName = textureInfos[i].brushName;
                // ������ڸ�Ŀ¼�£���ֱ���û�ˢ��������Ϊ·����
                wxTreeItemId id = mBrushesTree->AppendItem( GetParentId(brushName, catalogMap), wxT(textureInfos[i].textureName) );
				mCatalogMap.insert(CatalogMap::value_type( textureInfos[i].textureName, id ));
			}				
        }
        ++it;
    }

	wxString lostTexNames = wxEmptyString;
	for (OwnerTexs::iterator ownerIt = ownerTextures.begin(); ownerIt != ownerTextures.end(); ++ownerIt)
	{
		Ogre::String texName = ownerIt->first;

		Ogre::FileInfoListPtr fileInfoList =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("Brushes",texName);
		
		Ogre::FileInfoList::const_iterator itBegin = fileInfoList->begin();
		Ogre::FileInfoList::const_iterator itEnd = fileInfoList->end(); 
		if (itBegin == itEnd)
		{
			lostTexNames+=wxT("\n");
			lostTexNames+=texName.c_str();
			 continue;
		}

		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(texName);

		if (!texture.isNull())
		{
			Ogre::TextureManager::getSingleton().remove(texName);
			Ogre::Image image;
			image.load(texName, Fairy::BRUSH_RESOURCE_GROUP_NAME);
			texture = Ogre::TextureManager::getSingleton()
				.loadImage(texName, Fairy::BRUSH_RESOURCE_GROUP_NAME, image);
		}
	}	

	if (!lostTexNames.empty())
	{
		wxMessageBox(wxT("������ͼ�޷��ҵ�:")+lostTexNames);
	}

    mBrushesTree->Thaw();
}

wxTreeItemId  
BrushSelector::GetParentId( Ogre::String &catalog, CatalogMap &map )
{
	// �����������·��Ϊ�գ�˵��������������ԴĿ¼�ĸ�Ŀ¼��
	if ( catalog == "" )
		// ���ԣ����Ľڵ�Ӧ�ñ����ڸ�Ŀ¼��
		return mBrushesTree->GetRootItem();
	else
	{
		// ��������ͼ��������һ��Ŀ¼������еĻ���˵�����Ŀ¼�������Ѿ����ã���������Ϊ��ͬ��Ŀ¼�µ�����
		// ��������ˣ����ͷ������Ŀ¼
		CatalogMap::iterator i = map.find(catalog);

		if ( i != map.end() )
			return i->second;
		// ���û�У��͵õݹ������ң�������
		else
		{
			Ogre::String path, baseName;
			Ogre::StringUtil::splitFilename(catalog, baseName, path);
			if (!path.empty() && path[path.length()-1] == '/')
				path.erase(path.length() - 1);

			wxTreeItemId id = mBrushesTree->AppendItem( GetParentId(path, map), wxT(baseName) );

			map.insert( CatalogMap::value_type( catalog, id ) );

			mCatalogMap.insert(CatalogMap::value_type( baseName, id ));

			return id;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void
BrushSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void BrushSelector::setBrushByTexName(const Ogre::String& textureName)
{
	Ogre::String brushName = GetSceneManipulator()->getTerrainPaintInfoContainer()->findBrushNameByTexName(textureName);
	CatalogMap::iterator it = mCatalogMap.find(brushName);
	if (it != mCatalogMap.end())
	{
		mBrushesTree->SelectItem(it->second);
	}	
}

const Fairy::Action * 
BrushSelector::OnStartAutoTexAction( const wxTreeItemId &itemId, const wxString &brushName )
{
    // �����Զ�����ƴ��action
    Fairy::Action* action = GetSceneManipulator()->_getAction("AutoTexPaintAction");

    // ��ȡ�������
    wxString groupName = mBrushesTree->GetItemText(itemId);

    // ���õ�ǰ�������
    action->setParameter( "%GroupName", AS_STRING(brushName) );
 
    // ��֮ǰ��·������ȥ
    wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
    wxTreeItemId rootId = mBrushesTree->GetRootItem();

    while ( parentId != rootId )
    {
        wxString parentText = mBrushesTree->GetItemText(parentId);
        parentText += '/';
        groupName.Prepend( parentText );

        parentId = mBrushesTree->GetItemParent(parentId);   
    }

    groupName += "/";

    // ��ȡ��Ŀ¼�µĵ�һ���ڵ�
    wxTreeItemIdValue dummy;
    wxTreeItemId childId = mBrushesTree->GetFirstChild(itemId,dummy);

    while ( childId.IsOk() == true )
    {
        // ����ÿ���ڵ㣬������������
        wxString texName = mBrushesTree->GetItemText(childId);

        childId = mBrushesTree->GetNextChild(itemId,dummy);

        if ( texName.find('|') == wxString::npos )
            continue;

        action->setParameter( "%TextureName", AS_STRING(texName) );        
    }

    // ����InputFinish��ʾ���������������
    action->setParameter( "%TextureName", "InputFinish" );
    
    // �ж������Ƿ���ȷ�ؽ����˳�ʼ��
    if ( action->getParameter("%IsInit") == "Yes" )
    {
        GetSceneManipulator()->setActiveAction(action);
        return action;
    }
    else
    {
        // ����simplepaintAction
        action = GetSceneManipulator()->_getAction("SimplePaintAction");

        action->setParameter( "%TextureName", "InputBegin" );

        // ��ȡ��Ŀ¼�µĵ�һ���ڵ�
        wxTreeItemIdValue dummy;
        wxTreeItemId childId = mBrushesTree->GetFirstChild(itemId,dummy);

        while ( childId.IsOk() == true )
        {
            // ����ÿ���ڵ㣬������������
            wxString texName = mBrushesTree->GetItemText(childId);

            childId = mBrushesTree->GetNextChild(itemId,dummy);

            if ( texName.find('|') == wxString::npos )
                continue;

            const Fairy::TextureInfos &currentPaintInfos = 
                GetSceneManipulator()->getTextureInfos(mCurrentFileName.c_str());

            const Fairy::TextureInfo &currentPaintInfo =
                GetSceneManipulator()->getTextureInfo(mCurrentFileName.c_str(), texName.c_str());

            Ogre::Image *previewImage = GetSceneManipulator()->getPreviewImage(currentPaintInfo.ownerTextureName);

            // ���������Ϣ�ַ���
            Ogre::String texInfoString;
            texInfoString += currentPaintInfo.ownerTextureName;
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.height);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.width);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.leftCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.topCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getHeight());
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getWidth());
			texInfoString += "|";
			texInfoString += Ogre::StringConverter::toString(currentPaintInfo.rotateType);

            action->setParameter( "%TextureName", AS_STRING(texInfoString) );  

            // �����и����󣿣�����Ϊû�����ø������Ƿ�Ϊ��ͨ�����С�����������һ��������һ����������ô
            // �����ܻ�ֻ���ŵ�һ��
        }

        if ( action->getParameter("%IsInit") == "Yes" )
        {
            GetSceneManipulator()->setActiveAction(action);
            return action;
        }
        else
        {
            // Ϊ�ܳɹ���ʼ�����Ȱ�active action��ΪNULL
            GetSceneManipulator()->setActiveAction(NULL);
            return NULL;
        }
    }
}

void
BrushSelector::OnPixmapListSelected(wxTreeEvent& event)
{
	try
	{
    // �����Ǹĳɶ����ļ�����޸�
    //  mPaintInOneGrid = false;
    // ���浱ǰѡ�е�����Ĵ�С
    int selectedTexXSize = 0;
    int selectedTexZSize = 0;

    // ����ˢ��ѡ��ʱ��Ҫ�����ж�
    wxTreeItemId itemId = event.GetItem();

    wxTreeItemId rootId = mBrushesTree->GetRootItem();

    // ���ѡ�и����ͷ���
    if ( itemId == rootId )
    {
        // ѡ�и�Ŀ¼��ʱ������ѡ����
        mFlipHorizontal->Enable(false);
        mFlipVertical->Enable(false);
        mRotateRightAngle->Enable(false);
        mMirrorDiagonal->Enable(false);
        mRandom->Enable(false);
        mEnablePreview->Enable(false);

        // ����ͼ�úڰ�ͼ
        mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);

        return;
    }

    if (itemId.IsOk())
    {
        mCurrentOrientationType = 0;

        // ���ѡ��ĸ���root��˵���û�ˢ�Ƿ��ڸ�Ŀ¼�µģ�Ҳ�п����Ǹ�Ŀ¼�µ���Ŀ¼
        if ( mBrushesTree->GetItemParent(itemId) == rootId )
        {
            mCurrentFileName = mBrushesTree->GetItemText(itemId);
        }
        else
        {
            // ���´����Ƿ��ڸ�Ŀ¼�µĻ�ˢ

            wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
            mCurrentFileName = mBrushesTree->GetItemText(itemId);

            // �Դ�ȡ����Ŀ¼��Ŀ¼������ϳ����·�������������ļ���
            while ( parentId != rootId )
            {
                wxString parentText = mBrushesTree->GetItemText(parentId);
                parentText += '/';
                mCurrentFileName.Prepend( parentText );

                parentId = mBrushesTree->GetItemParent(parentId);
            }		
        }

        // �����������.��˵�����ļ���������Ŀ¼��
        if ( mCurrentFileName.find_first_of('|') != wxString::npos )
        {
            mFlipHorizontal->Enable(true);
            mFlipVertical->Enable(true);
            mRotateRightAngle->Enable(true);
            mMirrorDiagonal->Enable(true);
            mRandom->Enable(true);
            mEnablePreview->Enable(true);

            Ogre::String currentFileName(mCurrentFileName.c_str());

            size_t pos = currentFileName.find_last_of('/');

            // ȡ��brush����
            Ogre::String brushName = currentFileName.substr(0,pos);
            Ogre::String textureName = currentFileName.substr(pos+1);

            const Fairy::TextureInfos &currentPaintInfos = 
                GetSceneManipulator()->getTextureInfos(brushName);

            const Fairy::TextureInfo &currentPaintInfo = GetSceneManipulator()->getTextureInfo(brushName, textureName);

            Ogre::Image *previewImage = GetSceneManipulator()->getPreviewImage(currentPaintInfo.ownerTextureName);

            // ���������Ϣ�ַ���
            Ogre::String texInfoString;
            texInfoString += currentPaintInfo.ownerTextureName;
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.height);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.width);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.leftCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.topCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getHeight());
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getWidth());
			texInfoString += "|";
			texInfoString += Ogre::StringConverter::toString(currentPaintInfo.rotateType);

            Fairy::Action* action = GetSceneManipulator()->_getAction("SimplePaintAction");
            action->setParameter( "%TextureName", "InputBegin" );
            action->setParameter( "%TextureName", AS_STRING(texInfoString) );
            action->setParameter( "%OrientationType", Ogre::StringConverter::toString(mCurrentOrientationType) );

            if ( (currentPaintInfo.width > 1 || currentPaintInfo.height > 1) && !mPaintInOneGrid )
            {               
                action->setParameter( "%TexNormalSize", "false" );
                mFlipHorizontal->Enable(false);
                mFlipVertical->Enable(false);
                mRotateRightAngle->Enable(false);
                mMirrorDiagonal->Enable(false);
                mRandom->Enable(false);
            }
            else
            {
                action->setParameter("%TexNormalSize","true");
            }

            GetSceneManipulator()->setActiveAction(action);

            buildPreviewBitmap(currentPaintInfo);
         
            if ( mEnablePreview->IsChecked() == false )
            {
                // �������ʾ����ͼ������ʾĬ�ϵĺڰ�ͼ
                mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
            }
        }
        // �����������.��˵�������ļ�������Ŀ¼��
        else
        {
            mFlipHorizontal->Enable(false);
            mFlipVertical->Enable(false);
            mRotateRightAngle->Enable(false);
            mMirrorDiagonal->Enable(false);
            mRandom->Enable(false);
            mEnablePreview->Enable(true);

            // ������Ŀ¼����������жϣ������뵽͸�������б�

            const Fairy::Action* action = OnStartAutoTexAction(itemId, mCurrentFileName);

            if ( action != 0 )
            {
                // �Ӹ��������ʵ�������������һ������Ϊ����ͼ
                // ��ȡ�������������ŵ�mPreviewImageWidth*mPreviewImageHeight����������ͼ
              //  Ogre::String tempTexName = action->getParameter( "%TextureName" );

                const Fairy::TextureInfos &currentPaintInfos = 
                    GetSceneManipulator()->getTextureInfos(mCurrentFileName.c_str());

              //  const Fairy::TextureInfo &currentPaintInfo = currentPaintInfos[0];

                // ����preview image
                buildPreviewBitmap(currentPaintInfos[0]);                
            }
            else
            {
                mEnablePreview->Enable(false);
                mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
            }

            if ( mEnablePreview->IsChecked() == false )
            {
                // �������ʾ����ͼ������ʾĬ�ϵĺڰ�ͼ
                mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
            }
        }
    }
   
    // �����е�checkbox�ظ�δѡ���״̬
    mFlipHorizontal->SetValue(false);
    mFlipVertical->SetValue(false);
    mRotateRightAngle->SetValue(false);
    mMirrorDiagonal->SetValue(false);
    mRandom->SetValue(false);
	}
	catch (Ogre::Exception &e)
	{
		wxMessageBox(
			e.getDescription().c_str(),
           /* _("Texture Wrong")*/wxT("�����������Ҳ�������ͼƬ"),
            wxOK|wxCENTRE|wxICON_ERROR, this);
	}
}

void
BrushSelector::OnCheck( wxCommandEvent &event )
{
	if ( event.GetId() == feID_RANDOM )
	{
		// ��ֹ������ť
		if ( mRandom->IsChecked() )
		{
            mIsRandomOrientation = true;

			mFlipHorizontal->Enable(false);
			mFlipVertical->Enable(false);
			mRotateRightAngle->Enable(false);
            mMirrorDiagonal->Enable(false);
		}
		else
		{
            mIsRandomOrientation = false;

			mFlipHorizontal->Enable(true);
			mFlipVertical->Enable(true);
			mRotateRightAngle->Enable(true);
            mMirrorDiagonal->Enable(true);
		}
	}

	else
	{
		mCurrentOrientationType = 0;

		// ������checkbox״̬���ж�
		if ( mFlipHorizontal->IsChecked() == true )
		{
			mCurrentOrientationType |= 1;
		}

		if ( mFlipVertical->IsChecked() == true )
		{
			mCurrentOrientationType |= 2;
		}

		if ( mRotateRightAngle->IsChecked() == true )
		{
			mCurrentOrientationType |= 4;
		}

        if ( mMirrorDiagonal->IsChecked() == true )
        {
            mCurrentOrientationType |= 8;
        }
	}
	
	Fairy::Action* action = GetSceneManipulator()->_getAction("SimplePaintAction");

    assert (action);

    action->setParameter( "%UseRandomOrientation", mIsRandomOrientation ? "true" : "false" );
	action->setParameter( "%OrientationType", Ogre::StringConverter::toString(mCurrentOrientationType) );

	GetSceneManipulator()->setActiveAction(action);

 //   wxImage tempPreview;
	if ( mEnablePreview->IsChecked() )
	{
		// ѡ�а�ťʱ������ͼҲҪ��Ӧ�仯
		if ( event.GetId() == feID_FLIP_HORIZONTAL )
		{
			mCurrentPreviewImage = mCurrentPreviewImage.Mirror();
			mBrushesPreview->SetBitmap(mCurrentPreviewImage);
		}

		if ( event.GetId() == feID_FLIP_VERTICAL )
		{
			mCurrentPreviewImage = mCurrentPreviewImage.Mirror(false);
			mBrushesPreview->SetBitmap(mCurrentPreviewImage);
		}

		if ( event.GetId() == feID_ROTATE_RIGHT_ANGLE )
		{
			if ( mRotateRightAngle->IsChecked() == true )
			{
				mCurrentPreviewImage = mCurrentPreviewImage.Rotate90(false);
				mBrushesPreview->SetBitmap(mCurrentPreviewImage);
			}
			else
			{
				mCurrentPreviewImage = mCurrentPreviewImage.Rotate90();
				mBrushesPreview->SetBitmap(mCurrentPreviewImage);
			}
		}
	}
	else
	{
		// �������ʾ����ͼ������ʾĬ�ϵĺڰ�ͼ
        mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
	}

    // Ԥ��ͼ�Ŀ���
	if ( event.GetId() == feID_ENABLE_PIXMAP_PREVIEW )
	{
		if ( mEnablePreview->IsChecked() )
		{
            // ����û��򿪿��أ�����ǰ��OnPixmapListSelected�м���õ�Ԥ��ͼ����ʾ
            // ��Ҳ��Ϊʲôǰ��OnPixmapListSelected��������������Ƿ�򿪶�Ҫ����Ԥ��ͼ��ԭ��
            // ��Ȼ�Ļ�������򿪿��غ���ʾ�����ϴμ����Ԥ��ͼ�������ǵ�ǰ��ѡ�����Ԥ��ͼ
            mBrushesPreview->SetBitmap(mCurrentPreviewImage);
		}
		else
		{
            mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
		}
	}
}

void 
BrushSelector::OnReloadTextureList( wxCommandEvent &event )
{
	ReloadTextureList();
}

void 
BrushSelector::OnRightButtonUp(wxTreeEvent& event)
{
    wxTreeItemId selectedId = event.GetItem();
    
    ShowBrushRightButtonMenu(selectedId);
}

void
BrushSelector::ShowBrushRightButtonMenu(const wxTreeItemId &itemId)
{
    // ���ȫ����ƽ�̵Ļ�ˢ����
    mPixmapArray.clear();

    wxMenu menu;

    // ���Ӳ˵���
    menu.Append(feID_PIXMAP_FULLTERRAINPAINT, _T("&Full Terrain Paint"));
    // ��ʼʱΪ������
    menu.Enable(feID_PIXMAP_FULLTERRAINPAINT,false);

    // �����Ǽ���Ŀ¼�»���ļ��Ƿ����Ҫ�󣨲���Ϊtga�ļ���
    wxTreeItemId rootId = mBrushesTree->GetRootItem();

    if ( itemId == rootId )
        return;

    if (itemId.IsOk())
    {
        // ���ѡ��ĸ���root��˵���û�ˢ�Ƿ��ڸ�Ŀ¼�µģ�Ҳ�п����Ǹ�Ŀ¼�µ���Ŀ¼
        if ( mBrushesTree->GetItemParent(itemId) == rootId )
        {
            mCurrentFileName = mBrushesTree->GetItemText(itemId);
        }
        else
        {
            // ���´����Ƿ��ڸ�Ŀ¼�µĻ�ˢ
            wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
            mCurrentFileName = mBrushesTree->GetItemText(itemId);

            // �Դ�ȡ����Ŀ¼��Ŀ¼������ϳ����·�������������ļ���
            while ( parentId != rootId )
            {
                wxString parentText = mBrushesTree->GetItemText(parentId);
                parentText += '/';
                mCurrentFileName.Prepend( parentText );

                parentId = mBrushesTree->GetItemParent(parentId);
            }		
        }

        // �����������.��˵�����ļ���������Ŀ¼��
        if ( mCurrentFileName.find_first_of('|') != wxString::npos )
        {
            Ogre::String currentFileName(mCurrentFileName.c_str());

            size_t pos = currentFileName.find_last_of('/');

            // ȡ��brush����
            Ogre::String brushName = currentFileName.substr(0,pos);
            Ogre::String textureName = currentFileName.substr(pos+1);

            const Fairy::TextureInfos &currentPaintInfos = 
                GetSceneManipulator()->getTextureInfos(brushName);

            const Fairy::TextureInfo &currentPaintInfo = GetSceneManipulator()->getTextureInfo(brushName, textureName);

            // �����������͸��������������ƽ����������
            if ( !mSceneManipulator->IsTransparentTexture(currentPaintInfo.ownerTextureName) )
            {
                Fairy::PaintPixmap pixmap;
                
                mSceneManipulator->setPaintPixmapInfo(pixmap, currentPaintInfo);
                mPixmapArray.push_back(pixmap);      
            }
        }
        // �����������.��˵�������ļ�������Ŀ¼��
        else
        {
            // ��ȡ�������
            wxString groupName = mBrushesTree->GetItemText(itemId);

            // ��֮ǰ��·������ȥ
            wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
            wxTreeItemId rootId = mBrushesTree->GetRootItem();

            while ( parentId != rootId )
            {
                wxString parentText = mBrushesTree->GetItemText(parentId);
                parentText += '/';
                groupName.Prepend( parentText );

                parentId = mBrushesTree->GetItemParent(parentId);   
            }

            groupName += "/";

            // ��ȡ��Ŀ¼�µĵ�һ���ڵ�
            wxTreeItemIdValue dummy;
            wxTreeItemId childId = mBrushesTree->GetFirstChild(itemId,dummy);

            while ( childId.IsOk() == true )
            {
                // ����ÿ���ڵ㣬������������
                wxString texName = mBrushesTree->GetItemText(childId);

                childId = mBrushesTree->GetNextChild(itemId,dummy);

                if ( texName.find('|') == wxString::npos )
                    continue;

                texName.Prepend(groupName);

                Ogre::String currentFileName(texName.c_str());

                size_t pos = currentFileName.find_last_of('/');

                // ȡ��brush����
                Ogre::String brushName = currentFileName.substr(0,pos);
                Ogre::String textureName = currentFileName.substr(pos+1);

                const Fairy::TextureInfos &currentPaintInfos = 
                    GetSceneManipulator()->getTextureInfos(brushName);

                const Fairy::TextureInfo &currentPaintInfo = GetSceneManipulator()->getTextureInfo(brushName, textureName);

                // �����������͸��������������ƽ����������
                if ( !mSceneManipulator->IsTransparentTexture(currentPaintInfo.ownerTextureName) )
                {
                    Fairy::PaintPixmap pixmap;

                    mSceneManipulator->setPaintPixmapInfo(pixmap, currentPaintInfo);
                    mPixmapArray.push_back(pixmap);      
                }
            }
        }

        if ( mPixmapArray.size() > 0 )
        {
            // �ò˵���ʾ
            menu.Enable(feID_PIXMAP_FULLTERRAINPAINT,true);
        }
        else
            menu.Enable(feID_PIXMAP_FULLTERRAINPAINT,false);
    }

    // ����һ�ѡ�к������һ���������Ƕ�Ӧһ���񣨿�����hack�Ĵ��룬���ܸó������ļ���������Զ���һ�������Ӧ���ٸ�
    // ���Ӳ˵���
    menu.AppendCheckItem(feID_PIXMAP_PAINTINONEGRID, _T("&Paint this texture in one grid"));
    // ��ʼʱΪ������
   // menu.Enable(feID_PIXMAP_PAINTINONEGRID,false);
    menu.Check(feID_PIXMAP_PAINTINONEGRID,mPaintInOneGrid);

    PopupMenu(&menu);
}

void
BrushSelector::OnFullTerrainPaint(wxCommandEvent& event)
{
    // ���ֶԻ������û�ѡ���Ƿ�����˲���
    wxString prompt = /*_("The operations before can not be undo,Do you want to continue the operation?")*/wxT("������������Իָ�����ȷ��Ҫ�������в�����");

    int res = wxMessageBox(prompt, /*_("Warning")*/wxT("����"),
        wxYES_NO|wxICON_QUESTION,
        this);

    switch (res)
    {
    default:   
    case wxNO:
        return;

    case wxYES:
        {
            int texCount = mPixmapArray.size();
            int pixmapId = 0;

            const Fairy::TerrainData::GridInfoArray &gridInfos = mSceneManipulator->getTerrainData()->mGridInfos;

            if (!gridInfos.empty())
            {
                // ���ֻѡ��һ��������һ���ļ�����ֻ��һ������
                if ( texCount == 1 )
                {
                    Fairy::PaintPixmap &pixmap = mPixmapArray[0];
                    pixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                        pixmap.textureName, pixmap.left, pixmap.top, pixmap.right, pixmap.bottom );
                }

                size_t gridCount = gridInfos.size();

                for ( size_t i = 0; i < gridCount; ++i )
                {
                    Fairy::TerrainData::GridInfo info = gridInfos[i];

                    if ( texCount > 1 )
                    {
                    //    Ogre::String texName;
						int index = static_cast<int>(Ogre::Math::RangeRandom(0,texCount));
						if (index == texCount)
						{
							index = 0;
						}
						assert (index < texCount);
                        const Fairy::PaintPixmap &pixmap = mPixmapArray[ index ];

                        // �����id������pixmap
                        pixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                            pixmap.textureName, pixmap.left, pixmap.top, pixmap.right, pixmap.bottom );        
                    }

                    info.layers[0].pixmapId = pixmapId;
                    info.layers[0].orientation = 0;

                    info.layers[1].pixmapId = 0;
                    info.layers[1].orientation = 0;         

                    mSceneManipulator->getTerrainData()->setGridInfo(i,info);

                } 

                mSceneManipulator->getTerrain()->notifyGridInfoModified(0,0,mSceneManipulator->getTerrainData()->getXSize(),mSceneManipulator->getTerrainData()->getZSize());
            }

            // ���redo undo�б�
            mSceneManipulator->getOperatorManager()->reset();
        }
    }
}

void 
BrushSelector::OnPaintInOneGrid(wxCommandEvent& event)
{
    mPaintInOneGrid = event.IsChecked();
    wxTreeEvent treeEvent;
    treeEvent.SetItem(mBrushesTree->GetSelection());
    OnPixmapListSelected(treeEvent);
}

void 
BrushSelector::buildPreviewBitmap( const Fairy::TextureInfo texInfo )
{
    const Ogre::uchar BytePerPixel = 8;

    // ��ȡԭʼimage
    Ogre::Image *oriImage = GetSceneManipulator()->getPreviewImage(texInfo.ownerTextureName);
    // Դ������Ĵ�С
    size_t oriImageHeight = oriImage->getHeight();
    size_t oriImageWidth = oriImage->getWidth();

    Ogre::uchar *oriImageData = oriImage->getData();
    // ��ѡ����Ĵ�С
    size_t newImageWidth = texInfo.width*TexTileSize;
    size_t newImageHeight = texInfo.height*TexTileSize;

    // ����һ���㹻��Ŀռ��������½���image������
    size_t newImagegetRowSpan = newImageWidth*oriImage->getBPP()/BytePerPixel;  // �½���image���п���λΪ�ֽڣ�
    Ogre::uchar *newImageData = OGRE_ALLOC_T(Ogre::uchar, oriImageHeight*newImagegetRowSpan, Ogre::MEMCATEGORY_GENERAL);//new Ogre::uchar[newImageHeight*newImagegetRowSpan];
    
    Ogre::uchar *newImageDataPointer = newImageData;
    // ��֪��ʼ���ص�
    size_t startPoint = ( oriImageWidth * texInfo.topCorner + texInfo.leftCorner )
        * TexTileSize * oriImage->getBPP()/BytePerPixel;

    Ogre::uchar *oriImagedataPointer = oriImageData + startPoint;

    // ����ѡ�������������ȡ������������һ���µ�image
    for ( Ogre::uint i=0; i<newImageHeight; ++i )
    {
        memcpy(newImageDataPointer, oriImagedataPointer, newImagegetRowSpan);
        newImageDataPointer += newImagegetRowSpan;
        oriImagedataPointer += oriImage->getRowSpan();
    }

    Ogre::Image newImage;
    newImage.loadDynamicImage(newImageData,newImageWidth,newImageHeight,1,oriImage->getFormat(),true);

    // �����ѡ�������64*64������resize
    if ( texInfo.width > 1 || texInfo.height > 1 )
        newImage.resize(mPreviewImageWidth, mPreviewImageHeight);

    // �����alpha��Ҫ��ڰ�ͼ���л��
    if ( newImage.getHasAlpha() )
    {  
		Ogre::ColourValue col;

		for ( int i=0; i<mPreviewImageWidth; ++i )
		{
			for ( int j=0; j<mPreviewImageWidth; ++j )
			{
				col = newImage.getColourAt(j,i,0);

				float alphaValue = col.a;

				unsigned char r = col.r*255 * alphaValue + mBlackWhitePreviewImage.GetRed(i,j) * ( 1.0f - alphaValue);
				unsigned char g = col.g*255 * alphaValue + mBlackWhitePreviewImage.GetGreen(i,j) * ( 1.0f - alphaValue);
				unsigned char b = col.b*255 * alphaValue + mBlackWhitePreviewImage.GetBlue(i,j) * ( 1.0f - alphaValue);

				// ���õ�image��
				mCurrentPreviewImage.SetRGB(j,i,r,g,b);
			}
		}
        // ���õ�����ͼ�ؼ���
        mBrushesPreview->SetBitmap(mCurrentPreviewImage);
    }
    // û��alpha����ֱ�ӿ�������
    else
    {
		Ogre::ColourValue col;

		for ( int i=0; i<mPreviewImageWidth; ++i )
		{
			for ( int j=0; j<mPreviewImageWidth; ++j )
			{
				col = newImage.getColourAt(j,i,0);

				unsigned char r = col.r*255;
				unsigned char g = col.g*255;
				unsigned char b = col.b*255;

				// ���õ�image��
				mCurrentPreviewImage.SetRGB(j,i,r,g,b);
			}
		}

        mBrushesPreview->SetBitmap(mCurrentPreviewImage);    
    }
}


