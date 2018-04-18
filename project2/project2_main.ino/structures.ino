#include "structures.h"

void measure (void* data) {
  MeasureData* data_in = (MeasureData*) data;
  unsigned int temp = *(data_in->temperatureRaw);
  if (temp > 50) {
    tempUp = 0;
  } else if (temp < 15) {
    tempUp = 1;
  }
  if (tempUp == 1) {
    if (times == EVEN) {
        *(data_in->temperatureRaw) += 2;
    } else {
        *(data_in->temperatureRaw) -= 1;
    }
  } else {
        if (times == EVEN) {
        *(data_in->temperatureRaw) -= 2;
    } else {
        *(data_in->temperatureRaw) += 1;
    }
  }
  
  unsigned int pulse = *(data_in->pulseRateRaw);
  if (pulse > 40) {
    pulseUp = 0;
  } else if (pulse < 15) {
    pulseUp = 1;
  }

  if (pulseUp) {
    if (times == ODD) {
        *(data_in->pulseRateRaw) += 3;
    } else {
        *(data_in->pulseRateRaw) -= 1;
    }
  } else {
    if (times == ODD) {
        *(data_in->pulseRateRaw) -= 3;
    } else {
        *(data_in->pulseRateRaw) += 1;
    }
  }



    if (systoDone == 0) {
        if (times == EVEN) {
            *(data_in->systolicPressRaw) += 3;
        } else {
            *(data_in->systolicPressRaw) -= 1;
        }
        if (*(data_in->systolicPressRaw) > 100) {
            systoDone = 1;
            *(data_in->systolicPressRaw) = 80;
        }
    } else {
        if (times == EVEN) {
            *(data_in->diastolicPressRaw) -= 2;
        } else {
            *(data_in->diastolicPressRaw) += 1;
        }
        if (*(data_in->diastolicPressRaw) < 40) {
            systoDone = 0;
            *(data_in->diastolicPressRaw) = 80;
        }
    }
  if (times == EVEN) {
    times = ODD;
  } else {
    times = EVEN;
  }
}

void intToChar(unsigned char* result , int num) {
  result[0] = (unsigned char) (num / 100 + 48);
  result[1] = (unsigned char) (num % 100 / 10 + 48);
  result[2] = (unsigned char) (num % 10 + 48);
}



void compute (void* data) {
  ComputeData* data_in = (ComputeData*) data;
  int tempFixed = 5 + 0.75 * *(data_in->temperatureRaw);
  int systoFixed = 9 + 2 * *(data_in->systolicPressRaw);
  int diasFixed = 6 + 1.5 * *(data_in->diastolicPressRaw);
  int pulseFixed = 8 + 3 * *(data_in->pulseRateRaw);
  intToChar(data_in->tempCorrected, tempFixed); 
  intToChar(data_in->sysPressCorrected, systoFixed); 
  intToChar(data_in->diastolicPressCorrected, diasFixed); 
  intToChar(data_in->pulseRateCorrected, pulseFixed);
}

void displayF (void* data) {
  tft.fillScreen(BLACK);
  tft.setCursor(0,0);
  tft.print("   E-Doc: Your Private Doctor (^ w ^)   ");
  DisplayData* data_in = (DisplayData*) data;
  tft.print("|--------------------------------------|");
  tft.print("|                                      |");
  tft.print("| Temperature: ");
  tft.print((char) data_in->tempCorrected[1]);
  tft.print((char) data_in->tempCorrected[2]);
  tft.print(" C                    |");
  tft.print("|                                      |");
  tft.print("| Systolic Pressure: ");
  tft.print((char) data_in->sysPressCorrected[0]);
  tft.print((char) data_in->sysPressCorrected[1]);
  tft.print((char) data_in->sysPressCorrected[2]);
  tft.print(" mmHg          |");
  tft.print("|                                      |");
  tft.print("| Diastolic Pressure: ");
  tft.print((char)data_in->diastolicPressCorrected[0]);
  tft.print((char)data_in->diastolicPressCorrected[0]);
  tft.print((char)data_in->diastolicPressCorrected[0]);
  tft.print(" mmHg         |");
  tft.print("|                                      |");
  tft.print("| Pulse Rate: ");
  tft.print((char)data_in->pulseRateCorrected[0]);
  tft.print((char)data_in->pulseRateCorrected[1]);
  tft.print((char)data_in->pulseRateCorrected[2]);
  tft.print(" BPM                  |");
  tft.print("|                                      |");
  tft.print("| Battery: ");
  unsigned char battery[3];
  intToChar(battery, (int)*(data_in->batteryState));
  tft.print((char)battery[0]);
  tft.print((char)battery[1]);
  tft.print((char)battery[2]);
  tft.print("%                        |");
  tft.print("|                                      |");
  tft.print("|--------------------------------------|");
  tft.print("     We hope you are in good health!    ");
}

void warningAlarm (void* data) {
  WarningAlarmData* data_in = (WarningAlarmData*) data;
  if (*(data_in->temperatureRaw) < 36.1 || *(data_in->temperatureRaw) > 37.8) {
    if (*(data_in->temperatureRaw) > 37.8) {
      tempHigh = TRUE;  
    }
    tft.setTextColor(RED);
  } else {
    tempHigh = FALSE;
    tft.setTextColor(GREEN);
    
  }
  if (*(data_in->systolicPressRaw) > 120) {
    bpHigh = TRUE;
    tft.setTextColor(RED);
  } else {
    bpHigh = FALSE;
    tft.setTextColor(GREEN);
  }
  if (*(data_in->diastolicPressRaw) < 80) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN); 
  }
  if (*(data_in->pulseRateRaw) < 60  || *(data_in->pulseRateRaw) > 100  ) {
     if (*(data_in->pulseRateRaw) < 60) {
      pulseLow = TRUE;  
    }
    tft.setTextColor(RED);
  } else {
    pulseLow = FALSE;
    tft.setTextColor(GREEN);
    
  }
  if (*(data_in->batteryState) < 20  ) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(GREEN); 
  } 
}

void statusF (void* data) {
  Status* data_in = (Status*) data;
  *(data_in->batteryState) -= 1;
}

// precondition: there are five blocks in TCB array
void issue(volatile int* count, int period, int which, TCB* blocks) {
  if (*count == 0) {
    (*blocks[which].myTask)(blocks[which].taskDataPr);
  }
  if (*count == period) {
    *count = 0;
  } else {
    *count+=1;
  }
}

void scheduler(TCB* blocks) {
  issue(&mCount, mP, 0, blocks);
  issue(&cCount, cP, 1, blocks);
  issue(&wCount, wP, 2, blocks);
  issue(&dCount, dP, 3, blocks);
 
  issue(&sCount, sP, 4, blocks);
  delay(1000);
}

