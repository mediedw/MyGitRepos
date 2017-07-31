// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INTERVIEWTASKSCHEDULER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INTERVIEWTASKSCHEDULER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INTERVIEWTASKSCHEDULER_EXPORTS
#define INTERVIEWTASKSCHEDULER_API __declspec(dllexport)
#else
#define INTERVIEWTASKSCHEDULER_API __declspec(dllimport)
#endif

// This class is exported from the InterviewTaskScheduler.dll
class INTERVIEWTASKSCHEDULER_API CInterviewTaskScheduler {
public:
	CInterviewTaskScheduler(void);
	
	void SetTaskName(WCHAR* taskName);

	void SetTaskObject(WCHAR* taskObject);

	void SetStartDateTime(WCHAR* startDateTime);

	void SetEndDateTime(WCHAR* endDateTime);

	HRESULT CreateTask(void);

	WCHAR* GetTaskName();

	WCHAR* GetTaskObject(); 

	WCHAR* GetTaskStartTime(); 

	WCHAR* GetTaskEndTime();

	LPCWSTR wszTaskName;
	LPCWSTR wszTaskObject;
	LPCWSTR wszStartDateTime;
	LPCWSTR wszEndDateTime;

private:
	

	HRESULT CreateTask(LPCWSTR wszTaskName, LPCWSTR wszTaskObject, LPCWSTR wszStartDateTime, LPCWSTR wszEndDateTime);
};

//extern INTERVIEWTASKSCHEDULER_API int nInterviewTaskScheduler;
//
//INTERVIEWTASKSCHEDULER_API int fnInterviewTaskScheduler(void);
