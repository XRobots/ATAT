void kinematics (int leg, float xIn, int yIn, float zIn, int interOn, int dur) {

    #define length1 0.152 
    float angle1;
    float angle1a;
    float angle1b;
    float angle1c;
    float angle2;
    float angle2Degrees;
    float angle2aDegrees;
    float angle1Degrees;
    float z3;
    float shoulderAngle2;
    float shoulderAngle2Degrees;

    float shoulderAngleServo;
    float shoulderAngleServo2;
    float kneeAngleServo;

    float zOut;
    float xOut;
    float yOut;   

     
    // ** INTERPOLATION **
    // use Interpolated values if Interpolation is on
    if (interOn == 1) {

        zIn = zIn * 1000;     // refactor because the ramp calss doesn't seem to like floats
        xIn = xIn * 1000;
            
        if (leg == 1) {        // front right
            z = interpFRZ.go(zIn,dur);
            x = interpFRX.go(xIn,dur);
            y = interpFRY.go(yIn,dur);
        }
      
        else if (leg == 2) {    // front left
            z = interpFLZ.go(zIn,dur);
            x = interpFLX.go(xIn,dur);
            y = interpFLY.go(yIn,dur);           
        }
      
        else if (leg == 4) {   // back right
            z = interpBRZ.go(zIn,dur);
            x = interpBRX.go(xIn,dur);
            y = interpBRY.go(yIn,dur); 
        }
      
        else if (leg == 3) {    // back left
            z = interpBLZ.go(zIn,dur);
            x = interpBLX.go(xIn,dur);
            y = interpBLY.go(yIn,dur); 
        }

        
        z = z/1000;         // refactor again
        x = x/1000;

        z = constrain(z,0.25,0.29);
        x = constrain(x,-0.4,0.4);
    } 

   
      
    // calculate the shoulder joint offset and new leg length based on now far the foot moves forward/backwards
    shoulderAngle2 = atan(x/z);     // calc how much extra to add to the shoulder joint    
    z3 = z/cos(shoulderAngle2);     // calc new leg length to feed to the next bit of code below

    // calculate leg length based on shin/thigh length and knee and shoulder angle
    angle1a = sq(length1) + sq(z3) - sq(length1);
    angle1b = 2 * length1 * z3;
    angle1c = angle1a / angle1b;
    angle1 = acos(angle1c);     // hip angle RADIANS
    angle2 = PI - (angle1 * 2); // knee angle RADIANS
    

    //calc degrees from angles
    angle2Degrees = angle2 * (180/PI);    // knee angle DEGREES
    angle2aDegrees = angle2Degrees / 2;    // half knee angle for each servo DEGREES
    shoulderAngle2Degrees = shoulderAngle2 * (180/PI);    // front/back shoulder offset DEGREES

    shoulderAngleServo = (angle2aDegrees-68)*50;        // remove defualt angle offset. Multiple degrees to get servo microseconds.
    kneeAngleServo = (angle2aDegrees-68)*50;
    shoulderAngleServo2 = shoulderAngle2Degrees*50;

    int factor = 20;

    if(leg == 1) {
        pos1 = 1500 - shoulderAngleServo - shoulderAngleServo2;
        pos2 = 1500 + kneeAngleServo - shoulderAngleServo2;
        pos1Filtered = filter(pos1, pos1Filtered, factor);
        pos2Filtered = filter(pos2, pos2Filtered, factor);
        servo1.writeMicroseconds(pos1Filtered + pos1Offset);
        servo2.writeMicroseconds(pos2Filtered + pos2Offset);
        pos9 = 1500 - y;
        servo9.writeMicroseconds(pos9 + pos9Offset);
    }
    else if (leg == 2) {
        pos5 = 1500 + shoulderAngleServo + shoulderAngleServo2;
        pos6 = 1500 - kneeAngleServo + shoulderAngleServo2;
        pos5Filtered = filter(pos5, pos5Filtered, factor);
        pos6Filtered = filter(pos6, pos6Filtered, factor);
        servo5.writeMicroseconds(pos5Filtered + pos5Offset);
        servo6.writeMicroseconds(pos6Filtered + pos6Offset);
        pos11 = 1500 - y; 
        servo11.writeMicroseconds(pos11 + pos11Offset);               
    }
    else if (leg == 3) {
        pos3 = 1500 - shoulderAngleServo - shoulderAngleServo2;
        pos4 = 1500 + kneeAngleServo - shoulderAngleServo2;
        pos3Filtered = filter(pos3, pos3Filtered, factor);
        pos4Filtered = filter(pos4, pos4Filtered, factor);
        servo3.writeMicroseconds(pos3Filtered + pos3Offset);
        servo4.writeMicroseconds(pos4Filtered + pos4Offset);
        pos10 = 1500 - y;
        servo10.writeMicroseconds(pos10 + pos10Offset);      
    }
    else if (leg == 4) {
        pos7 = 1500 + shoulderAngleServo + shoulderAngleServo2;
        pos8 = 1500 - kneeAngleServo + shoulderAngleServo2;
        pos7Filtered = filter(pos7, pos7Filtered, factor);
        pos8Filtered = filter(pos8, pos8Filtered, factor);
        servo7.writeMicroseconds(pos7Filtered + pos7Offset);
        servo8.writeMicroseconds(pos8Filtered + pos8Offset);
        pos12 = 1500 - y;
        servo12.writeMicroseconds(pos12 + pos12Offset);
    }


    
    

  
}


