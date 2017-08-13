#include "Countdown.h"
#include <Arduino.h>

Countdown::Countdown()
{  
	interval_end_ms = 0L;
}

Countdown::Countdown(int ms)
{
	countdown_ms(ms);   
}

bool Countdown::expired()
{
	return (interval_end_ms > 0L) && (millis() >= interval_end_ms);
}

void Countdown::countdown_ms(unsigned long ms)  
{
	interval_end_ms = millis() + ms;
}

void Countdown::countdown(int seconds)
{
	countdown_ms((unsigned long)seconds * 1000L);
}

int Countdown::left_ms()
{
	return interval_end_ms - millis();
}
