#include "stdafx.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include "..\InterviewTaskScheduler\InterviewTaskScheduler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InterviewTaskSchedulerUnitTest
{		
	TEST_CLASS(InterviewTskSchedUnitTest)
	{
	public:
		
		TEST_METHOD(SetTaskNameUnitTest)
		{
			CInterviewTaskScheduler tskScheduler; 

			WCHAR* wszTaskName = L"My Interview Task";	// expected value
			tskScheduler.SetTaskName(wszTaskName);

			Assert::AreEqual(wszTaskName, tskScheduler.wszTaskName);
		}

		TEST_METHOD(SetTaskObjectUnitTest)
		{
			CInterviewTaskScheduler tskScheduler;

			WCHAR* wszTask = L"NotePad.exe";	// expected value
			tskScheduler.SetTaskObject(wszTask);

			Assert::AreEqual(wszTask, tskScheduler.wszTaskObject);
		}

		TEST_METHOD(SetStartDateTimeUnitTest)
		{
			CInterviewTaskScheduler tskScheduler;

			WCHAR* wszStartDateTime = L"2017-07-30T14:50:00";	// expected value
			tskScheduler.SetStartDateTime(wszStartDateTime);

			Assert::AreEqual(wszStartDateTime, tskScheduler.wszStartDateTime);
		}

		TEST_METHOD(SetEndDateTimeUnitTest)
		{
			CInterviewTaskScheduler tskScheduler;

			WCHAR* wszEndDateTime = L"2017-08-02T08:00:00";	// expected value
			tskScheduler.SetEndDateTime(wszEndDateTime);

			Assert::AreEqual(wszEndDateTime, tskScheduler.wszEndDateTime);
		}
	};
}