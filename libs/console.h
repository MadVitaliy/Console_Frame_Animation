//
// Created by Vitalik on 10.04.2021.
//

#ifndef CONSOLE_FRAME_ANIMATION_CONSOLE_H
#define CONSOLE_FRAME_ANIMATION_CONSOLE_H

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>

class Image {
protected:
  std::vector<char> _image;
  short _height = 0;
  short _width = 0;
  short _posX = 0;
  short _posY = 0;
public:
  Image() = default;

  Image(const std::vector<std::string>& img) : _height(static_cast<short>(img.size())), _width(0), _posX(0), _posY(0) {
    for (auto row : img)
      _width = std::max(_width, short(row.size()));

    _image.resize(_width * _height, ' ');

    for (size_t i = 0; i < img.size(); ++i)
      std::memcpy(_image.data() + i * _width, img[i].data(), img[i].size());
  }

  void SetPosition(short posX, short posY) {
    this->_posX = posX;
    this->_posY = posY;
  }

  void DrawTest() const {
    for (int i = 0; i < _height; ++i)
      for (int j = 0; j < _width; ++j)
        std::cout << _image[i * _width + j];
    std::cout << '\n';
  }

  void writeMirror(std::ofstream& of) {
    if (of.is_open()) {
      for (int i = 0; i < _height; ++i) {
        for (int j = _width - 1; j >= 0; --j) {
          if (_image[i * _width + j] == '(')
            of << ')';
          else if (_image[i * _width + j] == ')')
            of << '(';
          else if (_image[i * _width + j] == '\\')
            of << '/';
          else if (_image[i * _width + j] == '/')
            of << '\\';
          else
            of << _image[i * _width + j];
        }
        of << std::endl;
      }
    }
  }

  const char* getImage() const { return _image.data(); }
  short getHeight() const { return _height; }
  short getWidth() const { return _width; }
  short getPosX() const { return _posX; }
  short getPosY() const { return _posY; }
};



class ConsoleFirtual {
protected:
  short _height = -1;
  short _width = -1;
  short m_offset = 1;
  short _maxHeight = -1;
  short _maxWidth = -1;
  std::vector<char> _buffer;

  ConsoleFirtual() {}

  ConsoleFirtual(short width, short height) : _height(height), _width(width), _maxHeight(-1), _maxWidth(-1) {
    _buffer.resize(_height * (_width + m_offset));
  }

  virtual void SetWindowSizes(short width, short height)
  {
    _height = height;
    _width = width;
    _buffer.resize((width + 1) * height);
  }

public:
  virtual void Clear() = 0;
  virtual void Draw(const Image& drawable) = 0;
  virtual void Display() = 0;
};


#include <windows.h>

class Console : protected ConsoleFirtual {
public:
  Console() = default;
  Console(short width, short height) : ConsoleFirtual(width, height) {
    std::cout << "initialized" << std::endl;
    for (size_t i = 0; i < _height; ++i)
      _buffer[(i + 1) * (_width + m_offset) - 1] = '\n';
  }

  void Clear() {
    std::memset(_buffer.data(), '*', _height * (_width + 1));
  }


  void Draw(const Image& img) {
    const char* image = img.getImage();
    short height = img.getHeight();
    short width = img.getWidth();
    short posX = img.getPosX();
    short posY = img.getPosY();
    short drawHeight = height;
    short drawWidth = width;

    if (posY + drawHeight > _height) drawHeight = _height - posY;
    if (posX + drawWidth > _width) drawWidth = _width - posX;


    for (size_t i = 0; i < drawHeight; ++i) {
      for (size_t j = 0; j < drawWidth; ++j) {
        _buffer[(i + posY) * (_width + m_offset) + posX + j] = image[i * width + j];
      }
    }
    for (size_t i = 0; i < _height; ++i)
      _buffer[(i + 1) * (_width + m_offset) - 1] = '\n';

  }

  void SetCursor(short x, short y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
  }


  void Display() {
    SetCursor(0, 0);
    fwrite(_buffer.data(), sizeof(char), _height * (_width + m_offset), stdout);
  }
};


#endif
