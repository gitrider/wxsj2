
#include "FalStopWatch.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardStopWatch
	//
	//===================================================================================
	const utf8  FalagardStopWatch::WidgetTypeName[] = "Falagard/StopWatch";

	// Event Strings
	const String FalagardStopWatch::EventNamespace("Falagard/FalagardStopWatch");

	const String FalagardStopWatch::EventReachedZero("ReachedZero");

	FalagardStopWatchProperties::Step			FalagardStopWatch::d_stepProperty;
	FalagardStopWatchProperties::Timer			FalagardStopWatch::d_timeProperty;
	FalagardStopWatchProperties::HourText 		FalagardStopWatch::d_hourTextProperty;
	FalagardStopWatchProperties::DayText 		FalagardStopWatch::d_dayTextProperty;

	FalagardStopWatch::FalagardStopWatch(const String& type, const String& name) :
	FalagardSimpleText(type, name),
		d_currentTime(0),
		d_timeStep(-1)
	{
		addStopWatchProperties();

		//utf-8 "小时"
		const char* GB_HOUR = "\xE5\xB0\x8F\xE6\x97\xB6";
		//utf-8 "天"
		const char* GB_DAY = "\xE5\xA4\xA9";

		m_strHourText = (utf8*)GB_HOUR;
		m_strDayText = (utf8*)GB_DAY;

		f_lastSecond = System::getSingleton().getCurTimeElapsed();
	}

	FalagardStopWatch::~FalagardStopWatch()
	{
	}

	void FalagardStopWatch::setHourText(const String& hourText)
	{
		m_strHourText = (utf8*)(hourText.c_str());
	}

	void FalagardStopWatch::setDayText(const String& dayText)
	{
		m_strDayText = (utf8*)(dayText.c_str());
	}

	void FalagardStopWatch::addStopWatchProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardStopWatch )
			CEGUI_ADD_STATICPROPERTY( &d_stepProperty );
		CEGUI_ADD_STATICPROPERTY( &d_timeProperty );
		CEGUI_ADD_STATICPROPERTY( &d_hourTextProperty );
		CEGUI_ADD_STATICPROPERTY( &d_dayTextProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	void FalagardStopWatch::setStep(int step)
	{
		d_timeStep = step;
		requestRedraw();
	}

	void FalagardStopWatch::setTimer(int time)
	{
		d_currentTime = time;
		f_lastSecond = System::getSingleton().getCurTimeElapsed();
		if(d_currentTime == 0 || d_currentTime < -1) 
		{
			setText("");
			requestRedraw();		
			return;
		}
		char value[128];
		int hour;
		int minute;
		int second;
		int	day;
		day = (int)(d_currentTime/86400);
		hour =  d_currentTime/3600;
		minute = (d_currentTime%3600)/60;
		second = (d_currentTime%3600)%60; // - (60 * minute)//精确校正值 +1

		if(day == 0)
		{
			if( hour == 0 )
			{
				if(second < 10)
					_snprintf(value, 128, "%d:%d%d", minute,0,second);
				else
					_snprintf(value, 128, "%d:%d", minute,second);
			}
			else
			{
				_snprintf(value, 128, "%d%s", hour, m_strHourText.c_str());
			}
		}
		else
		{
			_snprintf(value, 128, "%d%s", day,m_strDayText.c_str());
		}
		setText((utf8*)value);
		requestRedraw();
	}

	void FalagardStopWatch::updateSelf(float elapsed)
	{
		if(d_currentTime == 0 || d_currentTime < -1) 
		{
			//			char value[128];
			//			_snprintf(value, 128, "0:%d", d_currentTime);
			setText("");
			requestRedraw();		
			return;
		}
		if(d_currentTime == -1 )
		{
			//			char value[128];
			//			_snprintf(value, 128, "1:%d", d_currentTime);
			setText("N/A");
			//			setText(value);
			requestRedraw();
			return;
		}
		float time_now = System::getSingleton().getCurTimeElapsed();

		//
		// Calculate the time
		//
		if( time_now - f_lastSecond >= 1.0f )
		{
			d_currentTime += d_timeStep*((int)(time_now-f_lastSecond));
			f_lastSecond = time_now;

			char value[128];
			int hour;
			int minute;
			int second;
			int	day;

			int show_time = d_currentTime; //- d_timeStep;// - (60 * minute)//精确校正值 +1

			day = (int)(show_time/86400);
			hour =  show_time/3600;
			minute = (show_time%3600)/60;
			second = (show_time%3600)%60;

			if(day == 0)
			{
				if( hour == 0 )
				{
					if(second < 10)
						_snprintf(value, 128, "%d:%d%d", minute,0,second);
					else
						_snprintf(value, 128, "%d:%d", minute,second);
				}
				else
				{
					//				_snprintf(value, 128, "%d:%d:%d", hour, minute,second);
					_snprintf(value, 128, "%d%s", hour, m_strHourText.c_str());
				}
			}
			else
			{
				_snprintf(value, 128, "%d%s", day,m_strDayText.c_str());
			}

			//首次到达0，产生事件
			if(d_currentTime <= 0)
			{
				WindowEventArgs args(this);
				fireEvent(EventReachedZero, args, EventNamespace);
			}

			setText((utf8*)value);

			requestRedraw();
		}

		FalagardSimpleText::updateSelf(elapsed);
	}

	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardStopWatchFactory::createWindow(const String& name)
	{
		return new FalagardStopWatch(d_type, name);
	}

	void FalagardStopWatchFactory::destroyWindow(Window* window)
	{
		delete window;
	}

}