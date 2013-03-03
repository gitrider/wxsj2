#include "SimplePaintAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"
#include "BrushShape.h"

#include <OgreStringConverter.h>

namespace Fairy {
    //-----------------------------------------------------------------------
    SimplePaintAction::SimplePaintAction(SceneManipulator* sceneManipulator)
        : PaintAction(sceneManipulator),
        mbTexNormalSize(true)
    {
        mCurrentGrids = new GridSelection(getTerrain(),sceneManipulator);
        mHintModified = new GridSelection(getTerrain(),sceneManipulator);
        mModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);

        mbUseRandomOrientation = false;
    }
    //-----------------------------------------------------------------------
    SimplePaintAction::~SimplePaintAction()
    {
        delete mCurrentGrids;
        delete mHintModified;
        delete mModifiedGrids;
    }
    //-----------------------------------------------------------------------
    const String& SimplePaintAction::getName(void) const
    {
        static const String name = "SimplePaintAction";
        return name;
    }
    //-----------------------------------------------------------------------
	void SimplePaintAction::setParameter(const String& name, const String& value)
	{
        /** ��ǿ��SimplePaintAction�Ĺ��ܣ����������õ���������з��Զ�ƴ�ӵ�paint��Ҳ����
            ��һ������ͬ���ļ����£�����Ϊ��ˢ
            ���ֻѡ��һ�������Ϳ��Ե�������ķ������ѡ��һ����������ķ����������
            ���ڵ�����action��˳��Ϊ
            GetSceneManipulator()->_getAction("SimplePaintAction");

            action->setParameter( "%TextureName", "InputBegin" );
            action->setParameter( "%TextureName", �������� );

            ������Ҫ�����÷���
            action->setParameter( "%OrientationType"��������)

        */
		if (Ogre::StringUtil::startsWith(name, "%", false))
		{
			Ogre::String parameter = name.substr(1);

			if ( parameter == "TextureName" )
            {
                if ( value == "InputBegin" )
                {
                    mPixmapArray.clear();
                }
                else
                {
                    /* ����������Ϣ�ַ���
                        0 -- ��������
                        1 -- height
                        2 -- width 
                        3 -- left 
                        4 -- top
                        5 -- Դ����ĸ߶�
                        6 -- Դ����Ŀ��
						7 -- ���������
						8 -- ����ķ���

                        �м���"|"����
                    */

                    Ogre::StringVector infoVector = Ogre::StringUtil::split(value, "|");

                    assert (infoVector.size() == 8);

                    PaintPixmap pixmap(infoVector[0]);
                    
                    // �������������
                    uint height = Ogre::StringConverter::parseUnsignedInt(infoVector[1]);
                    uint width = Ogre::StringConverter::parseUnsignedInt(infoVector[2]);

                    mXBrushSize = width;
                    mZBrushSize = height;
                   
                    uint leftcorner = Ogre::StringConverter::parseUnsignedInt(infoVector[3]);
                    uint topcorner = Ogre::StringConverter::parseUnsignedInt(infoVector[4]);

                    uint ownerTexHeight = Ogre::StringConverter::parseUnsignedInt(infoVector[5]);
                    uint ownerTexWidht = Ogre::StringConverter::parseUnsignedInt(infoVector[6]);

                    pixmap.left = (float)(TexTileSize*leftcorner) / (float)ownerTexWidht;
                    pixmap.top = (float)(TexTileSize*topcorner) / (float)ownerTexHeight;
                    pixmap.bottom = (float)(TexTileSize*(topcorner+height)) / (float)ownerTexHeight;
                    pixmap.right = (float)(TexTileSize*(leftcorner+width)) / (float)ownerTexWidht;		

					uint rotateType = Ogre::StringConverter::parseUnsignedInt(infoVector[7]);

					enum 
					{
						Left,
						Top,
						Right,
						Bottom,
					};

					enum
					{
						LeftTop,
						RightBottom = 3,
					};

					// һ���������������ڷ�����ҳ���ת��������������Ķ�Ӧ��ϵ
					static const int cornerIndexMap[4][4][2] =
					{
						Left,	Top,
						Right,	Top,
						Left,	Bottom,
						Right,	Bottom,

						Right,	Top,
						Left,	Top,
						Right,	Bottom,
						Left,	Bottom,

						Left,	Bottom,
						Right,	Bottom,
						Left,	Top,
						Right,	Top,

						Right,	Bottom,
						Left,	Bottom,
						Right,	Top,
						Left,	Top,		
					};

					float texCoord[4] = {	pixmap.left, pixmap.top, pixmap.right, pixmap.bottom	};

					// ���һ�������0��ʾ����u���꣬1��ʾ����v����
					pixmap.left = texCoord[ cornerIndexMap[ rotateType ][ LeftTop ][0] ];	
					pixmap.top = texCoord[ cornerIndexMap[ rotateType ][ LeftTop ][1] ];
					pixmap.right = texCoord[ cornerIndexMap[ rotateType ][ RightBottom ][0] ];
					pixmap.bottom = texCoord[ cornerIndexMap[ rotateType ][ RightBottom ][1] ];

                    mPixmapArray.push_back(pixmap);
                }
            }
            else if ( parameter == "TexNormalSize" )
            {
                if ( value == "true" )
                    mbTexNormalSize = true;
                else if ( value == "false" )
                    mbTexNormalSize = false;
            }          
			else if ( parameter == "OrientationType" )
			{
				setPixmapOrientationType( atoi(value.c_str()) );
			}
            else if ( parameter == "UseRandomOrientation" )
            {
                if ( value == "true" )
                    mbUseRandomOrientation = true;
                else if ( value == "false" )
                    mbUseRandomOrientation = false;
            }
		}
		else
		{
			Action::setParameter(name, value);
		}
	}
    //-----------------------------------------------------------------------
	String SimplePaintAction::getParameter(const String& name) const
	{
        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "IsInit" )
            {
                if ( mPixmapArray.size() > 0 )
                {
                    return "Yes";
                }                     
                else
                    return "No";
            }
             else
                return Action::getParameter(name);
        }
        else
            return Action::getParameter(name);
	}  
    //-----------------------------------------------------------------------
    void SimplePaintAction::_buildHitIndicator(const Point& pt)
    {
        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (!intersected)
        {
            mCurrentGrids->reset();
            return;
        }

        // �����ǰ��ѡ�õ�����Ĵ�С������64����ôҪ��������Ĵ�С�ͻ�ˢ�Ĵ�С������ѡ������
        if (!mbTexNormalSize)
        {
            int oldBrushXSize = getSceneManipulator()->getBrushXSize();
            int oldBrushZSize = getSceneManipulator()->getBrushZSize();

            // ��ʱ���û�ˢ��С
            getSceneManipulator()->setBrushXSize(oldBrushXSize*mXBrushSize);
            getSceneManipulator()->setBrushZSize(oldBrushZSize*mZBrushSize);
            getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);
            getSceneManipulator()->_buildSelection(mCurrentGrids, position.x, position.z);

            // ����������������û�ȥ
            getSceneManipulator()->setBrushXSize(oldBrushXSize);
            getSceneManipulator()->setBrushZSize(oldBrushZSize);
        }
        else
        {
            getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);
            getSceneManipulator()->_buildSelection(mCurrentGrids, position.x, position.z);
        }

        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_doPaint(GridSelection* modified)
    {
        // �����ǰ�������pixmapΪ0��ֱ�ӷ���
        if (mPixmapArray.empty())
        {
            return;
        }

        int texCount = mPixmapArray.size();

        int pixmapId = 0;
        int orientataionType = getPixmapOrientationType();
        int orientation = 0;
        String mCurrentTexName;

        const GridSelection::GridMap& grids = mCurrentGrids->getGrids();

        if (!grids.empty())
        {
            // ���ֻѡ��һ��������һ���ļм���ֻ��һ������
            if ( texCount == 1 )
            {
                // �����ǰ������Ϊ��ͨ��С����������������
                if ( mbTexNormalSize == true )
                {
                    setPixmap(mPixmapArray[0].textureName, mPixmapArray[0].left, mPixmapArray[0].top,
                        mPixmapArray[0].right, mPixmapArray[0].bottom);
                    pixmapId = getPixmapId();

                }
                // ����ֻ�õ��������ƣ���ѭ������������
                else
                {
                    mCurrentTexName = mPixmapArray[0].textureName;

                    orientation = 0;
                }
            }
          
            // �������һ������������Ҳ�����ȡ��һ�����������
            if ( texCount > 1 && mbTexNormalSize == false )
            {
				int index = static_cast<int>(Ogre::Math::RangeRandom(0,texCount));
				if (index == texCount)
				{
					index = 0;
				}
				assert (index < texCount);
                mCurrentTexName = mPixmapArray[ index ].textureName;
            }

			bool anyModified = false;

            // ��ȡ���Ͻǵ�����
            const GridSelection::Grid &firstGrid = grids.begin()->second;

            int minx = firstGrid.x;
            int minz = firstGrid.z;

            // �����֮���������������
            float xInc = (mPixmapArray[0].right- mPixmapArray[0].left) / mXBrushSize;
            float zInc = (mPixmapArray[0].bottom - mPixmapArray[0].top) / mZBrushSize;

            for (GridSelection::GridMap::const_iterator it = grids.begin(); it != grids.end(); ++it)
            {
                // ��������������1����ʹ�����������ʹ���������
                if ( texCount > 1 )
                {
                    if ( mbTexNormalSize )
                    {
                  //      String texName;

                        int index = 0;
                        if ( modified != mHintModified )
                        {
                            index = static_cast<int>(Ogre::Math::RangeRandom(0,texCount));
							if (index == texCount)
							{
								index = 0;
							}
							assert (index < texCount);
                        //    texName = mPixmapArray[index].textureName;
                           // orientation = static_cast<int>(Ogre::Math::RangeRandom(0,7));
                            orientation = 0;
                        }
                        /*else
                        {
                            texName = mPixmapArray[0].textureName;
                        }*/

                        // �����id������pixmap
                        setPixmap(mPixmapArray[index].textureName, mPixmapArray[index].left, mPixmapArray[index].top,
                            mPixmapArray[index].right, mPixmapArray[index].bottom);
                        pixmapId = getPixmapId(); 


                    }
                }
                else
                {
                    if ( mbTexNormalSize )
                    {
                        if ( mbUseRandomOrientation )
                        {
                            if (modified != mHintModified)
							{
								const int oriTypeCount = 8;
                                orientation = static_cast<int>(Ogre::Math::RangeRandom(0,oriTypeCount));
								if (orientation == oriTypeCount)
								{
									orientation = 0;
								}
								assert ( orientation < oriTypeCount);
							}
                            else
                                orientation = 0; 

                        }
                        else
                            orientation = orientataionType;
                    }
                }
                

                const GridSelection::Grid& grid = it->second;

                if ( mbTexNormalSize == false )
                {
                    int x = grid.x;
                    int z = grid.z;

                    float left = (x-minx)%mXBrushSize*xInc;
                    float top = (z-minz)%mZBrushSize*zInc;
                   
                    setPixmap( mPixmapArray[0].textureName, mPixmapArray[0].left + left, mPixmapArray[0].top + top,
                        mPixmapArray[0].left + left + xInc, mPixmapArray[0].top + top + zInc );
                    pixmapId = getPixmapId();     

                }

                TerrainData::GridInfo info = grid.info;

                const bool isTransparent = getPixmapTransparent();

                if (isTransparent)
                {
                    if (!info.layers[0].pixmapId)
                        continue;

					/** �����и����صĴ���
						û�жϵ�ǰ������������õ�͸�������Ƿ�����ͬһ���
						���ͬһ��ģ��Ͳ�Ӧ��Ϳ�ϸ�͸��������Ϊ�ӻ����Ͽ��ܿ�����Ч����
						���һ����Ч���ϵ�һ������⣬��Ϊ����˫��������Ⱦ�ķ�������Ⱦ�ø�����
						������ʱ�������������������жϣ��ж��Ƿ�Ϊͬ������
					*/
					TerrainData *terrainData = getTerrainData();
					assert (terrainData);

					// �Ƚ����²�Ļ�ˢ���ƣ����һ����˵����ͬһ�黭ˢ
					String layerOneTexName = terrainData->mTextures[ terrainData->mPixmaps[info.layers[0].pixmapId-1].textureId ];
					assert ( !layerOneTexName.empty() );

					String layerOneBrushName = 
						mSceneManipulator->getTerrainPaintInfoContainer()->findBrushNameByTexName(layerOneTexName);				

					String layerTwoTexName = terrainData->mTextures[ terrainData->mPixmaps[pixmapId-1].textureId ];
					assert ( !layerTwoTexName.empty() );
						
					String layerTwoBrushName = 
						mSceneManipulator->getTerrainPaintInfoContainer()->findBrushNameByTexName(layerTwoTexName);

					if ( layerOneBrushName == layerTwoBrushName )
						continue;

                    info.layers[1].pixmapId = pixmapId;

                    info.layers[1].orientation = orientation;
                }
                else
                {
                    info.layers[0].pixmapId = pixmapId;
                    info.layers[0].orientation = orientation;
                    /*if (info.layers[1].pixmapId > 0)
                    {*/
                        info.layers[1].pixmapId = 0;
                        info.layers[1].orientation = 0;
                 //  }
                }

                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);
            }

            if (anyModified)
            {
                mCurrentGrids->notifyModified();
            }
        }

    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onActive(bool active)
    {
        if (!active)
        {
            mHintModified->apply();
            mHintModified->reset();
        }
    }
	void SimplePaintAction::resetHintModified(bool bApply)
	{
		if (bApply)
		{
			mHintModified->apply();
		}		
		mHintModified->reset();
	}

	void SimplePaintAction::resetCurrentGrids()
	{
		mCurrentGrids->reset();
	}

	GridSelection* SimplePaintAction::getHintModified()
	{
		return mHintModified;
	}

    //-----------------------------------------------------------------------
    void SimplePaintAction::_onMove(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
		resetCurrentGrids();
        _buildHitIndicator(pt);
        _doPaint(mHintModified);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onBegin(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onDrag(const Point& pt)
    {
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onEnd(const Point& pt, bool canceled)
    {
        /*if (canceled)
        {
            mModifiedGrids->apply();
        }*/
        doFinish(mModifiedGrids,canceled);
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
    }
}
