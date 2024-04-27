#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;
bool continue_counting = true;
bool continue_running = true;
short timecount = 0;
char input;

HANDLE hProc = NULL;
HWND Window = NULL;
HHOOK hHook = NULL;
HANDLE hThread = NULL;
DWORD procID;

void Tcounting(){
  unsigned long Value = 0;
  while (continue_running){ 
    if (continue_counting){
      Value++;
      WriteProcessMemory(hProc,(BYTE*)0x0703A070,&Value,sizeof(Value),NULL);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void Twaitforstop(){
  char input;
  while (continue_running){
    input = _getch();
    if (input == 'x') continue_counting = !continue_counting;
    if (input == 'e') continue_running = false;
  } 
  CloseHandle(Window);
  CloseHandle(hProc);
}


void hack(){ 
  //const BYTE Y = 0x0A;
  const BYTE timer = 0x00;
    while (continue_running)
    {
      if (continue_counting){
        //WriteProcessMemory(hProc,(BYTE*)0x07039600,&Y,sizeof(Y),NULL);
        WriteProcessMemory(hProc,(BYTE*)0x07039663,&timer,sizeof(timer),NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
      }
      else std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
  std::cout << "working\n"; 

 
    Window = FindWindow(NULL,"PuyoVS");
 
    if(!Window){
        cout << "Window not found" << endl;
        return 1;
    }
    else {
        if (!GetWindowThreadProcessId(Window,&procID)){
            cerr << "Get window process ID error: " << GetLastError() << endl;
            return 1;
        }
    }
 
    if(procID != 0) hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
 
    if(!hProc){
        cerr << "Can not open process: " << GetLastError() << endl;
        return 2;
    }
    else{
        cout << "Process Found Ready to write\n" << endl;
    } 
  
  std::thread CounterThread (Tcounting);
  std::thread StopperThread (Twaitforstop);
  std::thread hackThread (hack);
 
  CounterThread.join();
  StopperThread.join();
  hackThread.join();
  std::cout << "ending";
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}
