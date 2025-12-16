// ELEC2645 Unit 2 Project Template
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

static void main_menu(void);
static void print_main_menu(void);
static int  get_user_input(void);
static void select_menu_item(int input);
static int  is_integer(const char *s);

int main(void)
{
    // Runs forever , until you exit the program
    for(;;) {
        main_menu();
    }
    // not reached 
    return 0;
}

static void main_menu(void)
{
    print_main_menu();
    {
        int input = get_user_input();
        select_menu_item(input);
    }
}

static int get_user_input(void)
{
    enum { MENU_ITEMS = 5 };   // 1->4 = Calculations, 5 = Exit 
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) {
            // Input fails
            puts("\nInput error. Exiting.");
            exit(1);
        }
        // removes the newline at the end
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter a valid integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}

static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            menu_item_1();
            break;
        case 2:
            menu_item_2();
            break;
        case 3:
            menu_item_3();
            break;
        case 4:
            menu_item_4();
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n----------- Main menu -----------\n");
    printf("\n"
           "1.Ohm's Law Calculator\t\t\n"
           "2.RC Filter Analyser\t\t\n"
           "3.Resistor Colour Code\t\t\n"
           "4.Binary,Hex,Decimal Converter\n"
           "5.Exit\n"
           "\n");
    printf("---------------------------------------------\n");
}

// Checks whether the string is an integer
static int is_integer(const char *s)
{
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-') s++;
    // Must be atleast 1 digit
    if (!isdigit((unsigned char)*s)) return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}