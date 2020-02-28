#include "cvc_can.h"
#include "charger.h"

//these most likely work - got from bridger
queue_msg_t charger_enable =
{
		.Tx_header = {270, 0, STD, RTR, 0x8 },													// (0)
		.data._8 = {0xF8, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
//starts charger at 300v
queue_msg_t charger_start =
{
		.Tx_header = {618, 0, STD, RTR, 0x8 },													// (0)
		.data._8 = {0x80, CHARGER_ID, 0x00, 0x0B, 0xB8, 0x00, 0x64, 0x00}
};

//starts charger at 12v??? NEEDS TO BE TESTED
queue_msg_t charger_precharge =
{
		.Tx_header = {618, 0, STD, RTR, 0x8 },													// (0)
		.data._8 = {0x80, CHARGER_ID, 0x00, 0x0B, 0x0B, 0x00, 0x64, 0x00}
};



//these are from spreadsheet, 50/50 chance
queue_msg_t charger_disable =
{
		.Tx_header = {270, 0, STD, RTR, 0x2 },													// (0)
		.data._8 = {0x44, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

queue_msg_t charger_reset =
{
		.Tx_header = {270, 0, STD, RTR, 0x2},													// (0)
		.data._8 = {0x45, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

void start_charger(void){
	CAN_Send(charger_enable);
	CAN_Send(charger_start);
}

void disable_charger(void){
	CAN_Send(charger_disable);
}

void reset_charger(void){
	CAN_Send(charger_reset);
}
