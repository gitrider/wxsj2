/**********************************************************************
 *<
	FILE: building_collision_exp.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "building_collision_exp.h"
#include ".\exportdatabuf.h"

#define BUILDING_COLLISION_EXP_CLASS_ID	Class_ID(0x77efa596, 0x372dc8a8)

class Building_collision_exp : public SceneExport 
{
	public:

        static HWND hParams;

		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int	 DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);


		//Constructor/Destructor
		Building_collision_exp();
		~Building_collision_exp();	

public:

	Interface*	ip;
	BOOL        m_bIsExport;

	// 数据缓冲区.
	CExportDataBuf m_MeshDataBuf;

	// 把行走面信息写到文件中去.
	void WriteDataToFile( const TCHAR *name);

	// 导出物体  非0导出行走面成功, 没有导出行走面.
	BOOL ExportOneMesh(INode* node, CExportDataBuf* pDataBuf);

	// 把物体转换成三角形面.
	TriObject* GetTriObjectFromNode(INode *node, TimeValue t, BOOL &deleteIt);

};


class Building_collision_expClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new Building_collision_exp(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return BUILDING_COLLISION_EXP_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("Building_collision_exp"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

};



static Building_collision_expClassDesc Building_collision_expDesc;
ClassDesc2* GetBuilding_collision_expDesc() { return &Building_collision_expDesc; }


BOOL CALLBACK Building_collision_expOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static Building_collision_exp *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (Building_collision_exp *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}
	return FALSE;
}


//--- Building_collision_exp -------------------------------------------------------
Building_collision_exp::Building_collision_exp()
{
	m_bIsExport = FALSE;
}

Building_collision_exp::~Building_collision_exp() 
{

}

int Building_collision_exp::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *Building_collision_exp::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("cll");
}

const TCHAR *Building_collision_exp::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("火狐_建筑行走面导出");
}
	
const TCHAR *Building_collision_exp::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("火狐_建筑行走面导出");
}

const TCHAR *Building_collision_exp::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("pp");
}

const TCHAR *Building_collision_exp::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *Building_collision_exp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *Building_collision_exp::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int Building_collision_exp::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void Building_collision_exp::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL Building_collision_exp::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}


void Building_collision_exp::WriteDataToFile( const TCHAR *name)
{
	FILE* pFile = NULL;
	pFile = ::fopen(name, "w+b");

	DWORD dwVersion = 0xf0000001;

	// 写版本号
	::fwrite(&dwVersion, sizeof(dwVersion), 1, pFile);
	
	CExportDataBuf* pMeshData = &m_MeshDataBuf;
	int iFaceCount   = pMeshData->m_iFaceCount;
	int iVertexCount = iFaceCount * 3; 
	int iVertexIndex = 0;
		
	// 写点的个数.
	::fwrite(&iFaceCount, sizeof(iFaceCount), 1, pFile);
	for(int j = 0; j < iFaceCount; j++)
	{
		// 写位置第一个pos 坐标
		iVertexIndex =  pMeshData->m_posFaceVector[j].iF1;
		::fwrite(&pMeshData->m_posVector[iVertexIndex].x, sizeof(float), 1, pFile);
		::fwrite(&pMeshData->m_posVector[iVertexIndex].y, sizeof(float), 1, pFile);
		::fwrite(&pMeshData->m_posVector[iVertexIndex].z, sizeof(float), 1, pFile);

		// 写位置第二个pos 坐标
		iVertexIndex =  pMeshData->m_posFaceVector[j].iF2;
		::fwrite(&pMeshData->m_posVector[iVertexIndex].x, sizeof(float), 1, pFile);
		::fwrite(&pMeshData->m_posVector[iVertexIndex].y, sizeof(float), 1, pFile);
		::fwrite(&pMeshData->m_posVector[iVertexIndex].z, sizeof(float), 1, pFile);

		// 写位置第三个pos 坐标
		iVertexIndex =  pMeshData->m_posFaceVector[j].iF3;
		::fwrite(&pMeshData->m_posVector[iVertexIndex].x, sizeof(float), 1, pFile);
		::fwrite(&pMeshData->m_posVector[iVertexIndex].y, sizeof(float), 1, pFile);
		::fwrite(&pMeshData->m_posVector[iVertexIndex].z, sizeof(float), 1, pFile);

	}

	::fclose(pFile);

}








int	Building_collision_exp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	//TODO: Implement the actual file Export here and 
	//		return TRUE If the file is exported properly

	//if(!suppressPrompts)
	//	DialogBoxParam(hInstance, 
	//			MAKEINTRESOURCE(IDD_PANEL), 
	//			GetActiveWindow(), 
	//			Building_collision_expOptionsDlgProc, (LPARAM)this);//

	m_bIsExport = false;
	ip = i;
	int numChildren = ip->GetRootNode()->NumberOfChildren();
	
	if(0 == numChildren)
	{
		MessageBox(NULL,  "3dmax中没有物体！", "警告！", MB_OK);
		return TRUE;
	}

	for(int i = 0; i < numChildren; i++)
	{
		if(ExportOneMesh(ip->GetRootNode()->GetChildNode(i), &m_MeshDataBuf))
		{
			break;
		}
	}

	if(m_bIsExport)
	{
		WriteDataToFile(name);
	}

	if(!m_bIsExport)
	{
		MessageBox(NULL,  "没有选择一个行走面！", "警告！", MB_OK);
		return TRUE;
	}
	MessageBox(NULL,  "导出完毕！", "提示！", MB_OK);
	return TRUE;
}

BOOL Building_collision_exp::ExportOneMesh(INode* node, CExportDataBuf* pDataBuf)
{
	
	if(node && pDataBuf)
	{
		pDataBuf->strName = node->GetName();

		ObjectState os = node->EvalWorldState(0);
	
		if (!os.obj)
		{
			return FALSE;
		}
	
		// Targets are actually geomobjects, but we will export them
		// from the camera and light objects, so we skip them here.
		if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		{
			return FALSE;
		}
	
		int i;		// 以后需要用到的循环变量.

		Matrix3 tm = node->GetObjTMAfterWSM(0);
		
		// 三角形面的索引值.
		int vx1 = 0, vx2 = 1, vx3 = 2;
		
		BOOL needDel;
		TriObject* tri = GetTriObjectFromNode(node, 0, needDel);
		if (!tri) 
		{
			return FALSE;
		}
	
		Mesh* mesh = &tri->GetMesh();
	
		pDataBuf->m_iFaceCount     = mesh->getNumFaces();
		pDataBuf->m_posVectorCount = mesh->getNumVerts();
	
		Point3 v;
		POS    pos;
		for (i=0; i<mesh->getNumVerts(); i++) 
		{
			v = tm * mesh->verts[i];
			pos.x = v.x;
			pos.y = v.z;
			pos.z = -v.y;
			pDataBuf->m_posVector.push_back(pos);
		
		}
	
	
		FACE face;
		for (i=0; i<mesh->getNumFaces(); i++) 
		{
			face.iF1 = (WORD)mesh->faces[i].v[vx1];
			face.iF2 = (WORD)mesh->faces[i].v[vx2];
			face.iF3 = (WORD)mesh->faces[i].v[vx3];

			pDataBuf->m_posFaceVector.push_back(face);
		}
		
		if (needDel) 
		{
			delete tri;
		}

	}

	m_bIsExport = TRUE;
	return TRUE;
}


TriObject* Building_collision_exp::GetTriObjectFromNode(INode *node, TimeValue t, BOOL &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{ 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		
		if (obj != tri) 
		{
			deleteIt = TRUE;
		}

		return tri;
	}
	else 
	{
		return NULL;
	}
}
