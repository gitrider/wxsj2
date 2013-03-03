//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLASEXPORTINTERFACES_H_
#define _ATLASEXPORTINTERFACES_H_

/// Interface for queries against the terrain dataset.
class ITerrainInfo
{
public:
   /// Return the minimum height of the heightfield.
   virtual float getMinHeight()=0;

   /// Return the maximum height of the heightfield.
   virtual float getMaxHeight()=0;

   /// Width of the heightfield.
   virtual int getWidth()=0;

   /// Height of the heightfield.
   virtual int getHeight()=0;

   /// Distance in meters between samples on the X or Y axes.
   virtual float getSampleSpacing()=0;

   /// Get the altitude at a given sample on the heightfield.
   virtual float getAltitude(int x, int y)=0;

   /// Called back with console output during the export process.
   virtual void handleLog(const char *str)=0;

   /// Initialize the progress bar.
   virtual void startProgress(const char *title)=0;

   /// Update progress bar status.
   ///
   /// @param  curAmt   Current number of completed work units.
   /// @param  totalAm  Total number of work units to do.
   /// @param  newTitle Task description to display. NULL if you want to leave
   ///                  unchanged.
   virtual void updateProgress(int curAmt, int totalAmt, const char *newTitle)=0;

   /// Indicate we're done with the progress bar.
   virtual void endProgress()=0;

   virtual bool hasMaterialFilenames() { return false; }
   virtual const char *getMaterialName(const int i) { return NULL; }

};

/// Interface for image sources. This is pretty naive, but it's very
/// flexible, and in practice, seems to run fast enough.
class IAtlasImageImportSource
{
public:

   /// Return the size of the image through width and height.
   virtual void getExtents(int &width, int &height)=0;

   /// Get an RGBA color from a given pixel in the image. Alpha may not be set.
   virtual void getColor(int x, int y, unsigned char &r, unsigned char &b, unsigned char &g, unsigned char &a)=0;

   /// Get the number of channels for this image; mainly useful if we're an
   /// opacity map that might have many channels.
   virtual int getNumberChannels()=0;

   /// And read out the channels for a location into an array of U8s equal
   /// in size to the number of reported channels.
   virtual void sampleChannels(int x, int y, unsigned char *out)=0;
};

/// Entry point for export operations.
///
/// @param outFile   .atlas file to create.
/// @param info      Terrain information interface.
/// @param src       Image source to use for a unique texture map on the
///                  terrain.
/// @param opacity   Image source for the opacity map. May be NULL. IF set
///                  we assume that src is a lightmap.
extern bool atlasGenerateFromAtlasInterface(const char *outFile, ITerrainInfo *info, 
                                            IAtlasImageImportSource *src, 
                                            IAtlasImageImportSource *opacity=NULL);

#endif
