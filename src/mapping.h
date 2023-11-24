#ifndef MAPPING_H
#define MAPPING_H
#include <vector>
#include <iostream>
#include "Drivetrain.h"
#include "Movements.h"


struct Instruction
{
    int slits;
    Drivetrain::Movement movement;
    int speed_perc;
};

class Point {
    public: //constructor
        Point(int x, int y) {
            xCord = x;
            yCord = y;
        }

        int getX() {
            return xCord;
        }

        int getY() {
            return yCord;
        }

        void incX() {
            xCord ++;
        }

        void decX() {
            xCord --;
        }

        void incY() {
            yCord ++;
        }

        void decY() {
            yCord --;
        }


    private:
        int xCord;
        int yCord;
};



std::vector<Instruction> getPath(Point cords[], int len, Point pInitial, int int_Direction) { //note that the input points must draw ONLY vertical or horizontal lines
    int deltaX;
    int deltaY;
    int final_Direction;
    int current_Direction = int_Direction;
    Point current_Point = pInitial;
    std::vector<Instruction> instructions;
    instructions.push_back({CMtoSteps(25), Drivetrain::FORWARD, 50});


    // int numInstructions = std::extent<decltype(cords)>::value;

    for (int i = 0; i < len; i++) { //goes through all the list of cords
        deltaX = cords[i].getX() - current_Point.getX();
        deltaY = cords[i].getY() - current_Point.getY();


        if(deltaX > 0) { //right
            final_Direction = 90;
        } else if(deltaX < 0) { //left
            final_Direction = 270;
        } else if(deltaY > 0) { //up
            final_Direction = 0;
        } else if(deltaY < 0) { //down
            final_Direction = 180;
        } else {
            final_Direction = current_Direction;
        }
        Serial.print("deltaX: ");
        Serial.println(deltaX);
        Serial.print("deltaY: ");
        Serial.print(deltaY);

        Serial.print("Current Direction: ");
        Serial.println(current_Direction);
        Serial.print("Final Direction: ");
        Serial.println(final_Direction);
        Serial.println();

        while(final_Direction != current_Direction) { //turn right until robot is facing correct direction
            instructions.push_back({10000, Drivetrain::MOVE_RIGHT, 80});
            current_Direction += 90;
            if(current_Direction >= 360) {
                current_Direction -= 360;
            }
        }

        if(deltaX != 0) { //Move forwards and updates position
            for(int j = 0; j < abs(deltaX); j++) {
                instructions.push_back({CMtoSteps(50), Drivetrain::FORWARD, 50});

                if(deltaX > 0) {
                    current_Point.incX();
                } else if (deltaX < 0) {
                    current_Point.decX();
                }
                
            }
        } else if(deltaY != 0) {
            for(int j = 0; j < abs(deltaY); j++) {
                instructions.push_back({CMtoSteps(50), Drivetrain::FORWARD, 50});

                if(deltaY > 0) {
                    current_Point.incY();
                } else if (deltaY < 0) {
                    current_Point.decY();
                }
            }
        }
    }
    return instructions;
}
#endif
