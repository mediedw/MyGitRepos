// InterviewTaskSchedulerClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "..\InterviewTaskScheduler\InterviewTaskScheduler.h"

using namespace std; 

int main()
{
	HRESULT hr = S_OK; 

	// create a task name
	WCHAR* wszTaskName = L"My Interview Task";			// Task needs a name
	WCHAR* wszTask = L"NotePad.exe";					// for this test I will launch NotePad at the specified time
	WCHAR* wszStartDateTime = L"2017-07-30T14:50:00";	// Starts july 30, 2017 at 2pm - use 24hr military time
	WCHAR* wszEndDateTime = L"2017-08-02T08:00:00";		// ends Aug 2, 2017 at 8am

	CInterviewTaskScheduler tskScheduler; 
	// pass in some needed data
	tskScheduler.SetTaskName(wszTaskName);
	tskScheduler.SetTaskObject(wszTask);
	tskScheduler.SetStartDateTime(wszStartDateTime);
	tskScheduler.SetEndDateTime(wszEndDateTime);

	// create the task!
	hr = tskScheduler.CreateTask(); 
	if (hr == S_OK)
	{
		cout << "Task creation successfull!" << endl; 
	}
	else
	{
		cout << "Task creation failed miserably!" << endl; 
	}


    return 0;
}

