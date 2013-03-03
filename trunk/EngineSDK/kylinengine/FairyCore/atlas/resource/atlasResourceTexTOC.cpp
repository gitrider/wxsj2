//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "atlas/resource/atlasResourceTexTOC.h"

bool AtlasResourceTexTOC::getTexel( const Point2F& texCoords, U32 layer, ColorI& outColor )
{
	// Get some key figures.

	U32 treeDepth  = getTreeDepth();
	U32 chunkSize  = getTextureChunkSize();
	U32 mapSize    = BIT( treeDepth - 1 ) * chunkSize;

	// Scale the texcoords to map coords.

	Point2I mapCoords( texCoords.x * F32( mapSize ),
                      texCoords.y * F32( mapSize ) );

	// Scale the map coords into chunk coords.

	Point2I chunkXY( mapCoords.x / chunkSize, mapCoords.y / chunkSize );

	// Walk the TOC tree bottom up and stop at the first matching
	// piece of data that is available.

	for( int i = treeDepth - 1; i >= 0; -- i )
	{
		// Get the stub and, if its data is online, query its texture.

		AtlasResourceTexStub* stub = getStub( i, chunkXY );
		if( stub->hasResource() )
		{
			// Data is there.  Grab the bitmap and read out the color information.

			if( !stub->mChunk->isBitmapTexFormat( stub->mChunk->mFormat ) )
				return false;
			
			GBitmap* bitmap = &stub->mChunk->bitmap[ layer ];

			U32 shift = treeDepth - i - 1;
			U32 x = ( mapCoords.x >> shift ) - chunkXY.x * chunkSize;
			U32 y = ( mapCoords.y >> shift ) - chunkXY.y * chunkSize;

			bitmap->getColor( x, y, outColor );
			return true;
		}
		else
		{
			// No data.  Go up a level and scale our texcoords
			// to the new level dimensions.

			chunkXY.x >>= 1;
			chunkXY.y >>= 1;
		}
	}

	return false;
}
