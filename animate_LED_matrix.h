#ifndef ANIMATE_LED_MATRIX
#define ANIMATE_LED_MATRIX
#include <Arduino.h>

class Animate_LED_matrix{
  public:
    Animate_LED_matrix(int local_rows[9],int local_cols[9],String);
    int global_delay;
    void loop_animation();
    String user_msg;

  private:
    int rows[9];
    int cols[9];
    int num_of_rows;
    int num_of_cols;
    char letter_to_print[8][8];
    char next_letter_to_print[8][8];
    bool get_next_letter=true;
    void copy_letter(char letter_to_copy[8][8]);
    void get_letter(String);
    void get_custom_letter(String);
    String user_current_letter;
    String user_last_letter;
    void parse_msg(int);
    int index_of_current_letter;
    bool is_finished;
    void turn_on_everything();
    void turn_off_everything();
    char shifted_symbol[8][8];
    char copy_of_shifted_symbol[8][8];
    int current_base=0;
    int current_letter=0;
    bool is_there_next_letter=true;
  
    void shift_original_symbol();
    void make_copy_of_shifted_symbol();

    char which_col_active_in_all[8]={0,0,0,0,0,0,0,0};
  
    void check_cols_active_in_all();
    void turn_on_cols_active_in_all();
    char which_col_have_2_dots_turned_on_on_seris[8]={0,0,0,0,0,0,0,0};
  
    void check_cols_have_2_dots_turned_on_on_seris();
  
    void turn_on_cols_have_2_dots_turned_on_on_seris();
    void turn_on_cols_have_sperated_dots_turned_on();
  
    void reset_everything();
    void start_animation();

};

#endif //Animate_LED_matrix