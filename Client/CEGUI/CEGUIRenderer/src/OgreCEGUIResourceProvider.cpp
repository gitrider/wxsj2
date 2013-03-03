/************************************************************************
filename: 	OgreCEGUIResourceProvider.cpp
created:	8/7/2004
author:		James '_mental_' O'Sullivan

purpose:	Implements the Resource Provider common functionality
*************************************************************************/
/*************************************************************************
Crazy Eddie's GUI System (http://www.cegui.org.uk)
Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "OgreCEGUIResourceProvider.h"

#include <CEGUIExceptions.h>
#include <OgreArchiveManager.h>


// Start of CEGUI namespace section
namespace CEGUI
{
	//
	//
	//
	//	Data stream
	//
	//
	//
	class OgreCEGUIDataStream : public DataStream
	{
	public:
		OgreCEGUIDataStream(const Ogre::DataStreamPtr& ptr) 
			: d_ogreDataStream(ptr) {}
		~OgreCEGUIDataStream(){}

		/** Read the requisite number of bytes from the stream, 
		stopping at the end of the file.
		@param buf Reference to a buffer pointer
		@param count Number of bytes to read
		@returns The number of bytes read
		*/
		virtual size_t read(void* buf, size_t count)
		{
			return d_ogreDataStream->read(buf, count);
		}

		/** Repositions the read point to a specified byte.
		*/
		virtual void seek( size_t pos )
		{
			return d_ogreDataStream->seek(pos);
		}

		/** Returns the current byte offset from beginning */
		virtual size_t tell(void) const
		{
			return d_ogreDataStream->tell();
		}

		/** Returns true if the stream has reached the end.
		*/
		virtual bool eof(void) const
		{
			return d_ogreDataStream->eof();
		}

		/** Returns the total size of the data to be read from the stream, 
		or 0 if this is indeterminate for this stream. 
		*/
		virtual size_t size(void) const
		{
			return d_ogreDataStream->size();
		}

		/** Close the stream; this makes further operations invalid. */
		void close(void)
		{
			d_ogreDataStream->close();
		}

	protected:
		Ogre::DataStreamPtr	d_ogreDataStream;
	};

	//
	//
	//
	//	Resource provider
	//
	//
	//

	OgreCEGUIResourceProvider::OgreCEGUIResourceProvider() : ResourceProvider()
	{
		// set deafult resource group for Ogre
		d_defaultResourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME.c_str();
	}

	void OgreCEGUIResourceProvider::loadRawDataContainer(const String& filename, RawDataContainer& output,  const String& resourceGroup)
	{
		String orpGroup;
		if (resourceGroup.empty())
			orpGroup = d_defaultResourceGroup.empty() ? Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME.c_str() : d_defaultResourceGroup;
		else
			orpGroup = resourceGroup;

		Ogre::DataStreamPtr input = 
			Ogre::ResourceGroupManager::getSingleton().openResource(filename.c_str(), orpGroup.c_str());

		if (input.isNull())
		{
			throw InvalidRequestException(
				"OgreCEGUIResourceProvider::loadRawDataContainer - Unable to open resource file '" + filename + "' in resource group '" + orpGroup + "'.");
		}

		Ogre::String buf = input->getAsString();
		size_t buffsz = buf.length();
		unsigned char* mem = new unsigned char[buffsz];
		memcpy(mem, buf.c_str(), buffsz);

		output.setData(mem);
		output.setSize(buffsz);
	}

	void OgreCEGUIResourceProvider::unloadRawDataContainer(RawDataContainer& data)
	{
		if (data.getDataPtr())
		{
			delete[] data.getDataPtr();
			data.setData(0);
			data.setSize(0);
		}
	}

	DataStream* OgreCEGUIResourceProvider::openDataStream(const String& filename, const String& resourceGroup)
	{
		String orpGroup;
		if (resourceGroup.empty())
			orpGroup = d_defaultResourceGroup.empty() ? Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME.c_str() : d_defaultResourceGroup;
		else
			orpGroup = resourceGroup;

		Ogre::DataStreamPtr input = 
			Ogre::ResourceGroupManager::getSingleton().openResource(filename.c_str(), orpGroup.c_str());

		if (input.isNull())
		{
			throw InvalidRequestException(
				"OgreCEGUIResourceProvider::loadRawDataContainer - Unable to open resource file '" + filename + "' in resource group '" + orpGroup + "'.");
		}

		OgreCEGUIDataStream* pDataStream = new OgreCEGUIDataStream(input);

		return (DataStream*)pDataStream;
	}

	void OgreCEGUIResourceProvider::closeDataStream(DataStream* dataStream)
	{
		if(dataStream)
		{
			((OgreCEGUIDataStream*)dataStream)->close();
			delete ((OgreCEGUIDataStream*)dataStream);
		}
	}



	void OgreCEGUIResourceProvider::loadRawDataContainer2(const String& filename, RawDataContainer& output, const String& resourceGroup)
	{
		if (filename.empty())
		{
			throw InvalidRequestException(
				"DefaultResourceProvider::load - Filename supplied for data loading must be valid");
		}

		String final_filename(getFinalFilename(filename, resourceGroup));

		std::ifstream dataFile(final_filename.c_str(), std::ios::binary|std::ios::ate);
		if( dataFile.fail())
		{
			throw InvalidRequestException(
				"DefaultResourceProvider::load - " + filename + " does not exist");
		}
		std::streampos size = dataFile.tellg();
		dataFile.seekg (0, std::ios::beg);

		unsigned char* buffer = new unsigned char [size];

		try {
			dataFile.read(reinterpret_cast<char*>(buffer), size);
		}
		catch(std::ifstream::failure e) {
			delete [] buffer;
			throw GenericException(
				"DefaultResourceProvider::loadRawDataContainer - Problem reading " + filename);
		}

		dataFile.close();

		output.setData(buffer);
		output.setSize(size);
	}

	void OgreCEGUIResourceProvider::setResourceGroupDirectory(const String& resourceGroup, const String& directory)
	{
		d_resourceGroups[resourceGroup] = directory;
	}

	const String& OgreCEGUIResourceProvider::getResourceGroupDirectory(const String& resourceGroup)
	{
		return d_resourceGroups[resourceGroup];
	}

	void OgreCEGUIResourceProvider::clearResourceGroupDirectory(const String& resourceGroup)
	{
		ResourceGroupMap::iterator iter = d_resourceGroups.find(resourceGroup);

		if (iter != d_resourceGroups.end())
			d_resourceGroups.erase(iter);
	}

	String OgreCEGUIResourceProvider::getFinalFilename(const String& filename, const String& resourceGroup) const
	{
		String final_filename;

		// look up resource group directory
		ResourceGroupMap::const_iterator iter =
			d_resourceGroups.find(resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);

		// if there was an entry for this group, use it's directory as the
		// first part of the filename
		if (iter != d_resourceGroups.end())
			final_filename = (*iter).second;

		// append the filename part that we were passed
		final_filename += filename;

		// return result
		return final_filename;
	}


} // End of  CEGUI namespace section
