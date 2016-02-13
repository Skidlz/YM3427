void global_fx_set(byte val){
  //registers 16-22 = effects registers
  for(int i=16;i<22;i++){
    send_packet(i,val);
    delayMicroseconds(300);
  }
}

void global_env_set(byte val){
  //registers 24-29 = envelope registers
  for(int i=24;i<30;i++){
    send_packet(i,val);
    delayMicroseconds(300);
  }
}

void global_pwm_set(byte val){
  //registers 32-37 = pwm/vol registers
  for(int i=32;i<38;i++){
    send_packet(i,val);
    delayMicroseconds(300);
  }
  send_packet(42,129);
}

void note_on(byte chan, byte inst, byte note){
  send_packet(chan+8,inst); //instrument
  delay(3);
  send_packet(chan,note); //octave+note
  delayMicroseconds(300);
  send_packet(chan+8,inst+48); //instrument
}

void send_packet(byte adr, byte data){
  send_byte(adr);
  delayMicroseconds(80);
  send_byte(data);
}

void send_byte(byte data){
  for(int i=0;i<8;i++){
    PORTD = data<<7;
    bitClear(PORTB,0);
    delayMicroseconds(6);
    bitSet(PORTB,0);
    data >>= 1;
    delayMicroseconds(6);
  }
}

void reset_chip(){
  delay(1);
  bitClear(PORTB,1);
  delay(1);
  bitSet(PORTB,1);
  delay(1);
}
