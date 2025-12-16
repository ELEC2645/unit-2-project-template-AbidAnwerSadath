#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "funcs.h"

// Helper functions
static int is_integer(const char *s) {
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-') s++;
    if (!isdigit((unsigned char)*s)) return 0;
    while (*s && isdigit((unsigned char)*s)) s++;
    return *s == '\0';
}

float get_float_input(const char *prompt) {
    char buf[128];
    float value;

    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        buf[strcspn(buf, "\n")] = '\0';

        char *end;
        value = strtof(buf, &end);

        if (end == buf || *end != '\0') continue;
        return value;
    }
}

int get_int_input(const char *prompt) {
    char buf[128];
    int value;

    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        buf[strcspn(buf, "\n")] = '\0';

        if (!is_integer(buf)) continue;

        value = (int)strtol(buf, NULL, 10);
        return value;
    }
}

void get_string_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

static int get_menu_choice(void (*menu_func)(void)) {
    char buf[128];
    menu_func();
    printf("\nSelect option: ");
    fflush(stdout);
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    buf[strcspn(buf, "\n")] = '\0';
    return is_integer(buf) ? atoi(buf) : -1;
}

/* Ohm's Law */
void ohms_law_menu(void) {
    printf("\n>> Ohm's Law Calculator\n"
           "\n1. Calculate Voltage (V = I × R)\n"
           "2. Calculate Current (I = V / R)\n"
           "3. Calculate Resistance (R = V / I)\n"
           "4. Calculate Power (P = V × I)\n"
           "0. Back to Main Menu\n");
}

float calculate_voltage(float current, float resistance) { return current * resistance; }

float calculate_current(float voltage, float resistance) {
    if (resistance == 0) { printf("\nError: Resistance cannot be zero!\n"); return 0; }
    return voltage / resistance;
}

float calculate_resistance(float voltage, float current) {
    if (current == 0) { printf("\nError: Current cannot be zero!\n"); return 0; }
    return voltage / current;
}

float calculate_power(float voltage, float current) { return voltage * current; }

// Menu handling
void menu_item_1(void) {
    char buf[16];

    for(;;) {
        int choice = get_menu_choice(ohms_law_menu);
        if (choice == 0) return;
        
        float v, i, r;
        switch(choice) {
            case 1:
                i = get_float_input("\nEnter Current (A): ");
                r = get_float_input("Enter Resistance (Ω): ");
                printf("\nVoltage = %.2f V\n", calculate_voltage(i, r));
                break;
            case 2:
                v = get_float_input("\nEnter Voltage (V): ");
                r = get_float_input("Enter Resistance (Ω): ");
                if ((i = calculate_current(v, r)) != 0) printf("\nCurrent = %.2f A\n", i);
                break;
            case 3:
                v = get_float_input("\nEnter Voltage (V): ");
                i = get_float_input("Enter Current (A): ");
                if ((r = calculate_resistance(v, i)) != 0) printf("\nResistance = %.2f Ω\n", r);
                break;
            case 4:
                v = get_float_input("\nEnter Voltage (V): ");
                i = get_float_input("Enter Current (A): ");
                printf("\nPower = %.2f W\n", calculate_power(v, i));
                break;
            default: printf("Invalid option!\n"); continue;
        }
        get_string_input("\nPress Enter to continue...", buf, sizeof(buf));
    }
}

// RC Filter
void rc_filter_menu(void) {
    printf("\n>> RC Filter Analyser\n"
           "\n1. Calculate Time Constant (τ = R × C)\n"
           "2. Calculate Cutoff Frequency (fc = 1 / (2π × R × C))\n"
           "0. Back to Main Menu\n");
}

float calculate_time_constant(float resistance, float capacitance) { return resistance * capacitance; }

float calculate_cutoff_frequency(float resistance, float capacitance) {
    if (resistance == 0 || capacitance == 0) { 
        printf("\nError: Resistance and Capacitance must be non-zero!\n"); 
        return 0; 
    }
    return 1.0 / (2.0 * M_PI * resistance * capacitance);
}

void menu_item_2(void) {
    char buf[16];

    for(;;) {
        int choice = get_menu_choice(rc_filter_menu);
        if (choice == 0) return;
        
        float r = get_float_input("\nEnter Resistance (Ω): ");
        float c = get_float_input("Enter Capacitance (F): ");
        
        if (choice == 1) printf("\nTime Constant (τ) = %.6f seconds\n", calculate_time_constant(r, c));
        else if (choice == 2) {
            float fc = calculate_cutoff_frequency(r, c);
            if (fc != 0) printf("\nCutoff Frequency (fc) = %.2f Hz\n", fc);
        } else { printf("Invalid option!\n"); continue; }
        
        get_string_input("\nPress Enter to continue...", buf, sizeof(buf));
    }
}

// Resistor Colour Code
typedef struct {
    const char *name;
    int digit;
    int multiplier;
    const char *tolerance;
} ColourData;

static const ColourData colour_table[] = {
    {"black", 0, 1, NULL}, {"brown", 1, 10, "±1%"}, {"red", 2, 100, "±2%"},
    {"orange", 3, 1000, "±3%"}, {"yellow", 4, 10000, "±4%"}, {"green", 5, 100000, "±0.5%"},
    {"blue", 6, 1000000, "±0.25%"}, {"violet", 7, 10000000, "±0.1%"},
    {"grey", 8, 100000000, "±0.05%"}, {"white", 9, 1000000000, NULL}, {"gold", -1, -1, "±5%"}, {"silver", -1, -2, "±10%"},
    {NULL, -1, -999, NULL}
};

int colour_to_digit(const char *colour) {
    for (int i = 0; colour_table[i].name; i++)
        if (strcmp(colour, colour_table[i].name) == 0) return colour_table[i].digit;
    return -1;
}

int colour_to_multiplier(const char *colour) {
    for (int i = 0; colour_table[i].name; i++)
        if (strcmp(colour, colour_table[i].name) == 0) return colour_table[i].multiplier;
    return -999;
}

const char* colour_to_tolerance(const char *colour) {
    for (int i = 0; colour_table[i].name; i++)
        if (strcmp(colour, colour_table[i].name) == 0) 
            return colour_table[i].tolerance ? colour_table[i].tolerance : "Unknown";
    return "Unknown";
}

void resistor_code_menu(void) {
    printf("\n>> 4 Band Resistor Colour Code \n"
           "\n1. Colour codes to Resistance value\n"
           "2. Resistance value to Colour codes\n"
           "0. Back to Main Menu\n");
}

void colour_code_to_resistance(void) {
    char band1[20], band2[20], band3[20], tolerance[20];

    printf("\nValid colours for digits (bands 1 & 2): black, brown, red, orange, yellow, green, blue, violet, grey, white\n");
    printf("Valid colours for multiplier (band 3): black, brown, red, orange, yellow, green, blue, violet, grey, white, gold, silver\n");
    printf("Valid colours for tolerance (band 4): brown, red, orange, yellow, green, blue, violet, grey, gold, silver\n\n");

    get_string_input("Enter first digit colour: ", band1, sizeof(band1));
    get_string_input("Enter second digit colour: ", band2, sizeof(band2));
    get_string_input("Enter multiplier colour: ", band3, sizeof(band3));
    get_string_input("Enter tolerance colour: ", tolerance, sizeof(tolerance));
    
    int digit1 = colour_to_digit(band1);
    int digit2 = colour_to_digit(band2);
    int multiplier = colour_to_multiplier(band3);
    const char *tol = colour_to_tolerance(tolerance);
    
    if (digit1 == -1 || digit2 == -1 || multiplier == -999) {
        printf("\nInvalid colour entered!\n");
        return;
    }
    
    int base_value = (digit1 * 10) + digit2;
    float resistance = (multiplier == -1) ? base_value * 0.1 : 
                       (multiplier == -2) ? base_value * 0.01 : base_value * multiplier;
    const char *mult_str = (multiplier == -1) ? "gold (×0.1)" : 
                           (multiplier == -2) ? "silver (×0.01)" : "";
    
    printf("\n--- Resistor Value ---\n");
    printf("Base value: %d\n", base_value);
    if (multiplier < 0) printf("Multiplier: %s\n", mult_str);
    else printf("Multiplier: %d\n", multiplier);
    printf("Resistance: %.2f Ohms\n", resistance);
    printf("Tolerance: %s\n", tol);
}

void resistance_to_colour_code(void) {
    float resistance;
    resistance = get_float_input("\nEnter resistance value in Ohms: ");

    if (resistance <= 0) { printf("\nInvalid resistance value!\n"); return; }
    
    float temp = resistance;
    int multiplier_index = 0;
    while (temp >= 100) { temp /= 10; multiplier_index++; }
    while (temp < 10 && multiplier_index > -2) { temp *= 10; multiplier_index--; }
    
    int digit1 = (int)temp / 10;
    int digit2 = (int)temp % 10;
    const char *colours[] = {"black", "brown", "red", "orange", "yellow",
                             "green", "blue", "violet", "grey", "white"};
    
    const char *mult_colour = (multiplier_index == -1) ? "gold" :
                              (multiplier_index == -2) ? "silver" :
                              (multiplier_index >= 0 && multiplier_index <= 9) ? colours[multiplier_index] : NULL;
    
    if (!mult_colour) { printf("\nResistance value out of standard range!\n"); return; }
    
    float mult_val = (multiplier_index == -1) ? 0.1 : (multiplier_index == -2) ? 0.01 : 
                     pow(10, multiplier_index);
    
    printf("\n--- Colour Code ---\n");
    printf("First band (digit 1): %s\n", colours[digit1]);
    printf("Second band (digit 2): %s\n", colours[digit2]);
    printf("Multiplier band: %s (×%.2g)\n", mult_colour, mult_val);
    printf("Calculated value: %.2f Ohms\n", (digit1 * 10 + digit2) * mult_val);
}

void menu_item_3(void) {
    char buf[16];

    for(;;) {
        int choice = get_menu_choice(resistor_code_menu);
        if (choice == 0) return;
        
        if (choice == 1) colour_code_to_resistance();
        else if (choice == 2) resistance_to_colour_code();
        else { printf("Invalid option!\n"); continue; }
        
        get_string_input("\nPress Enter to continue...", buf, sizeof(buf));
    }
}

// Binary/Hex/Decimal Converter Functions
void number_converter_menu(void) {
    printf("\n>> Number Converter\n"
           "\n1. Decimal to Binary\n2. Decimal to Hex\n3. Binary to Decimal\n"
           "4. Binary to Hex\n5. Hex to Decimal\n6. Hex to Binary\n0. Back to Main Menu\n");
}

void decimal_to_binary(int decimal) {
    printf("\nDecimal %d = ", decimal);
    if (decimal == 0) { printf("0 (binary)\n"); return; }
    char binary[33]; int index = 0;
    for (int num = decimal; num > 0; num /= 2) binary[index++] = (num % 2) + '0';
    for (int i = index - 1; i >= 0; i--) printf("%c", binary[i]);
    printf(" (binary)\n");
}

void decimal_to_hex(int decimal) { printf("\nDecimal %d = 0x%X (hex)\n", decimal, decimal); }

void binary_to_decimal(const char *binary) {
    int decimal = 0;
    for (int i = 0; binary[i]; i++) {
        if (binary[i] != '0' && binary[i] != '1') { printf("\nInvalid binary input!\n"); return; }
        decimal = decimal * 2 + (binary[i] - '0');
    }
    printf("\nBinary %s = %d (decimal)\n", binary, decimal);
}

void binary_to_hex(const char *binary) {
    int decimal = 0;
    for (int i = 0; binary[i]; i++) {
        if (binary[i] != '0' && binary[i] != '1') { printf("\nInvalid binary input!\n"); return; }
        decimal = decimal * 2 + (binary[i] - '0');
    }
    printf("\nBinary %s = 0x%X (hex)\n", binary, decimal);
}

void hex_to_decimal(const char *hex) {
    printf("\nHex %s = %d (decimal)\n", hex, (int)strtol(hex, NULL, 16));
}

void hex_to_binary(const char *hex) {
    int decimal = (int)strtol(hex, NULL, 16);
    if (decimal == 0) { printf("\nHex %s = 0 (binary)\n", hex); return; }
    char binary[33]; int index = 0;
    for (int num = decimal; num > 0; num /= 2) binary[index++] = (num % 2) + '0';
    printf("\nHex %s = ", hex);
    for (int i = index - 1; i >= 0; i--) printf("%c", binary[i]);
    printf(" (binary)\n");
}

void menu_item_4(void) {
    char buf[128];
    for(;;) {
        int choice = get_menu_choice(number_converter_menu);
        if (choice == 0) return;
        
        int decimal; 
        char buffer[100];

        switch(choice) {
            case 1:
                decimal = get_int_input("\nEnter decimal number: ");
                decimal_to_binary(decimal);
                break;

            case 2:
                decimal = get_int_input("\nEnter decimal number: ");
                decimal_to_hex(decimal);
                break;

            case 3:
                get_string_input("\nEnter binary number: ", buffer, sizeof(buffer));
                binary_to_decimal(buffer);
                break;

            case 4:
                get_string_input("\nEnter binary number: ", buffer, sizeof(buffer));
                binary_to_hex(buffer);
                break;

            case 5:
                get_string_input("\nEnter hex number (without 0x): ", buffer, sizeof(buffer));
                hex_to_decimal(buffer);
                break;

            case 6:
                get_string_input("\nEnter hex number (without 0x): ", buffer, sizeof(buffer));
                hex_to_binary(buffer);
                break;

            default:
                printf("Invalid option!\n");
                continue;
        }

        get_string_input("\nPress Enter to continue...", buf, sizeof(buf));
    }
}