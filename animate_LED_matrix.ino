/* 
 * 
 * 
*/


#include "animate_LED_matrix.h"

char rows[9]={A3,A2,A1,A0,13,12,11,10,0};
char cols[9]={9,8,7,6,5,4,3,2,0};

Animate_LED_matrix Animate_var(rows,cols);

void setup(){
  //Serial.begin(9600);  
  Animate_var.global_delay=1;
 
  Animate_var.user_msg="abcdefghijklmnopqrstuvwxyz";
}


void loop(){
  //Animate_var.loop_animation();
  Animate_var.turn_on_everything();
}
