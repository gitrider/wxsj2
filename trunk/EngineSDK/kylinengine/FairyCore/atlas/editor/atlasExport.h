//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLAS_EDITOR_ATLASEXPORT_H_
#define _ATLAS_EDITOR_ATLASEXPORT_H_

/// Helper class to hook back into export feedback mechanisms from the Atlas
/// codebase.
class AtlasExport
{
public:
   /// Update a progress bar, if one is present.
   ///
   /// @param current  Current units of work done.
   /// @param total    Total units of work to do.
   /// @param newTitle Current task description (NULL to leave as previous).
   static void updateProgress(S32 current, S32 total, const char *newTitle);

   /// Function pointer to call back with status updates.. You probably won't set this.
   static void (*smProgressCallback)(S32 current, S32 total, const char *newTitle);
};

#endif
