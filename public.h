#ifndef PUBLIC_H
#define PUBLIC_H
#ifdef __cplusplus

typedef unsigned char uint8;
extern "C"
{
#endif

#define    MSG_PERIOD    0xAA
#define    MSG_RSP       0xDD
#define    MSG_RSC       0XCC

/****************************
typedef enum                 //中软原定义
{
  Coor = 0x00,   //网关
  Hall,       //霍尔
  PhotoRes,   //光照
  TempAndHum, //温湿度
  Shake,      //震动
  Reed,       //干簧管
  Accele,     //加速度
  Smoke,      //烟雾
  Doppler,    //多普勒
  Motor,      //电机
  LED_PWM,    //LED调光
  Sound,      //声音
  Voltage,    //电压  保留
  Current,    //电流  保留
  Touch,      //触摸
  Ultrasound, //超声波
  RFID_1356,  //13.56M射频卡
  RFID_125K,  //125K射频卡
  Flame,      //火焰
  Particle,   //微粒
  Color,      //颜色
  Gyroscope,  //陀螺仪
  IR_Code,    //红外编解码
  Alcohol,     //酒精
  Relay,
  RFID_900M
}DeviceAddrList;
*********************************/

 typedef enum
{
  Coor         = 0x00,   //网关
  Hall         = 0x01,       //霍尔
  TempHumLight = 0x02, //温湿度//20131017
  Shake        = 0x03,      //震动
  Smoke        = 0x04,      //烟雾
  PIR          = 0x05,        //热释电红外
  Motor        = 0x06,      //电机
  Touch        = 0x07,      //触摸
  Ultrasound   = 0x08, //超声波
  LED_PWM      = 0x09,    //LED调光
  Relay        = 0x0a,      //继电器
  Current      = 0x0b,    //电流检测
  Voltage      = 0x0c,    //电压检测
  VoltageOut   = 0x0d, //电压输出
  MilliVoltMeter,        //毫伏检测20140107
}DeviceAddrList;



typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[10];     //数据域
  uint8 Verify;       //校验和
}UART_Format;         //心跳包消息收发,数据位是10的









typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Verify;       //校验和
}UART_Format_End0;    //数据域是0位的消息收发

typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[1];      //数据域
  uint8 Verify;       //校验和
}UART_Format_End1;    //数据域是1位的消息收发


typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[2];      //数据域
  uint8 Verify;       //校验和
}UART_Format_End2;    //数据域是2位的消息收发


typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[4];      //数据域
  uint8 Verify;       //校验和
}UART_Format_End4;    //数据域是4位的消息收发


typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[6];      //数据域
  uint8 Verify;       //校验和
}UART_Format_End6;    //数据域是6位的消息收发

typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[8];      //数据域
  uint8 Verify;       //校验和
}UART_Format_End8;    //数据域是8位的消息收发

typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Data[9];      //数据域
  uint8 Verify;       //校验和
}UART_Format_End9;    //数据域是8位的消息收发


typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command;      //命令
  uint8 Verify;       //校验和
}UART_Format_Control;  //数据域是0位的基本控制消息消息收发

typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command[2];      //命令
  uint8 Verify;       //校验和
}UART_Format_Control2;    //控制域是2位的消息收发

typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command[9];      //命令
  uint8 Verify;       //校验和
}UART_Format_Control9;    //控制域是9位的消息收发


typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Command[14];     //数据域

}UART_Format_SerialApp;

typedef struct
{
  uint8 Header;       //帧头
  uint8 Len;          //消息长度
  uint8 NodeSeq;      //节点编号
  uint8 NodeID;       //模块ID
  uint8 Data[14];     //数据域

}UART_Format_AF;




#ifdef __cplusplus
}
#endif

#endif
