#ifndef IOF_H
#define IOF_H

/*void SetDelayTimer(unsigned char time);
void SetSoundTimer(unsigned char time);
unsigned char GetDelayTimer();
unsigned char GetSoundTimer();*/
extern "C" {
unsigned char GetRand();

unsigned char SetPixel(unsigned char x, unsigned char y, unsigned char bit);
//void ClearDisplay();

void IOInit();
void IOLoop();
}
/*
bool GetKeyPress();
bool WaitOnKeyPress();*/
#endif
