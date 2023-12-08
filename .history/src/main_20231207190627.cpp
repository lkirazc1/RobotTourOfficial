#include <Arduino.h>
#include <vector>
#include "Movements.h"
#include "Drivetrain.h"
#include "Mapping.h"


Drivetrain* drivetrain;

int cur_instruction = 0;

// 17 slits is a 90 degree turn.
// 
// 50 slits is 60cm. 21 slits is 25cm (half a maze square).

//Gyro calibration variables
float ax, ay, az;
float gx, gy, gz;
float t;
int counter = 0;

const int kTurnSpeed = 50;
const int kForwardSlits = 21;


Point points[] = {Point(1, 0), Point(1, 2), Point(0, 2), Point};
const int points_len = sizeof(points)/sizeof(points[0]);
Point starting_point = Point(0, 0);


std::vector<Instruction> kInstructionsBuffer = getPath(points, points_len, starting_point, 0);
const int kNumInstructions = kInstructionsBuffer.size();
Instruction* kInstructions = kInstructionsBuffer.data();


void setup()
{
  Serial.begin(57600);
  Wire.begin();
  // compass.init();
  //compass.setCalibration(-1701, 1307, -1743, 1027, -1305, 1391);
  // compass.read();
  delay(100);
  // while (gyro.wakeup() == false)
  // {
  //   Serial.print(millis());
  //   Serial.println("\tCould not connect to GY521");
  //   delay(1000);
  // }
  Serial.print("Is working: ");
  Serial.println(is_sensor_working());
  setup_sensor(0, 0);


  sensor.axe = 0;
  sensor.aye = 0;
  sensor.aze = 0;
  sensor.gxe = 0;
  sensor.gye = 0;
  sensor.gze = 0;
  


  drivetrain = new Drivetrain();
  setup_motors();
  drivetrain->Calibrate();
}

void loop()
{

  drivetrain->loop();
  if (!drivetrain->is_moving())
  {
    if (cur_instruction >= kNumInstructions)
    {
      return;
    }
    Serial.println("Starting next instruction after 500ms");
    delay(500);
    drivetrain->Go(kInstructions[cur_instruction].slits, kInstructions[cur_instruction].movement, kInstructions[cur_instruction].speed_perc);
    cur_instruction++;
  }
}

// void loop()
// {

//   ax = ay = az = 0;
//   gx = gy = gz = 0;
//   t = 0;
//   for (int i = 0; i < 100; i++)
//   {
//     sensor.read();
//     ax -= sensor.getAccelX();
//     ay -= sensor.getAccelY();
//     az -= sensor.getAccelZ();
//     gx -= sensor.getGyroX();
//     gy -= sensor.getGyroY();
//     gz -= sensor.getGyroZ();
//     t += sensor.getTemperature();
//   }

//   if (counter % 10 == 0)
//   {
//     Serial.println("\n\tCOPY CODE SNIPPET");
//     Serial.print("sensor.axe = ");
//     Serial.print(sensor.axe, 7);
//     Serial.print(";\n");

//     Serial.print("sensor.aye = ");
//     Serial.print(sensor.aye, 7);
//     Serial.print(";\n");

//     Serial.print("sensor.aze = ");
//     Serial.print(sensor.aze, 7);
//     Serial.print(";\n");
    
//     Serial.print("sensor.gxe = ");
//     Serial.print(sensor.gxe, 7);
//     Serial.print(";\n");

//     Serial.print("sensor.gye = ");
//     Serial.print(sensor.gye, 7);
//     Serial.print(";\n");

//     Serial.print("sensor.gze = ");
//     Serial.print(sensor.gze, 7);
//     Serial.print(";\n");

//     Serial.println("\taxe\taye\taze\tgxe\tgye\tgze\tT");
//   }

//   if (counter % 10 == 0)
//   {
//     Serial.print(counter);
//     Serial.print('\t');
//     Serial.print(ax * 0.01, 3);
//     Serial.print('\t');
//     Serial.print(ay * 0.01, 3);
//     Serial.print('\t');
//     Serial.print(az * 0.01, 3);
//     Serial.print('\t');
//     Serial.print(gx * 0.01, 3);
//     Serial.print('\t');
//     Serial.print(gy * 0.01, 3);
//     Serial.print('\t');
//     Serial.print(gz * 0.01, 3);
//     Serial.print('\t');
//     Serial.print(t * 0.01, 2);
//     Serial.println();
//   }
//   //  adjust calibration errors so table should get all zero's.
//   sensor.axe += ax * 0.01;
//   sensor.aye += ay * 0.01;
//   sensor.aze += az * 0.01;
//   sensor.gxe += gx * 0.01;
//   sensor.gye += gy * 0.01;
//   sensor.gze += gz * 0.01;

//   counter++;

// }