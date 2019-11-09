#include "Animate_LED_matrix.h"
#include "user_letters.h"
#include "user_custom_letters.h"
//#define DEBUG

Animate_LED_matrix::Animate_LED_matrix(char local_rows[9],char local_cols[9]){
//#ifdef DEBUG
//  Serial.begin(9600);
//#endif  

//copy cols&rows pins numbers from user array to our local copy
  for(int i=0; local_rows[i]; ++i){
    this->rows[i]=local_rows[i];
    ++this->num_of_rows;
  }
  for(int i=0; local_cols[i]; ++i){
    this->cols[i]=local_cols[i];
    ++this->num_of_cols;    
  }

// Set rows and cols pins as output
  for(int i=0; this->rows[i]; ++i){
    pinMode(this->rows[i],OUTPUT);
    this->turn_off_everything();
  }

  for(int i=0; this->cols[i]; ++i){
    pinMode(this->cols[i],OUTPUT);
    this->turn_off_everything();
  }

  this->add_letter_symbols_to_array();
  this->current_base=0;
  this->current_letter=0;
}

void Animate_LED_matrix::add_letter_symbols_to_array(){
  ascii_letters_symbols['a']=letter_a;
  ascii_letters_symbols['b']=letter_b;
  ascii_letters_symbols['c']=letter_c;
  ascii_letters_symbols['d']=letter_d;
  ascii_letters_symbols['e']=letter_e;
  ascii_letters_symbols['f']=letter_f;
  ascii_letters_symbols['g']=letter_g;
  ascii_letters_symbols['h']=letter_h;
  ascii_letters_symbols['i']=letter_i;
  ascii_letters_symbols['j']=letter_j;
  ascii_letters_symbols['k']=letter_k;
  ascii_letters_symbols['l']=letter_l;
  ascii_letters_symbols['m']=letter_m;
  ascii_letters_symbols['n']=letter_n;
  ascii_letters_symbols['o']=letter_o;
  ascii_letters_symbols['p']=letter_p;
  ascii_letters_symbols['q']=letter_q;
  ascii_letters_symbols['r']=letter_r;
  ascii_letters_symbols['s']=letter_s;
  ascii_letters_symbols['t']=letter_t;
  ascii_letters_symbols['u']=letter_u;
  ascii_letters_symbols['v']=letter_v;
  ascii_letters_symbols['w']=letter_w;
  ascii_letters_symbols['x']=letter_x;
  ascii_letters_symbols['y']=letter_y;
  ascii_letters_symbols['z']=letter_z;

  custom_symbols[heart_sym]=heart;
  custom_symbols[smiley_sym]=smiley;
}

void Animate_LED_matrix::turn_off_everything(){
  for(int i=0; rows[i]; ++i){
    digitalWrite(rows[i],HIGH);
    }
  for(int i=0; cols[i]; ++i){
    digitalWrite(cols[i],LOW);
    }    
 }
  
void Animate_LED_matrix::turn_on_everything(){
  for(int i=0; rows[i]; ++i){
    digitalWrite(rows[i],LOW);
  }
  for(int i=0; cols[i]; ++i){
    digitalWrite(cols[i],HIGH);
  }    
}

void Animate_LED_matrix::copy_letter(char letter_to_copy[8]){
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      if(this->get_next_letter==true){
        this->next_letter_to_print[x][i]=bitRead(letter_to_copy[x],7-i);
      }else{
        this->letter_to_print[x][i]=bitRead(letter_to_copy[x],7-i);
      }
    }
  }  
}

void Animate_LED_matrix::get_letter(char letter_to_get){
  this->copy_letter(ascii_letters_symbols[letter_to_get]);
}

void Animate_LED_matrix::get_custom_letter(char letter_to_get){
  this->copy_letter(custom_symbols[letter_to_get]);
}

void Animate_LED_matrix::parse_msg(int index){
  char local_current_letter;
//  bool is_symbol=false;
//  String symbol;
  if(index < this->user_msg.length()){
  char local_current_letter=this->user_msg.charAt(index);
    //here it checks if it's start of custom character 
//      if(local_current_letter== '&' ){
//        is_symbol=true;
//        ++index;
//        if(!this->get_next_letter) ++this->current_letter;
//      }else{
        if(this->get_next_letter){
          this->get_letter(local_current_letter);
        }else{
          if(local_current_letter != this->user_last_letter){
          this->get_letter(local_current_letter);
          this->user_last_letter=local_current_letter;
          }
        }
//      }
    
//    if(is_symbol){
//      for(int x=0; x<this->user_msg.length(); ++x){
//        local_current_letter=this->user_msg.charAt(index);
//        if(local_current_letter!=';'){
//          symbol+=String(this->user_msg.charAt(index));
//          ++index;
//          if(!this->get_next_letter) ++this->current_letter;
//        }else{
//          is_symbol=false;
//          int symbol_int=symbol.toInt();
//          if(this->get_next_letter){
//            this->get_custom_letter(symbol_int);
//          }else{
//            if(symbol_int != this->user_last_letter ){
//              this->get_custom_letter(symbol_int);
//              this->user_last_letter=symbol_int;
//            }
//          }
//        }
//      }
//    } // is_symbol
  }
}

void Animate_LED_matrix::shift_original_symbol(){
  char start_point=this->current_base;
  char current_col_of_current_letter=this->current_base;
  //copy the same column to all rows 
  for(char i=0; i<this->num_of_cols; ++i){
    for(char x=0; x<this->num_of_rows; ++x){
      if(current_col_of_current_letter >= 8){
        this->shifted_symbol[x][i]=this->next_letter_to_print[x][start_point-8];  
      }else{
        this->shifted_symbol[x][i]=this->letter_to_print[x][start_point];  
      }
    } // end row loop
    ++start_point;
    ++current_col_of_current_letter;
  } // end column loop
}  

void Animate_LED_matrix::check_cols_active_in_all(){
  bool is_active=false;
  for(char i=0; i<this->num_of_cols; ++i){
    for(char x=0; x<this->num_of_rows; ++x){
      if(this->shifted_symbol[x][i]){
        is_active=true;
      }else{
        is_active=false;
        break;
      }
    }
    if(is_active){
      this->which_col_active_in_all[i]=1;
    }
  }
  for(char i=0; i<this->num_of_cols; ++i){
    if(this->which_col_active_in_all[i]){
      for(char x=0; x<this->num_of_rows; ++x){
        this->shifted_symbol[x][i]=0;
      }
    }
  }
}
  
void Animate_LED_matrix::turn_on_cols_active_in_all(){
  this->check_cols_active_in_all();
  for(char i=0; rows[i]; ++i){
    digitalWrite(rows[i],LOW);
  }
  
  for(char i=0; cols[i]; ++i){
    if(this->which_col_active_in_all[i]){
      digitalWrite(cols[i],HIGH);
    }else{
      continue;  
    }
  }

  delay(this->global_delay);
}

void Animate_LED_matrix::check_cols_have_2_dots_turned_on_on_seris(){
  char num_of_dots_active=0;
  for(char i=0; i<this->num_of_cols; ++i){
    for(char x=0; x<this->num_of_rows; ++x){
      if(num_of_dots_active < 2){
        if(this->shifted_symbol[x][i]){
          ++num_of_dots_active;
        }  
      }else{
        break;
      }
    }
    if(num_of_dots_active>1){
      this->which_col_have_2_dots_turned_on_on_seris[i]=1;
    }
  }
}
  
  
void Animate_LED_matrix::turn_on_cols_have_2_dots_turned_on_on_seris(){
  this->check_cols_have_2_dots_turned_on_on_seris();

  for(char i=0; i<8; ++i){
    if(which_col_have_2_dots_turned_on_on_seris[i]){
      for(char x=0; rows[x]; ++x){
        if(this->shifted_symbol[x][i]){
          digitalWrite(rows[x],LOW);
        }
      }
    digitalWrite(cols[i],HIGH);
    delay(this->global_delay);
    this->turn_off_everything();
    }
  }

  for(char i=0; i<num_of_cols; ++i){
    if(this->which_col_have_2_dots_turned_on_on_seris[i]){
      for(char x=0; x<num_of_rows; ++x){
        this->shifted_symbol[x][i]=0;
      }
    }
  }
}
  
void Animate_LED_matrix::turn_on_cols_have_sperated_dots_turned_on(){
  for(char i=0; cols[i]; ++i){
    for(char x=0; rows[x]; ++x){
      if(this->shifted_symbol[x][i]){
        digitalWrite(rows[x],LOW);
        digitalWrite(cols[i],HIGH);
      }
    }
  }
  delay(this->global_delay);
  this->turn_off_everything();
}
  
  
void Animate_LED_matrix::start_animation(){
  this->shift_original_symbol();
  
#ifdef DEBUG
  Serial.println("call turn_on_cols_active_in_all()");
#endif

  this->turn_on_cols_active_in_all();
  this->turn_off_everything();

#ifdef DEBUG
  Serial.println("call turn_on_cols_have_2_dots_turned_on_on_seris()");
#endif      
  this->turn_on_cols_have_2_dots_turned_on_on_seris();
  this->turn_off_everything();

#ifdef DEBUG
  Serial.println("call turn_on_cols_have_sperated_dots_turned_on()");
#endif    
  this->turn_on_cols_have_sperated_dots_turned_on();
}
  
void Animate_LED_matrix::reset_everything(){
  for(char i=0; i<8; ++i){
    this->which_col_active_in_all[i]=0;
    this->which_col_have_2_dots_turned_on_on_seris[i]=0;
  }
  if(this->current_base==8){
    this->current_base=0;
    if(this->current_letter+1 < this->user_msg.length()){
      this->current_letter+=1;
    }else if(this->current_letter+1 >= this->user_msg.length()){
      this->current_letter=0;
    }
    if(this->current_letter+2 <= this->user_msg.length()){
      this->is_there_next_letter=true;
    }else if(this->current_letter+2 > this->user_msg.length()){
      this->is_there_next_letter=false;
    }
  }  
}

void Animate_LED_matrix::loop_animation() {
#ifdef DEBUG
  Serial.println("start animation");
#endif

//this get first letter of the text once to start animating the text
  if(this->current_letter == this->user_msg.length()-1){
//    this->parse_msg(0);
    this->get_next_letter=true;
    this->parse_msg(0);
    this->get_next_letter=false;
//    this->parse_msg(0);
//    this->current_letter=0;
//    this->current_base=0;
  }else if(this->current_letter < this->user_msg.length()-1 ){
    this->parse_msg(this->current_letter);
    this->get_next_letter=true;
    this->parse_msg((this->is_there_next_letter? this->current_letter+1 : 0));
    this->get_next_letter=false;
  }

//repeat each animation n of times 
// increase n to make the animation slower
  int counter=0;
  start:
  this->start_animation();
  delay(1);
  if(counter<10){
    ++counter;
    goto start;
  }
  
#ifdef DEBUG
  Serial.println("finished animation");
#endif     
  this->turn_off_everything();
  ++this->current_base;
  this->reset_everything();
}

