void sleep(){
  // ATmega328P, ATmega168
      /*LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);*/
      // ATmega2560
      LowPower.idle(SLEEP_8S, ADC_OFF, TIMER5_OFF, TIMER4_OFF, TIMER3_OFF,
                    TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART3_OFF, USART2_OFF, USART1_OFF,
                  USART0_OFF, TWI_OFF);
                  
      Serial.println("SleepCycle");
      delay(50);
}

