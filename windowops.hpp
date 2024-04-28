#include <iostream>
#include <windows.h>
using namespace std;
class windowops{
  public:
    static bool gameWindowOps(HANDLE& hProc, HWND& gameWindow, DWORD& procID){
      gameWindow = FindWindow(NULL,"PuyoVS");
 
      if(!gameWindow){
        std::cout << "Window not found" << std::endl;
        return 1;
      }
      else {
        if (!GetWindowThreadProcessId(gameWindow,&procID)){
          std::cerr << "Get window process ID error: " << GetLastError() << std::endl;
          return 1;
        }
      }
      if(procID != 0) hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
      if(!hProc){
        std::cerr << "Can not open process: " << GetLastError() << std::endl;
        return 1;
      }
      else std::cout << "Process Found Ready to write\n" << std::endl; 
      return 0;
    }
/*      idk what the ms docs are trying to do with this weird deprecated code??
    static bool createInterface(){
      const wchar_t CLASS_NAME[] = L"Sample Window Class";
      WNDCLASS wc = { };
      //wc.lpfnWndProc = WindowProc;
      //wc.hInstance = hInstance;
      wc.lpszClassName = CLASS_NAME;
      RegisterClass(&wc);
  
      HWND hInterface = CreateWindowEX( //return handle to window
          0, 
          CLASS_NAME, 
          L"hello", 
          WS_OVERLAPPEDWINDOW, 
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
          NULL,
          NULL,
          NULL, // remove this line for next line
          //hInstance,
          NULL
          );
      if (hInterface == NULL) return 1;

      ShowWindow(hInterface, nCmdShow);
      return 0;
    }*/
};
