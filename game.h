#include <sstream>
#include <windows.h>
#include <vector>
#include <time.h>

#include <ios>
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <psapi.h>
#include <exception>
#include <winternl.h>
#include <iostream>
#include "Python.h"

using namespace std;
using std::vector;

struct Functions {
    // Functions
    char* name;
    DWORD address;
};

struct Python {
    // Modules
    char* name;
    vector<Functions>func;
};

struct Parameters {
    // Parameters
    vector<Python> &pyList; // init python modules list
    DWORD pid; // init PID from the process
};
class Game {
public:
    Game();
    ~Game();

    void init(vector<Python>);
    void start();
private:
    vector<Python> module;

    // Bot functions
    void AutoRedPotion(void *);
    void AutoBluePotion(void *);
    char* PlayerGetName();

    // Other functions
    int strpos(char *haystack, char *needle);

    // Python template functions
    typedef PyObject *(__stdcall * _HidePlayer)();
    _HidePlayer HidePlayer = 0;

    typedef PyObject *(__stdcall * _ShowPlayer)();
    _ShowPlayer ShowPlayer = 0;

    typedef PyObject* (__stdcall * _GetStatus)(PyObject * self, PyObject * args);
    _GetStatus GetStatus = 0;

    typedef PyObject* (__stdcall * _SendItemUsePacket)(PyObject * self, PyObject * args);
    _SendItemUsePacket SendItemUsePacket = 0;

    typedef PyObject* (__stdcall * _GetItemIndex)(PyObject * self, PyObject * args);
    _GetItemIndex GetItemIndex = 0;

    typedef PyObject* (__stdcall * _GetTargetVID)();
    _GetTargetVID GetTargetVID = 0;

    typedef PyObject* (__stdcall * _SendChatPacket)(PyObject * self, PyObject * args);
    _SendChatPacket SendChatPacket = 0;

    typedef PyObject* (__stdcall * _GetServerInfo)();
    _GetServerInfo GetServerInfo = 0;

    typedef PyObject* (__stdcall * _GetInstanceType)(PyObject * self, PyObject * args);
    _GetInstanceType GetInstanceType = 0;
};
