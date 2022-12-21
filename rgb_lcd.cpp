#include "rgb_lcd.h"
/*
    rgb_lcd library
    Created by: Isaac Maximiliano Martinez Govea
    15/04/2019
*/


/*
        ////////////////////////////////////////////////////////////////////
                         Local comunication fonctions
        ////////////////////////////////////////////////////////////////////
*/
/*
     Communication with the LCD driver
*/
void rgb_lcd::command(uint8_t comm){
    uint8_t data[2] = {0x80, comm};     //  0x80 : Configuration register
    i2c_send_byteS(data,2);
}

/*
     Communication with the RGB driver
*/
void rgb_lcd::setReg(uint8_t reg, uint8_t value){
    uint8_t data[2] = {reg,value};
    i2c->write(RGB_ADDRESS, (char*)data, 2);
}
/*
     Communication with the print register of the LCD
*/
void rgb_lcd::write(uint8_t value){
    uint8_t data[2] = {0x40, value};    // 0x40 : writing register
    i2c_send_byteS(data, 2);
}
/*
     Communication I2C
*/
void rgb_lcd::i2c_send_byteS(uint8_t *data, uint8_t len){
    i2c->write(LCD_ADDRESS, (char*) data, len);
}

////////////////////////////////////////////////////////////////////////////////

/*
        ////////////////////////////////////////////////////////////////////
                                User Fonctions
        ////////////////////////////////////////////////////////////////////
*/
/*
    Constructeur
*/
rgb_lcd::rgb_lcd(I2C* i2c_bus){
    i2c = i2c_bus;
    _displayfunction = 0x00;
    _displaycontrol = 0x00;
    _displaymode = 0x00;
}
/*
    Initialize the LCD
*/
void rgb_lcd::begin(){
    
    _displayfunction |= LCD_2LINE;  // We use a 2 line display
    _displayfunction |= LCD_5X8DOTS;   //  We set the size of characters 
    
    wait_us(50000);    //  We wait for the power rise
    
    //We send many times the command to guarantee the proper fonctioning of the LCD
    command(LCD_FUNCTIONSET | _displayfunction);
    wait_us(4500);
    command(LCD_FUNCTIONSET | _displayfunction);
    wait_us(150);
    command(LCD_FUNCTIONSET | _displayfunction);
    command(LCD_FUNCTIONSET | _displayfunction);
    
    // We turn on the display without cursor
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();
    clear();
    
    // We set up the text direction ( From left to right )
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode );
    //We set up the backlight
    setReg( REG_MODE1, 0);  // We turn on the backlight
    setReg( REG_OUTPUT, 0xFF);  //  We allow the control of the RGB
    setReg( REG_MODE2,   0x20);  //  Blinky Mode
    setRGB(255,255,255);    //We set the white color
}
/*
    Set backlight-color
*/
void rgb_lcd::setRGB(uint8_t r, uint8_t g, uint8_t b){
    setReg(REG_RED, r);
    setReg(REG_GREEN, g);
    setReg(REG_BLUE, b);
}
/*
    Set cursor position
*/
void rgb_lcd::setCursor(uint8_t col, uint8_t row){
    col = (row == 0 ? col|0x80 : col|0xC0);
    uint8_t data[2] = {0x80 , col};
    i2c_send_byteS(data, 2);
}
/*
    Display text
*/
void rgb_lcd::display(){
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*
    Clear display
*/
void rgb_lcd::clear(){
    command(LCD_CLEARDISPLAY);  // Clear and set cursor to position zero
    wait_us(2000);
}
////////////////////////////////////////////////////////////////////////////////

/*
        ////////////////////////////////////////////////////////////////////
                     FONCTIONS TO ENABLE PRINTF FROM STREAM
        ////////////////////////////////////////////////////////////////////
*/

int rgb_lcd::_putc(int c){
    write((uint8_t)c);
    return 0;
}
int rgb_lcd::_getc(){
    return -1;
}