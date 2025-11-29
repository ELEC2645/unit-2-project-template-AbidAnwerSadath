#ifndef FUNCS_H
#define FUNCS_H

//Ohm's Law Functions
void ohms_law_menu(void);
float calculate_voltage(float current, float resistance);
float calculate_resistance(float voltage, float current);
float calculate_current(float voltage, float resistance);
float calculate_power(float votage, float current);

//RC Filter fc = 1/ (2*pi*RC)
void rc_filter_menu(void);
float calculate_cutoff_frequency(float resistance, float capacitance);

// Binary/Hexadecimal/Decimal Converter
void number_converter_menu(void);

void decimal_to_binary(int decimal);  //Decimal Conversions
void decimal_to_hex(int decimal);

void binary_to_decimal(const char *binary); // Binary Conversions
void binary_to_hex(const char *binary);

void hex_to_decimal(const char *hex); // Hexadecimal conversions
void hex_to_binary(const char *hex);

// Resistor colour code -> value converter
void resistor_code_menu(void);
int read_resistor_code(int band1, int band2, int band3);

#endif