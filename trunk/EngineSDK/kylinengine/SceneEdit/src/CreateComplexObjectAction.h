#ifndef __CREATECOMPLEXOBJECTACTION_H__
#define __CREATECOMPLEXOBJECTACTION_H__

#include "Action.h"

#include "Core/FairyObject.h"
namespace Fairy
{

	/** ������CreateObjectAction������ֻ�е�����������ʱ�������ش������壬������������ƶ���ʱ��Ҳ���������£�
		���壬��Ϊ�����action������������һ�㶼�ǱȽϸ��ӵģ���ˮ��������Ե�����
	*/
	class CreateComplexObjectAction : public Action
	{
	protected:
		String mObjectType;
		Fairy::PropertyNameStringPairList mParameters;
		bool mDirty;
		ObjectPtr mObject;

	public:
		CreateComplexObjectAction(SceneManipulator* sceneManipulator);
		~CreateComplexObjectAction();

		const String& getName(void) const;

		void setParameter(const String& name, const String& value);
		String getParameter(const String& name) const;

	protected:
		void _onActive(bool active);

		void _onMove(const Point& pt);
		void _onBegin(const Point& pt);
		void _onDrag(const Point& pt);
		void _onEnd(const Point& pt, bool canceled);

	protected:
		void hide(void);
		bool updateInstance(const Point& pt);
	};

}

#endif // __CreateComplexObjectAction_H__
