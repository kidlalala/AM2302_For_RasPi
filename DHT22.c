#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>

#define PIN 4 // GPIO_4 P_16

#define Start_sign 800
#define Release 20
//#define Respond 80
#define DigitZero 30
#define DigitOne 70


int main(int argc,char **args)
{
    char Data[5] = {0};
    int Temp = 0,Humi = 0;

    if(wiringPiSetup() == -1)
    {
        perror("Fail To Setup");
        exit(-1);
    }

    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    delayMicroseconds(Start_sign);

    digitalWrite(PIN, HIGH);
    pinMode(PIN, INPUT);
    delayMicroseconds(Release);

    int lastlevel = 0,counter;
    
    while(digitalRead(PIN) == LOW);
    while(digitalRead(PIN) == HIGH);

    for(int i = 0; i < 5; i++)
    {
	for(int j = 0; j < 16; j++)
	{
		if(j%2 && j > 0)
        	{
			counter = 0;
            		while(digitalRead(PIN) == lastlevel)
			{
				counter++;
            			delayMicroseconds(1);
            			if(counter == 255)
            			{
                			perror("Overtime error");
                			exit(-1);
            			}
			}
			Data[i] = Data[i] << 1;	
			if(counter > 35)
			    Data[i] = Data[i] | 1;

			lastlevel = digitalRead(PIN);	
			continue;
		}
		while(digitalRead(PIN) == lastlevel);
		lastlevel = digitalRead(PIN);
    	}
    }

    for(int i=0; i<5;i++)
	    printf("%d ",Data[i]);

    int checksum = (Data[0] + Data[1] + Data[2] + Data[3])%256;

    if(Data[4] != checksum)
    {
         perror("CheckData error");
         exit(-1);
    }

    Humi = Data[0];
    Humi = Humi << 8;
    Humi += Data[1];

    if(Data[2] & 0x80)
    {
        Temp = Data[2] & 0x7f;
        Temp = Temp << 8;
        Temp += Data[3];
        Temp = -Temp;
    }
    else
    {
        Temp = Data[2];
	Temp = Temp << 8;
        Temp += Data[3];
    }
    printf("%.2f %.2f\n",Humi*1.0/10,Temp*1.0/10);
}

//os will waste time so cut delaytime   low efficient
//only get 39 times data
