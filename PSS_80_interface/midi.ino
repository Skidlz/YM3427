void handle_control(byte value, byte param){
  switch(param){
  case 1: //instrument selection
  /*registers 8-13 = Instrument
    1Rxx xxxx;6 = Retrigger
    1xSx xxxx;5 = Staccato
    1xxE xxxx;4 = Envelope sustain
    1xxx Vxxx;3 = Vibrato
    1xxx xDDx;2-1 = Detune
    1xxx xxxE;0 = Volume/Emphasis*/
    inst &= 0b10000111;
    inst |= (value & ~0b10000111);
    break;
  case 2:
  /*registers 24-29 = Envelope
    1??x xxxx;5-6 same as brightness?
    1xxr xxxx;4 = Reverb (instead of release)
    1xxx Sxxx;3 = Sustain
    1xxx xRxx;2 = Release
    1xxx xxDx;1 = Decay
    1xxx xxxA;0 = Attack */
    global_env_set(value >>2 | 0x80);
    break;
  case 3:
  /*registers 32-37 = PWM/Volume
    1Exx xxxx; 6 = effect PWM
    1xBB Bxxx; 3-5 = brightness
    1xxx xVVV; 0-2 = volume
    1111 0VVV; value 240-247 = cymbal noise
    1111 1VVV; value 240-247 = white noise */
    global_pwm_set(value & 0b11111000 | 0x80);
    break;
  case 4:
    //Speed
    test &= 0b11111000;
    value >>= 4;
    test |= (value & (1<<0))? 0:(1<<2);
    test |= (value & (1<<1))? (1<<0):0;
    test |= (value & (1<<2))? (1<<1):0;
    
    global_fx_set(test);
    break;
  case 5:
  //Depth
    test &= 0b11001111;
    test |= value >>1 & 0b00110000;
    for(int i=16;i<22;i++){
      send_packet(i,test);
      delayMicroseconds(300);
    }
    break;
  case 6:
  //Retrig
    test &= ~(1<<6);
    test |= (value>=64)? (1<<6):0;
    global_fx_set(test);
    break;
  case 7:
    test &= ~(1<<3);
    test |= (value>=64)? (1<<3):0;
    global_fx_set(test);
    break;
    
  } 
}

void handle_com(int data){
  if(last_com == 0x90){ //key press
    if(serial_wait()){ //if vol > 0
      /*
      //multivoice setting code:
      byte chan = 0;
      for(int i=0;i<voices;i++){
        chan = write_note(data,inst[i],offset[i]);
        send_packet(chan+16,fx[i]);
        send_packet(chan+24,env[i]);
        send_packet(chan+32,pwm[i]);
      }
      */
      write_note(data,inst,0);
      write_note(data,inst|0b010,0);//detune
      write_note(data,inst|0b100,0);
    }else{
      clear_chan(data);
      clear_chan(data);
      clear_chan(data);
    }
  }else if(last_com == 0x80){ //key release
    serial_wait(); //scrap byte
    clear_chan(data);
    clear_chan(data);
    clear_chan(data);
  }else if(last_com == 0xb0){ //control
    handle_control(serial_wait(), data);
  }else if(last_com == 0xe0){//pitch bend
    int value = serial_wait(); //msb
    //TODO
  }
}

byte serial_wait(){
  while(!Serial.available());
  byte temp = Serial.read();
  return (temp < 0xf0)? temp:serial_wait(); //skip realtime(0xf0)
}
