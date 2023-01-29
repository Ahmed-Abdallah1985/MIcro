#include <MKL25Z4.h>
                                //difination pin 7segment
#define Segment_a (0)
#define Segment_b (1)
#define Segment_c (2)
#define Segment_d (3)
#define Segment_e (4)
#define Segment_f (5)
#define Segment_g (6)
#define Segment_dot (7)
#define increment_sw (0)
#define decrement_sw (1)
#define MASK(x)  (1UL << (x))
volatile unsigned int count;

                                                // delay(ms) -- Spin wait delay (in ms)
                                                 // Note:  uses low power timer (LPTMR)
void delay(unsigned int length_ms)
{
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;        // Make sure clock is enabled
    LPTMR0->CSR = 0;                           // Reset LPTMR settings
    LPTMR0->CMR = length_ms;                   // Set compare value (in ms)

                                               // Use 1kHz LPO with no prescaler
    LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;

    // Start the timer and wait for it to reach the compare value
    LPTMR0->CSR = LPTMR_CSR_TEN_MASK;
    while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK))
        ;

    LPTMR0->CSR = 0;                         // Turn off timer
}

void SevenSegment(uint8_t count,uint8_t dp, uint8_t dec_hex)
{
      switch (count)
      {
         case 0:
        	 PTC->PDOR = 0x3F; 		//0 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 1:
        	 PTC->PDOR = 0x06;		//1 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 2:
         	 PTC->PDOR = 0x5B;		//2 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 3:
        	 PTC->PDOR = 0x4F;		//3 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 4:
        	 PTC->PDOR = 0x66;		//4 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 5:
        	 PTC->PDOR = 0x6D;		//5 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 6:
        	 PTC->PDOR = 0x7D;		//6 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 7:
        	 PTC->PDOR = 0x07;		//7 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

         case 8:
        	 PTC->PDOR = 0x7F;		//8 in for h,g,f,e,d,c,b,a Cathode configuration
         break;

        
      }
   }
void PORTD_IRQHandler(void) { 
                                                          // clear pending interrupt from PORTD in NVIC
NVIC_ClearPendingIRQ(PORTD_IRQn);
                                                         // if the interrupt was caused by the Increment switch, increment count
if ((PORTD->ISFR & MASK(increment_sw))) {
if(count<8){
count++;}
else{count=0;}
}
                                                         // if the interrupt was caused by the Decrement switch, decrement count
else if ((PORTD->ISFR & MASK(decrement_sw))){
if(count>0){
count--;}
else{count=8;}
}
	
                                                                          // clear status flags in PORTD to prevent recurrent interrupts
PORTD->ISFR = 0xffffffff;
}
int main(void)
{
	   
	   int dec_hex=10;                                                  //change to 10 for decimal

			                                                  // Enable Clock to ports C&D
			SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK);
			                                                   // Make GPIO
			PORTC->PCR[Segment_a] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_a] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_b] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_b] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_c] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_c] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_d] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_d] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_e] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_e] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_f] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_f] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_g] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_g] |= PORT_PCR_MUX(1);
			PORTC->PCR[Segment_dot] &= ~PORT_PCR_MUX_MASK;
			PORTC->PCR[Segment_dot] |= PORT_PCR_MUX(1);
			PORTD->PCR[increment_sw] = ~PORT_PCR_MUX_MASK;
			PORTD->PCR[increment_sw] = PORT_PCR_MUX(1);
			PORTD->PCR[decrement_sw] = ~PORT_PCR_MUX_MASK;
			PORTD->PCR[decrement_sw] = PORT_PCR_MUX(1);
			                                                               // set LEDs bits to outputs
			PTC->PDDR |= MASK(Segment_a) | MASK(Segment_b)| MASK(Segment_c)| MASK(Segment_d)| MASK(Segment_e)| MASK(Segment_f)| MASK(Segment_g)| MASK(Segment_dot);
			PTD->PDDR = 0x00;
			                                                            // Configure Interrupts on PORTD& enable pullup resistors
			PORTD->PCR[increment_sw] |= PORT_PCR_IRQC(9)| PORT_PCR_PS_MASK | PORT_PCR_PE_MASK; //Interrupt on rising edge
			PORTD->PCR[decrement_sw] |= PORT_PCR_IRQC(9)| PORT_PCR_PS_MASK | PORT_PCR_PE_MASK; //Interrupt on rising edge
			                                                            // Enable Interrupts
			NVIC_SetPriority(PORTD_IRQn, 128);
			NVIC_ClearPendingIRQ(PORTD_IRQn);
			NVIC_EnableIRQ(PORTD_IRQn);
			while (1)
			{
				SevenSegment(count,0, dec_hex);
				delay(2000);
				__enable_irq(); // enable interrupts globally
				
			}
}
