//
// Created by Vitalik on 10.04.2021.
//

#ifndef CONSOLE_FRAME_ANIMATION_CONSOLE_H
#define CONSOLE_FRAME_ANIMATION_CONSOLE_H

#include <iostream>
//#include <cstring>
#include <vector>
#include <fstream>

class Image {
protected:
    char* _image = nullptr;
    short _height = 0;
    short _width = 0;
    short _posX = 0;
    short _posY = 0;
public:
    Image(){}
    Image(std::vector<std::string> img) {
        _height = static_cast<short>(img.size());
        for (auto raw: img) {
            if(raw.length() > _width)
                _width = static_cast<short>(raw.length());
        }
        _image = new char[_width*_height];
        if(_image == nullptr)
            throw -1;
        for (int i = 0; i < _height*_width; ++i) {
            _image[i] = ' ';
        }
        for (int i = 0; i < img.size(); ++i) {
            for (int j = 0; j < img[i].length(); ++j) {
                _image[i*_width + j] = img[i][j];
            }
        }
    }
    Image(const char* image, short width, short height) {

        this->_height = height;
        this->_width = width;
        this->_image = new char[_width*_height];
        for (int i = 0; i < _width*_height; ++i) {
            this->_image[i] = image[i];
        }
    }

    Image(const Image& dr){
       // std::cout << "Cope const" << std::endl;
        this-> _height = dr._height;
        this-> _width = dr._width;
        this-> _posX = dr._posX;
        this-> _posY = dr._posY;
        this->_image = new char[_width*_height];
        for (int i = 0; i < _width*_height; ++i) {
            this->_image[i] = dr._image[i];
        }
    }
    Image(Image&& dr){
       // std::cout << "Move const" << std::endl;
        this-> _height = dr._height;
        this-> _width = dr._width;
        this-> _posX = dr._posX;
        this-> _posY = dr._posY;
        //this->_image = dr._image;
        this->_image = new char[_width*_height];
        for (int i = 0; i < _width*_height; ++i) {
            this->_image[i] = dr._image[i];
        }
        //dr._image = nullptr;
    }

    Image& operator=(const Image& dr){
       // std::cout << "=& const" << std::endl;
        this-> _height = dr._height;
        this-> _width = dr._width;
        this-> _posX = dr._posX;
        this-> _posY = dr._posY;
        this->_image = new char[_width*_height];
        for (int i = 0; i < _width*_height; ++i) {
            this->_image[i] = dr._image[i];
        }
        return *this;
    }

    Image& operator=(Image&& dr){
       // std::cout << "=&& const" << std::endl;
        this-> _height = dr._height;
        this-> _width = dr._width;
        this-> _posX = dr._posX;
        this-> _posY = dr._posY;
        this->_image = dr._image;
        dr._image = nullptr;
        return *this;
    }

    ~Image(){
        //not sure;
        if(_height == 0){
            std::cout << "stop";
        }
        delete [] _image;
    }
    void setPosition(short posX, short posY){
        this->_posX = posX;
        this->_posY = posY;
    }

    void drawTest(){
        for (int i = 0; i < _height; ++i) {
            for (int j = 0; j < _width; ++j) {
               std::cout << _image[i*_width + j];
            }
            std::cout << std::endl;
        }
    }

    void writeMirror(std::ofstream& of){
        if(of.is_open()) {
            for (int i = 0; i < _height; ++i) {
                for (int j = _width-1; j >=0 ; --j) {
                    if(_image[i*_width + j] == '(')
                        of << ')';
                    else if(_image[i*_width + j] == ')')
                        of << '(';
                    else if(_image[i*_width + j] == '\\')
                        of << '/';
                    else if(_image[i*_width + j] == '/')
                        of << '\\';
                    else
                        of << _image[i*_width + j];
                }
                of << std::endl;
            }
        }
    }

    char* getImage(){ return _image;}
    short getHeight(){ return _height;}
    short getWidth(){ return _width;}
    short getPosX(){ return _posX;}
    short getPosY(){ return _posY;}
};



class ConsoleFirtual {
protected:
    short _height = -1;
    short _width = -1;
    short _maxHeight = -1;
    short _maxWidth = -1;
    char* _buffer = nullptr;

    ConsoleFirtual(){}

    ConsoleFirtual(short width, short height) {
        this->_height = height;
        this->_width = width;
        _buffer = new char[_height*_width];

    }

    ConsoleFirtual(const ConsoleFirtual& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;

        _buffer = new char[_height*_width];
        for (int i = 0; i < _width*_height; ++i) {
            this->_buffer[i] = cv._buffer[i];
        }
    }

    ConsoleFirtual(ConsoleFirtual&& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;
        this-> _buffer = cv._buffer;
        cv._buffer = nullptr;
    }

    ConsoleFirtual& operator =(const ConsoleFirtual& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;

        _buffer = new char[_height*_width];
        for (int i = 0; i < _width*_height; ++i) {
            this->_buffer[i] = cv._buffer[i];
        }
        return *this;
    }

    ConsoleFirtual& operator=(ConsoleFirtual&& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;
        this-> _buffer = cv._buffer;
        cv._buffer = nullptr;

        return *this;
    }


    ~ConsoleFirtual(){if(_buffer)delete [] _buffer;}

    virtual void setWindowSizes(short width, short height) = 0;
public:
    virtual void clear() = 0;
    virtual void draw(Image drawable) = 0;
    virtual void display() = 0;

};


#ifdef WINDOWS

#include <windows.h>

class Console : protected ConsoleFirtual {
public:
    Console(){};
    Console(short width, short height) : ConsoleFirtual(height,width) {
        std::cout << "initialized" << std::endl;
        try {
            this->setWindowSizes(width,height);
        } catch (int a) {
            throw a;
        }
/*        _buffer = new char[_height*_width];
        for (int i = 0; i < _height*_width; ++i) {
            _buffer[i] = ' ';
        }*/
    }

/*    Console(const Console& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;

        _buffer = new char[_height*_width];
        strcpy (this->_buffer, cv._buffer);
    }

    Console(Console&& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;
        this-> _buffer = cv._buffer;
        cv._buffer = nullptr;
    }

    Console& operator =(const Console& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;

        _buffer = new char[_height*_width];
        strcpy (this->_buffer, cv._buffer);
        return *this;
    }

    Console& operator=(Console&& cv) {
        this->_height = cv._height;
        this->_width = cv._width;
        this->_maxHeight = cv._maxHeight;
        this->_maxWidth = cv._maxWidth;
        this-> _buffer = cv._buffer;
        cv._buffer = nullptr;

        return *this;
    }*/

    ~Console() {
        //delete [] _buffer;
    }

    void setWindowSizes(short width,short height) {
        if(this->_maxHeight > height || this->_maxWidth > width)
            throw 1;

        this->_height = height;
        this->_width = width;

        int success;
        _COORD coord;
        coord.X = this->_width;
        coord.Y =  this->_height;

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        _SMALL_RECT rect;

        rect.Top = 0;
        rect.Left = 0;
        rect.Bottom = this->_height;
        rect.Right = this->_width;

        success = SetConsoleWindowInfo(handle, TRUE, &rect);
        if(success == 0) {
            //throw 2;
        }
        success = SetConsoleScreenBufferSize(handle, coord);

        if(_buffer == nullptr)
            throw -1;
        for (int i = 0; i < _height*_width; ++i) {
            _buffer[i] = ' ';
        }
    }

    void clear(){
        for (int i = 0; i < _height*_width; ++i) {
            _buffer[i] = ' ';
        }
        system("CLS");
    }


    void draw(Image img){
        char* image = img.getImage();
        short height = img.getHeight();
        short width = img.getWidth();
        short posX = img.getPosX();
        short posY = img.getPosY();
        short drawHeight = height;
        short drawWidth = width;

        if(posY + drawHeight > _height) drawHeight = _height - posY;
        if(posX + drawWidth > _width) drawWidth = _width - posX;


        for (int i = 0; i < drawHeight; ++i) {
            for (int j = 0; j < drawWidth; ++j) {
                _buffer[(i + posY)*_width + posX +j] = image[i*width+j];
            }
        }
    }

    void setCursor(short x, short y) {
        COORD c;
        c.X = x;
        c.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }


    void display(){
        bool hzflag = false;
        for (int i = 0; i < this->_height; ++i) {
            for (int j = 0; j < this->_width; ++j) {

                if(_buffer[i*_width + j] == ' ') {
                    hzflag = false;
                } else {
                    if(!hzflag) {
                        setCursor(j,i);
                        hzflag = true;
                    }
                    putchar(_buffer[i*_width + j]);
                }
            }
            putchar('\n');
        }
    }

/*    void display(){
        for (int i = 0; i < this->_height; ++i) {
            for (int j = 0; j < this->_width; ++j) {
                //putchar(_buffer[i*_width + j]);
                putchar(_buffer[i*_width + j]);
                //output buffer;
            }
            putchar('\n');
        }
    }*/

};

#endif

#ifdef LINUX
//ahahahahahah

class Console : protected ConsoleFirtual {
public:
    Console(){};
    Console(short width, short height) : ConsoleFirtual(height,width) {
        std::cout << "initialized" << std::endl;
        try {
            this->setWindowSizes(width,height);
        } catch (int a) {
            throw a;
        }
    }

    void setWindowSizes(short width,short height) {

    }

    void clear(){
        for (int i = 0; i < this->_height; ++i) {
            for (int j = 0; j < this->_width; ++j) {
                putchar(' ');
            }
            putchar('\n');
        }
    };
    void draw(){

    };
    void display(){

    };

};

#endif




#endif //CONSOLE_FRAME_ANIMATION_CONSOLE_H
