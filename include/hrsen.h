#ifndef _HRSEN_H
#define _HRSEN_H

#include <Arduino.h>

const int trig = 25;
const int echo = 26;

void HrsenSetup(){
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

int distance(){
    unsigned long duration;
    int distance;
    digitalWrite(trig, 0);
    delayMicroseconds(2);
    digitalWrite(trig, 1); 
    delayMicroseconds(5);
    digitalWrite(trig, 0);
    duration = pulseIn(echo, HIGH);
    distance = int(duration/2/29.412);
    return distance;
}
#endif