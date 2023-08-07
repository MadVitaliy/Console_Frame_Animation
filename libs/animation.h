//
// Created by Vitalik on 10.04.2021.
//

#ifndef CONSOLE_FRAME_ANIMATION_ANIMATION_H
#define CONSOLE_FRAME_ANIMATION_ANIMATION_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>

#define WINDOWS
#include "./console.h"

class Drawable {
public:
  virtual void Draw(Console& console) = 0;
};

class Animation {
protected:
  short _frameWidth = 0;
  short _frameHeight = 0;
  short _posX = 0;
  short _posY = 0;
  short _numberOfFrames = 0;
  short _currentFrame = -1;

  std::vector<Image> _frames;

  Animation(const std::string& path) {
    std::vector<std::string> read_lines;

    {
      std::ifstream in(path, std::ifstream::in);
      if (!in.is_open())
        throw std::exception("File was not opened");
      while (!in.eof())
      {
        std::string read_line;
        std::getline(in, read_line);
        read_lines.emplace_back(std::move(read_line));
      }
    }

    std::vector<std::vector<std::string>> frames;

    for (size_t i = 0; i < read_lines.size();)
    {
      while (i < read_lines.size() && read_lines[i].rfind("//", 0) == 0)
        ++i;

      if (i < read_lines.size())
      {
        std::vector<std::string> frame;
        while (i < read_lines.size() && read_lines[i].rfind("//", 0) == std::string::npos)
        {
          frame.emplace_back(std::move(read_lines[i]));
          ++i;
        }
        frames.emplace_back(std::move(frame));
      }
    }

    for (size_t j = 1; j < frames.size(); ++j)
      if (frames[j - 1].size() != frames[j].size())
        throw std::exception("Frames have different height");
    
    _numberOfFrames = static_cast<short>(frames.size());
    std::cout << _numberOfFrames << " frames read" << std::endl;

    for (auto fr : frames)
      _frames.emplace_back(fr);
  }

public:

  virtual void Update(int time) = 0;
  virtual void NextFrame() = 0;
  void setPosition(short x, short y) {
    this->_posX = x;
    this->_posY = y;
  }
  void testDrawtAllFrames() {
    std::cout << "Start to test Draw frames" << std::endl;
    for (int i = 0; i < _frames.size(); ++i) {
      std::cout << "Frame: " << i << std::endl;
      _frames[i].DrawTest(); std::cout << std::endl;
    }
  }

};

class Bunny : public Drawable, public Animation {
public:
  Bunny(std::string path) : Animation(path) {  }

  void Draw(Console& console) override {
    _frames[_currentFrame].SetPosition(_posX, _posY);
    console.Draw(_frames[_currentFrame]);
  }

  void NextFrame() override {
    _currentFrame = (_currentFrame + 1) % _frames.size();
  }

  void Update(int time) override {
    NextFrame();
  }
};

#endif //CONSOLE_FRAME_ANIMATION_ANIMATION_H
