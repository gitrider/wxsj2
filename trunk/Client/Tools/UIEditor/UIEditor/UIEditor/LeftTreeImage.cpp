// LeftTreeImage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LeftTreeImage.h"
#include <afxcmn.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIImageset.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUITexture.h>
#include <CEGUIExceptions.h>

using namespace CEGUI;

// CLeftTreeImage

IMPLEMENT_DYNAMIC(CLeftTreeImage, CTreeCtrl)

CLeftTreeImage::CLeftTreeImage()
{
	g_leftTreeImage = this;
}

CLeftTreeImage::~CLeftTreeImage()
{
}


BEGIN_MESSAGE_MAP(CLeftTreeImage, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CLeftTreeImage::OnNMRclick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftTreeImage::OnTvnSelchanging)
	ON_COMMAND(ID_COPY_SET_NAME, &CLeftTreeImage::OnCopySetName)
END_MESSAGE_MAP()

int CLeftTreeImage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


void CLeftTreeImage::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	UINT cnt;
	MENU_ITEM* pMenuItem;
	CString SelectName;
	CPoint myPoint;
	GetCursorPos(&myPoint);
	ScreenToClient(&myPoint);
	UINT uFlags;
	HTREEITEM item = HitTest(myPoint, &uFlags);
	if (item == NULL)
	{
		return ;
	}
	SelectItem(item);
	SelectName = GetItemText(item);

	HTREEITEM hParent = GetParentItem(item);
	if (hParent)
	{

		CString szSetName = GetItemText(hParent);

		m_szCurrentSetName.Format("set:%s image:%s", szSetName, SelectName);

		CPoint oriPoint;
		GetCursorPos(&oriPoint);

		CMenu menu;
		menu.LoadMenu(IDR_IMAGESET_NAME);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, oriPoint.x, oriPoint.y, this);
	}

	*pResult = 0;
}
void CLeftTreeImage::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem = GetSelectedItem();

	HTREEITEM hParent = GetParentItem(hItem);

	CString strImageSetName ;
	CString strImageName;

	try
	{
	if (hParent == NULL)
		{
			strImageSetName = GetItemText(hItem);
			g_pEditorView->setImageQuad(CRect(0,0,0,0));
		}
		else
		{
			strImageSetName = GetItemText(hParent);

			strImageName = GetItemText(hItem);

			Imageset* pImageSet = ImagesetManager::getSingleton().getImageset(strImageSetName.GetString());

			if ( pImageSet )
			{
				Rect crect = pImageSet->getImage(strImageName.GetString()).getSourceTextureArea();

				CRect rect(crect.d_left, crect.d_top, crect.d_left + crect.getWidth(), crect.d_top + crect.getHeight());
				g_pEditorView->setImageQuad(rect);
			}
		}

		char szImagSet[256] = {0};

		sprintf(szImagSet, "set:%s image:full_image", strImageSetName);

		//��ʾ���ͼƬ
		m_pImageWindow->setProperty("BackgroundImage", szImagSet);

		//���ô��ڴ�С
		Imageset* pImageSet = ImagesetManager::getSingleton().getImageset((utf8*)mbcs_to_utf8(strImageSetName.GetString()));
		float h = pImageSet->getTexture()->getHeight();
		float w = pImageSet->getTexture()->getWidth();
		m_pImageWindow->setWidth(CEGUI::Absolute, w);
		m_pImageWindow->setHeight(CEGUI::Absolute, h);
	}
	catch (CEGUI::UnknownObjectException& e)
	{
		char szmsg[256] = {0};
		sprintf(szmsg, "�޷��ҵ�ͼ��set:%s image:%s",strImageSetName,  strImageName);
		MessageBox(szmsg,"��ʾ");
	}

	

	*pResult = 0;
}


void CLeftTreeImage::Initialize(void)
{
	//����һ����������
	m_pBackWindow = WindowManager::getSingleton().createWindow("DefaultWindow", "___Image_BackGround___");

	m_pImageWindow = WindowManager::getSingleton().createWindow("WoWLook/StaticImage", "___Image_Shower_Window___");

	m_pImageWindow->setPosition(CEGUI::Absolute, Point(0,0));
	m_pImageWindow->setWidth(CEGUI::Relative, 1.f);
	m_pImageWindow->setHeight(CEGUI::Relative, 1.f);
	m_pImageWindow->setProperty("BackgroundEnabled", "True");
	

	m_pBackWindow->addChildWindow(m_pImageWindow);

	ImagesetManager::ImagesetIterator it = ImagesetManager::getSingleton().getIterator();

	try
	{
		bool flag = true;
		//�������е�
		while ( !it.isAtEnd() )
		{
			Imageset* pImageSet = (Imageset*)(it.getCurrentValue());

			pImageSet->defineImage("full_image", Rect(Point(0,0), Size(pImageSet->getTexture()->getWidth(), pImageSet->getTexture()->getHeight())), Point(0,0));

			//��ӵ�����
			HTREEITEM hParent = InsertItem(utf8_to_mbcs(it.getCurrentKey().c_str()),NULL, NULL);
			Imageset::ImageIterator it2 = pImageSet->getIterator();

			//�������е�ͼ�񼯺�
			while ( !it2.isAtEnd() )
			{
				if( it2.getCurrentKey() == "full_image" ) 
				{
					++it2;
					continue;
				}

				InsertItem(utf8_to_mbcs(it2.getCurrentKey().c_str()), hParent, NULL);
				//��һ��
				++it2;
			}

			//��һ��
			++it;
		}
	}
	catch (...)
	{
		MessageBox("����ͼ�񼯺ϼ���ʧ��,��鿴CEGUI.log,�޸ĳ����.imageset.xml�ļ�","��ʾ",MB_OK);
	}
	
}

void CLeftTreeImage::OnTabFoucsEvent(bool lose)
{
	static Window* pDefaultGUISheet = NULL;
	if(lose)
	{
		if (pDefaultGUISheet)
		{
			System::getSingleton().setGUISheet(pDefaultGUISheet);
			pDefaultGUISheet = NULL;
			g_pEditorView->setShowMode(true);
		}
	}
	else
	{
		pDefaultGUISheet = System::getSingleton().getGUISheet();
		System::getSingleton().setGUISheet(m_pBackWindow);
		g_pEditorView->setShowMode(false);
	}
}
void CLeftTreeImage::OnCopySetName()
{
	//���浽���а�
	//paste to clipboard
	if(::OpenClipboard(::GetTopWindow(NULL)))
	{
		::EmptyClipboard();

		//alloc mem
		INT size = m_szCurrentSetName.GetLength();
		HGLOBAL hglb = ::GlobalAlloc(GMEM_MOVEABLE,(size_t)size + 1);
		char* ptxt = (char*)::GlobalLock(hglb);
		
		strcpy(ptxt, m_szCurrentSetName.GetString());

		ptxt[size] = 0;

		::GlobalUnlock(hglb);
		//copy to clipboard

		::SetClipboardData(CF_TEXT, hglb);
		::CloseClipboard();
		//free mem
		::GlobalFree(hglb);
	}
}
