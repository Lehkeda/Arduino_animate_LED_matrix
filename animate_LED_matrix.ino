/* 
 * 
 * 
*/


#include "animate_LED_matrix.h"
#include "user_string.h"

int rows[9]={10,11,12,13,A0,A1,A2,A3,0};
int cols[9]={9,8,7,6,5,4,3,2,0};

Animate_LED_matrix Animate_var(rows,cols,"ya&heart;&smiley;&smiley;&smiley;hay&heart;ba&smiley;");

void setup(){
  //Serial.begin(9600);  
  Animate_var.global_delay=1;
}


void loop(){
  Animate_var.loop_animation();
}
