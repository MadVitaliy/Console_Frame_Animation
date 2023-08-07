#include <iostream>
#include <ctime>
//#include <windows.h>

#define WINDOWS
#include "./libs/console.h"
#include "./libs/animation.h"

const std::string path("C:/Users/vomelchenko/source/repos/Console_Frame_Animation/Bunny.txt");
int main() {
  bool res = false;

  Bunny bunny(path);
  bunny.setPosition(0, 0);

  constexpr int width = 140;
  constexpr int height = 35;
  Console console(width, height);

  try {
    //console = Console(width,height);
    std::cout << "Console size set!" << std::endl;
  }
  catch (int a) {
    if (a == -1) {
      std::cout << "Memory was not locate" << std::endl;
      getchar();
      return 1;
    }
    else if (a == 1) {
      std::cout << "Put too big console size" << std::endl;
      getchar();
      return 1;
    }
    else if (a == 2) {
      std::cout << "Cant Set Console Window Info" << std::endl;
      getchar();
      return 2;
    }
    else if (a == 3) {
      std::cout << "Cant Set Console Screen Buffer Size" << std::endl;
      getchar();
      return 3;
    }
  }

  getchar();

  //bunny.testDrawtAllFrames();

  getchar();
  for (;;) {
    console.Clear();
    bunny.Update(100);
    bunny.Draw((console));
    console.Display();
    Sleep(100);
    //getchar();
  }

  return 0;
}