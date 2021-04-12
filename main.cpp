#include <iostream>
#include <ctime>
//#include <windows.h>

#define WINDOWS
#include "./libs/console.h"
#include "./libs/animation.h"

std::string path = "../Bunny.txt";
int main() {
    bool res = false;

    Bunny bunny(path);
    bunny.setPosition(5,0);

    bunny.setRightJumpingFramesNumb(0, 4);
    bunny.setRightTurningFramesNumb(5, 10);
    bunny.setLeftJumpingFramesNumb(11, 15);
    bunny.setLeftTurningFramesNumb(16, 20);


    bunny.setNumberOfJumps(2);
   // bunny.testDrawtAllFrames();
    //getchar();

    int width = 140;
    int height = 35;
    Console console (width,height);

    try{
        //console = Console(width,height);
        std::cout << "Console size set!" << std::endl;
    }catch(int a){
        if(a == -1) {
            std::cout << "Memory was not locate" << std::endl;
            getchar();
            return 1;
        } else if(a == 1) {
            std::cout << "Put too big console size" << std::endl;
            getchar();
            return 1;
        } else if( a == 2){
            std::cout << "Cant Set Console Window Info" << std::endl;
            getchar();
            return 2;
        }else if( a == 3){
            std::cout << "Cant Set Console Screen Buffer Size" << std::endl;
            getchar();
            return 3;
        }
    }


    getchar();
    for (;;) {
        console.clear();
        bunny.update(100);
        bunny.draw((console));
        console.display();
        Sleep(250);
    }

    return 0;
}