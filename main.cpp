#include "main.h"

// Define template function
typedef void (*_GetModules)(Parameters &);
_GetModules GetModules = 0;

VOID CreateConsole(VOID)
{
	int  hConHandle;
	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	AllocConsole();

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = 1500;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	std::ios::sync_with_stdio();
}

BOOL WINAPI main()
{
    // Init variables
    vector<Python> pyList;
    Parameters params = {pyList, GetCurrentProcessId()}; // First parameter is python list and second parameter is PID from metin2client

    // Load library
    HMODULE module = LoadLibrary("M2DragoDumperLibrary.dll");

    // Set address to template function
    GetModules = (_GetModules) GetProcAddress(module, "GetModules");

    if (GetModules == 0) {
        MessageBox(0, "M2DragoDumperLibrary.dll is not loaded!", 0, 0);
        return 0;
    }

    // Create console
    CreateConsole();

    // Message
    cout<<"Wait until the program finish work!"<<endl;

    // Get modules and functions from "DragoLibrary"
    GetModules(params); // We need to call function to write name of python functions and address to our list
    /*for(unsigned int i=0; i<pyList.size(); i++) {
        cout<<"--- module : "<<pyList[i].name<<" ----\n"; // this is name of module
        for(unsigned int j=0; j<pyList[i].func.size(); j++) {
            cout<<pyList[i].func[j].name<<" - "<<uppercase<<hex<<pyList[i].func[j].address<<"\n"; // for each module we show all functions
        }
        cout<<"---------------------------------------\n";
    }*/

    Game game = Game();
    game.init(pyList);
    game.start();

    return 0;
}



extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
