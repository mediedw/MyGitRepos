// InterviewTaskScheduler.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "InterviewTaskScheduler.h"

#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>
//  Include the task header file.
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

using namespace std;


//// This is an example of an exported variable
//INTERVIEWTASKSCHEDULER_API int nInterviewTaskScheduler=0;
//
//// This is an example of an exported function.
//INTERVIEWTASKSCHEDULER_API int fnInterviewTaskScheduler(void)
//{
//    return 42;
//}

// This is the constructor of a class that has been exported.
// see InterviewTaskScheduler.h for the class definition
CInterviewTaskScheduler::CInterviewTaskScheduler()
{
    return;
}

void CInterviewTaskScheduler::SetTaskName(WCHAR * taskName)
{
	wszTaskName = taskName;
}

void CInterviewTaskScheduler::SetTaskObject(WCHAR * taskObject)
{
	wszTaskObject = taskObject;
}

void CInterviewTaskScheduler::SetStartDateTime(WCHAR * startDateTime)
{
	wszStartDateTime = startDateTime;
}

void CInterviewTaskScheduler::SetEndDateTime(WCHAR * endDateTime)
{
	wszEndDateTime = endDateTime;
}

HRESULT CInterviewTaskScheduler::CreateTask(void)
{
	HRESULT hr = CreateTask(wszTaskName, wszTaskObject, wszStartDateTime, wszEndDateTime);
	return hr;
}

WCHAR * CInterviewTaskScheduler::GetTaskName()
{
	return (wchar_t*)(wszTaskName);
}

WCHAR * CInterviewTaskScheduler::GetTaskObject()
{
	return (wchar_t*)(wszTaskObject);
}

WCHAR * CInterviewTaskScheduler::GetTaskStartTime()
{
	return (wchar_t*)(wszStartDateTime);
}

WCHAR * CInterviewTaskScheduler::GetTaskEndTime()
{
	return (wchar_t*)(wszEndDateTime);
}

HRESULT CInterviewTaskScheduler::CreateTask(LPCWSTR wszTaskName, LPCWSTR wszTaskObject, LPCWSTR wszStartDateTime, LPCWSTR wszEndDateTime)
{
	// init COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("\nCoInitializeEx failed: %x", hr);
		return 1;
	}

	//  Set general COM security levels.
	hr = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		0,
		NULL);

	if (FAILED(hr))
	{
		printf("\nCoInitializeSecurity failed: %x", hr);
		CoUninitialize();
		return 1;
	}

	//  Get the windows directory and set the path to notepad.exe.
	wstring wstrExecutablePath = _wgetenv(L"WINDIR");
	wstrExecutablePath += L"\\SYSTEM32\\";
	// Add the notepad
	wstrExecutablePath += wszTaskObject;

	//  Create an instance of the Task Service. 
	ITaskService *pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&pService);
	if (FAILED(hr))
	{
		printf("Failed to create an instance of ITaskService: %x", hr);
		CoUninitialize();
		return 1;
	}

	//  Connect to the task service.
	hr = pService->Connect(_variant_t(), _variant_t(),
		_variant_t(), _variant_t());
	if (FAILED(hr))
	{
		printf("ITaskService::Connect failed: %x", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}

	//  This folder will hold the new task that is registered.
	ITaskFolder *pRootFolder = NULL;
	hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
	if (FAILED(hr))
	{
		printf("Cannot get Root folder pointer: %x", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}

	//  If the same task exists, remove it.
	pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);

	//  Create the task definition object to create the task.
	ITaskDefinition *pTask = NULL;
	hr = pService->NewTask(0, &pTask);

	pService->Release();  // COM clean up.  Pointer is no longer used.
	if (FAILED(hr))
	{
		printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
		pRootFolder->Release();
		CoUninitialize();
		return 1;
	}

	//  Get the registration info for setting the identification.
	IRegistrationInfo *pRegInfo = NULL;
	hr = pTask->get_RegistrationInfo(&pRegInfo);
	if (FAILED(hr))
	{
		printf("\nCannot get identification pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	hr = pRegInfo->put_Author(L"Author Name");
	pRegInfo->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put identification info: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Create the principal for the task
	IPrincipal *pPrincipal = NULL;
	hr = pTask->get_Principal(&pPrincipal);
	if (FAILED(hr))
	{
		printf("\nCannot get principal pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Set up principal logon 
	hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
	pPrincipal->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put principal info: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Create the settings for the task
	ITaskSettings *pSettings = NULL;
	hr = pTask->get_Settings(&pSettings);
	if (FAILED(hr))
	{
		printf("\nCannot get settings pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Set setting values for the task.  
	hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
	pSettings->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put setting information: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	// Set the idle settings for the task.
	IIdleSettings *pIdleSettings = NULL;
	hr = pSettings->get_IdleSettings(&pIdleSettings);
	if (FAILED(hr))
	{
		printf("\nCannot get idle setting information: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	hr = pIdleSettings->put_WaitTimeout(L"PT5M");
	pIdleSettings->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put idle setting information: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Get the trigger collection to insert the time trigger.
	ITriggerCollection *pTriggerCollection = NULL;
	hr = pTask->get_Triggers(&pTriggerCollection);
	if (FAILED(hr))
	{
		printf("\nCannot get trigger collection: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Add the time trigger to the task.
	ITrigger *pTrigger = NULL;
	hr = pTriggerCollection->Create(TASK_TRIGGER_TIME, &pTrigger);
	pTriggerCollection->Release();
	if (FAILED(hr))
	{
		printf("\nCannot create trigger: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	ITimeTrigger *pTimeTrigger = NULL;
	hr = pTrigger->QueryInterface(
		IID_ITimeTrigger, (void**)&pTimeTrigger);
	pTrigger->Release();
	if (FAILED(hr))
	{
		printf("\nQueryInterface call failed for ITimeTrigger: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	hr = pTimeTrigger->put_Id(_bstr_t(L"Trigger1"));
	if (FAILED(hr))
		printf("\nCannot put trigger ID: %x", hr);

	hr = pTimeTrigger->put_EndBoundary(_bstr_t(wszEndDateTime));
	if (FAILED(hr))
		printf("\nCannot put end boundary on trigger: %x", hr);

	//  Set the task to start at a certain time. The time 
	//  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).
	//  Use military time for afternoon for example 1pm is 13:00:00
	hr = pTimeTrigger->put_StartBoundary(_bstr_t(wszStartDateTime));
	pTimeTrigger->Release();
	if (FAILED(hr))
	{
		printf("\nCannot add start boundary to trigger: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Add an action to the task, in this case I am launching notepad.exe.     
	IActionCollection *pActionCollection = NULL;

	//  Get the task action collection pointer.
	hr = pTask->get_Actions(&pActionCollection);
	if (FAILED(hr))
	{
		printf("\nCannot get Task collection pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Create the action, specifying that it is an executable action.
	IAction *pAction = NULL;
	hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
	pActionCollection->Release();
	if (FAILED(hr))
	{
		printf("\nCannot create the action: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	IExecAction *pExecAction = NULL;
	hr = pAction->QueryInterface(
		IID_IExecAction, (void**)&pExecAction);
	pAction->Release();
	if (FAILED(hr))
	{
		printf("\nQueryInterface call failed for IExecAction: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Set the path of the executable to notepad.exe.
	hr = pExecAction->put_Path(_bstr_t(wstrExecutablePath.c_str()));
	pExecAction->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put action path: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Save the task in the root folder.
	IRegisteredTask *pRegisteredTask = NULL;
	hr = pRootFolder->RegisterTaskDefinition(_bstr_t(wszTaskName), pTask, TASK_CREATE_OR_UPDATE, _variant_t(), _variant_t(),
		TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(L""), &pRegisteredTask);
	if (FAILED(hr))
	{
		printf("\nError saving the Task : %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	printf("\n Success! Task successfully registered. ");

	//  Clean up.
	pRootFolder->Release();
	pTask->Release();
	pRegisteredTask->Release();
	CoUninitialize();
	return hr;
}
