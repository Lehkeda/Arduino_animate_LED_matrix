#include "Animate_LED_matrix.h"
#include "user_string.h"
//#define DEBUG

Animate_LED_matrix::Animate_LED_matrix(int local_rows[9],int local_cols[9],String user_msg){
//#ifdef DEBUG
  Serial.begin(9600);
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

  this->user_msg=user_msg;
  
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

void Animate_LED_matrix::copy_letter(char letter_to_copy[8][8]){
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      if(this->get_next_letter==true){
        this->next_letter_to_print[x][i]=letter_to_copy[x][i];
      }else{
        this->letter_to_print[x][i]=letter_to_copy[x][i];
      }
    }
  }  
}

void Animate_LED_matrix::get_letter(String letter_to_get){
    if(letter_to_get=="a"){this->copy_letter(letter_a);}
    if(letter_to_get=="b"){this->copy_letter(letter_b);}
    if(letter_to_get=="p"){this->copy_letter(letter_p);}
    if(letter_to_get=="y"){this->copy_letter(letter_y);}
    if(letter_to_get=="h"){this->copy_letter(letter_h);}
}

void Animate_LED_matrix::get_custom_letter(String letter_to_get){
    if(letter_to_get=="heart"){this->copy_letter(heart);}
    if(letter_to_get=="smiley"){this->copy_letter(smiley);}
}

void Animate_LED_matrix::parse_msg(int index){
  String local_current_letter;
  bool is_symbol=false;
  String symbol;
  if(index < this->user_msg.length()){
    String local_current_letter=String(this->user_msg.charAt(index));
    //here it checks if it's start of custom character 
      if(local_current_letter== "&" ){
        is_symbol=true;
        ++index;
        if(!this->get_next_letter) ++this->current_letter;
      }else{
        if(this->get_next_letter){
          this->get_letter(local_current_letter);
        }else{
          if(local_current_letter != this->user_last_letter){
            this->get_letter(local_current_letter);
            this->user_last_letter=local_current_letter;
          }
        }
      }
    
    if(is_symbol){
      for(int x=0; x<this->user_msg.length(); ++x){
        local_current_letter=String(this->user_msg.charAt(index));
        if(local_current_letter!=";"){
          symbol+=local_current_letter;
          ++index;
          if(!this->get_next_letter) ++this->current_letter;
        }else{
          is_symbol=false;
          if(this->get_next_letter){
            this->get_custom_letter(symbol);
          }else{
            if(symbol != this->user_last_letter){
              this->get_custom_letter(symbol);
              this->user_last_letter=symbol;
            }
          }
        }
      }
    } // is_symbol
  }
}

void Animate_LED_matrix::shift_original_symbol(){
  int start_point=this->current_base;
  int current_col_of_current_letter=this->current_base;
  //copy the same column to all rows 
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
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
  
void Animate_LED_matrix::make_copy_of_shifted_symbol(){
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      this->copy_of_shifted_symbol[x][i]=this->shifted_symbol[x][i];
    }
  }
}
  
void Animate_LED_matrix::check_cols_active_in_all(){
  bool is_active=false;
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      if(this->copy_of_shifted_symbol[x][i]){
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
  for(int i=0; i<this->num_of_cols; ++i){
    if(this->which_col_active_in_all[i]){
      for(int x=0; x<this->num_of_rows; ++x){
        this->copy_of_shifted_symbol[x][i]=0;
      }
    }
  }
}
  
void Animate_LED_matrix::turn_on_cols_active_in_all(){
  this->check_cols_active_in_all();
  for(int i=0; rows[i]; ++i){
    digitalWrite(rows[i],LOW);
  }
  
  for(int i=0; cols[i]; ++i){
    if(this->which_col_active_in_all[i]){
      digitalWrite(cols[i],HIGH);
    }else{
      continue;  
    }
  }

  delay(this->global_delay);
}

void Animate_LED_matrix::check_cols_have_2_dots_turned_on_on_seris(){
  int num_of_dots_active=0;
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      if(num_of_dots_active < 2){
        if(this->copy_of_shifted_symbol[x][i]){
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

  for(int i=0; i<8; ++i){
    if(which_col_have_2_dots_turned_on_on_seris[i]){
      for(int x=0; rows[x]; ++x){
        if(this->copy_of_shifted_symbol[x][i]){
          digitalWrite(rows[x],LOW);
        }
      }
    digitalWrite(cols[i],HIGH);
    delay(this->global_delay);
    this->turn_off_everything();
    }
  }

  for(int i=0; i<num_of_cols; ++i){
    if(this->which_col_have_2_dots_turned_on_on_seris[i]){
      for(int x=0; x<num_of_rows; ++x){
        this->copy_of_shifted_symbol[x][i]=0;
      }
    }
  }
}
  
void Animate_LED_matrix::turn_on_cols_have_sperated_dots_turned_on(){
  for(int i=0; cols[i]; ++i){
    for(int x=0; rows[x]; ++x){
      if(this->copy_of_shifted_symbol[x][i]){
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
  this->make_copy_of_shifted_symbol();
  
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
  for(int i=0; i<8; ++i){
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
    this->get_next_letter=true;
    this->parse_msg(0);
    this->get_next_letter=false;
    //this->current_letter=0;
    //this->current_base=0;
  }else if(this->current_letter < this->user_msg.length()-1){
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

