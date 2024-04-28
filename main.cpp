/*
TODO:
  put the code that i copy-pasted into either a header or DLL
  make win32 window for interface instead of console
  get around ALSR for getting scoreAddress without manual searching
  figure out why the threads have a weird sleep time sometimes
*/
#define puyoYCoordOffset -2672
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include <math.h>
using namespace std;
bool continue_counting = true;
bool continue_running = true;
short timecount = 0;
char input;
unsigned long long scoreAddress; // use the address of the score for offsets

HANDLE hProc = NULL;
HWND Window = NULL;
HHOOK hHook = NULL;
HANDLE hThread = NULL;
DWORD procID;

inline void hexStringToValue(string s){
  short current;
  for (short i = 1; i < 8; i++){
    current = (short)s[i]-48;
    if (current > 9) current -= 7;
    scoreAddress += current * pow(16,(8-i-1));
    std::cout << "current letter: " << s[i] << " and its val: " << current;
    std::cout << "scoreAddress: " << scoreAddress << std::endl;
  }
  std::cout << " | converted to int -> " << scoreAddress << std::endl;
}

void Tcounting(){
  unsigned long Value = 0;
  while (continue_running){ 
    if (continue_counting){
      Value++;
      WriteProcessMemory(hProc,(BYTE*)scoreAddress,&Value,sizeof(Value),NULL);
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
  const BYTE Y = 0x0A;
  //const BYTE timer = 0x00; un-comment when not using the height reset
    while (continue_running)
    {
      if (continue_counting){
        WriteProcessMemory(hProc,(BYTE*)(scoreAddress+puyoYCoordOffset),&Y,sizeof(Y),NULL);
        //WriteProcessMemory(hProc,(BYTE*)0x07039663,&timer,sizeof(timer),NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
      }
      else std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
  std::cout << "working\n"; 

 //start of obviously copy-pasted code from nullbyte 
  Window = FindWindow(NULL,"PuyoVS");
 
  if(!Window){
    std::cout << "Window not found" << std::endl;
    return 1;
  }
  else {
    if (!GetWindowThreadProcessId(Window,&procID)){
      std::cerr << "Get window process ID error: " << GetLastError() << std::endl;
      return 1;
    }
  }
  if(procID != 0) hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
  if(!hProc){
    std::cerr << "Can not open process: " << GetLastError() << std::endl;
    return 2;
  }
  else std::cout << "Process Found Ready to write\n" << std::endl;
  //end of copied code
  
  std::cout << "address of score: ";
  string hexString = "";
  std::cin >> hexString;
  hexStringToValue(hexString);
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
