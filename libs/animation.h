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

#define WINDOWS
#include "./console.h"

class Drawable{
public:
    virtual void draw(Console& console) = 0;
};

class Animation{
protected:
    short _frameWidth = 0;
    short _frameHeight = 0;
    short _posX = 0;
    short _posY = 0;
    short _numberOfFrames = 0;
    short _currentFrame = -1;

    std::vector<Image> _frames;

    Animation(std::string path) {
        std::ifstream in(path, std::ifstream::in);
        std::vector<std::vector<std::string>> frames;
        std::vector<std::string> frame;
        std::string line;

        int lineCounter = 0;
        int numberOfLine = 0;

       // std::stringstream ss;
        if(!in.is_open())
            std::cout << _numberOfFrames << "File was not opened" << std::endl;

        while(!in.eof()) {
            do{
                getline(in, line);
                lineCounter++;
            }while(line[0] == '/' && line[1] == '/' && !in.eof());

            while(!(line[0] == '/' && line[1] == '/') && !in.eof()){
                frame.push_back(line);
                numberOfLine ++;
               // ss << line << std::endl;
                getline(in, line);

            }
            frames.push_back(frame);
            frame.clear();
        }
        in.close();

        _numberOfFrames = static_cast<short>(frames.size());
        std::cout << _numberOfFrames << " frames read" << std::endl;

        for (auto fr: frames) {
            _frames.emplace_back(Image(fr));
        }



/*        std::ofstream out("mirror.txt");
        if(!out.is_open())
            std::cout << _numberOfFrames << " mirror.txt was not opened" << std::endl;
        for (int i = 0; i < _frames.size(); ++i) {
            _frames[i].writeMirror(out);
            out << std::endl;
        }
        out.close();*/
    }

 /*  ~Animation(){
       delete [] _framesMemory;
   }*/
public:

    virtual void update(int time) = 0;
    virtual void nextFrame() = 0;
    void setPosition(short x, short y) {
        this->_posX = x;
        this->_posY = y;
    }
    void testDrawtAllFrames() {
        std::cout << "Start to test draw frames" << std::endl;
        for (int i = 0; i < _frames.size(); ++i) {
            std::cout << "Frame: " << i << std::endl;
            _frames[i].drawTest();std::cout << std::endl;
        }
    }

};

#define RIGHT true
#define LEFT false

class Bunny : public Drawable, public Animation{

private:
    bool _direction = RIGHT; //RIGHT -- jump from lef to right or turn to left, LEFT -- for another directions
    bool _isJumping;
    bool _isTurning;
    short _jumpStage = -1;
    short _turnStage = -1;
    std::pair<short,short> _rightJumpingFramesNumb;
    std::pair<short,short> _rightTurningFramesNumb;
    std::pair<short,short> _leftJumpingFramesNumb;
    std::pair<short,short> _leftTurningFramesNumb;
    short _numOfJumps;
    short _currJump = 0;
    short _prevFrame = -1;



    void jump() {
        if(_direction == RIGHT) {
            _jumpStage = static_cast<short>((_jumpStage + 1)%(_rightJumpingFramesNumb.second-_rightJumpingFramesNumb.first+1));
            _currentFrame = _rightJumpingFramesNumb.first + _jumpStage;

            if(_jumpStage == _rightJumpingFramesNumb.second-_rightJumpingFramesNumb.first) {
                _currJump++;
            }

        } else {
            _jumpStage = static_cast<short>((_jumpStage + 1)%(_leftJumpingFramesNumb.second-_leftJumpingFramesNumb.first+1));
            _currentFrame = _leftJumpingFramesNumb.first + _jumpStage;

            if(_jumpStage == _leftJumpingFramesNumb.second-_leftJumpingFramesNumb.first) {
                _currJump++;
            }
        }

        if(_currJump == _numOfJumps){
            _jumpStage = -1;
            _currJump = 0;
            _isJumping = false;
            _isTurning = true;
        }

    }
    void turn(){
        if(_direction == RIGHT) {
            _turnStage = static_cast<short>((_turnStage + 1)%(_rightTurningFramesNumb.second-_rightTurningFramesNumb.first+1));
            _currentFrame = _rightTurningFramesNumb.first + _turnStage;

            if(_turnStage == _rightTurningFramesNumb.second-_rightTurningFramesNumb.first) {
                _turnStage = -1;
                _currJump = 0;
                _isTurning = false;
                _isJumping = true;
                _direction = !_direction;
            }
        } else {
            _turnStage = static_cast<short>((_turnStage + 1)%(_leftTurningFramesNumb.second-_leftTurningFramesNumb.first+1));
            _currentFrame = _leftTurningFramesNumb.first + _turnStage;

            if(_turnStage == _leftTurningFramesNumb.second-_leftTurningFramesNumb.first) {
                _turnStage = -1;
                _currJump = 0;
                _isTurning = false;
                _isJumping = true;
                _direction = !_direction;
            }
        }
    }

public:
    Bunny(std::string path) : Animation(path){
        _isJumping = true;
        _isTurning = !_isJumping;
    }

    void setRightJumpingFramesNumb(short lower, short upper){
        _rightJumpingFramesNumb = std::make_pair(lower,upper);
    }
    void setRightTurningFramesNumb(short lower, short upper){
        _rightTurningFramesNumb = std::make_pair(lower,upper);
    }
    void setLeftJumpingFramesNumb(short lower, short upper){
        _leftJumpingFramesNumb = std::make_pair(lower,upper);
    }
    void setLeftTurningFramesNumb(short lower, short upper){
        _leftTurningFramesNumb = std::make_pair(lower,upper);
    }

    void setNumberOfJumps(short numOfJumps){
        _numOfJumps = numOfJumps;
    }

    void draw(Console& console) override {
        _frames[_currentFrame].setPosition(_posX,_posY);
        console.draw(_frames[_currentFrame]);
    }

    void nextFrame() override{
        if(_isJumping) {
            jump();
        } else if(_isTurning)
            turn();
    };

    void update(int time) override{
        _prevFrame = _currentFrame;
        nextFrame();


        if(_prevFrame == _rightJumpingFramesNumb.first)
            _posX += -5;
        else if(_prevFrame == _rightJumpingFramesNumb.first+2 || _prevFrame == _rightJumpingFramesNumb.first+3 )
            _posX += 5;
        else if(_prevFrame == _rightJumpingFramesNumb.second)
            _posX += 20;
        ////////////////////////////////////////////////////////
        else if(_prevFrame == _rightTurningFramesNumb.first)
            _posX += -1;
        else if(_prevFrame == _rightTurningFramesNumb.first +1)
            _posX += 6;
        //////////////////////////////////////////////////////
        else if(_prevFrame == _leftJumpingFramesNumb.first)
            _posX += 1;
        else if(_prevFrame == _leftJumpingFramesNumb.first + 2)
            _posX += -9;
        else if(_prevFrame == _leftJumpingFramesNumb.first + 3)
            _posX += -8;
        else if(_prevFrame == _leftJumpingFramesNumb.second)
            _posX += -10;
        ////////////////////////////////////////////
        else if(_prevFrame == _leftTurningFramesNumb.first)
            _posX += 4;
        else if(_prevFrame == _rightTurningFramesNumb.first +1)
            _posX += 6;
        else if(_prevFrame == _rightTurningFramesNumb.second)
            _posX += -4;
    };

};

#endif //CONSOLE_FRAME_ANIMATION_ANIMATION_H
