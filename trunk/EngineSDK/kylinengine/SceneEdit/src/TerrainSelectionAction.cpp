#include "TerrainSelectionAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectGrids.h"
#include "TerrainSelections.h"
#include "TerrainSelectionActionOperator.h"
#include "WXModifyTerrainHeightOperator.h"
#include "WXModifyTerrainPaintOperator.h"
#include "WXModifyTerrainDiffuseOperator.h"
#include "GridInfoFlagModifyOperator.h"
#include "WXOperatorManager.h"

#include <OgreStringConverter.h>
#include <OgreDataStream.h>

namespace Fairy {

    //////////////////////////////////////////////////////////////////////////  

    TerrainSelectionAction::TerrainSelectionAction(SceneManipulator* sceneManipulator)
        : Action(sceneManipulator), mPasteHeightInfo(true), mPasteDiffuseInfo(true), mPasteTextureInfo(true), mPasteDiagonalInfo(true),
        mCurrentMousePos(Ogre::Vector3::ZERO), mCurrentMode("SingleSelecting")
    {
        mTerrainSelectionGrids = static_cast<TerrainSelectionGrids *>(sceneManipulator->getHitIndicator("TerrainSelectionGrids"));
		mAfterCopy = false;
		mRotateCount = 0;
	//	mMaxJunctionX = 0;
	//	mMaxJunctionZ = 0;
	}

    TerrainSelectionAction::~TerrainSelectionAction()
    {        
    }

    void TerrainSelectionAction::setParameter(const String& name, const String& value)
    {
        if (name == "%CurrentMode")
        {
            mCurrentMode = value;
        }   
        else if (name == "%PasteHeightInfo")
        {
            mPasteHeightInfo = Ogre::StringConverter::parseBool(value);
        }
		else if (name == "%PasteDiffuseInfo")
		{
			mPasteDiffuseInfo = Ogre::StringConverter::parseBool(value);
		}
        else if (name == "%PasteTextureInfo")
        {
            mPasteTextureInfo = Ogre::StringConverter::parseBool(value);
        }
        else if (name == "%PasteDiagonalInfo")
        {
            mPasteDiagonalInfo = Ogre::StringConverter::parseBool(value);
        }        
        else if (name == "%CurrentMousePosition")
        {
            mCurrentMousePos = Ogre::StringConverter::parseVector3(value);
            pasteTerrainInfo();
        }
		else if (name == "%AfterCopy")
		{
			mAfterCopy = true;
			mRotateCount = 0;
		}
		else if (name == "%BeginRotateTerrain")
		{
			++mRotateCount;
			beginRotateTerrain();
		}
		else
        {
            Action::setParameter(name, value);
        }
    }

    String TerrainSelectionAction::getParameter(const String& name) const
    {
        if (name == "%CurrentMode")
        {
            return mCurrentMode;
        }
        else if (name == "%JunctionHeightInfo")
        {
			// ��������־λ��û���ã�˵�������ȡ������Ϣ�������ǻ�ȡ��ת�����Ϣ
			if (!mAfterCopy)
				getJunctionInfoAsString();

            assert( !mJunctionHeightInfoString.empty() );
            return mJunctionHeightInfoString;
        }
		else if (name == "%JunctionDiffuseInfo")
		{
			if (!mAfterCopy)
			{
				getJunctionDiffuseInfoAsString();
			}
			assert( !mJunctionDiffuseInfoString.empty() );
			return mJunctionDiffuseInfoString;
		}
        else if (name == "%GridTextureInfo")
        {
			if (!mAfterCopy)
				getGridTextureInfoAsString();

            assert( !mGridTextureInfoString.empty() );
            return mGridTextureInfoString;
        }
        else if (name == "%GridDiagonalInfo")
        {
			if (!mAfterCopy)
				getGridDiagonalInfoAsString();

            assert( !mGridDiagonalInfoString.empty() );
            return mGridDiagonalInfoString;
        }
        else if (name == "%PasteHeightInfo")
        {
            return Ogre::StringConverter::toString(mPasteHeightInfo);
        }
		else if (name == "%PasteDiffuseInfo")
		{
			return Ogre::StringConverter::toString(mPasteDiffuseInfo);
		}
        else if (name == "%PasteTextureInfo")
        {
            return Ogre::StringConverter::toString(mPasteTextureInfo);
        }
        else if (name == "%PasteDiagonalInfo")
        {
            return Ogre::StringConverter::toString(mPasteDiagonalInfo);
        }
		else if (name == "%AfterCopy")
		{
			return Ogre::StringConverter::toString(mAfterCopy);
		}

        return Action::getParameter(name);
    }

    void 
    TerrainSelectionAction::_onActive(bool active)
    {
        if (!active)
        {
            mTerrainSelectionGrids->hide();
        }
        else
        {
            mTerrainSelectionGrids->show();
        }
    }

    void 
    TerrainSelectionAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);

		Ogre::Vector3 pos;
		bool intersected = getSceneManipulator()->getTerrainIntersects(pt, pos);

		pos = getSceneManipulator()->_alignPosition(pos,1.0f);
		if (intersected && mAfterCopy)
		{
			mTerrainSelectionGrids->setSceneNodePos(pos);
		}
    }

    void 
    TerrainSelectionAction::_onBegin(const Point& pt)
    {
        if ( mCurrentMode == "MultiSelecting" )
            mTerrainSelectionGrids->setCurrentGridSelection(true);
        else
            mTerrainSelectionGrids->setCurrentGridSelection(false);
    }

    void 
    TerrainSelectionAction::_onDrag(const Point& pt)
    {
       mTerrainSelectionGrids->setHitPoint(pt);

	   // ���½�������������˵��Ҫ���¿������Σ��Ͱ������־λ���false����ʾ��û���п�������
	   mAfterCopy = false;
	   mTerrainSelectionGrids->setOriginSceneNodeOrientation();
    }

    void 
    TerrainSelectionAction::_onEnd(const Point& pt, bool canceled)
    {
        if ( mCurrentMode == "MultiSelecting" )
            mTerrainSelectionGrids->addCurrentGridSelection(true);
        else
            mTerrainSelectionGrids->addCurrentGridSelection(false);
    }

    const String& TerrainSelectionAction::getName(void) const
    {
        static const String name = "TerrainSelectionAction";
        return name;
    }

    void TerrainSelectionAction::getJunctionInfoAsString(void) const
    {
        mJunctionHeightInfoString.clear();

        // ��ȡ��ǰ��ѡ���ȫ��junctionselection
        Fairy::JunctionSelection* selection = mTerrainSelectionGrids->getTotalJunctionSelection();

        assert ( selection->empty() == false );

        Fairy::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const Fairy::JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        Fairy::JunctionSelection::JunctionMap::const_iterator it;

        int minJunctionX = 0;
        int minJunctionZ = 0;

		int middleJunctionX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX() + 1) / 2 + mTerrainSelectionGrids->getMinX();
		int middleJunctionZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ() + 1) / 2 + mTerrainSelectionGrids->getMinZ();
		float minJunctionHeight = manipulator->getTerrainData()->getHeight(middleJunctionX, middleJunctionZ);
        // ���浱ǰѡ���junction ����͸߶�
        /** FIXME ���ﱣ����ѡ��ĵ��εĸ߶ȱ�����������Ϣ������ڵ�һ��junction�ĸ߶ȣ����
            ��ѡ�ĵ�����һ��������ɽ�����ַ���û���⣬�����һ��junction��λ����ɽ���Ļ���������
            ����ǲ���ȷ�ģ���Ϊ����junction��λ�ö��ȵ�һ���ͣ�������Ը߶�Ϊ����
        */
   //     float minJunctionHeight = 0.0f;
        float worldHeight = 0.0f;

        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            mJunctionHeightInfoString += "junction\n{\n";

            const Fairy::JunctionSelection::Junction& junction = it->second;

            // ��һ��junction���������Ϊ0 0
   //         if ( it == junctions.begin() )
   //         {
   //             minJunctionX = junction.x;
   //             minJunctionZ = junction.z;

   //             mJunctionHeightInfoString += Ogre::StringConverter::toString(0);
   //             mJunctionHeightInfoString += " ";
   //             mJunctionHeightInfoString += Ogre::StringConverter::toString(0);
   //             mJunctionHeightInfoString += " ";

   //             // ��һ��junction�ĸ߶�Ϊ�����͸߶ȣ�����junction�ĸ߶ȶ�������ڴ˸߶ȵ���Ը߶�
   //             minJunctionHeight = manipulator->getTerrainData()->getHeight(junction.x, junction.z);
			//}
   //         // ������junction�����궼������ڵ�һ��junction���������
   //         else
   //         {
				int x = junction.x - middleJunctionX;
				/*if (x>mMaxJunctionX)
					mMaxJunctionX = x;*/

                mJunctionHeightInfoString += Ogre::StringConverter::toString(x);
                mJunctionHeightInfoString += " ";

				int z = junction.z - middleJunctionZ;
			/*	if (z>mMaxJunctionZ)
					mMaxJunctionZ = z;*/

                mJunctionHeightInfoString += Ogre::StringConverter::toString(z);
                mJunctionHeightInfoString += " ";
        //    }

            worldHeight = manipulator->getTerrainData()->getHeight(junction.x, junction.z);

			// ֱ��ʹ���������꣬��ʹ���������
         //   float relativeHeight = worldHeight - minJunctionHeight;

		//	mHeightArray.push_back(relativeHeight);
            mJunctionHeightInfoString += Ogre::StringConverter::toString(worldHeight);

            mJunctionHeightInfoString += "\n}\n";
       }
    }

	void TerrainSelectionAction::getJunctionDiffuseInfoAsString(void) const
	{
		mJunctionDiffuseInfoString.clear();

        // ��ȡ��ǰ��ѡ���ȫ��junctionselection
        Fairy::JunctionSelection* selection = mTerrainSelectionGrids->getTotalJunctionSelection();

        assert ( selection->empty() == false );

        Fairy::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const Fairy::JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        Fairy::JunctionSelection::JunctionMap::const_iterator it;

        int minJunctionX = 0;
        int minJunctionZ = 0;

		int middleJunctionX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX() + 1) / 2 + mTerrainSelectionGrids->getMinX();
		int middleJunctionZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ() + 1) / 2 + mTerrainSelectionGrids->getMinZ();
		float minJunctionHeight = manipulator->getTerrainData()->getHeight(middleJunctionX, middleJunctionZ);

		Ogre::ColourValue diffuseColour = Ogre::ColourValue::White;

        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            mJunctionDiffuseInfoString += "diffuse\n{\n";

            const Fairy::JunctionSelection::Junction& junction = it->second;

			int x = junction.x - middleJunctionX;

            mJunctionDiffuseInfoString += Ogre::StringConverter::toString(x);
            mJunctionDiffuseInfoString += " ";

			int z = junction.z - middleJunctionZ;

            mJunctionDiffuseInfoString += Ogre::StringConverter::toString(z);
            mJunctionDiffuseInfoString += " ";

            diffuseColour = manipulator->getTerrainData()->getDiffuseInfo(junction.x, junction.z);
		
            mJunctionDiffuseInfoString += Ogre::StringConverter::toString(diffuseColour);

            mJunctionDiffuseInfoString += "\n}\n";
       }
	}

    void TerrainSelectionAction::getGridTextureInfoAsString(void) const
    {
        mGridTextureInfoString.clear();

        Fairy::GridSelection* gridSelection = mTerrainSelectionGrids->getTotalGridSelection();

        assert ( gridSelection->empty() == false );

        Fairy::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const Fairy::GridSelection::GridMap& grids = gridSelection->getGrids();
        Fairy::GridSelection::GridMap::const_iterator it;

        int minGridX = 0;
        int minGridZ = 0;

		int middleGridX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX()) / 2 + mTerrainSelectionGrids->getMinX();
		int middleGridZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ()) / 2 + mTerrainSelectionGrids->getMinZ();

        for (it = grids.begin(); it != grids.end(); ++it)
        {
            mGridTextureInfoString += "grid_texture\n{\n";

            const Fairy::GridSelection::Grid& grid = it->second;

            // ��һ��grid���������Ϊ0 0
            //if ( it == grids.begin() )
            //{
            //    minGridX = grid.x;
            //    minGridZ = grid.z;

            //    mGridTextureInfoString += Ogre::StringConverter::toString(0);
            //    mGridTextureInfoString += " ";
            //    mGridTextureInfoString += Ogre::StringConverter::toString(0);
            //    mGridTextureInfoString += " ";         
            //}
            //// ������grid�����궼������ڵ�һ��grid���������
            //else
            //{
                mGridTextureInfoString += Ogre::StringConverter::toString(grid.x - middleGridX);
                mGridTextureInfoString += " ";
                mGridTextureInfoString += Ogre::StringConverter::toString(grid.z - middleGridZ);
                mGridTextureInfoString += " ";
          //  }

            const Fairy::TerrainData::GridInfo &gridInfo = grid.info;

			/** ԭ����ֻ���浱ǰ���ӵ������id������Ҫ����
			1 ��������
			2 ������������������
			3 ����ķ���
			*/
            // ��ȡ��һ�����Ϣ
			ushort id = gridInfo.layers[0].pixmapId;
			assert (id>=0);
			Fairy::TerrainData *pTerrainData = manipulator->getTerrainData();
			assert (pTerrainData);
			
			if (id>0)
			{
				mGridTextureInfoString += pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].left);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].top);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].right);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].bottom);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(gridInfo.layers[0].orientation);
				mGridTextureInfoString += " ";
			}
			else
			{
				mGridTextureInfoString += "UnTextured 0.0 0.0 1.0 1.0 0.0";				
			}

            // ��ȡ�ڶ������Ϣ
			id = gridInfo.layers[1].pixmapId;
			// ����ڶ���������ſ���������Ϣ
			if (id>0)
			{
				mGridTextureInfoString += pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].left);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].top);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].right);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].bottom);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(gridInfo.layers[1].orientation);
				mGridTextureInfoString += " ";
			}

            mGridTextureInfoString += "\n}\n";
        }
    }

    void TerrainSelectionAction::getGridDiagonalInfoAsString(void) const
    {
        mGridDiagonalInfoString.clear();

        Fairy::GridSelection* gridSelection = mTerrainSelectionGrids->getTotalGridSelection();

        assert ( gridSelection->empty() == false );

        Fairy::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const Fairy::GridSelection::GridMap& grids = gridSelection->getGrids();
        Fairy::GridSelection::GridMap::const_iterator it;
       
		int middleGridX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX()) / 2 + mTerrainSelectionGrids->getMinX();
		int middleGridZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ()) / 2 + mTerrainSelectionGrids->getMinZ();

        for (it = grids.begin(); it != grids.end(); ++it)
        {
            mGridDiagonalInfoString += "grid_diagonal\n{\n";

            const Fairy::GridSelection::Grid& grid = it->second;

            // ��һ��grid���������Ϊ0 0
            //if ( it == grids.begin() )
            //{
            //    minGridX = grid.x;
            //    minGridZ = grid.z;

            //    mGridDiagonalInfoString += Ogre::StringConverter::toString(0);
            //    mGridDiagonalInfoString += " ";
            //    mGridDiagonalInfoString += Ogre::StringConverter::toString(0);
            //    mGridDiagonalInfoString += " ";         
            //}
            //// ������grid�����궼������ڵ�һ��grid���������
            //else
            //{
                mGridDiagonalInfoString += Ogre::StringConverter::toString(grid.x - middleGridX);
                mGridDiagonalInfoString += " ";
                mGridDiagonalInfoString += Ogre::StringConverter::toString(grid.z - middleGridZ);
                mGridDiagonalInfoString += " ";
         //   }

            const Fairy::TerrainData::GridInfo &gridInfo = grid.info;

            // ��ȡ��һ�����Ϣ
            mGridDiagonalInfoString += Ogre::StringConverter::toString(gridInfo.flags);

            mGridDiagonalInfoString += "\n}\n";
        }
    }  

    void TerrainSelectionAction::pasteTerrainInfo(void)
    {
        Fairy::SceneManipulator *manipulator = getSceneManipulator();
		Fairy::TerrainData *pTerrainData = manipulator->getTerrainData();
		assert (pTerrainData);

        String infoString;

        // ������Ҫ����Ҫ�������ַ���
        if ( mPasteHeightInfo )
            infoString += mJunctionHeightInfoString;
		if ( mPasteDiffuseInfo )
			infoString += mJunctionDiffuseInfoString;
        if ( mPasteDiagonalInfo )
            infoString += mGridDiagonalInfoString;
        if ( mPasteTextureInfo )
            infoString += mGridTextureInfoString;

        // ����clipboard�е�����������һ����ʱ�������������еĽ��������������������������
        Ogre::MemoryDataStream* stream = new Ogre::MemoryDataStream((void *)(infoString.c_str()),
            infoString.length(), false);

        // ����������selection�����뵱ǰ�ı�ĵ��Σ���Ϊnotify��
        Fairy::JunctionSelection* junctionSelection = new Fairy::JunctionSelection( manipulator->getTerrain(),getSceneManipulator() );
        Fairy::GridSelection* gridSelection = new Fairy::GridSelection( manipulator->getTerrain(),getSceneManipulator() );

        // �½�һ��operator��������height��paint��diagonal����������
        std::auto_ptr<Fairy::TerrainSelectionActionOperator> op( new Fairy::TerrainSelectionActionOperator(manipulator) );

        String line;
        // ��ʼ�����������Ĳ��� 
        std::vector<TerrainInfo> terrainInfo;
        while(!stream->eof())
        {
            line = stream->getLine();

            // junction height
            if ( line.substr(0,8) == "junction" && mPasteHeightInfo )
            {
				mCurrentMousePos = getSceneManipulator()->_alignPosition(mCurrentMousePos,1.0f);
                std::pair<int, int> mouseJunction = pTerrainData->getJunction(mCurrentMousePos.x, mCurrentMousePos.z);

                // �ų� { ��һ��
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    assert ( vecparams.size() == 3 );            

                    // ������һ��
                    valueLine = stream->getLine();

                    // ��ȡ��ǰҪ������junction�ľ�������
                    int absJunctionX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseJunction.first;
                    int absJunctionZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseJunction.second;

                    if ( false == pTerrainData->isValidJunction(absJunctionX, absJunctionZ) )
                        continue;

                    // ������Ը߶ȣ�Ҳ�����µĸ߶ȣ�
					// ��ʹ����Ը߶�
					float worldHeight = Ogre::StringConverter::parseReal(vecparams[2]);

                   // float absHeight = Ogre::StringConverter::parseReal(vecparams[2]) +
                     //   pTerrainData->getHeight(absJunctionX, absJunctionZ);

                    // ����ǰ�ȵõ���λ�õ��ϵĸ߶�
                    float oldHeight = pTerrainData->getHeight(absJunctionX, absJunctionZ);
                    // �����µĸ߶�

                    TerrainInfo terrInfo;
                    terrInfo.x = absJunctionX;
                    terrInfo.z = absJunctionZ;
                    terrInfo.oldHeight = oldHeight;
                    terrInfo.nowHeight = worldHeight;
                    terrainInfo.push_back(terrInfo);                    

                    pTerrainData->setHeight(absJunctionX, absJunctionZ, worldHeight);

                    // ���뵽operator��
                    if (worldHeight != oldHeight)
                    {
                        op->mModifyHeightOperator->add(absJunctionX, absJunctionZ, oldHeight, worldHeight);
                    }

                    // �ѵ�ǰ��һ��junction���뵽notify��selection�� TODO
                    junctionSelection->add(absJunctionX, absJunctionZ, 1.0);
                }
            } // end of junction
			else if ( line.substr(0,7) == "diffuse" && mPasteDiffuseInfo )
			{
				mCurrentMousePos = getSceneManipulator()->_alignPosition(mCurrentMousePos,1.0f);
				std::pair<int, int> mouseJunction = pTerrainData->getJunction(mCurrentMousePos.x, mCurrentMousePos.z);

				// �ų� { ��һ��
				stream->skipLine();

				String valueLine;
				Ogre::StringVector vecparams;

				valueLine = stream->getLine();

				while ( valueLine != "}" )
				{
					vecparams = Ogre::StringUtil::split(valueLine);

					assert ( vecparams.size() == 6 );            

					// ������һ��
					valueLine = stream->getLine();

					// ��ȡ��ǰҪ������junction�ľ�������
					int absJunctionX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseJunction.first;
					int absJunctionZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseJunction.second;

					if ( false == pTerrainData->isValidJunction(absJunctionX, absJunctionZ) )
						continue;

					// get the colour of the terrain vertex
					Ogre::ColourValue diffuseColour = Ogre::ColourValue::White;
					diffuseColour.r = Ogre::StringConverter::parseReal(vecparams[2]);
					diffuseColour.g = Ogre::StringConverter::parseReal(vecparams[3]);
					diffuseColour.b = Ogre::StringConverter::parseReal(vecparams[4]);
					diffuseColour.a = Ogre::StringConverter::parseReal(vecparams[5]);

					// ����ǰ�ȵõ���λ�õ��϶���ɫ
					Ogre::ColourValue oldCol = pTerrainData->getDiffuseInfo(absJunctionX, absJunctionZ);				                  

					pTerrainData->setDiffuseInfo(absJunctionX, absJunctionZ, diffuseColour);

					// ���뵽operator��
					if (oldCol != diffuseColour)
					{
						op->mModifyTerrainDiffuseOperator->add(absJunctionX, absJunctionZ, oldCol, diffuseColour);
					}				
					junctionSelection->add(absJunctionX, absJunctionZ, 1.0);
				}
			} // end of diffuse
            // texture
            else if ( line.substr(0,12) == "grid_texture" && mPasteTextureInfo )
            {
                std::pair<int, int> mouseGrid = pTerrainData->getGrid(mCurrentMousePos.x, mCurrentMousePos.z);

                // �ų� { ��һ��
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    size_t paraSize = vecparams.size();
					// һ�����ٵĲ���������8�����ֱ��Ǹ��ӵ�x��y���꣬�������ƣ�left��top��right��bottom��orientation
					assert (paraSize>=8);

                    // ������һ��
                    valueLine = stream->getLine();

                    // ��ȡ��ǰҪ������grid�ľ�������
                    int absGridX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseGrid.first;
                    int absGridZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseGrid.second;

                    if ( false == pTerrainData->isValidGrid(absGridX, absGridZ) )
                        continue;

                    Fairy::TerrainData::GridInfo gridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    // �õ�gridԭ�е���Ϣ
                    const Fairy::TerrainData::GridInfo oldGridInfo = gridInfo;                    

					int layerPixmapId = 0;

					if (vecparams[2] != "UnTextured")
					{
						layerPixmapId = pTerrainData->_registerPixmap(
						vecparams[2], // texture name
						Ogre::StringConverter::parseReal(vecparams[3]),  // left
						Ogre::StringConverter::parseReal(vecparams[4]),  // top
						Ogre::StringConverter::parseReal(vecparams[5]),  // right
						Ogre::StringConverter::parseReal(vecparams[6])  // bottom
						);
					}					 

					assert (layerPixmapId>=0);
					gridInfo.layers[0].pixmapId = layerPixmapId;
					ushort orientation = Ogre::StringConverter::parseUnsignedInt(vecparams[7]);
					assert (0 <= orientation && orientation < 16 && "Invalid orientation");
					gridInfo.layers[0].orientation = orientation;

					// ����еڶ����������Ϣ���ͽ��д���
					if (paraSize>8)
					{
						assert (paraSize == 14);

						layerPixmapId = pTerrainData->_registerPixmap(
							vecparams[8], // texture name
							Ogre::StringConverter::parseReal(vecparams[9]),  // left
							Ogre::StringConverter::parseReal(vecparams[10]),  // top
							Ogre::StringConverter::parseReal(vecparams[11]),  // right
							Ogre::StringConverter::parseReal(vecparams[12])  // bottom
							);

						assert (layerPixmapId>0);
						gridInfo.layers[1].pixmapId = layerPixmapId;
						ushort orientation = Ogre::StringConverter::parseUnsignedInt(vecparams[13]);
						assert (0 <= orientation && orientation < 16 && "Invalid orientation");
						gridInfo.layers[1].orientation = orientation;
					}
					else
					{
						// ��ԭ���ε��ϲ�����ɾ��
						gridInfo.layers[1].pixmapId = 0;
						gridInfo.layers[1].orientation = 0;
					}

                    pTerrainData->setGridInfo(absGridX, absGridZ, gridInfo);

                    // ��ȡ��grid���ڵ���Ϣ
                    const Fairy::TerrainData::GridInfo &newGridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    if (newGridInfo != oldGridInfo)
                    {
                        op->mModifyPaintOperator->add(absGridX, absGridZ, oldGridInfo, newGridInfo);
                    }

                    // ������grid�Ѿ������뵽notify��selection���ˣ��Ͳ��ټ��루��Ϊdiagonal��texture��grid
                    // �����뵽ͬһ��notify��grid selection��
                    if ( !gridSelection->exist(absGridX,absGridZ) )
                        gridSelection->add(absGridX, absGridZ);
                }
            } // end of texture 
            // diagonal
            else if ( line.substr(0,13) == "grid_diagonal" && mPasteDiagonalInfo )
            {
                std::pair<int, int> mouseGrid = pTerrainData->getGrid(mCurrentMousePos.x, mCurrentMousePos.z);

                // �ų� { ��һ��
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    assert ( vecparams.size() == 3 );            

                    // ������һ��
                    valueLine = stream->getLine();

                    // ��ȡ��ǰҪ������grid�ľ�������
                    int absGridX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseGrid.first;
                    int absGridZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseGrid.second;

                    if ( false == pTerrainData->isValidGrid(absGridX, absGridZ) )
                        continue;

                    Fairy::TerrainData::GridInfo gridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    unsigned int oldFlipFlag = gridInfo.flags;

                    gridInfo.flags = Ogre::StringConverter::parseUnsignedInt(vecparams[2]);

                    pTerrainData->setGridInfo(absGridX, absGridZ, gridInfo);

                    unsigned int newFlipFlag = pTerrainData->getGridInfo(absGridX, absGridZ).flags;

                    if (newFlipFlag != oldFlipFlag)
                    {
                        op->mFlipDiagonalOperator->add(absGridX, absGridZ, oldFlipFlag, newFlipFlag);
                    }

                    if ( !gridSelection->exist(absGridX,absGridZ) )
                        gridSelection->add(absGridX, absGridZ);
                }
            } // end of diagonal
        }

        getSceneManipulator()->_fireTerrainHeightChanged(terrainInfo);

        // notify
        junctionSelection->notifyModified();
		junctionSelection->notifyColourModified();
        delete junctionSelection;

        gridSelection->notifyModified();
        delete gridSelection;    

        // ���뵽operator
        if (!op->empty())
            manipulator->getOperatorManager()->addOperator(op.release());

        // ɾ����ʱ������
        delete stream;
    }

	void TerrainSelectionAction::beginRotateTerrain()
	{
		// ��ת�ڵ㣨ʹ���ο�ָʾ��������ת��
		mTerrainSelectionGrids->rotateSceneNode();

		// ���ڴ������ݵ�������
		Ogre::MemoryDataStream* stream = NULL;

		String line;
		String tempStr("");

		// ���¸�ʽ������
		if (mPasteHeightInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mJunctionHeightInfoString.c_str()),
            mJunctionHeightInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "junction" || line == "{" || line == "}")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";
					tempStr += vecparams[2];
					tempStr += "\n";				 
				}
			}

			mJunctionHeightInfoString = tempStr;

			delete stream; 
			stream = NULL;
		}

		if (mPasteDiffuseInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mJunctionDiffuseInfoString.c_str()),
				mJunctionDiffuseInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "diffuse" || line == "{" || line == "}")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";
					tempStr += vecparams[2];
					tempStr += "\n";				 
				}
			}

			mJunctionDiffuseInfoString = tempStr;

			delete stream; 
			stream = NULL;
		}

		if (mPasteTextureInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mGridTextureInfoString.c_str()),
            mGridTextureInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "{" || line == "}" || line == "grid_texture")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";

					//tempStr += vecparams[2];	// ����
					//tempStr += " ";

					//Ogre::String l = vecparams[3];
					//Ogre::String t = vecparams[4];
					//Ogre::String r = vecparams[5];
					//Ogre::String b = vecparams[6];

					//tempStr += r;
					//tempStr += " ";
					//tempStr += b;
					//tempStr += " ";
					//tempStr += l;
					//tempStr += " ";
					//tempStr += t;
					//tempStr += " ";

					//tempStr += vecparams[7];
					//tempStr += " ";

					//if (vecparams.size()>8)
					//{
					//	tempStr += vecparams[8];	// ����
					//	tempStr += " ";

					//	l = vecparams[9];
					//	t = vecparams[10];
					//	r = vecparams[11];
					//	b = vecparams[12];

					//	tempStr += r;
					//	tempStr += " ";
					//	tempStr += b;
					//	tempStr += " ";
					//	tempStr += l;
					//	tempStr += " ";
					//	tempStr += t;
					//	tempStr += " ";

					//	tempStr += vecparams[13];
					//}
					// ����������Ϣ��ֹ�������Ҫ��һ��ѭ��������
					for ( size_t i=2; i<vecparams.size()-1; ++i )
					{
						tempStr += vecparams[i];
						tempStr += " ";
					}

					// ��ת�������ĵ��ο�ʱ�����е�������ת
					/** ��������ֻ������l t r b�ĸ���ֵ������û�취������������ת
						�������ַ�������mRotateCount�����Ա��������ת���ο�Ĵ�����
						�������øÿ������orientation��ֵ��ÿ�ν����µĵ��ο�copyʱ
						�����Ա���ᱻ���㡣
						���ַ�����ȱ���ǣ�Ҫ�����ĵ��ο������ĳ�ʼ����ֻ����0��Ҳ�����������
						��Ϊ������ֱ�Ӹ�orientation�����ֵ�ģ�����ճ���Ľ������ȷ
					*/
					int nowRotateCount = mRotateCount % 4;

					switch (nowRotateCount)
					{
					case 1:
						tempStr += "6";
						break;
					case 2:
						tempStr += "3";
						break;
					case 3:
						tempStr += "5";
						break;
					default:
						tempStr += vecparams[vecparams.size()-1];
						break;
					}
					tempStr += " ";

					tempStr += "\n";
				}
			}

			mGridTextureInfoString = tempStr;

			delete stream;
			stream = NULL;
		}

		if (mPasteDiagonalInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mGridDiagonalInfoString.c_str()),
            mGridDiagonalInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "{" || line == "}" || line == "grid_diagonal")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";
					tempStr += vecparams[2];
					tempStr += "\n";				 
				}
			}

			mGridDiagonalInfoString = tempStr;

			delete stream; 
			stream = NULL;
		}		 
	}
}