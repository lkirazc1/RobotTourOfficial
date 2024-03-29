#ifndef MAPPING_H
#define MAPPING_H
#include <vector>
#include <iostream>
#include "Drivetrain.h"
#include "Movements.h"

int getPredictedDirection(int currentDirection, int angleOffset);

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



std::vector<Instruction> getPath(Point cords[], int len, Point pInitial, int int_Direction, int turning_speed, int straight_speed) { //note that the input points must draw ONLY vertical or horizontal lines
    Serial.println("mapping");
    int deltaX;
    int deltaY;
    int final_Direction;
    int current_Direction = int_Direction;
    int predicted_Direction;
    boolean goReverse;
    Point current_Point = pInitial;
    std::vector<Instruction> instructions;
    instructions.push_back({CMtoSteps(25), Drivetrain::FORWARD, straight_speed});


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
        Serial.println(deltaY);

        Serial.print("Current Direction: ");
        Serial.println(current_Direction);
        Serial.print("Final Direction: ");
        Serial.println(final_Direction);
        Serial.println();

        goReverse = false;

        predicted_Direction = getPredictedDirection(current_Direction, 90); //gives turning direction
        if(predicted_Direction == final_Direction) {
            instructions.push_back({10000, Drivetrain::MOVE_RIGHT, turning_speed});
            current_Direction = final_Direction;
        } else {
            predicted_Direction = getPredictedDirection(current_Direction, 180);
            
            if(predicted_Direction == final_Direction) {
                goReverse = true;
            } else {
                predicted_Direction = getPredictedDirection(current_Direction, 270);

                if(predicted_Direction == final_Direction) {
                    instructions.push_back({10000, Drivetrain::MOVE_LEFT, turning_speed});
                    current_Direction = final_Direction;
                }
            }
        }

        if(deltaX != 0) { //Move forwards and updates position
            for(int j = 0; j < abs(deltaX); j++) {
                if(goReverse) {
                    instructions.push_back({CMtoSteps(50), Drivetrain::BACKWARD, straight_speed});
                } else {
                    instructions.push_back({CMtoSteps(50), Drivetrain::FORWARD, straight_speed});
                }

                if(deltaX > 0) {
                    current_Point.incX();
                } else if (deltaX < 0) {
                    current_Point.decX();
                }
                
            }
        } else if(deltaY != 0) {
            for(int j = 0; j < abs(deltaY); j++) {
                if(goReverse) {
                    instructions.push_back({CMtoSteps(50), Drivetrain::BACKWARD, straight_speed});
                } else {
                    instructions.push_back({CMtoSteps(50), Drivetrain::FORWARD, straight_speed});
                }

                if(deltaY > 0) {
                    current_Point.incY();
                } else if (deltaY < 0) {
                    current_Point.decY();
                }
            }
        }
    }
    if (instructions.at(instructions.size() - 1).movement == Drivetrain::BACKWARD)
    {
        instructions.at(instructions.size() - 1).slits = 500;
    }
    else
    {
        instructions.at(instructions.size() - 1).slits = 340;
    }
    return instructions;
}

int getPredictedDirection(int currentDirection, int angleOffset) {
    int prediction = currentDirection + angleOffset;
    if(prediction >= 360) {
        prediction -= 360;
    }
    return prediction;
}
#endif