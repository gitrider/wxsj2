//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "gfx/D3D9/gfxD3D9Device.h"
#include "core/stream/fileStream.h"
#include "core/frameAllocator.h"
#include "core/stream/memstream.h"
#include "console/console.h"
#include "gfx/primBuilder.h"
#include "platform/profiler.h"
#include "gfx/D3D9/gfxD3D9CardProfiler.h"
#include "gfx/D3D9/gfxD3D9VertexBuffer.h"
#include "gfx/D3D/screenShotD3D.h"
#include "atlas/resource/atlasTexChunk.h"

void AtlasTexChunk::writeDDS(Stream *s)
{
   // Load the DDS into a texture, then use D3DX to write it out.
   GFXTexHandle tex;
   tex.set(dds, &GFXDefaultStaticDiffuseProfile, false, avar("%s - %s", __FUNCTION__, ( const char* ) dds->mSourcePath.getFullPath() ) );
   GFXD3D9TextureObject *gdto = (GFXD3D9TextureObject *)tex.getPointer();

   // Write to buffer.
   ID3DXBuffer *buffer = NULL;
   GFXD3DX.D3DXSaveTextureToFileInMemory(&buffer, D3DXIFF_DDS, gdto->getTex(), NULL);

   // And write that buffer... to a stream. Ho ho ho!
   s->write(buffer->GetBufferSize(), buffer->GetBufferPointer());
}

GBitmap *AtlasTexChunk::loadDDSIntoGBitmap(const U8 *ddsBuffer, U32 ddsBufferSize)
{
   D3DXIMAGE_INFO info;
   D3D9Assert(GFXD3DX.D3DXGetImageInfoFromFileInMemory(ddsBuffer, ddsBufferSize, &info),
      "AtlasTexChunk::loadDDSIntoGBitmap - failed to get image info.");

   IDirect3DSurface9 *surf = NULL;
   D3D9Assert(((GFXD3D9Device*)GFX)->getDevice()->CreateOffscreenPlainSurface(
      info.Width, info.Height, info.Format, D3DPOOL_SCRATCH, &surf, NULL), 
      "AtlasTexChunk::loadDDSIntoGBitmap - failed to allocate scratch surface.");

   // We want JPEGs, let's convert it in a klunky way...
   D3D9Assert(GFXD3DX.D3DXLoadSurfaceFromFileInMemory(surf, NULL, NULL, 
                                             ddsBuffer, ddsBufferSize, NULL,
                                             D3DX_DEFAULT, 0, NULL), 
                     "AtlasTexChunk::loadDDSIntoGBitmap - D3DX failed to load from buffer.");

   ID3DXBuffer *buff = NULL;
   D3D9Assert(GFXD3DX.D3DXSaveSurfaceToFileInMemory(&buff, D3DXIFF_PNG, surf, NULL, NULL), 
                     "AtlasTexChunk::loadDDSIntoGBitmap - D3DX failed to save back to buffer.");
   MemStream ms(buff->GetBufferSize(), buff->GetBufferPointer(), true, false);

   GBitmap *bitmap = new GBitmap[1];
   bitmap->readBitmap( "png", ms );

   // Cleanup!
   buff->Release();
   surf->Release();

   return bitmap;
}
