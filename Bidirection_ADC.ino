#define adcpin A0

void setup() {
  Serial.begin(9600);
 pinMode(adcpin,INPUT);

}

void loop() 
{

Serial.println(String(voltmeasure(5.00/*max measureble voltage*/,  -0/*calibrate ADC for zero set*/),3)+String(" V"));
delay(100);

}


float voltmeasure(float maxvoltagerange, float calibration)
{   int number_avg=100;
    float zero=(511.5+calibration); // set zero
    float voltage=0;
    for(int x=0;x<number_avg;x++)
    {
    voltage=((analogRead(adcpin)-zero)*4.882)+voltage;  // avarage calculation
    }

    voltage=voltage/number_avg;
    if(maxvoltagerange>0)
    return(map(voltage, 0, 2496.12, 0, maxvoltagerange)); // maping voltage

    else
    return(voltage/1000);
}
