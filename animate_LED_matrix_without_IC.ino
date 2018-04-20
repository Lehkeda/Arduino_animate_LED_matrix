/* 
 * 
 * 
*/


#include "animate_LED_matrix_without_IC.h"
#include "user_string.h"

int rows[9]={10,11,12,13,A0,A1,A2,A3,0};
int cols[9]={9,8,7,6,5,4,3,2,0};
//char (*original_symbol[])[8]={heart,letter_h,letter_a,letter_p,letter_p,letter_y,heart, /*happy*/
//                             letter_d,letter_a,letter_y,heart,0}; /*day*/

Animate_LED_matrix_without_IC Animate_var(rows,cols,"yahay");

void setup(){
  //Serial.begin(9600);  
  //Serial.print("original_text_to_display_lenth= ");
  //Serial.println(Animate_var.original_text_to_display_lenth);
  Animate_var.global_delay=1;
}




void loop(){
  Animate_var.loop_animation();
}
