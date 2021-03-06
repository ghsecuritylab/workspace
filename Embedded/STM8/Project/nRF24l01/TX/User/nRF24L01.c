#include "nRF24L01.h"

u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10, 0x01};  
u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  

u8 sta;

void delay_us(u16 x){  // 16NOP = 1us
  u16 i;
  for(i=0;i < x;i++){
    asm("nop"); asm("nop"); asm("nop"); asm("nop");  
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); 
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); 
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); 
  }
}

void delay_ms(u16 x){
  while(x){
    x--;
    delay_us(700);
  }
}

void nRF_Init(void){
  delay_us(1000);
  CE=0;    // chip enable
  CSN=1;   // Spi disable 
  SCK=0;   // Spi clock line init high
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); 
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   
  SPI_RW_Reg(WRITE_REG + RF_CH, 0);         	
  SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); 
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);  
  SPI_RW_Reg(WRITE_REG + CONFIG, 0X0F);
  delay_ms(2);
}

u8 SPI_RW(u8 byte){       
  u8 i;
  for(i=0;i < 8;i++){
    if((byte & 0x80) == 0){       
      MOSI = 0;       
    }
    else{       
      MOSI = 1;       
    }
    
    byte = (byte << 1);                
    SCK = 1;
    
    if(MISO == 1){       
      byte |= 1;      
    }
    else{       
      byte |= 0;      
    }
    SCK = 0;
  }
  return (byte);    
}
/***************************************************/
u8 SPI_RW_Reg(u8 reg, u8 value){
  u8 status;
  CSN = 0;                   
  status = SPI_RW(reg);      
  SPI_RW(value);             
  CSN = 1;                   
  return(status);            
}
/**************************************************/
u8 SPI_Read(u8 reg){
  u8 reg_val;
  CSN = 0;                    
  SPI_RW(reg);                
  reg_val = SPI_RW(0);        
  CSN = 1;                    
  return(reg_val);            
}

u8 SPI_Read_Buf(u8 reg, u8* pBuf, u8 bytes){
  u8 status, i;
  CSN = 0;                    
  status = SPI_RW(reg);      
  for(i=0; i<bytes; i++)
  pBuf[i] = SPI_RW(0);    
  CSN = 1;                    
  return(status);             
}
/**************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
  u8 status, i;
  CSN = 0;                    
  status = SPI_RW(reg);       
  delay_us(10);
  for(i=0; i<bytes; i++)
  SPI_RW(*pBuf++);        
  CSN = 1;                    
  return(status);             
}
/**************************************************/

void SetRX_Mode(void){
  CE = 1; 
  delay_us(1000); 
}
/**************************************************/

u8 nrf_read(u8* rx_buf){
  u8 revale=0;
  sta = SPI_Read(STATUS);	
  if(RX_DR)			{
    CE = 0; 			
    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
    revale =1;			
  }
  SPI_RW_Reg(WRITE_REG+STATUS,sta);   
  return revale;
}
/**********************************************************************************************************/
void nrf_send(u8* tx_buf){
  CE=0;			                                         	
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); 
  SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 		 
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);       
  CE = 1;		                            
  delay_us(1000);     
  CE = 0;                                     
}
