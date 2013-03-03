#ifndef __TerrainPaintInfoContainer_H__
#define __TerrainPaintInfoContainer_H__

#include "Core/FairyPrerequisites.h"

namespace Fairy {

    enum TexType
    {
        InnerTex,
        InnerCornerTex,	
		OuterCornerTex,		
		EdgeTex,	
        DuijiaoTex,
        Other,
    };

	enum ExtraTexType
	{
		LT, RT, LB, RB, LE, RE, TE, BE, EOther
	};

    struct TextureInfo
    {
        TextureInfo( String texName, uint w, uint h, uint lc, uint tc, 
			String bName, TexType type, ExtraTexType eType, uint rt = 0 )
            : ownerTextureName(texName), width(w), height(h), leftCorner(lc), rotateType(rt),
			  topCorner(tc), brushName(bName), textureType(type), extraTexType(eType)
        {
        }

        TextureInfo()
            : ownerTextureName(), width(0), height(0), leftCorner(), topCorner(), brushName(), 
			textureType(Other), rotateType(0), extraTexType(EOther)
        {
        }

        void clear(void)
        {
            textureName.clear();
            width = height = leftCorner = topCorner = 0;
            brushName.clear();
            ownerTextureName.clear();
            textureType = Other;
			rotateType = 0;
			extraTexType = EOther;
        }

        String  textureName; // ������������ƣ��Զ����ɣ������ڻ�ˢ���е���ʾ
        // �����ĸ������ĵ�λ�������أ����Ǳ�׼�����С��64���ı�������1��2��4
        uint    width;
        uint    height;
        // ���������Ͻ��ڴ������е�λ��
        uint    leftCorner;
        uint    topCorner;
        String  brushName;
        TexType textureType;
        String  ownerTextureName;   // �����Ĵ����������
		uint    rotateType;	// ����ı�׼���Ͻ�������ת���ĸ�����
		ExtraTexType extraTexType;
    };

    typedef std::vector<TextureInfo> TextureInfos;
    // keyΪ��ˢ���ƣ�valueΪ�û�ˢ����������
    typedef std::map<String, TextureInfos> TextureInfoMap;

	typedef std::map<String, String> TexNameAndBrushMap;

    /** ���ڱ��浱ǰ��ˢ�е���Ϣ
    */
    class TerrainPaintInfoContainer
    {
    public:

        TerrainPaintInfoContainer();
        ~TerrainPaintInfoContainer();

        void addTextureInfo( TextureInfo &info );

        const TextureInfoMap &getTextureInfoMap(void) const
        {
            return mTextureInfoMap;
        }

        void clearTextureInfos(void)
        {
            mTextureInfoMap.clear();
        }

		void addTexNameAndBrushMapInfo( const String &texName, const String &brushName );

		const String &findBrushNameByTexName( const String &texName ) const;

		const TextureInfo &findTextureInfoByTexName( const String &texName );

    public:

        /// ��������ö���������ַ����Ķ�Ӧ��
        typedef std::map<String, TexType> StringTexTypeMap;
        StringTexTypeMap mStringTexTypeMap;
		typedef std::map<String, ExtraTexType> StringExtraTexTypeMap;
		StringExtraTexTypeMap mStringExtraTexTypeMap;

    protected:

        bool isTextureInfoOk( const TextureInfo &info );

    protected:

        TextureInfoMap mTextureInfoMap;
		TexNameAndBrushMap mTexNameAndBrushMap;
    };
}

#endif