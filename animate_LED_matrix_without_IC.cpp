#include "Animate_LED_matrix_without_IC.h"
#include "user_string.h"
//#define DEBUG

Animate_LED_matrix_without_IC::Animate_LED_matrix_without_IC(int local_rows[9],int local_cols[9],String user_msg){
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
  
//count the length of user string
  this->count_original_symbol_lenth();

}

void Animate_LED_matrix_without_IC::count_original_symbol_lenth(){
  bool is_symbol=false;
  for (int i=0; i< this->user_msg.length(); ++i){
    String current_user_msg_letter=String(this->user_msg.charAt(i));
    if(is_symbol){
      if(current_user_msg_letter==";"){
        is_symbol=false;
        ++this->original_text_to_display_lenth;
      }
    }else{
    //here it checks if it's start of custom character 
      if(current_user_msg_letter== "&" ){
        is_symbol=true;
      }else{
        ++this->original_text_to_display_lenth;
      }
    }
  }
}

void Animate_LED_matrix_without_IC::turn_off_everything(){
  for(int i=0; rows[i]; ++i){
    digitalWrite(rows[i],HIGH);
    }
  for(int i=0; cols[i]; ++i){
    digitalWrite(cols[i],LOW);
    }    
 }
  
void Animate_LED_matrix_without_IC::turn_on_everything(){
  for(int i=0; rows[i]; ++i){
    digitalWrite(rows[i],LOW);
  }
  for(int i=0; cols[i]; ++i){
    digitalWrite(cols[i],HIGH);
  }    
}

void Animate_LED_matrix_without_IC::copy_letter(char letter_to_copy[8][8]){
#ifdef DEBUG
  Serial.println("here is copy_letter()");
#endif    
  for(int i=0; i<this->num_of_cols; ++i){
    Serial.print(i);
    Serial.print(": ");
    for(int x=0; x<this->num_of_rows; ++x){
      this->letter_to_print[x][i]=letter_to_copy[x][i];
      String a=String(this->letter_to_print[x][i]);
      a.toInt();
      Serial.print(a);
      Serial.print(",");
    }
    Serial.println("");
  }  
}

void Animate_LED_matrix_without_IC::get_letter(String letter_to_get){
#ifdef DEBUG
  Serial.println("here is get_letter();");
#endif 
#ifdef DEBUG
  Serial.print("letter_to_get = ");
  Serial.println(letter_to_get);
#endif 
    if(letter_to_get=="a"){
#ifdef DEBUG
  Serial.println("call copy_letter()");
#endif        
      this->copy_letter(letter_a);
    }
    if(letter_to_get=="p"){
#ifdef DEBUG
  Serial.println("call copy_letter()");
#endif        
      this->copy_letter(letter_p);
    }
    if(letter_to_get=="y"){this->copy_letter(letter_y);}
    if(letter_to_get=="h"){this->copy_letter(letter_h);}
}

void Animate_LED_matrix_without_IC::parse_msg(int index){
#ifdef DEBUG
  Serial.println("here is parse_msg();");
#endif  
  String local_current_letter;
  bool is_symbol=false;
  String symbol;
  if(index < this->user_msg.length()){
    String local_current_letter=String(this->user_msg.charAt(index));
    
    //here it checks if it's start of custom character 
      if(local_current_letter== "&" ){
        is_symbol=true;
        ++index;
        ++this->current_letter;
      }else{
#ifdef DEBUG
  Serial.println("the letter is not symbol");
#endif   
#ifdef DEBUG
  Serial.print("current letter is: ");
  Serial.println(local_current_letter);
  Serial.print("user last letter is: ");
  Serial.println(this->user_last_letter);
#endif           
        if(local_current_letter != this->user_last_letter){
#ifdef DEBUG
  Serial.println("call get_letter()");
#endif             
          this->get_letter(local_current_letter);
          this->user_last_letter=local_current_letter;
        }
      }
    
    if(is_symbol){
#ifdef DEBUG
  Serial.println("the letter is symbol");
#endif       
      for(int x=0; x<this->user_msg.length(); ++x){
        local_current_letter=String(this->user_msg.charAt(index));
        if(local_current_letter!=";"){
          symbol+=local_current_letter;
          ++index;
          ++this->current_letter;
        }else{
          is_symbol=false;
          if(symbol != this->user_last_letter){
            //this->get_custom_letter(symbol);
            this->user_last_letter=symbol;
          }
        }
      }
    }
  }
}

void Animate_LED_matrix_without_IC::shift_original_symbol(){
#ifdef DEBUG
  int print_debug_info_once=0;  
#endif
  int start_point=this->current_base;
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      if(start_point>=8){
        if(this->is_there_next_letter){
#ifdef DEBUG
  Serial.println("call parse_msg() to get the next letter");
#endif           
          this->parse_msg(this->current_letter+1);  
        }else{
#ifdef DEBUG
  Serial.println("call parse_msg() to get the next letter");
#endif           
          this->parse_msg(0);  
        }
        this->shifted_symbol[x][i]=this->letter_to_print[x][start_point-8];   
      }else{
#ifdef DEBUG       
  if(print_debug_info_once <1){
    ++print_debug_info_once;
    Serial.println("call parse_msg() to get the current letter");
  }
#endif           
        //this->parse_msg(this->current_letter); 
        this->shifted_symbol[x][i]=this->letter_to_print[x][start_point];
      }
    }
    ++start_point;
  }
}  
  
void Animate_LED_matrix_without_IC::make_copy_of_shifted_symbol(){
  for(int i=0; i<this->num_of_cols; ++i){
    for(int x=0; x<this->num_of_rows; ++x){
      this->copy_of_shifted_symbol[x][i]=this->shifted_symbol[x][i];
    }
  }
}
  
void Animate_LED_matrix_without_IC::check_cols_active_in_all(){
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
  
void Animate_LED_matrix_without_IC::turn_on_cols_active_in_all(){
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

void Animate_LED_matrix_without_IC::check_cols_have_2_dots_turned_on_on_seris(){
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
  
  
void Animate_LED_matrix_without_IC::turn_on_cols_have_2_dots_turned_on_on_seris(){
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
  
void Animate_LED_matrix_without_IC::turn_on_cols_have_sperated_dots_turned_on(){
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
  
  
void Animate_LED_matrix_without_IC::start_animation(){
#ifdef DEBUG
  Serial.println("call shift_original_symbol()");
#endif  
  this->shift_original_symbol();
#ifdef DEBUG
  Serial.println("call make_copy_of_shifted_symbol()");
#endif    
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
  
void Animate_LED_matrix_without_IC::reset_everything(){
  for(int i=0; i<8; ++i){
    this->which_col_active_in_all[i]=0;
    this->which_col_have_2_dots_turned_on_on_seris[i]=0;
  }
  if(this->current_base==7){
    this->current_base=0;
    if((this->current_letter+1) < this->original_text_to_display_lenth){
      ++this->current_letter;
    }else if(this->current_letter+1 >=this->original_text_to_display_lenth){
      this->current_letter=0;
    }
    if((this->current_letter+2) <= this->original_text_to_display_lenth){
      this->is_there_next_letter=true;
    }else if((this->current_letter+2) > this->original_text_to_display_lenth){
      this->is_there_next_letter=false;
    }
  }
}

void Animate_LED_matrix_without_IC::loop_animation() {
#ifdef DEBUG
  Serial.println("start animation");
#endif
this->parse_msg(this->current_letter);
//repeat each animation n of times
  int counter=0;
  start:
  this->start_animation();
  delay(1);
  if(counter<10){
#ifdef DEBUG
  Serial.print("Counter value is: ");
  Serial.println(counter);
#endif    
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

