#include <stdio.h>
#include <wiringPi.h>

#define	LED	0

int main (void)
{
  printf ("Raspberry Pi blink\n") ;

  wiringPiSetup () ;
  pinMode (LED, INPUT) ;

  for (;;)
  {
    int led = digitalRead(LED);
    printf("%d\n",led);
    delay (25) ;
  }
  return 0 ;
}