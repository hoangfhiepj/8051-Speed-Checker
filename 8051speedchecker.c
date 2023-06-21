#include<REGX51.H>                       //8051 library
#define RS P2_0                          //LCD data pins
#define RW P2_1
#define EN P2_2
#define LCD_D4 P2_4                   
#define LCD_D5 P2_5
#define LCD_D6 P2_6
#define LCD_D7 P2_7
#define SS1 P0_0                        //output sensor 1
#define SS2 P0_1                        //output sensor 2

void delay_us(unsigned int t){
    unsigned int i;
    for(i=0; i<t; i++)
}
void delay_ms(unsigned int t){
    unsigned int x,y;
    for(x=0; x<t; x++){
        for(y=0; y<123; y++)
    }
}
void enable(void){
    EN=1;
    delay_us(3);
    EN=0;
    delay_us(2);
}
void LCD_send(unsigned char Data){
    LCD_D4 = Data & 0x01;
    LCD_D5 = (Data>>1) & 1;
    LCD_D6 = (Data>>2) & 1;
    LCD_D7 = (Data>>3) & 1;
}
void LCD_cmd(unsigned char command){
    LCD_send(command>>4);
    enable();
    LCD_send(command);
    enable();
}
void LCD_clear(){
    LCD_send(0x01);
    delay_us(10);
}
void LCD_Init(){
    LCD_send(0x00);
    delay_ms(10);
    RS=0;
    RW=0;
    enable();
    LCD_send(0x02);
    enable();
    LCD_cmd(0x28);
    LCD_cmd(0x0C);
    LCD_cmd(0x06);
    LCD_cmd(0x01);
}
void LCD_gotoxy(unsigned char x, unsigned char y){
    unsigned char address;
    if(!y)  address = (0x80 + x);
    else    address = (0xC0 + x);
    delay_us(100);
    LCD_cmd(address);
    delay_ms(100);
}
void LCD_char(unsigned char Data){
    RS=1;
    LCD_cmd(Data);
    RS=0;
}
void LCD_string(char *s){
    while(*s){
        LCD_char(*s);
        s++;
    }
}

void main(){
    while(1){
        unsigned int t = 0;         //time
        LCD_Init();
        delay_ms(100);
        if(SS1==0){
            LCD_cmd(0x80);
            LCD_string("Detected!");
            while(SS2==1){
                delay_mws(100);
                t++;
            }
        }
        if(SS2==0){
            LCD_cmd(0x80);
            LCD_string("Speed:        cm/s");
            LCD_gotoxy(7,0);
            LCD_char(((300/t)/10)+48);
            LCD_char(((300/t)%10)+48);
            LCD_char('.');
            LCD_char((((3000)/t)%10)+48);
            delay_ms(5000);
        }
        do{
            LCD_clear();
            LCD_cmd(0x80);
            LCD_string("Detecting.......");
        }while(SS1==1 && SS2==1);
    }
}