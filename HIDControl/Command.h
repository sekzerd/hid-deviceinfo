#include <QObject>
#define UCHARS_TO_HEX_STRING(data,len) UCHARS_TO_HexString(data,len) // uchar*数组转换成文本
#define GENRATEBYTEPTR(Count) GenrateBytePtr(Count)                  //分配内存并初始化
static bool mhexok=true;


#define COMD_SETREPORT 0 //重置端口报文
#define COMD_GETREPORT 1 //获取端口报文
#define COMD_GETINPUT 2 //上报读数据
#define COMD_SETINPUT 3 //上报写数据


#define Remainder(len,div)(len%div) //求余数
#define DataCount(size,div) ((0) < (size%div) ?(size/div)+1:size/div) //计算数据段数 计算余数
#define isRemainder(size,div) ((0) < (size%div) ?true:false) //是否有余数

#define HEXOK &mhexok

#define  Comd_EnterUpgradeMode     0x11  //进入固件更新模式
#define  Comd_SendDataSize         0x22  //发送整个固件总长度字节数
#define  Comd_SendData             0x33  //发送需要更新的固件数据
#define  Comd_SendDataCheckSum32   0x44  //发送整个固件的32 位checksum 结果
#define  Comd_LeaveUpgradeMode     0x55  //发送固件更新成功，退出上位机操作

        //Byte1: 指令Status，
#define RT_Succeed 0x00 // 成功，可以下发下一条指令
#define RT_ReSend  0x01 // 校验失败，需要重传
#define RT_ReSet   0x02 // 操作失败，需要Reset
      //  Byte2：数据索引（低位）--》指令的数据超过58Bytes 时，需要分开多次传输时的索引递
struct usbIp{

    void build( QString vid, QString pid,QString rid)
    {
        bool mok;
        mok=true;
        _pid= pid.toUShort(&mok,16);
        _vid = vid.toUShort(&mok,16);
        _rid =rid.toUInt(&mok,16);;
    }
    ushort vid(){return _vid;}
    ushort pid(){return _pid;}
    uchar  rid() {return _rid;}

    ushort _pid =0x00;
    ushort _vid =0x00;
    ushort  _rid=0x00;
};
#define HID_OK 0 //hid成功


static QString UCHARS_TO_HexString (uchar *datas,int len)
{
    QString text="";

    for(int i = 0 ;i<len;++i)
    {
        text += QString().sprintf("%02x ",datas[i]);
    }
    return text;
}

static void numble_to_H_L (const int inputNum,uchar &h,uchar &l)
{
  ushort num =inputNum;
  QString str =  QString().sprintf("%04x",num);
  QString strH =str.left(2);
  QString strL =str.mid(2,2);
  h =strH.toInt(HEXOK,16);
  l =strL.toInt(HEXOK,16);
}

static uint Check_Sum_32(uchar * Bytes,uint size)
{
    volatile uint chk=0;
    for(uint i = 0; i < size; i++)
    {
        chk+=Bytes[i];
    }
    return chk;
}
static uchar Check_Sum(uchar * Bytes,uint size)
{
    volatile uchar chk=0;
    for(uint i = 0; i < size; i++)
    {
        chk+=Bytes[i];
    }
    return chk;
}


static uchar* GenrateBytePtr (int size)
{
    uchar* ptr = (uchar*)malloc(size);
    memset(ptr,0,size);
    return ptr;
}


