/********************************************************************
filename:   TerrainTypeInfos.h

purpose:    ���ڱ��棬��ȡ�ر������������Ϣ���ݵأ����صȣ������ڿͻ�
            �˽��и����߼�����
*********************************************************************/

#ifndef _TERRAINTYPEINFO_H_
#define _TERRAINTYPEINFO_H_

#include "FairyPrerequisites.h"

#include <OgreMath.h>

namespace Fairy {
    
    class TerrainData;

    class TerrainTypeInfos
    {
    public:

        /// ���ֵ������͵ı�־λ
        enum TerrainTypeFlag
        {
            TerrainType_NORMAL          = 1 << 0,   // ��ͨ���Σ�������Ч
            TerrainType_GRASS           = 1 << 1,   // �ݵ�
            TerrainType_SOIL            = 1 << 2,   // ����
            TerrainType_SNOW            = 1 << 3,   // ѩ��
            TerrainType_LIQUID           = 1 << 4,   // ˮ
            TerrainType_STONE           = 1 << 5,
        };  

        typedef uchar TerrainType;

        struct TerrainTypeInfo
        {
            TerrainTypeInfo() :
            mTerrainType(TerrainType_NORMAL),
                mExtraData(Ogre::Math::POS_INFINITY)
            {

            }

            TerrainTypeInfo(TerrainType type, float extraData) :
            mTerrainType(type),
            mExtraData(extraData)
            {

            }

            /// ��������
            TerrainType mTerrainType;
            /// ������Ϣ���������ڱ���߶���Ϣ
            float mExtraData;
        };
        typedef std::vector<TerrainTypeInfo> TerrainTypeInfoArray;

    public:

        TerrainTypeInfos(void);

        /** ���õ��θ��������Ϣ
        @param x ���θ�x����
        @param z ���θ�z����
        @param type ������Ϣ
        @param extraData ������Ϣ
        */
        void setTerrainTypeInfo(int x, int z, TerrainType type, float extraData = Ogre::Math::POS_INFINITY);

        /** ��ȡ���ε�������Ϣ
        @param x ���θ�x����
        @param z ���θ�z����
        */
        const TerrainTypeInfo& getTerrainTypeInfo(int x, int z);

        /** ��ȡ���ε�������Ϣ
        @param x ����x����
        @param z ����z����
        */
        const TerrainTypeInfo& getTerrainTypeInfo(float x, float z);

        /** ���µ�ǰ�����ĵ���������Ϣ
        @remarks �ڵ����������֮ǰ�������setTerrainData�ѵ�ǰ�����������ý�ȥ
        */
        void updateTerrainTypeInfos(void);

        /** ����������Ϣ�����ļ�
        */
        void parseTerrainTypeConfigFile(const Ogre::String& filename);

        /// ��������ı��ļ�
        void outputDebugText(const Ogre::String& filename);

        /// ���õ�ǰ�ĵ�������
        void setTerrainData(TerrainData* terrainData)
        {
            mTerrainData = terrainData;
        }

        void getEffectInfo(float x, float z, const Ogre::String& templateName,Ogre::String& effectName, float& height);

    protected:

        TerrainTypeInfoArray mTerrainTypeInfos;

        TerrainData* mTerrainData;

        typedef std::map<String, TerrainType> TerrainTypeMap;
        TerrainTypeMap mTerrainTypeMap;

        typedef std::map<String, TerrainType> TextureNameTerrainTypeMap;
        TextureNameTerrainTypeMap mTextureNameTerrainTypeMap;

        struct EffectTemplate
        {
           Ogre::String mEffectName;
           Ogre::String mTemplateName;
        };

        typedef std::list<EffectTemplate> EffectTemplateList;
        typedef std::map<TerrainType, EffectTemplateList> TerrainTypeEffectMap;
        TerrainTypeEffectMap mTerrainTypeEffectMap;

    protected:

        /// ���ݵ������ͻ�ȡ��Ч����
        const Ogre::String _getEffectName(TerrainType type, const Ogre::String& templateName);
    };
}

#endif