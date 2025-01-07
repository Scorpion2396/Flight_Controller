#include "SSD1306.h"
#include "I2C_Interface.h"
#include "TM4C123GH6PM.h"
#include "Common_Macros.h"
#include "Font.h"

/**********************************************************************************/
const uint8_t font_width = 7;

/*******************************************************************************/
uint8_t ssd1306_current_col  = 0;
uint8_t ssd1306_current_page = 0;


// The SSD1306 display buffer
uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

I2C_h i2c_2_h;

/*********************** Initialize the SSD1306 display*************************/
void ssd1306_init(void) 
{
  I2C_Init(&i2c_2_h, I2C_2, 400);

  ssd1306_command(SSD1306_DISPLAY_OFF);              // 0xAE = Set Display OFF
 
  ssd1306_command(SSD1306_SET_DIS_CLK_DIV);          // Set display clock divide ratio/oscillator frequency
  ssd1306_command(0x80);

  ssd1306_command(SSD1306_SET_MUX_RATIO);            // 0xA8 - 64MUX for 128 x 64 version
  ssd1306_command(0x3F);
											
  ssd1306_command(SSD1306_MEMORY_ADDR_MODE);         // 0x20 = Set Memory Addressing Mode
  ssd1306_command(0x00);                                                                                                                                            // 0x01 - Vertical Addressing Mode
                                                                                                                                            // 0x02 - Page Addressing Mode (RESET)
  ssd1306_command(SSD1306_SET_COLUMN_ADDR);          // 0x21 = Set Column Address, 0 - 127
  ssd1306_command(START_COLUMN_ADDR); 	
  ssd1306_command(END_COLUMN_ADDR); 

  ssd1306_command(SSD1306_SET_PAGE_ADDR);            // 0x22 = Set Page Address, 0 - 7
  ssd1306_command(START_PAGE_ADDR); 	
  ssd1306_command(END_PAGE_ADDR); 
  
  ssd1306_command(SSD1306_SET_START_LINE);            // 0x40
  
  ssd1306_command(SSD1306_DISPLAY_OFFSET);            // 0xD3
  ssd1306_command(0x00);   
  
  ssd1306_command(SSD1306_SEG_REMAP_OP);              //  remap 0xA1 ( Set segment re-map)
  
  ssd1306_command(SSD1306_COM_SCAN_DIR_OP);           //  remap 0xC8  (Set COM output scan direction)

  ssd1306_command(SSD1306_COM_PIN_CONF);              // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
  ssd1306_command(0x12);                              //       0x12 - for 128 x 64 version  ,        0x02 - for 128 x 32 version
 
  ssd1306_command(SSD1306_SET_CONTRAST);              // 0x81, 
  ssd1306_command(0x7F);                              // 0x7F - reset value (max 0xFF)
  
  ssd1306_command(SSD1306_DIS_ENT_DISP_ON);           // 0xA4 (Entire display on)
  
  ssd1306_command(SSD1306_DIS_NORMAL);                // 0xA6 ( Set normal display )
  
  ssd1306_command(SSD1306_SET_OSC_FREQ);              // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
  ssd1306_command(0x80);  

  ssd1306_command(SSD1306_SET_PRECHARGE);             // 0xD9, higher value less blinking
  ssd1306_command(0xc2);                              // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
											  
  ssd1306_command(SSD1306_VCOM_DESELECT);             // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
  ssd1306_command(0x20); 

  ssd1306_command(SSD1306_SET_CHAR_REG);              // 0x8D, Enable charge pump during display on
  ssd1306_command(0x14); 
 
  ssd1306_command(SSD1306_DISPLAY_ON);                // 0xAF = Set Display ON
  
  ssd1306_clear();
  ssd1306_refresh();   
}

/****************** Send a command byte to the SSD1306 display *****************/
void ssd1306_command(uint8_t command) 
{
    // Send the control and command bytes to the display over I2C
    I2C_Write_Memory(i2c_2_h, SSD1306_ADDRESS, SSD1306_COMMAND, command);
}


/*************** Send a data byte to the SSD1306 display ***********************/
void ssd1306_data(uint8_t data) 
{	
	// Send the control and data bytes to the display over I2C
	I2C_Write_Memory(i2c_2_h, SSD1306_ADDRESS, SSD1306_DATA, data);
}

/***************** Set the cursor (x = column , y = page) **********************/
void ssd1306_setcursor(uint8_t page, uint8_t column) 
{
    if (column >= SSD1306_WIDTH || page >= SSD1306_PAGES) 
    {
        return; // Invalid coordinates
    }

    ssd1306_current_col = column;
    ssd1306_current_page = page;

    ssd1306_command(SSD1306_SET_COLUMN_ADDR);
    ssd1306_command(column);
    ssd1306_command(SSD1306_WIDTH - 1);
    ssd1306_command(SSD1306_SET_PAGE_ADDR);
    ssd1306_command(page);
    ssd1306_command(SSD1306_PAGES - 1);
}

/************************************ Print string *****************************/
void ssd1306_Print_String(uint8_t* str)
{
    uint16_t str_cnt  = 0;
    uint8_t  font_col = 0;
	
    for(str_cnt = 0 ; str[str_cnt] != '\0' ; str_cnt++)
    {
        if((str[str_cnt] >= 32) && (str[str_cnt] <= 127))
	{
            for(font_col = 0 ; font_col < font_width ; font_col++)
            {
                ssd1306_data(font_5x8[str[str_cnt] - 32][font_col]);
                                
                if(ssd1306_current_col < (SSD1306_WIDTH - font_width))
                {
                    ssd1306_current_col += font_width;
                }
                else
                {
                    ssd1306_current_col += font_width;
                    ssd1306_current_col %= SSD1306_WIDTH ;
                                   
                    if(ssd1306_current_page < (SSD1306_PAGES - 1))
                    {
                        ssd1306_current_page++;
                    }
                    else
                    {
                        ssd1306_current_page = 0;
                    }
                 }
             }	
	}	
    }
}

/******************** Clear the SSD1306 display buffer *************************/
void ssd1306_clear(void) 
{
    for (uint16_t i = 0; i < sizeof(ssd1306_buffer); i++) 
    {
        ssd1306_buffer[i] = 0x00;
    }
}


/******************** Clear the SSD1306 row display buffer **********************/
void ssd1306_clear_row(uint8_t row_num)
{
    ssd1306_setcursor(row_num, 0);

    // Send the display buffer
    for (uint16_t i = 0; i < SSD1306_WIDTH; i++)
    {
        ssd1306_data(0);
    }
}

/***** Refresh the display with the contents of the display buffer *************/
void ssd1306_refresh() 
{
    // Set the column address range
    ssd1306_command(SSD1306_SET_COLUMN_ADDR);
    ssd1306_command(0);
    ssd1306_command(SSD1306_WIDTH - 1);

    // Set the page address range
    ssd1306_command(SSD1306_SET_PAGE_ADDR);
    ssd1306_command(0);
    ssd1306_command((SSD1306_HEIGHT / 8) - 1);

    // Send the display buffer

   ssd1306_buffer_flush(ssd1306_buffer, SSD1306_BUFFER_SIZE);

}

/**************** Send a data buffer to the SSD1306 display ********************/
void ssd1306_buffer_flush(uint8_t *data, uint32_t buffer_size) 
{	
    I2C_Write_Memory_Multiple(i2c_2_h, SSD1306_ADDRESS , 0x40 , data , buffer_size);
}



/************* Set a pixel in the SSD1306 display buffer ***********************/
/*
* The function takes three arguments: x, y, and value. 
* x and y specify the coordinates of the pixel, 
* and value specifies whether the pixel should be turned on (color = 1) or off (color = 0).
*/
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color) 
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
        return;
    }

    if (color) 
	{
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } 
	else 
	{
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

// Draw a line on the SSD1306 display buffer
void ssd1306_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value) 
{
    // Bresenham's line algorithm
    int16_t dx = DIFF(x1, x0);
    int16_t dy = DIFF(y1, y0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx - dy;

    // Set the pixels along the line
    while (1) 
	{
        // Set the current pixel
        ssd1306_draw_pixel(x0, y0, value);

        // Check if the end point is reached
        if (x0 == x1 && y0 == y1) 
		{
            break;
        }

        // Update the error and move to the next pixel
        int16_t e2 = 2 * err;
        if (e2 > -dy) 
		{
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) 
		{
            err += dx;
            y0 += sy;
        }
    }
}


/**********************************************************************************/

// Set the text size (1 or 2)
void ssd1306_set_text_size(uint8_t size) 
{
    if (size > 2) size = 2;  // Limit size to 2

    // Set the text size register
    ssd1306_command(SSD1306_SET_TEXT_SIZE);
    ssd1306_command(size == 2 ? SSD1306_TEXT_SIZE_2X : SSD1306_TEXT_SIZE_1X);
}



/**********************************************************************************/
void ssd1306_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) 
{
    uint8_t x_end = x + width - 1;
    uint8_t y_end = y + height - 1;
    
    for (uint8_t row = y; row <= y_end; row++) 
	{
            for (uint8_t col = x; col <= x_end; col++) 
            {
                ssd1306_draw_pixel(col, row, color);
            }
    }
    
    ssd1306_refresh();
}

/**********************************************************************************/
void ssd1306_DrawRect(int x, int y, int width, int height) 
{
    // Set the draw window to the rectangle boundaries
    ssd1306_SetDrawWindow(x, y, x + width - 1, y + height - 1);

    // Draw the rectangle pixel by pixel
    for (int i = 0; i < width; i++) {
        ssd1306_draw_pixel(i, 0, SSD1306_COLOR_WHITE);
        ssd1306_draw_pixel(i, height - 1, SSD1306_COLOR_WHITE);
    }
    for (int i = 0; i < height; i++) {
        ssd1306_draw_pixel(0, i, SSD1306_COLOR_WHITE);
        ssd1306_draw_pixel(width - 1, i, SSD1306_COLOR_WHITE);
    }

    // Reset the draw window to the full screen
    ssd1306_SetDrawWindow(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1);
}

/****************************************************************************/
void ssd1306_SetDrawWindow(int x0, int y0, int x1, int y1) {
    // Set the lower and higher column address of the display RAM
    ssd1306_command(SSD1306_SET_COLUMN_ADDR);
    ssd1306_command(x0);
    ssd1306_command(x1);

    // Set the lower and higher page address of the display RAM
    ssd1306_command(SSD1306_SET_PAGE_ADDR);
    ssd1306_command(y0 / 8);
    ssd1306_command(y1 / 8);
}

/***************************************************************************/
void ssd1306_set_display_mode(uint8_t mode) 
{
    // Set the display mode based on the specified value
    switch (mode) {
        case SSD1306_DISPLAY_HORIZONTAL:
            ssd1306_command(SSD1306_SETDISPLAYMODE | SSD1306_HORIZONTAL_MODE);
            break;
        case SSD1306_DISPLAY_VERTICAL:
            ssd1306_command(SSD1306_SETDISPLAYMODE | SSD1306_VERTICAL_MODE);
            break;
        case SSD1306_DISPLAY_PAGE:
            ssd1306_command(SSD1306_SETDISPLAYMODE | SSD1306_PAGE_MODE);
            break;
        default:
            // Invalid mode specified, do nothing
            break;
    }
}

/********************************************************************************/
void ssd1306_DrawCircle(int16_t x0, int16_t y0, uint8_t r) 
{
  int16_t x = r, y = 0;
  int16_t err = 0;

  while (x >= y) {
    ssd1306_draw_pixel(x0 + x, y0 + y, 1);
    ssd1306_draw_pixel(x0 + y, y0 + x, 1);
    ssd1306_draw_pixel(x0 - y, y0 + x, 1);
    ssd1306_draw_pixel(x0 - x, y0 + y, 1);
    ssd1306_draw_pixel(x0 - x, y0 - y, 1);
    ssd1306_draw_pixel(x0 - y, y0 - x, 1);
    ssd1306_draw_pixel(x0 + y, y0 - x, 1);
    ssd1306_draw_pixel(x0 + x, y0 - y, 1);

    y++;
    err += 1 + 2*y;
    if (2*(err - x) + 1 > 0) {
      x--;
      err += 1 - 2*x;
    }
  }
}

/*****************************************************************************/
void ssd1306_clear_refresh() 
{
    // Set the column address range
    ssd1306_command(SSD1306_SET_COLUMN_ADDR);
    ssd1306_command(0);
    ssd1306_command(SSD1306_WIDTH - 1);

    // Set the page address range
    ssd1306_command(SSD1306_SET_PAGE_ADDR);
    ssd1306_command(0);
    ssd1306_command((SSD1306_HEIGHT / 8) - 1);

    // Send the display buffer
    for (uint16_t i = 0; i < SSD1306_BUFFER_SIZE; i++) 
    {
        ssd1306_data(0);
    }
}


/******************************************************************************/

void ssd1306_shiva_logo(void)
{
    for (uint16_t i = 0; i < sizeof(ssd1306_buffer); i++)
    {
        ssd1306_buffer[i] = shiva_logo[i];
    }
    ssd1306_refresh();
}

/******************************************************************************/

void ssd1306_snake_logo(void)
{
    for (uint16_t i = 0; i < sizeof(ssd1306_buffer); i++)
    {
        ssd1306_buffer[i] = snake[i];
    }
    ssd1306_refresh();
}
