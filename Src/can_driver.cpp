/** *****************************************************************************
 * @file    CANdriver.cpp
 * @author  Dr. Klaus Schaefer Hochschule Darmstadt schaefer@eit.h-da.de
 * @brief   Driver module for CAN 1
 ******************************************************************************/

#include "freertos_wrapper.h"
#include "can_driver.h"
#include "stm32f7xx_it.h"
#include "stm32f7xx_hal_def.h"
#include "stm32f7xx_hal_can.h"

//! CAN driver for CAN1
CAN_driver_t::CAN_driver_t() :
		RX_queue(20), TX_queue(20) {
	//hardware_initialization();
}

bool CAN_driver_t::send(const CAN_packet &packet, uint32_t wait) {
#if CAN_LOOPBACK
	RX_queue.send( packet);
#endif
	// Temporarily disable Transmit mailbox empty Interrupt
	CAN1->IER &= ~CAN_IT_TME; // to avoid race condition

	if (TX_queue.messages_waiting() == 0) {
		if (send_packet(packet) == true) // hardware FIFO available ?
				{
			/* Enable Transmit mailbox empty Interrupt */
			CAN1->IER |= CAN_IT_TME;
			return true;
		}
	}

	/* Enable Transmit mailbox empty Interrupt */
	CAN1->IER |= CAN_IT_TME;
	bool ret = TX_queue.send(packet, wait);
	return ret;
}

bool CAN_driver_t::send_packet(const CAN_packet &msg) {
	uint8_t transmitmailbox;

	/* Select one empty transmit mailbox */
	if ((CAN1->TSR & CAN_TSR_TME0) == CAN_TSR_TME0)
		transmitmailbox = 0;
	else if ((CAN1->TSR & CAN_TSR_TME1) == CAN_TSR_TME1)
		transmitmailbox = 1;
	else if ((CAN1->TSR & CAN_TSR_TME2) == CAN_TSR_TME2)
		transmitmailbox = 2;
	else
		return false; // give up, no mailbox empty

	/* Set up the Id */
	CAN1->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
	CAN1->sTxMailBox[transmitmailbox].TIR |= msg.id << 21;
	if (msg.is_remote)
		CAN1->sTxMailBox[transmitmailbox].TIR |= 0x02;

	/* Set up the DLC */
	CAN1->sTxMailBox[transmitmailbox].TDTR &= (uint32_t) 0xFFFFFFF0;
	CAN1->sTxMailBox[transmitmailbox].TDTR |= msg.dlc;

	/* Set up the data field */
	CAN1->sTxMailBox[transmitmailbox].TDLR = msg.data_w[0];
	CAN1->sTxMailBox[transmitmailbox].TDHR = msg.data_w[1];

	/* Request transmission */
	CAN1->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;
	return true;
}

void CAN_driver_t::hardware_initialization(void) {

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

	CAN1->IER |= (CAN_IT_FMP0 | CAN_IT_FMP1);//enable CAN1 FIFO 0 and 1 RX interrupt
}

namespace CAN_driver_ISR {
//! CAN receive interrupt service routine
extern "C" void CAN1_RX0_IRQHandler(void) {
	//  portENTER_CRITICAL();
	ENTER_ISR;

	CAN_packet msg;

	msg.id = 0x07FF & (uint16_t)(CAN1->sFIFOMailBox[CAN_FIFO0].RIR >> 21);
	msg.dlc = (uint8_t) 0x0F & CAN1->sFIFOMailBox[CAN_FIFO0].RDTR;
	msg.is_remote = ((CAN1->sFIFOMailBox[CAN_FIFO0].RIR & 0x02) == 0) ? 0 : 1;
	msg.data_w[0] = CAN1->sFIFOMailBox[CAN_FIFO0].RDLR;
	msg.data_w[1] = CAN1->sFIFOMailBox[CAN_FIFO0].RDHR;
	CAN1->RF0R |= CAN_RF0R_RFOM0; // release FIFO 0

	bool result = CAN_driver.RX_queue.send_from_ISR(msg);
	CAN_driver.send_packet(msg);
	//		ASSERT( result == true); // trap for RX queue overrun

	LEAVE_ISR;
}
//! CAN transmit interrupt service routine
extern "C" void CAN1_TX_IRQHandler(void) {
	//  portENTER_CRITICAL();
	ENTER_ISR;

	CAN_packet msg;

	if (CAN_driver.TX_queue.receive_from_ISR(msg)) {
		bool result = CAN_driver.send_packet(msg);
		ASSERT(result);
	} else
		CAN1->IER &= ~CAN_IT_TME; // interrupt off, no more work to do
	LEAVE_ISR;
}
}

COMMON CAN_driver_t CAN_driver;

CAN_packet::CAN_packet(const CAN_packet& other) {
	id = other.id;
	dlc = other.dlc;
	is_remote = other.is_remote;
	data_l = other.data_l;
}

void CAN_driver_t::setCANOperatingMode(bool silent) {
	CAN_HandleTypeDef hcan;
	wipe(hcan);
	hcan.Instance = CAN1;
	hcan.Init.TTCM = DISABLE;
	hcan.Init.ABOM = DISABLE;
	hcan.Init.AWUM = DISABLE;
	hcan.Init.NART = DISABLE;
	hcan.Init.RFLM = DISABLE;
	hcan.Init.TXFP = DISABLE;
	if (silent) {
		hcan.Init.Mode = CAN_MODE_SILENT;
	} else {
		hcan.Init.Mode = CAN_MODE_NORMAL;
	}
	hcan.Init.Prescaler = 5000000 / CAN_BITRATE_HZ;
	hcan.Init.SJW = CAN_SJW_1TQ;
	hcan.Init.BS1 = CAN_BS1_7TQ;
	hcan.Init.BS2 = CAN_BS2_2TQ;

	if (HAL_CAN_Init(&hcan) != HAL_OK) {
		ASSERT(0);
	}
}

