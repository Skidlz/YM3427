//YM3427
int offset[] = {0,0,0,0,0,0,0};
byte inst = 152; //0b10011000
byte env = 128;
byte fx = 128;
byte pwm = 176; //max volume 128, 138 lower
int voices = 1; //number of voices per key

//interface variables
byte osc = 0;
byte param = 0;

//midi notes
byte last_com = 0x90; //last midi command
const int ch_ct = 6; //number of chan/voices
byte chan[] = {
  0xff,0xff,0xff,0xff,0xff,0xff}; //fe=disabled, ff=free
byte order[] = {
  0xff,0xff,0xff,0xff,0xff,0xff,0xff}; //extra -1 to shift in
int re_order[] = {
  0,1,2,3,4,5,5}; //release order, left to right:oldest to newest

byte test = 0x80;

void setup(){
  Serial.begin(31250); //MIDI baud
  // Setup IO Pins
  // PORTB pin1 = reset
  // PORTB pin0 = serial clock 
  // PORTD pin7 = serial data
  DDRB = 0xff;
  DDRD = 0b11111110; //0=input
    
  global_pwm_set(129);
  global_env_set(163);
  reset_chip();
  delay(3);
}

void loop(){
  
  if(Serial.available() > 1) {
    int loop_temp = serial_wait();
    if(loop_temp & 0x80){ //command
      last_com = loop_temp;
      handle_com(serial_wait());
    }else{ //multi-note message
      handle_com(loop_temp);
    }
  }
}
