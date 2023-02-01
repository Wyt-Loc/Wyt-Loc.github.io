#include "spi.h"
#include "delay.h"
#include "stdio.h"
#include "led.h"
#include "timer.h"
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
uint16_t SPI1_ReadWriteByte(uint16_t TxData);//SPI总线读写一个字节

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI1的初始化

SPI_InitTypeDef  SPI_InitStructure;

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
/************引脚配置*************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI_NSS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //上拉输入 浮空输入
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_AF_OD;  //复用开漏输出（多从机）   /推挽复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}




void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 

/*这里没有SPI1_SetSpeed函数哦！*/

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint16_t SPI1_ReadWriteByte(uint16_t TxData)
{		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	  
		
}


u8 recv = 0;

//保存将接收到的数据
uint16_t Recv_Data_SPI(void){
	recv = SPI1_ReadWriteByte(0);   //收到并给一个反馈  随便发一个数据
	return recv;
}


void Control_Motor(uint16_t id){
		switch(id){
			case LEFT:{
				EN1;
				EN2;
				SPI1_ReadWriteByte(LEFT);
				printf("左转了");
				LEFT1;
				LEFT2;
				break;
			}
			case RIGHT:{
				EN1;
				EN2;
				SPI1_ReadWriteByte(RIGHT);
				printf("右转了");
				RIGHT1;
				RIGHT2;
				break;
			}
			case RING:{
				EN1;
				EN2;
				SPI1_ReadWriteByte(RING);
				RING1;
				RING2;
				break;
			}
		}
}










