#include "FairyPlacementObject.h"
#include "FairySystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace Fairy
{
    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct PositionCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->mTransform.position;
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->mTransform.position);
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.position = VariantCast<Ogre::Vector3>(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setPosition(object->mTransform.position);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.position = Ogre::StringConverter::parseVector3(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setPosition(object->mTransform.position);
            }
        };

		struct OrientationCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->mTransform.orientation;
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->mTransform.orientation);
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.orientation = VariantCast<Ogre::Quaternion>(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setOrientation(object->mTransform.orientation);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.orientation = Ogre::StringConverter::parseQuaternion(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setOrientation(object->mTransform.orientation);
            }
        };

		// 欧拉角到四元数的转换步骤： 欧拉角->矩阵->四元数
		// 四元数到欧拉角的转换步骤： 四元数->矩阵->欧拉角
		struct OrientationXYZCmd : PropertyCommand
		{
			Variant doGet(const void* source) const
			{
				const PlacementObject* object = static_cast<const PlacementObject*>(source);
				Ogre::Radian radianX, radianY, radianZ;
				Ogre::Matrix3 mtemp;
				object->mTransform.orientation.ToRotationMatrix(mtemp);
				mtemp.ToEulerAnglesXYZ(radianX, radianY, radianZ);
				Ogre::Vector3 vectorXYZ(radianX.valueDegrees(), radianY.valueDegrees(), radianZ.valueDegrees());
				return vectorXYZ;
			}

			Ogre::String doGetAsString(const void* source) const
			{
				const PlacementObject* object = static_cast<const PlacementObject*>(source);
				Ogre::Radian radianX, radianY, radianZ;
				Ogre::Matrix3 mtemp;
				object->mTransform.orientation.ToRotationMatrix(mtemp);
				mtemp.ToEulerAnglesXYZ(radianX, radianY, radianZ);
				Ogre::Vector3 vectorXYZ(radianX.valueDegrees(), radianY.valueDegrees(), radianZ.valueDegrees());
				return Ogre::StringConverter::toString(vectorXYZ);
			}

			void doSet(void* target, const Variant& value)
			{
				PlacementObject* object = static_cast<PlacementObject*>(target);
				Ogre::Vector3 vectorOrient = VariantCast<Ogre::Vector3>(value);
				Ogre::Degree degreeX(vectorOrient.x), degreeY(vectorOrient.y), degreeZ(vectorOrient.z);
				Ogre::Radian radianX(degreeX), radianY(degreeY), radianZ(degreeZ);
				Ogre::Matrix3 mtemp;
				mtemp.FromEulerAnglesXYZ(radianX, radianY, radianZ);
				object->mTransform.orientation.FromRotationMatrix(mtemp);
				if (object->getSceneNode())
					object->getSceneNode()->setOrientation(object->mTransform.orientation);
			}

			void doSetAsString(void* target, const Ogre::String& value)
			{
				PlacementObject* object = static_cast<PlacementObject*>(target);
				Ogre::Vector3 vectorOrient = Ogre::StringConverter::parseVector3(value);
				Ogre::Degree degreeX(vectorOrient.x), degreeY(vectorOrient.y), degreeZ(vectorOrient.z);
				Ogre::Radian radianX(degreeX), radianY(degreeY), radianZ(degreeZ);
				Ogre::Matrix3 mtemp;
				mtemp.FromEulerAnglesXYZ(radianX, radianY, radianZ);
				object->mTransform.orientation.FromRotationMatrix(mtemp);
				if (object->getSceneNode())
					object->getSceneNode()->setOrientation(object->mTransform.orientation);
			}
		};

        struct ScaleCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->mTransform.scale;
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->mTransform.scale);
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.scale = VariantCast<Ogre::Vector3>(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setScale(object->mTransform.scale);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.scale = Ogre::StringConverter::parseVector3(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setScale(object->mTransform.scale);
            }
        };

        struct CreateLevelCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->getCreateLevel();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->getCreateLevel());
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                Ogre::Real createLevel = VariantCast<Ogre::Real>(value);
                object->setCreateLevel(createLevel);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                Ogre::Real createLevel = Ogre::StringConverter::parseReal(value);
                object->setCreateLevel(createLevel);
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // PlacementObject class
    //////////////////////////////////////////////////////////////////////////


    PlacementObject::PlacementObject(void)
        : Object()
        , mSceneNode(NULL)
        , mCreateLevel(50.0f)
        , mSystem(NULL)
    {
    }
    //-----------------------------------------------------------------------
    PlacementObject::~PlacementObject()
    {
        if (mSceneNode)
        {
            mSceneNode->getCreator()->destroySceneNode(mSceneNode);
			mSceneNode = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mSceneNode);

        mSceneNode = system->getBaseSceneNode()->createChildSceneNode(
            mTransform.position,
            mTransform.orientation);
        mSceneNode->setScale(mTransform.scale);
        mSceneNode->setInitialState();

        mSystem = system;
    }
    //-----------------------------------------------------------------------
    void PlacementObject::destroyRenderInstance(void)
    {
        if (mSceneNode)
        {
            mSceneNode->getCreator()->destroySceneNode(mSceneNode);
            mSceneNode = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::processMessage(const Ogre::String& msg, const Variant& params)
    {
        if (msg == System::CREATE_LEVEL_CHANGED_MSG)
        {
            _updateCreateLevel();
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::setCreateLevel(Ogre::Real level)
    {
        if (mCreateLevel != level)
        {
            mCreateLevel = level;
            _updateCreateLevel();
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::_updateCreateLevel(void)
    {
        if (mSceneNode)
        {
            mSceneNode->setVisible( mSystem->_determineCreateLevel(mCreateLevel) );
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::addBaseProperty(void)
    {
        static PositionCmd positionCmd;
        defineProperty(
            "position",
            "The position of this object.",
            "Position",
            &positionCmd,
            0);

        static OrientationCmd orientationCmd;
        defineProperty(
            "orientation",
            "The orientation of this object.",
            "Orientation",
            &orientationCmd,
            0);

		static OrientationXYZCmd orientationXYZCmd;
		defineProperty(
			"orientationXYZ",
			"The orientationXYZ of this object.",
			"OrientationXYZ",
			&orientationXYZCmd,
			0);

        static ScaleCmd scaleCmd;
        defineProperty(
            "scale",
            "The scaling factor applied to this object.",
            "Scale",
            &scaleCmd,
            0);

        static CreateLevelCmd createLevelCmd;
        defineProperty(
            "create level",
            "The create level of this object.",
            "Ogre::Real",
            &createLevelCmd,
            0);
    }
    //-----------------------------------------------------------------------
    Variant PlacementObject::getProperty(const Ogre::String& name) const
    {
        if (name == "grounding point")
        {
            // Using model centre as grounding point
            return Ogre::Vector3::ZERO;
        }

        return Object::getProperty(name);
    }
    //-----------------------------------------------------------------------
   Ogre::String PlacementObject::getPropertyAsString(const Ogre::String& name) const
    {
        if (name == "grounding point")
        {
            Ogre::Vector3 groundingPoint = VariantCast<Ogre::Vector3>(getProperty(name));
            return Ogre::StringConverter::toString(groundingPoint);
        }

        return Object::getPropertyAsString(name);
    }
}
