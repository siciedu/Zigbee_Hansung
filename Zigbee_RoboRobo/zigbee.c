#include "zgb_hal.h"
#include "zigbee.h"

#define USE_UCR	1

unsigned char gbRcvPacket[6];
unsigned char gbRcvPacketNum;
unsigned short gwRcvData;
unsigned char gbRcvFlag;

#define MODE_ROBOTIS 0
#define MODE_ROBOROBO 1
#define MODE_UCR 2

int currentMode = -1;

int zgb_initialize(int mode, int devIndex )
{
	if (mode == MODE_ROBOTIS)
	{
		if( zgb_hal_open( devIndex, 57600 ) == 0) // Always fixed baudrate
			return 0;
	}
	else if (mode == MODE_ROBOROBO)
	{
		if( zgb_hal_open( devIndex, 9600 ) == 0) // Always fixed baudrate
			return 0;
	}
	else if (mode == MODE_UCR)
	{
		if( zgb_hal_open( devIndex, 57600 ) == 0) // Always fixed baudrate
			return 0;
	}
	else
	{
		return 0;
	}

	gbRcvFlag = 0;
	gwRcvData = 0;
	gbRcvPacketNum = 0;
	currentMode = mode;
	return 1;
}

void zgb_terminate()
{
	zgb_hal_close();
}

#if USE_UCR
int zgb_tx_data(unsigned char* buf, int len)
{
	if (zgb_hal_tx(buf, len) != len)
		return 0;
	return 1;
}
#else
int zgb_tx_data(int data)
{
	if (currentMode == MODE_ROBOTIS)
	{
		unsigned char SndPacket[6];
		unsigned short word = (unsigned short)data;
		unsigned char lowbyte = (unsigned char)(word & 0xff);
		unsigned char highbyte = (unsigned char)((word >> 8) & 0xff);

		SndPacket[0] = 0xff;
		SndPacket[1] = 0x55;
		SndPacket[2] = lowbyte;
		SndPacket[3] = ~lowbyte;
		SndPacket[4] = highbyte;
		SndPacket[5] = ~highbyte;

		if( zgb_hal_tx( SndPacket, 6 ) != 6 )
			return 0;
	}
	else if (currentMode == MODE_ROBOROBO)
	{
		unsigned char SndPacket[4];
		unsigned short word = (unsigned short)data;
		unsigned char lowbyte = (unsigned char)(word & 0xff);
		unsigned char highbyte = (unsigned char)((word >> 8) & 0xff);

		SndPacket[0] = 0xAA;
		SndPacket[1] = highbyte;
		SndPacket[2] = lowbyte;
		SndPacket[3] = (lowbyte ^ highbyte);

		if( zgb_hal_tx( SndPacket, 4 ) != 4 )
			return 0;
	}
	else
		return 0;

	return 1;
}
#endif

int zgb_rx_check()
{
	int RcvNum;
	unsigned char checksum;
	int i, j;

	if(gbRcvFlag == 1)
		return 1;

	if (currentMode == MODE_ROBOTIS)
	{
		// Fill packet buffer
		if(gbRcvPacketNum < 6)
		{
			RcvNum = zgb_hal_rx( &gbRcvPacket[gbRcvPacketNum], (6 - gbRcvPacketNum) );
			if( RcvNum != -1 )
				gbRcvPacketNum += RcvNum;
		}

		// Find header
		if(gbRcvPacketNum >= 2)
		{
			for( i=0; i<gbRcvPacketNum; i++ )
			{
				if(gbRcvPacket[i] == 0xff)
				{
					if(i <= (gbRcvPacketNum - 2))
					{
						if(gbRcvPacket[i+1] == 0x55)
							break;
					}
				}
			}

			if(i > 0)
			{
				if(i == gbRcvPacketNum)
				{
					// Can not find header
					if(gbRcvPacket[i - 1] == 0xff)
						i--;
				}

				// Remove data before header
				for( j=i; j<gbRcvPacketNum; j++)
				{
					gbRcvPacket[j - i] = gbRcvPacket[j];
				}
				gbRcvPacketNum -= i;
			}
		}

		// Verify packet
		if(gbRcvPacketNum == 6)
		{
			if(gbRcvPacket[0] == 0xff && gbRcvPacket[1] == 0x55)
			{
				checksum = ~gbRcvPacket[3];
				if(gbRcvPacket[2] == checksum)
				{
					checksum = ~gbRcvPacket[5];
					if(gbRcvPacket[4] == checksum)
					{
						gwRcvData = (unsigned short)((gbRcvPacket[4] << 8) & 0xff00);
						gwRcvData += gbRcvPacket[2];
						gbRcvFlag = 1;
					}
				}
			}

			gbRcvPacket[0] = 0x00;
			gbRcvPacketNum = 0;
		}
	}
	else if (currentMode == MODE_ROBOROBO)
	{
		// Fill packet buffer
		if(gbRcvPacketNum < 4)
		{
			RcvNum = zgb_hal_rx( &gbRcvPacket[gbRcvPacketNum], (4 - gbRcvPacketNum) );
			if( RcvNum != -1 )
				gbRcvPacketNum += RcvNum;
		}

		// Find header
		if(gbRcvPacketNum >= 1)
		{
			for( i=0; i<gbRcvPacketNum; i++ )
			{
				if(gbRcvPacket[i] == 0xAA)
				{
					break;
				}
			}

			if(i > 0)
			{
				if(i == gbRcvPacketNum)
				{
					// Can not find header
					if(gbRcvPacket[i - 1] == 0xAA)
						i--;
				}

				// Remove data before header
				for( j=i; j<gbRcvPacketNum; j++)
				{
					gbRcvPacket[j - i] = gbRcvPacket[j];
				}
				gbRcvPacketNum -= i;
			}
		}

		// Verify packet
		if(gbRcvPacketNum == 4)
		{
			if(gbRcvPacket[0] == 0xAA)
			{
				checksum = gbRcvPacket[3];
				if((gbRcvPacket[1] ^ gbRcvPacket[2]) == checksum)
				{
					gwRcvData = (unsigned short)((gbRcvPacket[1] << 8) & 0xff00);
					gwRcvData += gbRcvPacket[2];
					gbRcvFlag = 1;
				}
				/*else
					return -1;*/
			}
			/*else
				return -2;*/

			gbRcvPacket[0] = 0x00;
			gbRcvPacketNum = 0;
		}
		/*else
			return -3;*/
	}
	return gbRcvFlag;
}

int zgb_rx_buf(unsigned char *buf, int len)
{
	return zgb_hal_rx(buf, len);
}

int zgb_rx_data()
{
	gbRcvFlag = 0;
	return (int)gwRcvData;
}
