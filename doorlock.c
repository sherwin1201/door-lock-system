#include<reg51.h>

// Port P1 for lcd commands (datalines)

// Port P2.0 to P2.3 for 4 rows of lcd
// Keypad pins
// Row pins
sbit r1=P2^0;		// Initializing rows		
sbit r2=P2^1;		// and setting them 
sbit r3=P2^2;		// with port p2.0,
sbit r4=P2^3;		// p2.1, p2.2, p2.3

// Port P3.0 to P3.2 for 3 columns if lcd
// Column pins
sbit c1=P3^0;		// Initializing columns
sbit c2=P3^1;		// and setting them with
sbit c3=P3^2;		// port p3.0 p3.1, p2.2

// Port P3.3 and P3.4 for motor
// Motor pins
sbit motp1=P3^3;
sbit motp2=P3^4;

// Port P3.5 to P3.7 for lcd
// LCD pins
sbit rs=P3^5;		// For lcd command(0)/data(1)
sbit rw=P3^6;		// For read(1)/write(0)
sbit en=P3^7;		// Enable

void lcdcmd(unsigned char);
void lcddat(unsigned char);
void lcddisplay(unsigned char *q);
char keypad();
void check();
void delay(unsigned int);
void reset();

unsigned char pin[] = "12345";	//password
unsigned char epin[5];  //password to be entered
int unlocked = 0;



void main(){
	lcdcmd(0x0F);		// Command for blinking cursor
	lcdcmd(0x38);		// Command to recieve input
	lcdcmd(0x01);		// Clear screen
	
	while(1){
			unsigned int i=0;
		
				lcdcmd(0x80);		// First row first column of lcd
				lcddisplay("Password: ");
				delay(1000);		
				lcdcmd(0xc0);		// Take cursor after "Password"
				while(pin[i] != '\0'){
					epin[i] = keypad();
					lcdcmd(0xc0+i);
					delay(1000);
					lcddat('*');
					delay(1000);
					i++;
				}
				check();
	}
}

void reset(){
	c1=c2=c3=1;
}

void delay(unsigned int i){		// Delay function
	int a, b;
	for(a=0;a<i;a++){
		for(b=0; b<10;b++);
	}
}

void lcdcmd(unsigned char a){   // LCD command function
	P1=a;													// Sends command from port to display
	rs=0;													// Input is a command
	rw=0;													// Input written to LCD 	
	en=1;													// Enable off
	
	delay(1000);
	en=0;													// Enable on
}

void lcddat(unsigned char i){		// LCD data function
	P1=i;													// Send data from port to display
	rs=1;													// Input is data		
	rw=0;													// Input is to be written to LCD
	en=1;													// Enable off
	
	delay(1000);
	en=0;													// Enable on
}

void lcddisplay(unsigned char *q){		//LCD display fucnction
	int k;
	for(k=0; q[k] != '\0'; k++){
			lcddat(q[k]);
	}
	delay(10000);
}

char keypad(){
	int x=0;
	
	while(x==0){
		//if(r1==0){
			r1=0; r2=1; r3=1; r4=1;		// For row 1
			if(c1==0){
			lcddat('1');
			delay(100);
			x=1;
			return '1';
		}
		if(c2==0){
			lcddat('2');
			delay(100);
			x=1;
			return '2';
		}
		if(c3==0){
			lcddat('3');
			delay(100);
			x=1;
			return '3';
		}
		//}
		
		r1=1; r2=0; r3=1; r4=1;		// For row 2
		//if(r2==0){
			if(c1==0){
			lcddat('4');
			delay(100);
			x=1;
			return '4';
		}
		
		if(c2==0){
			lcddat('5');
			delay(100);
			x=1;
			return '5';
		}
		if(c3==0){
			lcddat('6');
			delay(100);
			x=1;
			return '6';
		}
		//}
		
		r1=1; r2=1; r3=0; r4=1;		// For row 3
		//if(r3 == 0){
			if(c1==0){
			lcddat('7');
			delay(100);
			x=1;
			return '7';
		}
		if(c1==0){
			lcddat('8');
			delay(100);
			x=1;
			return '8';
		}
		if(c1==0){
			lcddat('9');
			delay(100);
			x=1;
			return '9';
		}
		//}
		
		r1=1; r2=1; r3=1; r4=0;		// For row 4
		//if(r4==0){
			if(c1==0){
			lcddat('*');
			delay(100);
			x=1;
			return '*';
		}
		
		if(c2==0){
			lcddat('0');
			delay(100);
			x=1;
			return '0';
		}
		
		if(c3==0){
			lcddat('#');
			delay(100);
			x=1;
			return '#';
		}
		//}
		
	}
}

void check(){		//To check for password
	if(pin[0] == epin[0] && pin[1] == epin[1] && pin[2] == epin[2] && pin[3] == epin[3] && pin[4] == epin[4]){
		delay(1000);
		lcdcmd(0x01);		// Command to clear screen
		lcdcmd(0x81);		// Command to print on first line
		
	if(!unlocked){
		lcddisplay("DOOR UNLOCKED!");
		delay(1000);	
		
		motp1=1;		// Controlling the 
		motp2=0;		//	motors
	
		lcdcmd(0xc1);		// Command to print on second line
		lcddisplay("WELCOME!");
		delay(100000);
		motp1=1;
		motp2=0;
		lcdcmd(0x01);		// Command to clear screen
	
		unlocked=1;
	}
	else{
		motp1=1;		// Controlling the 
		motp2=1;		//	motors
	
		lcdcmd(0x81);		// Command to print on first line
		lcddisplay("DOOR LOCKED!");
		delay(100000);
		motp1=1;
		motp2=1;
		lcdcmd(0x01);		// Command to clear screen
		
		unlocked=0;
	}
		reset();
	}
	else{
		lcdcmd(0x01);		// Command to clear screen
		lcdcmd(0x80);
		lcddisplay("INCORRECT PIN!");
		delay(1000000);
		lcdcmd(0x01);		// Command to clear screen
	}
}