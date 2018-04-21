/* 
 * 
 * 
*/


#include "animate_LED_matrix_without_IC.h"
#include "user_string.h"

int rows[9]={10,11,12,13,A0,A1,A2,A3,0};
int cols[9]={9,8,7,6,5,4,3,2,0};

Animate_LED_matrix_without_IC Animate_var(rows,cols,"ya&heart;hay&heart;");

void setup(){
  Serial.begin(9600);  
  Animate_var.global_delay=1;
}


void loop(){
  Animate_var.loop_animation();
}
