// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WXMaterialPreviewDialog.h"
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

#include "WXMaterialPreviewDialog.h"
#include "SceneManipulator.h"

// ----------------------------------------------------------------------------
// PropertyGrid headers
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// ParticleEditorDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(MaterialPreviewDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MaterialPreviewDialog, wxDialog)
	EVT_INIT_DIALOG(MaterialPreviewDialog::OnInitDialog)
	EVT_BUTTON(ID_BUTTON_USE, MaterialPreviewDialog::OnUseButtonDown)
END_EVENT_TABLE()


void
MaterialPreviewDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	InitPreviewGrids();
}

MaterialPreviewDialog::~MaterialPreviewDialog()
{
	clearPreviewImage();
}

void 
MaterialPreviewDialog::InitPreviewGrids(void)
{

	wxFlexGridSizer *item0 = new wxFlexGridSizer( 6, 0, 0 );

	typedef std::list<Ogre::String> MaterialFileNameList;
	MaterialFileNameList materialFileNameList;

	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"*.material");
	for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
	{
		if ( it->archive->getName() == EFFECT_PATH)
		{
			materialFileNameList.push_back(it->filename);
		}
	}

	wxComboBox *matCombo = mParentDialog->mMaterialComboBox;
	assert (matCombo);

	for ( int i=0; i<matCombo->GetCount(); ++i )
	{
		Ogre::String matName = matCombo->GetString(i);

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(matName);			

		if (mat.isNull())
		{
		}
		else
		{
			size_t numOfTex = mat->getTechnique(0)->getPass(0)->getNumTextureUnitStates();
			if (numOfTex > 0)
			{
				Ogre::String texName = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
				if (texName.empty() == false)
				{
					wxBoxSizer *item1 = new wxBoxSizer( wxVERTICAL );

					wxStaticBitmap *item2 = new wxStaticBitmap( this, ID_STATICBITMAP, TexturePreview( 0 ), wxDefaultPosition, wxSize(64,64) );
					item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

					wxStaticText *item3 = new wxStaticText( this, ID_TEXT_MATERIAL_NAME, _("text"), wxDefaultPosition, wxDefaultSize, 0 );
					item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

					wxButton *item4 = new wxButton( this, ID_BUTTON_USE, _("Use"), wxDefaultPosition, wxDefaultSize, 0 );
					item1->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

					item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

					buildPreviewBitmap( texName );
					item2->SetBitmap(mCurrentPreviewImage);
					item3->SetLabel(texName.c_str());
					item4->SetLabel(matName.c_str());
				}
			}
		}
	}

	this->SetSizer( item0 );
	item0->SetSizeHints( this );
}

void 
MaterialPreviewDialog::buildPreviewBitmap( const Ogre::String &texName )
{
	const Ogre::uchar BytePerPixel = 8;

	// ��ȡԭʼimage
	Ogre::Image *oriImage = getPreviewImage(texName);
	// Դ������Ĵ�С
	size_t oriImageHeight = oriImage->getHeight();
	size_t oriImageWidth = oriImage->getWidth();

	Ogre::uchar *oriImageData = oriImage->getData();

	// ����һ���㹻��Ŀռ��������½���image������
	size_t newImagegetRowSpan = oriImageWidth*oriImage->getBPP()/BytePerPixel;  // �½���image���п���λΪ�ֽڣ�
	Ogre::uchar *newImageData = OGRE_ALLOC_T(Ogre::uchar, oriImageHeight*newImagegetRowSpan, Ogre::MEMCATEGORY_GENERAL);//new Ogre::uchar[oriImageHeight*newImagegetRowSpan];

	Ogre::uchar *newImageDataPointer = newImageData;

	Ogre::uchar *oriImagedataPointer = oriImageData;

	// ����ѡ�������������ȡ������������һ���µ�image
	for ( Ogre::uint i=0; i<oriImageHeight; ++i )
	{
		memcpy(newImageDataPointer, oriImagedataPointer, newImagegetRowSpan);
		newImageDataPointer += newImagegetRowSpan;
		oriImagedataPointer += oriImage->getRowSpan();
	}

	Ogre::Image newImage;
	newImage.loadDynamicImage(newImageData,oriImageWidth,oriImageHeight,1,oriImage->getFormat(),true);

	// �����ѡ�������64*64������resize
	if ( oriImageWidth > mPreviewImageWidth || oriImageHeight > mPreviewImageHeight )
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

				unsigned char r = col.r*255 * alphaValue; 
				unsigned char g = col.g*255 * alphaValue; 
				unsigned char b = col.b*255 * alphaValue;

				// ���õ�image��
				mCurrentPreviewImage.SetRGB(j,i,r,g,b);
			}
		}
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
	}
}

wxBitmap MaterialPreviewDialog::TexturePreview( size_t index )
{
	if (index == 0)
	{
		/* XPM */
		static const char *xpm_data[] = {
			/* columns rows colors chars-per-pixel */
			"16 15 2 1",
				"a c Black",
				"b c #FF0000",
				/* pixels */
				"aaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaa",
				"aaabbbbbbbbbbaaa",
				"aaabaaaaaaaabaaa",
				"aaababaaababbaaa",
				"aaabaaaaaaabaaaa",
				"aaaababbbaabaaaa",
				"aaaabbaaaabaaaaa",
				"aabaaabbbbaababa",
				"aaabaaaaaabbbaaa",
				"aaabbbbabbbbaaaa",
				"aaaaaaabbbbaaaaa",
				"aaaabbbaabbaaaaa",
				"aabbaaaaaaabbbaa",
				"aaaaaaaaaaaaaaaa"
		};
		wxBitmap bitmap( xpm_data );
		return bitmap;
	}
	return wxNullBitmap;
}

Ogre::Image * 
MaterialPreviewDialog::getPreviewImage( const Ogre::String &textureName )
{
	PreviewImageMap::iterator it = mPreviewImageMap.find(textureName);

	if ( it == mPreviewImageMap.end() )
	{
		Ogre::Image *image = new Ogre::Image;
		image->load(textureName,"General");

		assert (image);

		std::pair< PreviewImageMap::iterator, bool > inserted = 
			mPreviewImageMap.insert(PreviewImageMap::value_type(textureName,image));

		assert (inserted.second);

		return image;
	}
	else
	{
		assert (it->second);
		return it->second;
	}
}

void MaterialPreviewDialog::clearPreviewImage(void)
{
	PreviewImageMap::iterator it = mPreviewImageMap.begin();

	while ( it != mPreviewImageMap.end() )
	{
		delete it->second;
		++it;
	}

	mPreviewImageMap.clear();
}

void MaterialPreviewDialog::OnUseButtonDown(wxCommandEvent &e)
{
	mSelectedMatName = static_cast<wxButton *>(e.GetEventObject())->GetLabel().c_str();
	mParentDialog->InitMaterialInfo(mSelectedMatName);
	this->Hide();
}