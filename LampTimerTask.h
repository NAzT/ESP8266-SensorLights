void Lamp_Publish();

class TaskLampTimer : public Task
{
public:
    TaskLampTimer(uint32_t timeInterval) : // pass any custom arguments you need
        Task(timeInterval)
    { };

private:
    // put member variables here that are scoped to this object
    int i,j;
    // const means can't change other than in constructor

    virtual bool OnStart() // optional
    {
        // put code here that will be run when the task starts
        pinMode(RelayPin1, OUTPUT); // hier alle Inputs und Outputs definieren
        pinMode(RelayPin2, OUTPUT);
        pinMode(RelayPin3, OUTPUT);
        pinMode(RelayPin4, OUTPUT);
        for (i=0;i<4;i++) {
          digitalWrite(relaypin[i], HIGH);   // turn the LED off (HIGH is the voltage level)
        }
        return true;
    }

    virtual void OnStop() // optional
    {
        // put code here that will be run when the task stops
    }

    virtual void OnUpdate(uint32_t deltaTime)
    {
      for (j=0;j<4;j++) {
        for (i=0;i<4;i++) {
          if (timermode[j][i] && lamptimeron[j][i]>0)
          {
            lamptimeron[j][i]-=1;
            if (lamptimeron[j][i]<=0) 
            {
              lamptimeron[j][i]=0;
              relayon[j][i]=false;
              if (!(relayon[0][i]||relayon[1][i]||relayon[2][i]||relayon[3][i]||relayon[4][i])&&(digitalRead(relaypin[i])==LOW))
              {
                digitalWrite(relaypin[i], HIGH);   // turn the LED on (LOW is the voltage level)
                Serial.print("Relay Off #");
                Serial.println(i);
                Lamp_Publish();
              }
            }
          }
          else if (!(relayon[0][i]||relayon[1][i]||relayon[2][i]||relayon[3][i]||relayon[4][i])&&(digitalRead(relaypin[i])==LOW)) //!= !(relayonold[0][i]||relayonold[1][i]||relayonold[2][i]||relayonold[3][i]))
          {
            digitalWrite(relaypin[i], HIGH);   // turn the LED on (LOW is the voltage level)
            Serial.print("Relay Off direct sensor #");
            Serial.println(i);
            Lamp_Publish();
          }
        }
      }

    }
};
