#include "AutoTexPaintAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

#include <OgreStringConverter.h>
#include <OgreLogManager.h>
#include <OgreImage.h>

#include "TerrainPaintInfoContainer.h"

namespace Fairy {

    //-----------------------------------------------------------------------
    AutoTexPaintAction::AutoTexPaintAction(SceneManipulator* sceneManipulator)
        : PaintAction(sceneManipulator)
        , mCurrentBrush(NULL),mInitCurrentBrush(false),
        mCurrentInnerArray(NULL), 

		mCurrentInnerCornerArray(NULL), 
		mCurrentLTInnerCornerArray(NULL), 
		mCurrentRTInnerCornerArray(NULL), 
		mCurrentLBInnerCornerArray(NULL), 
		mCurrentRBInnerCornerArray(NULL), 

		mCurrentOuterCornerArray(NULL),
		mCurrentLTOuterCornerArray(NULL),
		mCurrentRTOuterCornerArray(NULL),
		mCurrentLBOuterCornerArray(NULL),
		mCurrentRBOuterCornerArray(NULL),

        mCurrentEdgeArray(NULL), 
		mCurrentLEdgeArray(NULL), 
		mCurrentREdgeArray(NULL), 
		mCurrentTEdgeArray(NULL), 
		mCurrentBEdgeArray(NULL), 

		mCurrentDuijiaoArray(NULL),
		bDisableRandom(false)
    {
        mCurrentInnerGrids = new GridSelection(getTerrain(),sceneManipulator);
        mCurrentOuterGrids = new GridSelection(getTerrain(),sceneManipulator);

        mHintModified = new GridSelection(getTerrain(),sceneManipulator);
        mModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);
    }
    //-----------------------------------------------------------------------
    AutoTexPaintAction::~AutoTexPaintAction()
    {
        if ( mCurrentInnerGrids)
        {
            delete mCurrentInnerGrids;
            mCurrentInnerGrids = NULL;
        }

        if ( mCurrentOuterGrids)
        {
            delete mCurrentOuterGrids;
            mCurrentOuterGrids = NULL;
        }

        if ( mHintModified)
        {
            delete mHintModified;
            mHintModified = NULL;
        }

        if ( mModifiedGrids)
        {
            delete mModifiedGrids;
            mModifiedGrids = NULL;
        }

        for ( BrushMap::iterator i=mBrushMap.begin(); i!=mBrushMap.end(); ++i )
        {
            if ( i->second )
            {
                delete i->second;
                i->second = NULL;
            }
        }
        mBrushMap.clear();
    }
    //-----------------------------------------------------------------------
    const String& AutoTexPaintAction::getName(void) const
    {
        static const String name = "AutoTexPaintAction";
        return name;
    }

    void AutoTexPaintAction::setParameter(const String& name, const String& value)
    {
        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "GroupName" )
            {
                mCurrentBrushName = value;

                // ���BrushMap��û�����group����������Ϊ֮ǰ��û���û�ѡ���������½�һ�������뵽map��
                if ( mBrushMap.find(value) == mBrushMap.end() )
                {
                    mInitCurrentBrush = false;

                    mCurrentBrush = new Brush;
                    mBrushMap.insert( BrushMap::value_type( value, mCurrentBrush ) );    
                }
                else 
                {
                    // ����еĻ���ֱ�����õ�ǰgroupΪָ��group
                    mCurrentBrush = mBrushMap.find(value)->second;
                }
         
                // ��ȡ��ǰ���������������ָ��
                mCurrentInnerCornerArray   = &(mCurrentBrush->innerCornerTexArray);
				mCurrentLTInnerCornerArray = &(mCurrentBrush->ltInnerCornerTexArray);
				mCurrentRTInnerCornerArray = &(mCurrentBrush->rtInnerCornerTexArray);
				mCurrentLBInnerCornerArray = &(mCurrentBrush->lbInnerCornerTexArray);
				mCurrentRBInnerCornerArray = &(mCurrentBrush->rbInnerCornerTexArray);

                mCurrentOuterCornerArray   = &(mCurrentBrush->outerCornerTexArray);
				mCurrentLTOuterCornerArray = &(mCurrentBrush->ltOuterCornerTexArray);
				mCurrentRTOuterCornerArray = &(mCurrentBrush->rtOuterCornerTexArray);
				mCurrentLBOuterCornerArray = &(mCurrentBrush->lbOuterCornerTexArray);
				mCurrentRBOuterCornerArray = &(mCurrentBrush->rbOuterCornerTexArray);

                mCurrentInnerArray         = &(mCurrentBrush->innerTexArray);

                mCurrentEdgeArray         = &(mCurrentBrush->edgeTexArray);
				mCurrentLEdgeArray        = &(mCurrentBrush->lEdgeTexArray);
				mCurrentREdgeArray        = &(mCurrentBrush->rEdgeTexArray);
				mCurrentTEdgeArray        = &(mCurrentBrush->tEdgeTexArray);
				mCurrentBEdgeArray        = &(mCurrentBrush->bEdgeTexArray);

                mCurrentDuijiaoArray     = &(mCurrentBrush->duijiaoTexArray);

            }
            else if ( parameter == "TextureName" )
            {
                // �����һ��group�л�û��ʼ����ɣ��͸����ļ������ж�������������
                if ( mInitCurrentBrush == false )
                {

                    if ( value == "InputFinish" )
                    {
                        // �����һ���������������еĴ�СΪ0��˵��ȱ��ĳ���������͵�����
                        if ( ( mCurrentBrush->innerCornerTexArray.size() == 0 &&
							mCurrentBrush->ltInnerCornerTexArray.size() == 0 &&
							mCurrentBrush->rtInnerCornerTexArray.size() == 0 &&
							mCurrentBrush->lbInnerCornerTexArray.size() == 0 &&
							mCurrentBrush->rbInnerCornerTexArray.size() == 0 ) ||

                          ( mCurrentBrush->outerCornerTexArray.size() == 0 &&
							mCurrentBrush->ltOuterCornerTexArray.size() == 0 &&
							mCurrentBrush->rtOuterCornerTexArray.size() == 0 &&
							mCurrentBrush->lbOuterCornerTexArray.size() == 0 &&
							mCurrentBrush->rbOuterCornerTexArray.size() == 0 ) ||

                            mCurrentBrush->innerTexArray.size() == 0 ||

                            ( mCurrentBrush->edgeTexArray.size() == 0 &&
							mCurrentBrush->lEdgeTexArray.size() == 0 &&
							mCurrentBrush->rEdgeTexArray.size() == 0 &&
							mCurrentBrush->tEdgeTexArray.size() == 0 &&
							mCurrentBrush->bEdgeTexArray.size() == 0 ) ||

                            mCurrentBrush->duijiaoTexArray.size() == 0 )
                        {
                            // �Ѹս��õ�brush��ɾ����
                            if ( mCurrentBrush )
                            {
                                delete mCurrentBrush;
                                mCurrentBrush = NULL;
                            }

                            BrushMap::iterator i = mBrushMap.find(mCurrentBrushName);

                            if ( i != mBrushMap.end() )
                                mBrushMap.erase(i);

                            mInitCurrentBrush = false;

							mCurrentInnerCornerArray = NULL;
							mCurrentLTInnerCornerArray = NULL;
							mCurrentRTInnerCornerArray = NULL;
							mCurrentLBInnerCornerArray = NULL;
							mCurrentRBInnerCornerArray = NULL;

							mCurrentOuterCornerArray = NULL;
							mCurrentLTOuterCornerArray = NULL;
							mCurrentRTOuterCornerArray = NULL;
							mCurrentLBOuterCornerArray = NULL;
							mCurrentRBOuterCornerArray = NULL;

							mCurrentInnerArray       = NULL;

							mCurrentEdgeArray        = NULL;
							mCurrentLEdgeArray		 = NULL; 
							mCurrentREdgeArray		 = NULL; 
							mCurrentTEdgeArray		 = NULL; 
							mCurrentBEdgeArray		 = NULL; 

							mCurrentDuijiaoArray     = NULL;
                        }
                        else
                            mInitCurrentBrush = true;
                    }
                    else
                    {
						assert ( mCurrentBrush == mBrushMap.find(mCurrentBrushName)->second );
                        const Fairy::TextureInfo &currentPaintInfo = getSceneManipulator()->getTextureInfo(mCurrentBrushName, value);

                        PaintPixmap pixmap(currentPaintInfo.ownerTextureName);

                        Ogre::Image *ownerImage = getSceneManipulator()->getPreviewImage(currentPaintInfo.ownerTextureName);

                        assert (ownerImage);

                        // �������������               
                        uint ownerTexHeight = ownerImage->getHeight();
                        uint ownerTexWidht = ownerImage->getWidth();

						float texCoordIncX = 0.5f / (float)ownerTexWidht;
						float texCoordIncZ = 0.5f / (float)ownerTexHeight;

						pixmap.left = (float)(TexTileSize*currentPaintInfo.leftCorner) / (float)ownerTexWidht + texCoordIncX;
						pixmap.top = (float)(TexTileSize*currentPaintInfo.topCorner) / (float)ownerTexHeight + texCoordIncZ;
						pixmap.bottom = (float)(TexTileSize*(currentPaintInfo.topCorner + currentPaintInfo.height)) / (float)ownerTexHeight
							- texCoordIncZ;
						pixmap.right = (float)(TexTileSize*(currentPaintInfo.leftCorner + currentPaintInfo.width)) / (float)ownerTexWidht
							- texCoordIncX;

						assert ( (pixmap.left < pixmap.right) && (pixmap.top < pixmap.bottom) );

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

						// һ���������������ڷ�����ҳ���ת��������������Ķ�Ӧ��ϵ����
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
						pixmap.left = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ LeftTop ][0] ];	
						pixmap.top = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ LeftTop ][1] ];
						pixmap.right = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ RightBottom ][0] ];
						pixmap.bottom = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ RightBottom ][1] ];	

                        assert ( pixmap.left >= 0.0f && pixmap.left <= 1.0f );
                        assert ( pixmap.top >= 0.0f && pixmap.top <= 1.0f );
                        assert ( pixmap.right >= 0.0f && pixmap.right <= 1.0f );
                        assert ( pixmap.bottom >= 0.0f && pixmap.bottom <= 1.0f );

                        if ( currentPaintInfo.textureType == EdgeTex )
                        {
							if ( currentPaintInfo.extraTexType == EOther )
								mCurrentBrush->edgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LE )
								mCurrentBrush->lEdgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RE )
								mCurrentBrush->rEdgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == TE )
								mCurrentBrush->tEdgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == BE )
								mCurrentBrush->bEdgeTexArray.push_back(pixmap);

                            mInitCurrentBrush = false;
                        }						
                        else if ( currentPaintInfo.textureType == InnerCornerTex )
                        {
							if ( currentPaintInfo.extraTexType == EOther )
								mCurrentBrush->innerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LT )
								mCurrentBrush->ltInnerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RT )
								mCurrentBrush->rtInnerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LB )
								mCurrentBrush->lbInnerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RB )
								mCurrentBrush->rbInnerCornerTexArray.push_back(pixmap);

                            mInitCurrentBrush = false;
                        }						
                        else if ( currentPaintInfo.textureType == OuterCornerTex )
                        {
							if ( currentPaintInfo.extraTexType == EOther )
								mCurrentBrush->outerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LT )
								mCurrentBrush->ltOuterCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RT )
								mCurrentBrush->rtOuterCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LB )
								mCurrentBrush->lbOuterCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RB )
								mCurrentBrush->rbOuterCornerTexArray.push_back(pixmap);

                            mInitCurrentBrush = false;
                        }						
                        else if ( currentPaintInfo.textureType == DuijiaoTex )
                        {
                            mCurrentBrush->duijiaoTexArray.push_back(pixmap);
                            mInitCurrentBrush = false;
                        }                    
                        else if ( currentPaintInfo.textureType == InnerTex )
                        {
                            // ����ڲ���������
                            mCurrentBrush->innerTexArray.push_back(pixmap);
                            mInitCurrentBrush = false;
                        }
						else
						{
							OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
								"The texture type is wrong " + currentPaintInfo.brushName + " " + currentPaintInfo.ownerTextureName,
								"AutoTexPaintAction::setParameter");
						}
                    }                    
                }
            }else if ( parameter == "Random_Texture" )
            {
				if (value == "false")
					bDisableRandom = false;
				else
					bDisableRandom = true;
            }
            
        }
        else
        {
            Action::setParameter(name, value);
        }
    }
    //-----------------------------------------------------------------------
    String AutoTexPaintAction::getParameter(const String& name) const
    {

        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "IsInit" )
            {
                if ( mInitCurrentBrush )
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
    void AutoTexPaintAction::buildBoxEdgeSelection(GridSelection& selection, Real xcentre, Real zcentre, int xsize, int zsize)
    {
        assert(0 < xsize && 0 < zsize);

        selection.reset();
        Real cx = (xsize-1) * selection.getTerrainData()->mScale.x / 2;
        Real cz = (zsize-1) * selection.getTerrainData()->mScale.z / 2;
        Real xbais = xcentre - cx;
        Real zbais = zcentre - cz;

        // �õ�ʵ����������Ͻǵ����꣨������û�г�����Ե��������Ͻǳ����˱�Ե����ô���ֵ�ͻ��Ǹ��ģ�
        std::pair<int, int> bais = selection.getTerrainData()->getGrid(xbais, zbais);
        for (int z = -1; z < zsize+1; ++z)
        {
            for (int x = -1; x < xsize+1; ++x)
            {
                if ( z==-1 || z==zsize || x==-1 || x==xsize )
                    selection.add(x+bais.first, z+bais.second);
            }
        }  

        // �õ���ǰ��ѡ��box��������Ͻǣ�������û�г�����Ե��������Ͻǳ����˱�Ե����ô���ֵ�ͻ��Ǹ��ģ�
        mCurrentBoxSelectionMinX = bais.first-1;
        mCurrentBoxSelectionMinZ = bais.second-1;
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_buildHitIndicator(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (!intersected)
        {
            mCurrentInnerGrids->reset();
            mCurrentOuterGrids->reset();
            //hide();
            return;
        }

        // ѡȡ��ͬ�Ļ�ˢ��������ͬ��ѡ������
        getSceneManipulator()->_buildSelection(mCurrentInnerGrids, position.x, position.z);

        buildBoxEdgeSelection(*mCurrentOuterGrids,position.x,position.z,getSceneManipulator()->getBrushXSize(), getSceneManipulator()->getBrushZSize());
        //refresh();
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_doPaint(GridSelection* modified)
    {
        // �����ѡ���������ʼ��δ��ɣ����������
        if ( mInitCurrentBrush == false )
            return;

        bool anyModified = false;
   
        int pixmapId = 0;
        int orientationType = 0;

        int innercornerSize = mCurrentInnerCornerArray->size();
		int ltInnerCornerSize = mCurrentLTInnerCornerArray->size();
		int rtInnerCornerSize = mCurrentRTInnerCornerArray->size();
		int lbInnerCornerSize = mCurrentLBInnerCornerArray->size();
		int rbInnerCornerSize = mCurrentRBInnerCornerArray->size();

        int outercornerSize = mCurrentOuterCornerArray->size();
		int ltOutercornerSize = mCurrentLTOuterCornerArray->size();
		int rtOutercornerSize = mCurrentRTOuterCornerArray->size();
		int lbOutercornerSize = mCurrentLBOuterCornerArray->size();
		int rbOutercornerSize = mCurrentRBOuterCornerArray->size();

        int innerSize = mCurrentInnerArray->size();

        int edgeSize = mCurrentEdgeArray->size();
		int lEdgeSize = mCurrentLEdgeArray->size();
		int rEdgeSize = mCurrentREdgeArray->size();
		int tEdgeSize = mCurrentTEdgeArray->size();
		int bEdgeSize = mCurrentBEdgeArray->size();

        int duijiaoSize = mCurrentDuijiaoArray->size();

        // FIX IT ���µ������Զ�ƴ�ӵĴ��붼��Ӳ������if�ж���д�ģ����Ͽ��ܻ�ĳ���
        // һ��ƴ�ӹ�ϵ������Ĵ���

		// added by JiChong( �������������, ʵ�ڿ�������, sorry:) )
		// ����ʹ��ƴ�ӹ�ϵ��������ע��ħ������ĵ�����ͼ����ڷŷ�ʽΪ
		// 11 00 00 00 11 11 11 11     3  0  0  0
		// 11 01 10 11 11 11 11 11     3  1  2  3

		// 01 01 01 01 11 11 11 11     1  1  1  1
		// 00 01 10 11 11 11 11 11     0  1  2  3

		// 10 10 10 10 11 11 11 11     2  2  2  2
		// 00 01 10 11 11 11 11 11     0  1  2  3

		// 11 11 11 11 11 11 11 11     3  3  3  3
		// 00 01 10 11 11 11 11 11     0  1  2  3

		// ע��߽Ǵ���ͼС��ı����������С���ڴ�ͼ�е�λ��һ��(����Ϊy���꣬����Ϊx����)
		// ������ֻҪɨ��ÿ��tile���õ�����ͼ������ĳ�ֵ������ĸ��ǵ�ռ�����(ռ��Ϊ1, ��ռ��Ϊ0),���ɵõ���ȷ
		// �ı��룬Ȼ���Ըñ����ҵ���Ӧ��ͼ�е�С��,��Ȼ�����ǵ�����������Ҳ�����Ա�����������ͼ�ļ�
		// ps: ����̫��ط���������дif�ķ�ʽ�����Զ�ƴ�ӣ�bug����û�а취�����ӵ��������ʵ�Զ�ƴ�ӵĴ����Ƿǳ���
		// ���һ�㣬��Ϊ�ĽǶ�Ϊ0����ͼ��û���õģ�����ħ��III��ĵ�һ��С��ͼ��Ҳ���Ǳ�����0000����ͼ���õ���1111
		// ������������Ԥ��ͼ�������ǲ��뵽1111ͼ������ϵ�(���ߵĿ��������ҪСЩ)

        // �����ڲ�����
        const GridSelection::GridMap& innerGrids = mCurrentInnerGrids->getGrids();

        if (!innerGrids.empty())
        {
                    
            for (GridSelection::GridMap::const_iterator it = innerGrids.begin(); it != innerGrids.end(); ++it)
            {               
                // ����갴��ʱ�����ѡ������
                PaintPixmap pixmap;  

				assert ( false == mCurrentInnerArray->empty() );
                if ( modified != mHintModified )
                {
					int index = static_cast<int>(Ogre::Math::RangeRandom(0,innerSize));
					if (index == innerSize)
					{
						index = 0;
					}

					if(bDisableRandom)
					    index = 0;
						assert (index < innerSize);
                    pixmap = (*mCurrentInnerArray)[ index ];
                }
                else
                {
                    pixmap = (*mCurrentInnerArray)[0];
                }

               // �����id������pixmap
                setPixmap(pixmap);
                pixmapId = getPixmapId();

                // ��ȡ����ǰҪ�����������һ��С�����Ϣ
                const GridSelection::Grid &grid = it->second;
                TerrainData::GridInfo info = grid.info;
               
                info.layers[0].pixmapId = pixmapId;           

                // ʵ�ĵ�������Ҫ������ת����ԭ����
                info.layers[0].orientation = 0;            

                info.layers[1].pixmapId = 0;
                info.layers[1].orientation = 0;
           
                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);
            }
        }

        const GridSelection::GridMap &outerGrids = mCurrentOuterGrids->getGrids();

        if ( !outerGrids.empty() )
        { 
            TexType type = Other;

            // �������ǰselection�����½ǵ�����
            int maxx = mCurrentBoxSelectionMinX + getSceneManipulator()->getBrushXSize()+1;
            int maxz = mCurrentBoxSelectionMinZ + getSceneManipulator()->getBrushZSize()+1;

            for (GridSelection::GridMap::const_iterator it = outerGrids.begin(); it != outerGrids.end(); ++it)
            {     
                const GridSelection::Grid &grid = it->second;
                TerrainData::GridInfo info = grid.info;

                int x = grid.x;
                int z = grid.z;

                // �����0�������Ϊ����ģ��϶�Ϊ�ڲ�������������
                if ( isTexInCurrentGroup( info.layers[0].pixmapId ) )
                    continue;
     
                // ��ȡ��ǰ���1�����������
                TexType currentTexType = findTexTypeByPixmapId(info.layers[1].pixmapId);

                // �������Ͻ�
                if ( x == mCurrentBoxSelectionMinX && z == mCurrentBoxSelectionMinZ )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // ��������ұ�Ե
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;
                            orientationType = 1;
                        }
                        // ��������±�Ե
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 2;
                        }
                        // ����������Ե
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            continue;
                        }
                        // ��������ϱ�Ե
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // ����������Ͻǣ��ͱ���ϱ�Ե
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 5;
                        }
                        // ����������½ǣ�������Ե
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = EdgeTex;
                            orientationType = 0;
                        }  
                        // ����������½ǣ���ɶԽ�
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = DuijiaoTex;
                            orientationType = 1;
                        }
                        // �������Ͻǣ�Ҳ��������
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            continue;
                        }
                    }                    
                    // ������Ϸ���Եģ�����������������ڽ�Ϊ��Եģ��ڽǣ�ֱ�����������򣬾ͱ�ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3 ||
							 info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // ������������ǣ��ͽ��������������ǵĻ���
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 0;
                    }
                }
                // �������Ͻ�
                else if ( x == maxx && z == mCurrentBoxSelectionMinZ )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // ����������Ե
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;
                            orientationType = 0;
                        }
                        // ��������±�Ե
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 3;
                        }
                        // ��������ұ�Ե
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            continue;
                        }
                        // ��������ϱ�Ե
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������Ͻǣ�����ϱ�Ե
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = EdgeTex;
                            orientationType = 5;
                        }
                        // �������½ǣ�����ұ�Ե
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = EdgeTex;
                            orientationType = 1;
                        }
                        // ����������½ǣ���ɶԽ�
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = DuijiaoTex;
                            orientationType = 0;
                        }
                        // �������Ͻǣ�Ҳ��������
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            continue;
                        }
                    }                    
                    // ������Ϸ���Եģ�����������������ڽ�Ϊ��Եģ��ڽǣ�ֱ�����������򣬾ͱ�ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
							type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 2 
							 || info.layers[1].orientation == 4 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 1;
                        }
                        else
                            continue;
                    }
                    // ������������ǣ��ͽ��������������ǵĻ���
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 1;
                    }
                }
                // �������½�
                else if ( x == mCurrentBoxSelectionMinX && z == maxz )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // ��������ұ�Ե
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;
                            orientationType = 3;
                        }
                        // ��������ϱ�Ե
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 0;
                        }
                        // ����������Ե
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            continue;
                        }
                        // ��������±�Ե
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������½ǣ�����±�Ե
                        if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = EdgeTex;
                            orientationType = 4;
                        }
                        // �������Ͻǣ�������Ե
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = EdgeTex;
                            orientationType = 0;
                        }
                        // ����������Ͻǣ��ͱ�ɶԽ�
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = DuijiaoTex;
                            orientationType = 0;
                        }
                        // �������½ǣ�Ҳ��������
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            continue;
                        }
                    }                    
                    // ������Ϸ���Եģ�����������������ڽ�Ϊ��Եģ��ڽǣ�ֱ�����������򣬾ͱ�ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 2 
							 || info.layers[1].orientation == 4  || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 2;
                        }
                        else
                            continue;
                    }
                    // ������������ǣ��ͽ��������������ǵĻ���
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 2;
                    }
                }
                // �������½�
                else if ( x == maxx && z == maxz )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // ����������Ե
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;
                            orientationType = 2;
                        }
                        // ��������ϱ�Ե
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 1;
                        }
                        // ��������ұ�Ե
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            continue;
                        }
                        // ��������±�Ե
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������½ǣ�����±�Ե
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = EdgeTex;
                            orientationType = 4;
                        }
                        // ����������Ͻǣ��ͱ���ұ�Ե
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 1;
                        }
                        // �������Ͻǣ���ɶԽ�
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = DuijiaoTex;
                            orientationType = 1;
                        }
                        // �������½ǣ�Ҳ��������
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                    }             
                    // ������Ϸ���Եģ�����������������ڽ�Ϊ��Եģ��ڽǣ�ֱ�����������򣬾ͱ�ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = InnerTex;
							orientationType = 0;      
						}
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 3;
                        }
                        else
                            continue;
                    }
                    // ������������ǣ��ͽ��������������ǵĻ���
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 3;
                    }
                }
                // �����Ǵ����ĸ���Ե
                // LEFT
                else
                if ( x == mCurrentBoxSelectionMinX )
                {
                    // �����ǰ��ĵ�1�����������ΪOther�������������1 �����ĵ�1�㻹û��������
                    // 2 �����ĵ�1�����������ˣ����������ڵ�ǰ��
                    // ����ǵ�һ�����������ֱ������ȥ������ǵڶ��������ֱ������ȥ���ڲ�ͬ�������û��ƽ���Ĺ��ɣ�����һ��
                    // ������ĵط� FIX IT
                    if ( currentTexType == Other )
                    {         
                            type = EdgeTex;
                            orientationType = 0;                        
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            continue;
                        }
                        // top
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                        // bottom
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // right
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerTex;
							orientationType = 0;      
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������Ͻ�
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                        // �������½�
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // ����������Ͻǻ����½�
                        else if ( (info.layers[1].orientation == 0 ) || (info.layers[1].orientation == 2)
							 || info.layers[1].orientation == 4  || info.layers[1].orientation == 5 )
                        {
                            type = EdgeTex;
                            orientationType = 0;
                        }
                    }                    
                    // ������Ϸ���Ե��ڽǣ�����˵�����Ե�������ڽǺ������ڽǾ�����Եģ���������������Ҫ��ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 
							 || info.layers[1].orientation == 6 || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                           orientationType = 0;
                        }
                        else
                        {
                           orientationType = 2;
                        }
                    }
                    

                }
                // TOP
                else if ( z == mCurrentBoxSelectionMinZ )
                {                    
                    if ( currentTexType == Other )
                    {
                        type = EdgeTex;
                        orientationType = 5;
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7)
                        {
                            continue;
                        }
                        // right
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }                    
                        // bottom
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        // left
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������½�
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                        // �������½�
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }
                        // ����������Ͻǻ����Ͻ�
                        else if ( (info.layers[1].orientation == 1 ) || (info.layers[1].orientation == 0 )
							 || info.layers[1].orientation == 5  || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 5;
                        }
                    }                    
                    // ������Ϸ���Ե��ڽǣ�����˵�����Ե�������ڽǺ������ڽǾ�����Եģ���������������Ҫ��ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 3 
							 || info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 
							 || info.layers[1].orientation == 3  || info.layers[1].orientation == 6 )
                        {
                            orientationType = 0;
                        }
                        else
                        {
                            orientationType = 1;
                        }
                    }

                }
                // RIGHT
               // else if ( isTexInCurrentGroup( findTexTypeByPixmapId(leftInfo.layers[0].pixmapId) ) )
                else if ( x == maxx )
                {                    
                    if ( currentTexType == Other )
                    {
                        type = EdgeTex;
                        orientationType = 1;
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            continue;
                        }
                        // bottom
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // top
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }
                        // left
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������Ͻ�
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }
                        // �������½�
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // ����������Ͻǻ����½�
                        else if ( (info.layers[1].orientation == 1 ) || (info.layers[1].orientation == 3 ) 
							 || info.layers[1].orientation == 6 || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 1;
                        }
                    }                    
                    // ������Ϸ���Ե��ڽǣ�����˵�����Ե�������ڽǺ������ڽǾ�����Եģ���������������Ҫ��ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 0
							 || info.layers[1].orientation == 4 || info.layers[1].orientation == 5 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            orientationType = 3;
                        }
                        else
                        {
                            orientationType = 1;
                        }
                    }

                }
                // BOTTOM
               // else if ( isTexInCurrentGroup( findTexTypeByPixmapId(topInfo.layers[0].pixmapId) ) )
                else if ( z == maxz )
                {             
                    // �����ǰ��1��û��������ֱ���������ı�Ե������������������������
                  //  if ( info.layers[1].pixmapId < 1)
                    if ( currentTexType == Other )
                    {
                        type = EdgeTex;
                        orientationType = 4;
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                        // left edge
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // right edge
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // �����ǰ��Ϊ��Եı�Ե���ͻ��ɵ�
                        // top edge
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // �������Ͻ�
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // �������Ͻ�
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // ����������½ǻ����½�
                        else if ( (info.layers[1].orientation == 2 ) || (info.layers[1].orientation == 3 )
							 || info.layers[1].orientation == 4  || info.layers[1].orientation == 6 )
                        {
                            type = EdgeTex;
                            orientationType = 4;
                        }
                    }                    
                    // ������Ϸ���Ե��ڽǣ�����˵�����Ե�������ڽǺ������ڽǾ�����Եģ���������������Ҫ��ɵ�������
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 0 
							 || info.layers[1].orientation == 5  || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // �������λ����ͬ�ĶԽǣ��Խ�֮ǰ��λ������ǣ����ͱ���ڽ�
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3 
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            orientationType = 3;
                        }
                        else
                        {
                            orientationType = 2;
                        }
                    }
                }
              
				assert ( (type >= InnerTex) && (type < Other) );
				assert ( (orientationType >= 0) && (orientationType < 8) );

                switch( type )
                {
                    // ���ڲ���������Ⱦ����
                case InnerTex:
                    {
                        PaintPixmap pixmap;  

						assert ( false == mCurrentInnerArray->empty() );

                        if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,innerSize));
							if (index == innerSize)
							{
								index = 0;
							}
							assert (index < innerSize);
                            pixmap = (*mCurrentInnerArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*mCurrentInnerArray)[0];
                        }
                        // �����id������pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        info.layers[0].pixmapId = pixmapId;

						assert ( orientationType == 0 );
                        info.layers[0].orientation = orientationType;
                     
                        info.layers[1].pixmapId = 0;
                        info.layers[1].orientation = 0;
                    
                        break;
                    }
                    // �ñ�Ե��������Ⱦ����
                case EdgeTex:
                    {
						AutoTexGenPixmapArray *workingArray = NULL;
						int workingArraySize = 0;

						// 
						if ( orientationType == 0 )
						{
							if ( mCurrentLEdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentLEdgeArray;
								workingArraySize = lEdgeSize;
							}
						}
						else if ( orientationType == 1 )
						{
							if ( mCurrentREdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentREdgeArray;
								workingArraySize = rEdgeSize;
							}
						}	
						else if ( orientationType == 4 )
						{
							if ( mCurrentBEdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentBEdgeArray;
								workingArraySize = bEdgeSize;
							}
						}	
						else if ( orientationType == 5 )
						{
							if ( mCurrentTEdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentTEdgeArray;
								workingArraySize = tEdgeSize;
							}
						}

                        PaintPixmap pixmap;

						if ( modified != mHintModified )
						{
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,workingArraySize));
							if (index == workingArraySize)
							{
								index = 0;
							}
							assert (index < workingArraySize);
							pixmap = (*workingArray)[ index ];
						}
						else
						{	
							pixmap = (*workingArray)[0];
						}

                        // �����id������pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();

                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 5 ||
									 orientationType == 0 ||
									 orientationType == 4 ||
									 orientationType == 1 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 EdgeTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                    // ���ڽ���������Ⱦ����
                case InnerCornerTex:
                    {        
						AutoTexGenPixmapArray *workingArray = NULL;
						int workingArraySize = 0;

						if ( orientationType == 0 )
						{
							if ( mCurrentLTInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentLTInnerCornerArray;
								workingArraySize = ltInnerCornerSize;
							}
						}
						else if ( orientationType == 1 )
						{
							if ( mCurrentRTInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentRTInnerCornerArray;
								workingArraySize = rtInnerCornerSize;
							}
						}
						else if ( orientationType == 2 )
						{
							if ( mCurrentLBInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentLBInnerCornerArray;
								workingArraySize = lbInnerCornerSize;
							}
						}
						else if ( orientationType == 3 )
						{
							if ( mCurrentRBInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentRBInnerCornerArray;
								workingArraySize = rbInnerCornerSize;
							}
						}

                        PaintPixmap pixmap;  

                        if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,workingArraySize));
							if (index == workingArraySize)
							{
								index = 0;
							}
                            pixmap = (*workingArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*workingArray)[0];
                        }
                        // �����id������pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 0 ||
									 orientationType == 1 ||
									 orientationType == 2 ||
									 orientationType == 3 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 InnerCornerTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                    // ���
                case OuterCornerTex:
                    {    
						AutoTexGenPixmapArray *workingArray = NULL;
						int workingArraySize = 0;

						if ( orientationType == 0 )
						{
							if ( mCurrentLTOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentLTOuterCornerArray;
								workingArraySize = ltOutercornerSize;
							}
						}
						else if ( orientationType == 1 )
						{
							if ( mCurrentRTOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentRTOuterCornerArray;
								workingArraySize = rtOutercornerSize;
							}
						}
						else if ( orientationType == 2 )
						{
							if ( mCurrentLBOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentLBOuterCornerArray;
								workingArraySize = lbOutercornerSize;
							}
						}
						else if ( orientationType == 3 )
						{
							if ( mCurrentRBOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentRBOuterCornerArray;
								workingArraySize = rbOutercornerSize;
							}
						}

                        PaintPixmap pixmap;  

						if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,workingArraySize));
							if (index == workingArraySize)
							{
								index = 0;
							}
							assert (index < workingArraySize);
                            pixmap = (*workingArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*workingArray)[0];
                        }

                        // �����id������pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 0 ||
									 orientationType == 1 ||
									 orientationType == 2 ||
									 orientationType == 3 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 OuterCornerTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                    // �Խ�
                case DuijiaoTex:
                    {
                        PaintPixmap pixmap;  

						assert ( false == mCurrentDuijiaoArray->empty() );

                        if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,duijiaoSize));
							if (index == duijiaoSize)
							{
								index = 0;
							}
							assert (index < duijiaoSize);
                            pixmap = (*mCurrentDuijiaoArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*mCurrentDuijiaoArray)[0];
                        }
                        // �����id������pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 0 ||
									 orientationType == 1 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 DuijiaoTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                case Other:
					{
						/*OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
							"The texture type is wrong ",
							"AutoTexPaintAction::_doPaint");*/
                        Ogre::LogManager::getSingleton().logMessage("The texture type is wrong "
                            "AutoTexPaintAction::_doPaint");
                        return;
					}
                    break;
              
                }      

                assert(info.layers[0].pixmapId != 0 || info.layers[1].pixmapId == 0);
			
                assert(info.layers[0].pixmapId <= getTerrainData()->mPixmaps.size());

				if (info.layers[0].pixmapId > getTerrainData()->mPixmaps.size())
				{
					OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							"info.layers[0].pixmapId > getTerrainData()->mPixmaps.size()",
							"AutoTexPaintAction::_doPaint");
				}

                assert(info.layers[1].pixmapId <= getTerrainData()->mPixmaps.size());

				if (info.layers[1].pixmapId > getTerrainData()->mPixmaps.size())
				{
					OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							"info.layers[1].pixmapId > getTerrainData()->mPixmaps.size()",
							"AutoTexPaintAction::_doPaint");
				}

				if (getTerrainData()->mPixmaps.size() >= 256)
				{
					;
				}

                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);

                assert (info.layers[0].pixmapId !=0 || info.layers[1].pixmapId == 0);				
            }         
        }
               
        if (anyModified)
        {
            mCurrentInnerGrids->notifyModified();
            mCurrentOuterGrids->notifyModified();
        }
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onActive(bool active)
    {
        if (!active)
        {
            mHintModified->apply();
            mHintModified->reset();
        }
    }
    //-----------------------------------------------------------------------
	void AutoTexPaintAction::resetHintModified(bool bApply)
	{
		if (bApply)
		{
			mHintModified->apply();
		}
		
		mHintModified->reset();
	}

	void AutoTexPaintAction::resetCurrentGrids()
	{
		mCurrentInnerGrids->reset();
		mCurrentOuterGrids->reset();
	}

	GridSelection* AutoTexPaintAction::getHintModified()
	{
		return mHintModified;
	}

    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onMove(const Point& pt)
    {      
        mHintModified->apply();
        mHintModified->reset();
		resetCurrentGrids();
        _buildHitIndicator(pt);
        _doPaint(mHintModified);   
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onBegin(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onDrag(const Point& pt)
    {
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onEnd(const Point& pt, bool canceled)
    {
        /*if (canceled)
        {
            mModifiedGrids->apply();
        }*/
        doFinish(mModifiedGrids, canceled);
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
    }
    //-----------------------------------------------------------------------
    // �����ж����������Ƿ���ȵķº���
    namespace {
        struct IsCaseInsensitiveEqual
        {
            const String& value;
            Real left;
            Real top;

#define error 0.000001

            IsCaseInsensitiveEqual(const String& value, Real left, Real top) : value(value), left(left), top(top) {}

            bool operator() (const PaintPixmap& rhs) const
            {

                return (stricmp(value.c_str(), rhs.textureName.c_str()) == 0 &&
                    left <= rhs.left+error && left >= rhs.left-error &&
                    top <= rhs.top+error && top >= rhs.top-error );
            }
        };
    }
    //-----------------------------------------------------------------------
    bool AutoTexPaintAction::isTexInCurrentGroup( ushort id )
    {
        if ( id < 1 )
            return false;
        else
        {
            TerrainData::Pixmap pixmap = getTerrainData()->mPixmaps[id-1];
            String texName = getTerrainData()->mTextures[ pixmap.textureId ];

            return 
                std::find_if( mCurrentInnerCornerArray->begin(), mCurrentInnerCornerArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerCornerArray->end() ||
				std::find_if( mCurrentLTInnerCornerArray->begin(), mCurrentLTInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTInnerCornerArray->end() ||
				std::find_if( mCurrentRTInnerCornerArray->begin(), mCurrentRTInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTInnerCornerArray->end() ||
				std::find_if( mCurrentLBInnerCornerArray->begin(), mCurrentLBInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBInnerCornerArray->end() ||
				std::find_if( mCurrentRBInnerCornerArray->begin(), mCurrentRBInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBInnerCornerArray->end() ||

                std::find_if( mCurrentOuterCornerArray->begin(), mCurrentOuterCornerArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentOuterCornerArray->end() ||
				std::find_if( mCurrentLTOuterCornerArray->begin(), mCurrentLTOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTOuterCornerArray->end() ||
				std::find_if( mCurrentRTOuterCornerArray->begin(), mCurrentRTOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTOuterCornerArray->end() ||
				std::find_if( mCurrentLBOuterCornerArray->begin(), mCurrentLBOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBOuterCornerArray->end() ||
				std::find_if( mCurrentRBOuterCornerArray->begin(), mCurrentRBOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBOuterCornerArray->end() ||

                std::find_if( mCurrentInnerArray->begin(), mCurrentInnerArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerArray->end() ||

                std::find_if( mCurrentEdgeArray->begin(), mCurrentEdgeArray->end(), 
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentEdgeArray->end() ||
				std::find_if( mCurrentLEdgeArray->begin(), mCurrentLEdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLEdgeArray->end() ||
				std::find_if( mCurrentREdgeArray->begin(), mCurrentREdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentREdgeArray->end() ||
				std::find_if( mCurrentTEdgeArray->begin(), mCurrentTEdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentTEdgeArray->end() ||
				std::find_if( mCurrentBEdgeArray->begin(), mCurrentBEdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentBEdgeArray->end() ||

                std::find_if( mCurrentDuijiaoArray->begin(), mCurrentDuijiaoArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentDuijiaoArray->end();
        }        
    }   
   
    TexType 
    AutoTexPaintAction::findTexTypeByPixmapId( ushort id )
    {       
        if (id < 1)
        {
            return Other;
        }
        else
        {
            TerrainData::Pixmap pixmap = getTerrainData()->mPixmaps[id-1];
            String texName = getTerrainData()->mTextures[ pixmap.textureId ];

            if ( std::find_if( mCurrentInnerCornerArray->begin(), mCurrentInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerCornerArray->end() ||
				 std::find_if( mCurrentLTInnerCornerArray->begin(), mCurrentLTInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTInnerCornerArray->end() ||
				 std::find_if( mCurrentRTInnerCornerArray->begin(), mCurrentRTInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTInnerCornerArray->end() ||
				 std::find_if( mCurrentLBInnerCornerArray->begin(), mCurrentLBInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBInnerCornerArray->end() ||
				 std::find_if( mCurrentRBInnerCornerArray->begin(), mCurrentRBInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBInnerCornerArray->end()
				)
                return InnerCornerTex;

            else if ( std::find_if( mCurrentOuterCornerArray->begin(), mCurrentOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentOuterCornerArray->end() ||
				      std::find_if( mCurrentLTOuterCornerArray->begin(), mCurrentLTOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTOuterCornerArray->end() ||
					  std::find_if( mCurrentRTOuterCornerArray->begin(), mCurrentRTOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTOuterCornerArray->end() ||
					  std::find_if( mCurrentLBOuterCornerArray->begin(), mCurrentLBOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBOuterCornerArray->end() ||
					  std::find_if( mCurrentRBOuterCornerArray->begin(), mCurrentRBOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBOuterCornerArray->end()
				)
                return OuterCornerTex;

            else if ( std::find_if( mCurrentInnerArray->begin(), mCurrentInnerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerArray->end() )
                return InnerTex;

            else if ( std::find_if( mCurrentEdgeArray->begin(), mCurrentEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentEdgeArray->end() ||
				      std::find_if( mCurrentLEdgeArray->begin(), mCurrentLEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLEdgeArray->end() ||
					  std::find_if( mCurrentREdgeArray->begin(), mCurrentREdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentREdgeArray->end() ||
					  std::find_if( mCurrentTEdgeArray->begin(), mCurrentTEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentTEdgeArray->end() ||
					  std::find_if( mCurrentBEdgeArray->begin(), mCurrentBEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentBEdgeArray->end()
				)
                return EdgeTex;

            else if ( std::find_if( mCurrentDuijiaoArray->begin(), mCurrentDuijiaoArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentDuijiaoArray->end() )
                return DuijiaoTex;

            else
            {
                //Ogre::LogManager::getSingleton().logMessage("textype is Other, maybe something is wrong! texture name : "
                //    + texName + " pixmapid : " + Ogre::StringConverter::toString(id) + " AutoTexPaintAction::findTexTypeByPixmapId");
                return Other; 
            }
        }
    }
}
