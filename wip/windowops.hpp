#include <corecrt_math.h>
#include <ios>
#include <iostream>
#include <vector>
#include <windows.h>
#include <xlocmon>
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
    


    //function copied from chatgpt because i couldnt find anything else
    static void searchMemory(DWORD processId) {
      int valueToSearch;
      HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
      if (hProcess == NULL) {
          std::cerr << "Failed to open process. Error code: " << GetLastError() << std::endl;
          return;
      }

      SYSTEM_INFO sysInfo;
      GetSystemInfo(&sysInfo);

      MEMORY_BASIC_INFORMATION memInfo;
      unsigned char* address = 0;
      std::cout << "valueToSearch: ";
      std::cin >> valueToSearch;
      while (address < sysInfo.lpMaximumApplicationAddress) {
        std::cout << "\ndoing the big while loop\n";
        if (VirtualQueryEx(hProcess, address, &memInfo, sizeof(memInfo)) == sizeof(memInfo)) {
          if (memInfo.State == MEM_COMMIT && (memInfo.Type == MEM_MAPPED || memInfo.Type == MEM_PRIVATE)) {
            unsigned char* buffer = new unsigned char[memInfo.RegionSize];
            std::vector<unsigned long long> matched;
            SIZE_T bytesRead;

            if (ReadProcessMemory(hProcess, memInfo.BaseAddress, buffer, memInfo.RegionSize, &bytesRead)) {
              for (size_t i = 0; i < bytesRead; ++i) {
                if (*(int*)(buffer + i) == valueToSearch) {
                  std::cout << "Found at address: 0x" << std::hex << (uintptr_t)(memInfo.BaseAddress) + i << std::endl;
                  matched.push_back(i);
                }
              }
              delete[] buffer;
              while (matched.size() > 3){
                std::cout << "doing matched.size > 3 loop\n";
                unsigned char* buffer = new unsigned char[memInfo.RegionSize];
                if (ReadProcessMemory(hProcess, memInfo.BaseAddress, buffer, memInfo.RegionSize, &bytesRead)){
                  std::cout << "give new score: ";
                  std::cin>>valueToSearch;
                  for (int i = 0; i < matched.size(); i++){
                    if (*(int*)(buffer + matched[i]) != valueToSearch){
                      matched.erase(matched.begin()+matched[i]);
                      i--;
                    }
                  }
                

                }
                else {
                  std::cerr << "ReadProcessMemory() error within searchMemory()";
                  delete[] buffer;
                  return;
                }
                
              } 
              if (matched.size()==0){
                std::cerr << "matched.size() == 0, couldnt find any matches to the given score value";
              }
              else {
                std::cout << "matched.size(): " << matched.size() << '\n';
                std::cout<< "matched scoreAddress to maybe be this?: hex of this int: " << matched[0] << '\n';
                return;
              }
            }
          }
        }
        
        address += memInfo.RegionSize;
      }
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
