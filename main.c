#include "main.h"

static msg_t txMailboxBuffer[BUFFERSIZE];
static msg_t rxMailboxBuffer[BUFFERSIZE];

/*make it global.h*/
mailbox_t *rxBuffer, *txBuffer;
uint8_t cca_exp_io_mode;


int main(void) {
	halInit();
	chSysInit();
	
	init_globals();
	init_IO();
	//systickconfig'e gerek yok sanırım
	init_protocol();
	protocol_01 *telegram = new_protocol_01(0);
	protocol_01 *temp;
	msg_t addressOfTemp;
	set_module_addr(telegram);
	
	/*bi watchdog denmiş ama onun akıbeti ?*/
	int reset_counter = 0;
	int reset_comm_counter = 0;

	while(!0)
	{
		reset_comm_counter++;
		if (reset_comm_counter == RESET_LIMIT)
		{
			reset_comm_counter = 0;
			reset_counter += 1;
		}
		
		// refresh module_id
		set_module_addr(telegram);
		
		if(chMBGetUsedCountI(rxBuffer) != 0)/*There is a message inside buffer*/
		{
			chMbFetch(rxBuffer,&addressOfTemp,1000);
			temp = (protocol_01 *) addressOfTemp;
			reset_comm_counter = 0;
			
			if(temp->dest == telegram->super->self_id)/*Böyle uzatmaktansa bi tane who am i olsa ?*/
			{
				/*Message came to this device*/
				
				if( cca_exp_io_mode == CCA_EXP_IO_MODE_OUTPUT)
				{
					/*Set outputs*/
				}
				
				
				if(cca_exp_io_mode == CCA_EXP_IO_MODE_INPUT)
				{
					/*Read inputs*/
				}
			}
		}
		
	}
  
  return 0;/*never reach here*/
}

/*burda da tam olan nedir sor*/
void set_module_addr(protocol_01* telegram){
	  // set device address with physical switch in the circuit:
	  telegram->super->self_id = map_addrsw_to_logical(~GPIOB->IDR);
}

void init_IO()
{
	adc_configure();
	//bütün b portlarını (16 tane) pull up input olarak aç
	palSetGroupMode(GPIOB, PAL_GROUP_MASK(16), 0, PAL_MODE_INPUT_PULLUP);
	
	set_cca_exp_io_mode();
	
	if( cca_exp_io_mode == CCA_EXP_IO_MODE_OUTPUT)
	{
		palSetGroupMode(GPIOD, PAL_GROUP_MASK(16), 0, PAL_MODE_OUTPUT_PUSHPULL);
		palSetGroupMode(GPIOE, PAL_GROUP_MASK(16), 0, PAL_MODE_OUTPUT_PUSHPULL);
	}
	else if( cca_exp_io_mode == CCA_EXP_IO_MODE_INPUT)
	{
		palSetGroupMode(GPIOD, PAL_GROUP_MASK(16), 0, PAL_MODE_INPUT_PULLUP);
		palSetGroupMode(GPIOE, PAL_GROUP_MASK(16), 0, PAL_MODE_INPUT_PULLUP);
	}
	else
	{
		;// Never reach here
	}
	return;
}

/*Burda yapılan iş ne? B'nin 7. bitini okumak mı ? map fonksiyonu nedir ?*/
void set_cca_exp_io_mode()
{
	// if bit7 is 1: this is 1nput module
	// if bit7 is 0: this is 0utput module
	cca_exp_io_mode = (map_addrsw_to_logical(~GPIOB->IDR) >> 7);
	return;
}

void adc_configure()
{
	palSetPadMode(GPIOC, 0, PAL_MODE_INPUT_ANALOG); // this is 10th channel
	palSetPadMode(GPIOC, 1, PAL_MODE_INPUT_ANALOG); // this is 11th channel
	palSetPadMode(GPIOC, 2, PAL_MODE_INPUT_ANALOG); // this is 12th channel	
	adcStart(&ADCD1, &adcch10);/*bunlar burdayken adclerin clockları açık oluyor*/
	adcStart(&ADCD2, &adcch11);/*low power durumundan çıkıyor yani problem olur mu? ısınma güç falan*/
	adcStart(&ADCD3, &adcch12);/*convert yapmadan hemen açıp sonrasında kapanabilir de*/
	return;
}

uint16_t adc_convert(uint8_t channel)
{
	adcsample_t adc_sample;

    switch(Channel)
    {
        case 10:
			adcStartConversion(&ADCD1, &adcch10, &adc_sample, 1);
			break;
        case 11:
			adcStartConversion(&ADCD2, &adcch11, &adc_sample, 1);
			break;
        case 12:
			adcStartConversion(&ADCD3, &adcch12, &adc_sample, 1);
			break;
        default:
            adc_sample = 0;
            break;
    }
    return adc_sample;
}

void init_globals()
{
	/*initialize mailboxes*/
	/*SOR:buffersize yukarıda makro oldu sıkıntı olur mu?*/
	chMBObjectInit(txBuffer, txMailboxBuffer, BUFFERSIZE);
	chMBObjectInit(rxBuffer, rxMailboxBuffer, BUFFERSIZE);
	return;
}
