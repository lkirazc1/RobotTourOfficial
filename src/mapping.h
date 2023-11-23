#ifndef MAPPING_H
#define MAPPING_H
#include <vector>
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



std::vector<Instruction> Path(Point cords[], Point pInitial, int int_Direction) { //note that the input points must draw ONLY vertical or horizontal lines
    int deltaX;
    int deltaY;
    int final_Direction;
    int current_Direction = int_Direction;
    Point current_Point = pInitial;
    std::vector<Instruction> instructions;

    instructions.push_back(Instruction{CMtoSteps(25), Drivetrain::FORWARD, 50});
    for (int i = 0; i < sizeof(cords)/sizeof(cords[0]); i++) { //goes through all the list of cords
        deltaX = cords[i].getX() - pInitial.getX();
        deltaY = cords[i].getY() - pInitial.getY();


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

        while(final_Direction != current_Direction) { //turn right until robot is facing correct direction
            instructions.push_back(Instruction{10000, Drivetrain::MOVE_RIGHT, 80});
            current_Direction += 90;
            if(current_Direction >= 360) {
                current_Direction -= 360;
            }
        }

        if(deltaX != 0) { //Move forwards and updates position
            for(int j = 0; j < abs(deltaX); j++) {
                instructions.push_back(Instruction{CMtoSteps(50), Drivetrain::FORWARD, 50});

                if(deltaX > 0) {
                    current_Point.incX();
                } else if (deltaX < 0) {
                    current_Point.decX();
                }
                
            }
        } else if(deltaY != 0) {
            for(int j = 0; j < abs(deltaY); j++) {
                instructions.push_back(Instruction{CMtoSteps(50), Drivetrain::FORWARD, 50});

                if(deltaY > 0) {
                    current_Point.incY();
                } else if (deltaY < 0) {
                    current_Point.decY();
                }
            }
        }

        return instructions;
    }
}
#endif
