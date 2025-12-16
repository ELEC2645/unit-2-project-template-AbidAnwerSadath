#ifndef FUNCS_H
#define FUNCS_H

// Ohm's Law functions
void ohms_law_menu(void);
float calculate_voltage(float current, float resistance);
float calculate_current(float voltage, float resistance);
float calculate_resistance(float voltage, float current);
float calculate_power(float voltage, float current);

// RC Filter functions
void rc_filter_menu(void);
float calculate_time_constant(float resistance, float capacitance);
float calculate_cutoff_frequency(float resistance, float capacitance);

// Resistor colour code functions
void resistor_code_menu(void);
void colour_code_to_resistance(void);
void resistance_to_colour_code(void);
int colour_to_digit(const char *colour);
int colour_to_multiplier(const char *colour);
const char* colour_to_tolerance(const char *colour);

// Binary/Hex/Decimal Converter Functions 
void number_converter_menu(void);
void decimal_to_binary(int decimal);
void decimal_to_hex(int decimal);
void binary_to_decimal(const char *binary);
void binary_to_hex(const char *binary);
void hex_to_decimal(const char *hex);
void hex_to_binary(const char *hex);

//Main menu item handlers
void menu_item_1(void);
void menu_item_2(void);
void menu_item_3(void);
void menu_item_4(void);

#endif
