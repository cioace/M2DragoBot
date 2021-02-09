#include "Game.h"

#define playerHP  5
#define playerMAX_HP  6
#define playerHP_RECOVERY 35

#define playerSP    7
#define playerMAX_SP    8
#define playerSP_RECOVERY 36

#define playerINVENTORY_PAGE 4
#define playerINVENTORY_PAGE_SIZE 45
#define playerNORMAL_CHAT   0
#define playerALL_CHAT  1
#define chrINSTANCE_TYPE_PLAYER    6

Game::Game() {
}

Game::~Game() {
}

void Game::init(vector<Python> module) {
    this->module = module;
}

int Game::strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;
}

void Game::AutoBluePotion(void* params) {
    Game *This = (Game*)params;

    if(!Py_IsInitialized()) {
        Py_Initialize();
    }

    while(true) {
        PyObject *args = Py_BuildValue("(i)", playerMAX_HP);
        int maxSP = PyInt_AsLong(This->GetStatus(NULL, args));

        args = Py_BuildValue("(i)", playerSP);
        int SP = PyInt_AsLong(This->GetStatus(NULL, args));

        args = Py_BuildValue("(i)", playerSP_RECOVERY);
        int SP_RECOVERY = PyInt_AsLong(This->GetStatus(NULL, args));

        int curPoint = min(SP, maxSP);
        curPoint = max(SP, 0);
        int maxPoint = max(maxSP, 0);
        if( curPoint / max(1, maxPoint) * 100 < 80 && SP_RECOVERY == 0 ) {
            for(int i=0; i<playerMAX_SP * playerINVENTORY_PAGE; i++) {
                args = Py_BuildValue("(i)", i);
                int itemVNum = PyInt_AsLong(This->GetItemIndex(NULL, args));
                if(itemVNum == 27004 || itemVNum == 27005 || itemVNum == 27006) {
                    args = Py_BuildValue("(i)", i);
                    This->SendItemUsePacket(NULL, args);
                }
            }
        }
        Sleep(800); // Increase value because is possible to give crash from the client, because the function is called so many times
    }

    if(!Py_IsInitialized()) {
        Py_Finalize();
    }
}

void Game::AutoRedPotion(void* params) {
    Game *This = (Game*)params;

    if(!Py_IsInitialized()) {
        Py_Initialize();
    }

    while(true) {
        PyObject *args = Py_BuildValue("(i)", playerMAX_HP);
        int maxHP = PyInt_AsLong(This->GetStatus(NULL, args));

        args = Py_BuildValue("(i)", playerHP);
        int HP = PyInt_AsLong(This->GetStatus(NULL, args));

        args = Py_BuildValue("(i)", playerHP_RECOVERY);
        int HP_RECOVERY = PyInt_AsLong(This->GetStatus(NULL, args));

        int curPoint = min(HP, maxHP);
        curPoint = max(HP, 0);
        int maxPoint = max(maxHP, 0);
        if( curPoint / max(1, maxPoint) * 100 < 80 && HP_RECOVERY == 0 ) {
            for(int i=0; i<playerMAX_HP * playerINVENTORY_PAGE; i++) {
                args = Py_BuildValue("(i)", i);
                int itemVNum = PyInt_AsLong(This->GetItemIndex(NULL, args));
                if(itemVNum == 27001 || itemVNum == 27002 || itemVNum == 27003 || itemVNum == 27051) {
                    args = Py_BuildValue("(i)", i);
                    This->SendItemUsePacket(NULL, args);
                }
            }
        }
        Sleep(800); // Increase value because is possible to give crash from the client, because the function is called so many times
    }

    if(!Py_IsInitialized()) {
        Py_Finalize();
    }
}

void Game::start() {
    if(!Py_IsInitialized()) {
        Py_Initialize();
    }

    for(int i=0; i<this->module.size(); i++) {
        for(int j=0; j<this->module[i].func.size(); j++) {
            if(this->strpos(this->module[i].func[j].name, "HidePlayer") != -1 && this->HidePlayer == 0) {
                this->HidePlayer = (_HidePlayer)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "ShowPlayer") != -1 && this->ShowPlayer == 0) {
                this->ShowPlayer = (_ShowPlayer)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "GetStatus") != -1 && this->GetStatus == 0) {
                this->GetStatus = (_GetStatus)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "SendChatPacket") != -1 && this->SendChatPacket == 0) {
                this->SendChatPacket = (_SendChatPacket)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "GetTargetVID") != -1 && this->GetTargetVID == 0) {
                this->GetTargetVID = (_GetTargetVID)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "GetInstanceType") != -1 && this->GetInstanceType == 0) {
                this->GetInstanceType = (_GetInstanceType)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "GetServerInfo") != -1 && this->GetServerInfo == 0) {
                this->GetServerInfo = (_GetServerInfo)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "GetItemIndex") != -1 && this->GetItemIndex == 0) {
                this->GetItemIndex = (_GetItemIndex)this->module[i].func[j].address;
            }

            if(this->strpos(this->module[i].func[j].name, "SendItemUsePacket") != -1 && this->SendItemUsePacket == 0) {
                this->SendItemUsePacket = (_SendItemUsePacket)this->module[i].func[j].address;
            }
        }
    }

    bool status_player = false;
    bool bot_potion_red = false;
    bool bot_potion_blue = false;

    DWORD bot_potion_red_id = 0;
    DWORD bot_potion_blue_id = 0;

    while(true) {
        cout<<"Dragos bot activated"<<endl;
        cout<<"Keyboard: F1 - Player status"<<endl; // Is possible for other clients to haven't this HidePlayer|ShowPlayer, so for that clients this function doesn't work
        cout<<"Keyboard: F2 - Auto red potion bot -> "<<bot_potion_red<<endl;
        cout<<"Keyboard: F3 - Auto blue potion bot -> "<<bot_potion_blue<<endl;
        cout<<"Keyboard: F4 - Chat message"<<endl;
        cout<<"Keyboard: F5 - Get ID from target"<<endl;

        if(GetAsyncKeyState(VK_F1) & 1) {
            if(status_player) {
                this->HidePlayer();
                status_player = false;
                Sleep(100);
            } else {
                this->ShowPlayer();
                status_player = true;
                Sleep(100);
            }
        }

        if(GetAsyncKeyState(VK_F2) & 1 && (this->GetItemIndex != 0 && this->SendItemUsePacket != 0 && this->GetStatus != 0)) {
            if(!bot_potion_red) {
                bot_potion_red = true;
                CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Game::AutoRedPotion, this, 0, &bot_potion_red_id);
                Sleep(100);
            } else {
                bot_potion_red = false;
                HANDLE thread = OpenThread(THREAD_ALL_ACCESS, 0, bot_potion_red_id);
                TerminateThread(thread, 0);
                Sleep(100);
            }
        }

        if(GetAsyncKeyState(VK_F3) & 1 && (this->GetItemIndex != 0 && this->SendItemUsePacket != 0 && this->GetStatus != 0)) {
            if(!bot_potion_blue) {
                bot_potion_blue = true;
                CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Game::AutoBluePotion, this, 0, &bot_potion_blue_id);
                Sleep(100);
            } else {
                bot_potion_blue = false;
                HANDLE thread = OpenThread(THREAD_ALL_ACCESS, 0, bot_potion_blue_id);
                TerminateThread(thread, 0);
                Sleep(100);
            }
        }

        if(GetAsyncKeyState(VK_F5) & 1 && (this->GetTargetVID != 0)) {
            std::ostringstream stream;
            stream<<hex<<PyLong_AsLong(GetTargetVID());
            std::string x_str = stream.str();
            MessageBox(0, stream.str().c_str(), 0, 0);
        }

        if(GetAsyncKeyState(VK_F4) & 1 && (this->SendChatPacket != 0)) {
            PyObject *args = Py_BuildValue("(si)", "Dragos test message! Work 2021!", playerNORMAL_CHAT);
            this->SendChatPacket(NULL, args);
        }

        if(this->GetServerInfo != 0) {
            cout<<"SERVER: "<<PyString_AsString(this->GetServerInfo())<<endl;
        } else {
            cout<<"Addr of GetServerInfo is not founded!"<<endl;
        }
        system("cls");
    }

    if(!Py_IsInitialized()) {
        Py_Finalize();
    }
}
