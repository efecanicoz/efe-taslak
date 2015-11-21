 /*
 *
 * SOH
 * Protocol ID (HEX, UTF-8 like scheme (if frame[x] is part of Protocol_ID and frame[x] > 127 then frame[x+1] belongs to Protocol_ID  ))
 * == BEGIN PROTOCOL DATA ==
 * == END PROTOCOL DATA ==
 * EOT
 *
 *
 * Protocol ID: 0x01
 * -----------------
 * - Big Endian :
 * 		(0xA0B0C0D0 ->
 * 			Byte[0] = 0xA0,
 * 			Byte[1] = 0xB0,
 * 			Byte[2] = 0xC0,
 * 			Byte[3] = 0xD0
 * - 32 Digital Input
 * - 32 Digital Output
 * - 4 Analog Input (16 bits per channel)
 * - 4 Analog Output (16 bits per channel)
 * ------------------
 * SOURCE BYTE-0
 * SOURCE BYTE-1
 * SOURCE BYTE-2
 * SOURCE BYTE-3
 * DESTINATION BYTE-0
 * DESTINATION BYTE-1
 * DESTINATION BYTE-2
 * DESTINATION BYTE-3
 * #ifdef MASTER_PACKAGE
 * SET DIGITAL OUTPUTS 31..24
 * SET DIGITAL OUTPUTS 23..16
 * SET DIGITAL OUTPUTS 15..8
 * SET DIGITAL OUTPUTS 7..0
 * SET ANALOG OUTPUT 0 High Byte
 * SET ANALOG OUTPUT 0 Low Byte
 * SET ANALOG OUTPUT 1 High Byte
 * SET ANALOG OUTPUT 1 Low Byte
 * SET ANALOG OUTPUT 2 High Byte
 * SET ANALOG OUTPUT 2 Low Byte
 * SET ANALOG OUTPUT 3 High Byte
 * SET ANALOG OUTPUT 3 Low Byte
 * #endif // MASTER_PACKAGE
 * #ifdef SLAVE_PACKAGE
 * SEND DIGITAL INPUTS 31..24
 * SEND DIGITAL INPUTS 23..16
 * SEND DIGITAL INPUTS 15..8
 * SEND DIGITAL INPUTS 7..0
 * SEND ANALOG INPUT 0 High Byte
 * SEND ANALOG INPUT 0 Low Byte
 * SEND ANALOG INPUT 1 High Byte
 * SEND ANALOG INPUT 1 Low Byte
 * SEND ANALOG INPUT 2 High Byte
 * SEND ANALOG INPUT 2 Low Byte
 * SEND ANALOG INPUT 3 High Byte
 * SEND ANALOG INPUT 3 Low Byte
 * #endif // SLAVE_PACKAGE
 * CRC-16 OF PROTOCOL DATA SECTION High Byte
 * CRC-16 OF PROTOCOL DATA SECTION  Low Byte
 *
 *
 */
/*Macros*/
#define SOH 0x01
#define EOT 0x04

/*Structs*/
typedef struct _protocol_basic
{
	char frame_start;
	char protocol_id;
	char frame_end;

	uint32_t self_id;
} _protocol_basic;

struct protocol_01
{
	struct _protocol_basic super;
	uint32_t src;
	uint32_t dest;
	uint32_t digital_io;
	uint16_t analog_io[4];
	
	/*crc (*calc_crc)(char *message, int lenght);
	CircularBuffer * (*mk_resp)(cca_comm_prot_protocol_id_01 *);
	int (*get_req)(cca_comm_prot_protocol_id_01 *, CircularBuffer *);
	void (*del)(cca_comm_prot_protocol_id_01 *);*/

	int frame_size;
} protocol_01;

/*Templates*/

/*Functions*/
void init_protocol();
protocol_01* new_protocol_01(uint32_t);
void del_protocol_01(protocol_01 *);
static void *listenRx(void *);
