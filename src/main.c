int lmf=6,lmb=7,rmf=8,rmb=9;   //motor var
int sf1,sf2,sf3,sb1,sb2,sb3,sb4,sb5;   // sensor var
int w=43,g=45;    //white and green led var
int red[4]={53,51,49,47};   // red led array var
int lstrip[4]={0,0,0,0},rstrip[4]={0,0,0,0};  // strip array var
int ana_sb4=40,ana_sb2=40,ana_sf1=40,ana_sf3=40;  // analog threshold
int i;  // loop counter var
int set_count=-1;  //set ABCD var.... -1 for first turn.... increment on turn
int led_count=0;  // sets red led pattern for each set .... reset to 0 at each turn
int pattern[4]; //first coordinate pattern path  ... triggered at turn
int error=0;
float kpf=0.5;
float kpb=0.5;
float P;
int correction;
int motor_speed=200;
int motor_turn_speed=127;

void setup()
{
  pinMode(lmf,OUTPUT);
  pinMode(lmb,OUTPUT);
  pinMode(rmf,OUTPUT);
  pinMode(rmb,OUTPUT);
  pinMode(w,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(red[3],OUTPUT);
  pinMode(red[2],OUTPUT);
  pinMode(red[1],OUTPUT);
  pinMode(red[0],OUTPUT);
  pinMode(22,INPUT);
  pinMode(24,INPUT);
  pinMode(A0,INPUT);
  pinMode(26,INPUT);
  pinMode(A1,INPUT);
  pinMode(28,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  digitalWrite(lmb,LOW);
  digitalWrite(rmb,LOW);
}
void loop()
{
  sensorRead();
  if(sf2==HIGH) 
  {
    if(sb1==LOW && sb5==LOW)
    {
      error=err_func(sb2,ana_sb2,sb4,ana_sb4);
      P=error*kpb;
      correction=P;
      motor_drive(correction);
    }
    else if(sb5==HIGH && sb4<=ana_sb4)   //check threshold condition right strip
    {
      rstrip[set_count]++;     //...  set_count=0 for first set
      digitalWrite(red[led_count],HIGH);    //... red led glows
      led_count++;
      while(sb5==HIGH && sb4<=ana_sb4)
      {
         stripStraight(); 
      }
    }
    else if(sb1==HIGH && sb2<=ana_sb2)   //check threshold condition
    {
      lstrip[set_count]++;     //...  set_count=0 for first set
      digitalWrite(red[led_count],LOW);    //... red led glows
      led_count++;
      while(sb1==HIGH && sb2<=ana_sb2)
      {
         stripStraight(); 
      }
    }    
  }
  else
  {
    if(sb5==HIGH && sb4<=ana_sb4)
    {
      while(sf2!=HIGH)
      {
        analogWrite(lmf,motor_turn_speed);
        analogWrite(rmf,0);
      }
      for(i=0;i<4;i++)
      {
        digitalWrite(red[i],LOW);
      }
      led_count=0;
      if(set_count>-1)
      {
        pattern_create(rstrip[set_count],lstrip[set_count],set_count);  
      }
      set_count++;      
    }
    else if(sb1==HIGH && sb2<=ana_sb2)
    {
      while(sf2!=HIGH)
      {
        analogWrite(lmf,0);
        analogWrite(rmf,motor_turn_speed);
      }
      for(i=0;i<4;i++)
      {
        digitalWrite(red[i],LOW);
      }
      led_count=0;
      pattern_create(rstrip[set_count],lstrip[set_count],set_count);  
      set_count++;
    }
    else if(sb1==LOW && sb5==LOW)
    {
      error=err_func(sb2,ana_sb2,sb4,ana_sb4);
      P=error*kpb;
      correction=P;
      motor_drive(correction);
    }
  }
}
void sensorRead()
{
  sf2=digitalRead(22);
  sb1=digitalRead(24);
  sb2=analogRead(A0);
  sb3=digitalRead(26);
  sb4=analogRead(A1);
  sb5=digitalRead(28);
}
void aheadAnalogRead()
{
  sf1=analogRead(A2);
  sf3=analogRead(A3);
}
void stripStraight()
{
  aheadAnalogRead();
  error=err_func(sf1,ana_sf1,sf3,ana_sf3);
      P=error*kpf;
      correction=P;
      motor_drive(correction);
}
void pattern_create(int r,int l,int count)
{
  if(r>l)
  {
    pattern[count]=1;
  }
  else if(l>r)
  {
    pattern[count]=-1;
  }
  else
  {
    pattern[count]=0;
  }
}
int err_func(int s1,int set1,int s2,int set2)
{
    if((set1-s1)>(set2-s2))
      {
        return(set1-s1);   //bot tending rightwards
      }
    else if((set1-s1)<(set2-s2))
      {
        return((set2-s2)*(-1));   //bot tending lefttwards
      }
    else
      {
        return(0);    //bot moving perfect straight
      }
}
void motor_drive(int C)
{
  analogWrite(lmf,(motor_speed-C));
  analogWrite(rmf,(motor_speed+C));
}
