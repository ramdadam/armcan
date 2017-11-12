/** *****************************************************************************
 * @file    CANdriver.h
 * @author  Dr. Klaus Schaefer Hochschule Darmstadt schaefer@eit.h-da.de
 * @brief   Driver module for CAN 1
 ******************************************************************************/

 #ifndef DRIVERS_CANDRIVER_H_
 #define DRIVERS_CANDRIVER_H_

 #include "freertos_wrapper.h"
 enum PACKET_TYPE {
     RECEIVE,
     TRANSMIT
 };
 //! basic CAN packet type
 class CAN_packet
 {
 public:
     CAN_packet( uint16_t _id=0, uint8_t _dlc=0, uint64_t data=0, uint8_t _is_remote=0)
     : id(_id),
       dlc(_dlc),
       is_remote(_is_remote),
       data_l( data)
     {}
     uint16_t id; 	//!< identifier
     uint8_t dlc; 	//!< data length code
     uint8_t is_remote; 	//!< true for remote request
     union
     {
         uint8_t data_b[8];  //!< data as seen in bytes
         uint32_t data_w[2]; //!< data as two 32-bit words
         uint64_t data_l;    //!< data as 64-bit number
     };
     int32_t count = 0;
     int32_t cycleTime = 0;
     uint64_t lastTick = 0;
     CAN_packet(const CAN_packet& other);
     void bump(uint64_t tick) {
         cycleTime = tick - lastTick;
         lastTick = tick;
         count += 1;
     }
     PACKET_TYPE packetType = RECEIVE;
 };
 
 namespace CAN_driver_ISR // need a namespace to declare friend functions
 {
     extern "C" void CAN1_RX0_IRQHandler(void);
     extern "C" void CAN1_TX_IRQHandler(void);
 }
 
 //! CAN driver class including API, hardware driver and ISR
 class CAN_driver_t
 {
     friend void CAN_driver_ISR::CAN1_RX0_IRQHandler(void);
     friend void CAN_driver_ISR::CAN1_TX_IRQHandler(void);
 public:
     CAN_driver_t(); //!< hardware and software initialization of CAN driver
 
     /// API call to send CAN packets
     /// @param packet CAN packet to be sent
     /// @param wait optional wait time in RTOS ticks
     /// @return false if timeout
     bool send(const CAN_packet &packet, uint32_t wait = INFINITE_WAIT);
 
     /// API call to receive CAN packets
     /// @param packet CAN packet to be received
     /// @param wait optional wait time in RTOS ticks
     /// @return false if timeout
     inline bool receive(CAN_packet &packet, uint32_t wait = INFINITE_WAIT)
     {
         return RX_queue.receive(packet, wait);
     }
 
     void setCANOperatingMode(bool sleep);
 private:
     //! CAN interface initialization
     void hardware_initialization(void);
     //! internal function to copy CAN packet into the hardware
     bool send_packet(const CAN_packet &msg);
     Queue<CAN_packet> RX_queue; //!< receive queue
     Queue<CAN_packet> TX_queue; //!< transmit queue
 };
 
 extern CAN_driver_t CAN_driver; //!< singleton CAN driver object
 
 #endif /* DRIVERS_CANDRIVER_H_ */
 