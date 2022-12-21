/*
    rgb_lcd library
    Created by: Isaac Maximiliano Martinez Govea
    15/04/2019
*/

#ifndef RGB_LCD_H
#define RGB_LCD_H

/*  Constants */
//Addresses I2C
#define LCD_ADDRESS (0x7C)
#define RGB_ADDRESS (0xC4)

//Registres de la couleur
#define REG_MODE1   0x00
#define REG_MODE2   0X01
#define REG_OUTPUT  0x08

#define REG_RED 0x04
#define REG_GREEN   0x03
#define REG_BLUE    0x02

//COMMANDES LCD
#define LCD_FUNCTIONSET 0x20
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CLEARDISPLAY    0x01

//Flags
#define LCD_2LINE   0x08
#define LCD_5X10DOTS    0x04
#define LCD_5X8DOTS    0x00
#define LCD_DISPLAYON   0x04
#define LCD_CURSOROFF   0x00
#define LCD_BLINKOFF    0x00
#define LCD_ENTRYLEFT   0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

#include "mbed.h"

class rgb_lcd : public Stream{
    private:
        I2C *i2c;
        uint8_t _displayfunction;
        uint8_t _displaycontrol;
        uint8_t _displaymode;
        
        /*
            Local comunication fonctions
        */
        void setReg(uint8_t reg, uint8_t value);    // Communication with the RGB driver
        void command(uint8_t comm); // Communication with the LCD driver
    
        void write(uint8_t value);                  // Communication with the print register of the LCD
        void i2c_send_byteS(uint8_t *data, uint8_t len);    // Communication I2C
    protected:
        /* 
            We overdrive these functions to get acces to the function printf
        */
        virtual int _putc(int c);
        virtual int _getc();
    public:
    rgb_lcd(I2C* i2c_bus);
    
    /* 
        User Fonctions
    */
    void begin();       // Initialize the LCD
    void setRGB(uint8_t r, uint8_t g, uint8_t b);   // Set backlight-color
    void setCursor(uint8_t col, uint8_t row);       // Set cursor position
    void display();     //  Display text
    void clear();       //  Clear display
    
    void test_rgb();
};

#endif