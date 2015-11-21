#include "protocol.h"

extern mailbox_t *rxBuffer, *txBuffer;/*yer mi*/

MEMORYPOOL_DECL(mpool, 64, NULL);//sayı kaç olmalı belli değil
struct protocol_01 _protocol_01_buffer[64];

void init_protocol()
{
	chPoolLoadArray(mpool,_protocol_01_buffer,64);
	palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));
	sdStart(&SD1, NULL);
	return;
}

protocol_01* new_protocol_01(uint32_t device_id)
{
	protocol_01 *self = (protocol_01 *)chPoolAlloc(mpool);
	self->super.protocol_id = 1;
	self->super.self_id = device_id;
	self->super.frame_start = SOH;
	self->super.frame_end = EOT;
	self->frame_size = 25;
	return self;
}


void del_protocol_01(protocol_01 *frame_to_del)
{
	chPoolFree(mpool, frame_to_del);
	return;
}

static void *listenRx(void *arg)
{
	/*Maybe triggered by an interrupt routine, a semaphore ?*/
	/*New code*/
	uint8_t i = 0;
	char buffer[64];
	protocol_01 *temp;
	while(!0)//classic infinite thread loop
	{
		while(i < 25)
		{
			sdRead(&SD1, (buffer+i), 1);//wait until read
			i++;
		}
		//control if message valid
		if(buffer[0] != SOH || buffer[24] != EOT)//if its not a valid frame
		{
			uint8_t last = i, index;
			i = 0;
			while(buffer[i] != SOH)//find a start
				i++;
			for(index = 0; index < last; index++)//copy contents to head
				buffer[index] = buffer[i++];
			i = index;
			continue;
			/*Turn back to start of loop, maybe i is less than 25*/
		}
		else//valid message
		{
			/*Maybe first check if this packet to us
			 * if it isn't then we don't need to do all this stuff*/
			uint16_t crc = 0, index;
			temp = (protocol_01 *)chPoolAlloc(mpool);
			//0 is SOH
			temp->src = 0;//do i need to do this ?
			temp->dest = 0;
			temp->digital_io = 0;
			temp->analog_io[0] = 0;
			temp->analog_io[1] = 0;
			temp->analog_io[2] = 0;
			temp->analog_io[3] = 0;
			
			temp->src &= buffer[2] << 24;
			temp->src &= buffer[3] << 16;
			temp->src &= buffer[4] << 8;
			temp->src &= buffer[5];
			temp->dest &= buffer[6] << 24;
			temp->dest &= buffer[7] << 16;
			temp->dest &= buffer[8] << 8;
			temp->dest &= buffer[9];
			temp->digital_io &= buffer[10] << 24;
			temp->digital_io &= buffer[11] << 16;
			temp->digital_io &= buffer[12] << 8;
			temp->digital_io &= buffer[13];
			temp->analog_io[0] &= buffer[14] << 8;
			temp->analog_io[0] &= buffer[15];
			temp->analog_io[1] &= buffer[16] << 8;
			temp->analog_io[1] &= buffer[17];
			temp->analog_io[2] &= buffer[18] << 8;
			temp->analog_io[2] &= buffer[19];
			temp->analog_io[3] &= buffer[20] << 8;
			temp->analog_io[3] &= buffer[21];
			crc &= buffer[22] << 8;
			crc &= buffer[23];
			//24 is EOT
			/*CONTROL CRC*/
			if(crc control)
			{
				/*push to rx buffer*/
				chMbPost(rxBuffer,(msg_t) temp, 1000);
				
				/*move next part to start of the buffer*/
				for(index = 0; index + 25 < i; index++)
					buffer[index] = buffer[25 + index];
				i = i - 25;
			}
			else/*if it fails remove prepared frame*/
			{
				chPoolFree(mpool,temp);
				
				for(index = 0; index + 1 < i; index++)/*There may some other packet start within*/
					buffer[index] = buffer[1 + index];
				i = i - 1; 
			}
		}
	}
	///*Old code*/
	//int i=1;
	//char buffer[30];
	//protocol_01 *temp;
	///*get master requestten hallice frame yakalayan kod gelecek buraya*/
	///*Alttaki kadar çok uğraş yerine alayım 60 bayt soh index(soh)+25 == eot kontrolü yapayım 
	 //* daha kolay senkron olur, daha az maliyet*/
	//while(!0)
	//{
		//if(i != 0)
		//{
			//do
			//{
				//sdRead(&SD1,buffer,1);/*read one char and save it into buffer[0] until it's a SOH*/
			//}while(buffer[0] == SOH);//catch a start byte
			//i = 0;
		//}
		//i++;
		//while(i<25)
		//{
			//sdRead(&SD1,(buffer+i),1);
			//i++;
		//}
		//if(buffer[24] == EOT)//catched a frame sync'ed
			//break;
		//else
		//{
			//int j;
			//for(i = 1; i < 25;i++)//0 is always SOH
			//{
				//if(buffer[i] == SOH)//catch a start byte
				//{
					//for(j=0 ; j < 25 - i; j++)
						//buffer[j] = buffer[i+j];//copy new possible frame to beginning of array
					//i=0;
					//break;
				//}
			//}
		//}
	//}

	///*There should be a frame begins at index 0*/
	///*CRC Check*/
	//temp = (protocol_01 *)chPoolAlloc(mpool);
	
	return NULL;/*Never reach here*/
}

void putTx(protocol_01 *packet)
{
	/*Sending part must be non-preemptive otherwise packet sending may be interrupted by scheduler*/
	/*chsyslock */
	/*chsysunlock*/
	/*Must free the packet*/
	chPoolFree(mpool,packet);
	return;
}
