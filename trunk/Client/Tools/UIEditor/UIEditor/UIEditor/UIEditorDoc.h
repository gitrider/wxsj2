// UIEditorDoc.h : CUIEditorDoc ��Ľӿ�
//


#pragma once


class CUIEditorDoc : public CDocument
{
protected: // �������л�����
	CUIEditorDoc();
	DECLARE_DYNCREATE(CUIEditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CUIEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


