byte write_note(int num, int inst, int offset){
  for(int ch = 0; ch < ch_ct; ch++){
    if(chan[re_order[ch]] == 0xff){ //find free chan
      ch = re_order[ch];
      chan[ch] = num;

      for(int i = 0; i < ch_ct; i++){//store order
        if(order[i] == 0xff){
          order[i] = num;
          break;
        }
      }
      num += offset;
      byte oct = num/12;
      num %= 12;
      num += 1 + (num/3);
      
      note_on(ch, inst, (oct<<4)|num|0x80);
      return ch;
    }
  }
  clear_chan(order[0]); //default to oldest chan
  return write_note(num, inst, offset); 
}

void clear_chan(byte note){
  for(int ch = 0; ch < ch_ct; ch++){
    if(chan[ch] == note){
      chan[ch] = 0xff;
      send_packet(ch+8,136);
      re_order[ch_ct]=ch; //add to end
      for(int i=0;i<ch_ct-1;i++)if(re_order[i]==ch)for(;i<ch_ct;i++)re_order[i]=re_order[i+1];
      break;
    }
  }
  for(int i=0;i<ch_ct;i++)if(order[i]==note)for(;i<ch_ct;i++)order[i]=order[i+1];//fill in the gap
}
