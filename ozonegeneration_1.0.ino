#include <LiquidCrystal.h>

const int redPin 	= A0;		// choose the pin for each of the LEDs
const int greenPin	= A1;
const int bluePin	= A2;
const boolean invert = true; 	// set true if common anode, false if common cathode
int color = 0;
int R, G, B;
int brightness = 255;

int sekunti=0, oldSec, Delay, buttonflag, Case=1, screenFlag, ozFlag=0, vertailuAjastin, ozAjastin = 10, ajastin, buttonLong;
boolean case1Flag=0, case2Flag=0, case3Flag=0, case4Flag=0;
LiquidCrystal lcd(12, 11, 10, 5, 4, 3);

void setup() {
	Serial.begin(9600);
	Serial.println("Setup Start");
	lcd.begin(20, 4);			//Näytön koon asetus
	TCCR1A |= _BV(COM1A0); //Toggle OC1A on compare match
	// TCCR1B |= _BV(WGM12);  //mode:CTC  OCR1A
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= _BV(WGM12);
	TCCR1B |= (1<<CS12) | (1<<CS10);  // clk/1024
	TIMSK1 |= (1<<OCIE1A);  //Output Compare A Match Interrupt Enable
	OCR1A = 15625; ///16000000/1024=15625
	sei();
	attachInterrupt(digitalPinToInterrupt(2), button, FALLING);
	Serial.println("Setup End");
}

ISR(TIMER1_COMPA_vect)
{
    sekunti++;
}
void button()
{
	//Serial.println("button");
	//timerSelect++;
	if(ozFlag == 1)
	{
		digitalWrite(7, LOW);
		ozFlag = 0;
	}
	
}
void relaySW()
{	
	ozFlag = 1;
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);
	ajastin = 0;
	ajastin = sekunti;
}
void lcdSetup()
{
	lcd.setCursor(0, 0);
	lcd.print("OZONATOR");
}
/*void hueToRGB( int hue, int brightness)
{
	unsigned int scaleHue = (hue * 6);							// segment 0 to 5 around the coloe wheel
	unsigned int segment = scaleHue / 256;						// position within the segment
	unsigned int segmentOffset = scaleHue - (segment * 256);
	
	unsigned int complement = 0;
	unsigned int prev = (brightness * ( 255 - segmentOffset)) / 256;
	unsigned int next = (brightness * segmentOffset) / 256;
	if(invert)
	{
		brightness = 255-brightness;
		complement = 255;
		prev = 255-prev;
		next = 255-next;
	}
	
	switch(segment )
	{
		case 0:		// red
		R = brightness;
		G = next;
		B = complement;
		break;
		case 1:		// yellow
		R = prev;
		G = brightness;
		B = complement;
		break;
		case 2:		// green
		R = complement;
		G = brightness;
		B = next;
		break;
		case 3:		// cyan
		R = complement;
		G = prev;
		B = brightness;
		break;
		case 4:		// blue
		R = next;
		G = complement;
		B = brightness;
		break;
		case 5:		// magena
		R = brightness;
		G = complement;
		B = prev;
		break;
	}
}*/
void loop() 
{
	
	lcdSetup();
	if(ozFlag == 1)
	{
		vertailuAjastin = sekunti;
		lcd.setCursor(0,3);
		lcd.print(ozAjastin - (vertailuAjastin - ajastin));
		if(vertailuAjastin - ajastin >= ozAjastin)		/// ozAjastin => muutettava käytön ajastus
		{
			digitalWrite(7, LOW);
			ozFlag = 0;
		}
	}
	
	Delay 		= 0;
	buttonflag 	= 0;
	buttonLong 	= 0;
	
	while(digitalRead(2) == HIGH)					
	{
		Delay=Delay+1;
		delay(1);
		buttonflag=1;
	}
	if(buttonflag == 1)
	{
		Serial.print("Delay: ");
		Serial.println(Delay);
	}
	if(Delay<500 && buttonflag==1){				
	Case+=1;
	screenFlag=1;
	if(Case>4)
		{
			Case=1;
		}
	}
	
	if(Delay>500 && buttonflag==1 && Delay<3499){	
		Serial.println("Button long");
		buttonLong = 1;
	}
	
	switch(Case){
		case 1:
		Serial.println("case1");
		case1Flag=1;
		if(case4Flag==1)
		{
			lcd.clear();
			case4Flag=0;
		}
		lcd.setCursor(0, 1);
		lcd.print("aika");
		break;
		
		case 2:
		Serial.println("case2");
		case2Flag=1;
		if(case1Flag==1)
		{
			case1Flag=0;
			lcd.clear();
		}
		lcd.setCursor(0, 1);
		lcd.print("Ajan valinta:");
		lcd.print(ozAjastin);
		if(buttonLong == 1)
		{
			ozAjastin += 10;
			if(ozAjastin >= 60)
			{
				ozAjastin = 10;
			}
			
		}
		
		break;
		
		case 3:
		Serial.println("case3");
		case3Flag=1;
		if(case2Flag==1)
		{
			case2Flag=0;
			lcd.clear();
		}
		lcd.setCursor(0, 1);
		lcd.print("Kaynnista laite");
		lcd.setCursor(0,2);
		lcd.print("painamalla nappia.");
		if(buttonLong == 1)
		{
			relaySW();		// Laitteen käynnistys
		}
		break;
		
		case 4:
		Serial.println("case4");
		case4Flag=1;
		if(case3Flag==1)
		{
			case3Flag=0;
			lcd.clear();
		}
		lcd.setCursor(0, 1);
		break;
	}
	if(sekunti != oldSec)
	{
		oldSec = sekunti;
		Serial.print("Sec:");
		Serial.println(sekunti);
	}
	
	
/*	hueToRGB( color, brightness);	// call function to convert hue to RGB
	analogWrite(redPin, R);
	analogWrite(greenPin, G);
	analogWrite(bluePin, B);
	
	color++;
	if(color > 255)
		color = 0;
		delay(10);
	*/
}
