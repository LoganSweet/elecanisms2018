// From https://web.archive.org/web/20161223060411/http://www.engscope.com/pic24-tutorial/10-2-i2c-basic-functions/

// initiates I2C1 module to baud rate BRG
void i2c_init(int BRG){
   int temp;

   // I2CBRG = 194 for 10Mhz OSCI with PPL with 100kHz I2C clock
   I2C1BRG = BRG;
   I2C1CONbits.I2CEN = 0;	// Disable I2C Mode
   I2C1CONbits.DISSLW = 1;	// Disable slew rate control
   IFS1bits.MI2C1IF = 0;	 // Clear Interrupt
   I2C1CONbits.I2CEN = 1;	// Enable I2C Mode
   temp = I2CRCV;	 // read buffer to clear buffer full
   reset_i2c_bus();	 // set bus to idle
}


//function iniates a start condition on bus
void i2c_start(void){
   int x = 0;
   I2C1CONbits.ACKDT = 0;	//Reset any previous Ack
   DelayuSec(10);
   I2C1CONbits.SEN = 1;	//Initiate Start condition
   Nop();

   //the hardware will automatically clear Start Bit
   //wait for automatic clear before proceding
   while (I2C1CONbits.SEN){
      DelayuSec(1);
      x++;
      if (x > 20)
      break;
   }
   DelayuSec(2);
}



//Resets the I2C bus to Idle
void reset_i2c_bus(void)
{
   int x = 0;

   I2C1CONbits.PEN = 1;     //initiate stop bit
   while (I2C1CONbits.PEN) {     //wait for hardware clear of stop bit
      DelayuSec(1);
      x ++;
      if (x > 20) break;
   }

   I2C1CONbits.RCEN = 0;
   IFS1bits.MI2C1IF = 0; // Clear Interrupt
   I2C1STATbits.IWCOL = 0;
   I2C1STATbits.BCL = 0;
   DelayuSec(10);
}



char send_i2c_byte(int data){       //basic I2C byte send
   int i;

   while (I2C1STATbits.TBF) { }
   IFS1bits.MI2C1IF = 0; // Clear Interrupt
   I2CTRN = data; // load the outgoing data byte


   for (i=0; i<500; i++){           // wait for transmission
      if (!I2C1STATbits.TRSTAT) break;
      DelayuSec(1);
    }

    if (i == 500) {
        return(1);
    }

   if (I2C1STATbits.ACKSTAT == 1){          // Check for NO_ACK from slave, abort if not found
      reset_i2c_bus();
      return(1);
   }

   DelayuSec(2);
   return(0);
}


//function reads data, returns the read data, no ack
char i2c_read(void){
   int i = 0;
   char data = 0;
   I2C1CONbits.RCEN = 1;            //set I2C module to receive

   while (!I2C1STATbits.RBF) {           //if no response, break
      i ++;
      if (i > 2000) break;
   }


   data = I2CRCV;       //get data from I2CRCV register
   return data;
}


//function reads data, returns the read data, with ack
char i2c_read_ack(void){	//does not reset bus!!!
   int i = 0;
   char data = 0;
   I2C1CONbits.RCEN = 1;            //set I2C module to receive

   while (!I2C1STATbits.RBF) {       //if no response, break
      i++;
      if (i > 2000) break;
   }

   data = I2CRCV;               //get data from I2CRCV register
   I2C1CONbits.ACKEN = 1;       //set ACK to high
   DelayuSec(10);               //wait before exiting
   return data;
}
