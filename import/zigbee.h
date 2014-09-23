#ifndef _ZIGBEE_HEADER
#define _ZIGBEE_HEADER


#ifdef __cplusplus
extern "C" 
{
	#endif
	///////////// device control methods ////////////////////////
	int __stdcall zgb_initialize(int mode, int devIndex);
	void __stdcall zgb_terminate();

	////////// communication methods ///////////////////////
	int __stdcall zgb_tx_data(unsigned char* buf, int len);
	int __stdcall zgb_rx_check();
	int __stdcall zgb_rx_data();
	int __stdcall zgb_rx_buf(unsigned char *buf, int len);

	////////// define RC-100 button key value ////////////////
	#define RC100_BTN_U		(1)
	#define RC100_BTN_D		(2)
	#define RC100_BTN_L		(4)
	#define RC100_BTN_R		(8)
	#define RC100_BTN_1		(16)
	#define RC100_BTN_2		(32)
	#define RC100_BTN_3		(64)
	#define RC100_BTN_4		(128)
	#define RC100_BTN_5		(256)
	#define RC100_BTN_6		(512)

	#ifdef __cplusplus
}
#endif

#endif