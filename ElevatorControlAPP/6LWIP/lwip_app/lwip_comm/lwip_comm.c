#include "lwip_comm.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h"
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h"
#include "lwip/timers.h"
#include "malloc.h"
#include "delay.h"
#include <stdio.h>
#include <time.h>
#include "tool.h"

__lwip_dev lwipdev;						//lwip控制结构体
struct netif lwip_netif;				//定义一个全局的网络接口

extern u32 memp_get_memorysize ( void );	//在memp.c里面定义
extern u8_t* memp_memory;				//在memp.c里面定义.
extern u8_t* ram_heap;					//在mem.c里面定义.

/////////////////////////////////////////////////////////////////////////////////
//lwip两个任务定义(内核任务和DHCP任务)

//lwip DHCP任务
//设置任务优先级
#define LWIP_DHCP_TASK_PRIO       		7
#define LWIP_DHCP_STK_SIZE  		    256
TaskHandle_t LWIP_DHCP_TaskHandler;

//任务函数
void lwip_dhcp_task ( void* pdata );

//用于以太网中断调用
void lwip_pkt_handle ( void )
{
	ethernetif_input ( &lwip_netif );
}

//lwip中mem和memp的内存申请
//返回值:0,成功;
//    其他,失败
u8 lwip_comm_mem_malloc ( void )
{
	u32 mempsize;
	u32 ramheapsize;
	mempsize=memp_get_memorysize();			//得到memp_memory数组大小
	memp_memory=mymalloc ( SRAMIN,mempsize );	//为memp_memory申请内存
	ramheapsize=LWIP_MEM_ALIGN_SIZE ( MEM_SIZE )+2*LWIP_MEM_ALIGN_SIZE ( 4*3 )+MEM_ALIGNMENT; //得到ram heap大小
	ram_heap=mymalloc ( SRAMIN,ramheapsize );	//为ram_heap申请内存
	if ( !memp_memory||!ram_heap ) //有申请失败的
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;
}
//lwip中mem和memp内存释放
void lwip_comm_mem_free ( void )
{
	myfree ( SRAMIN,memp_memory );
	myfree ( SRAMIN,ram_heap );
}
//lwip 默认IP设置
//lwipx:lwip控制结构体指针
void lwip_comm_default_ip_set ( __lwip_dev* lwipx )
{
//    //02:00:00:52:48:50
//	uint32_t sn0 = *(__I uint32_t *)(0x1FFF7A10+0x08); //动态MAC

	//默认远端IP为:192.168.1.100
	lwipx->remoteip[0]=192;
	lwipx->remoteip[1]=168;
	lwipx->remoteip[2]=110;
	lwipx->remoteip[3]=78;

	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
//	lwipx->mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
//	lwipx->mac[1]=0;
//	lwipx->mac[2]=0;
//	lwipx->mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
//	lwipx->mac[4]=(sn0>>8)&0XFFF;
//	lwipx->mac[5]=sn0&0XFF;

	calcMac ( lwipx->mac );

	//默认本地IP为:192.168.1.30
	lwipx->ip[0]=192;
	lwipx->ip[1]=168;
	lwipx->ip[2]=110;
	lwipx->ip[3]=98;
	//默认子网掩码:255.255.255.0
	lwipx->netmask[0]=255;
	lwipx->netmask[1]=255;
	lwipx->netmask[2]=255;
	lwipx->netmask[3]=0;
	//默认网关:192.168.1.1
	lwipx->gateway[0]=192;
	lwipx->gateway[1]=168;
	lwipx->gateway[2]=110;
	lwipx->gateway[3]=1;
	lwipx->dhcpstatus=0;//没有DHCP
}

//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,LAN8720初始化失败
//      3,网卡添加失败.
u8 lwip_comm_init ( void )
{
	u8 retry=0;
	struct netif* Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	struct ip_addr ipaddr;  			//ip地址
	struct ip_addr netmask; 			//子网掩码
	struct ip_addr gw;      			//默认网关

	if ( ETH_Mem_Malloc() )
	{
		return 1;    //内存申请失败
	}
	printf ( "ETH_Mem_Malloc\r\n" );

	if ( lwip_comm_mem_malloc() )
	{
		return 2;    //内存申请失败
	}
	printf ( "lwip_comm_mem_malloc\r\n" );

	lwip_comm_default_ip_set ( &lwipdev );	//设置默认IP等信息
	printf ( "lwip_comm_default_ip_set\r\n" );

	while ( LAN8720_Init() )		     //初始化LAN8720,如果失败的话就重试5次
	{
		printf ( "LAN8720_Init\r\n" );
		retry++;
		if ( retry>5 )
		{
			retry=0; //LAN8720初始化失败
			printf ( "网卡初始化失败\n" );
			return 3;
		}
	}
	tcpip_init ( NULL,NULL );				//初始化tcp ip内核,该函数里面会创建tcpip_thread内核任务
	printf ( "tcpip_init\r\n" );

#if LWIP_DHCP		//使用动态IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//使用静态IP
	IP4_ADDR ( &ipaddr,lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3] );
	IP4_ADDR ( &netmask,lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3] );
	IP4_ADDR ( &gw,lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3] );
	printf ( "网卡的MAC地址为:................%02x.%02x.%02x.%02x.%02x.%02x\r\n",lwipdev.mac[0],lwipdev.mac[1],lwipdev.mac[2],lwipdev.mac[3],lwipdev.mac[4],lwipdev.mac[5] );
	printf ( "静态IP地址........................%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3] );
	printf ( "子网掩码..........................%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3] );
	printf ( "默认网关..........................%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3] );
#endif
	Netif_Init_Flag=netif_add ( &lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&tcpip_input ); //向网卡列表中添加一个网口
	if ( Netif_Init_Flag==NULL )
	{
		return 4;    //网卡添加失败
	}
	else//网口添加成功后,设置netif为默认值,并且打开netif网口
	{
		netif_set_default ( &lwip_netif ); //设置netif为默认网口
		netif_set_up ( &lwip_netif );		//打开netif网口
	}
	return 0;//操作OK.
}
//如果使能了DHCP
#if LWIP_DHCP
//创建DHCP任务
void lwip_comm_dhcp_creat ( void )
{
	xTaskCreate ( ( TaskFunction_t ) lwip_dhcp_task,
	              ( const char*  ) "DHCP_TASK",
	              ( uint16_t     ) LWIP_DHCP_STK_SIZE,
	              ( void*        ) NULL,
	              ( UBaseType_t  ) LWIP_DHCP_TASK_PRIO,
	              ( TaskHandle_t* ) &LWIP_DHCP_TaskHandler ); //创建DHCP任务
}
//删除DHCP任务
void lwip_comm_dhcp_delete ( void )
{
	dhcp_stop ( &lwip_netif ); 		//关闭DHCP
	vTaskDelete ( LWIP_DHCP_TaskHandler );	//删除DHCP任务
}
//DHCP处理任务
void lwip_dhcp_task ( void* pdata )
{
	u32 ip=0,netmask=0,gw=0;
	dhcp_start ( &lwip_netif ); //开启DHCP
	lwipdev.dhcpstatus=0;	//正在DHCP
	printf ( "正在查找DHCP服务器,请稍等...........\r\n" );
	while ( 1 )
	{
		printf ( "正在获取地址...\r\n" );
		ip=lwip_netif.ip_addr.addr;		//读取新IP地址
		netmask=lwip_netif.netmask.addr;//读取子网掩码
		gw=lwip_netif.gw.addr;			//读取默认网关
		if ( ip!=0 )   					//当正确读取到IP地址的时候
		{
			lwipdev.dhcpstatus=2;	//DHCP成功
			printf ( "网卡的MAC地址为:................%02x.%02x.%02x.%02x.%02x.%02x\r\n",lwipdev.mac[0],lwipdev.mac[1],lwipdev.mac[2],lwipdev.mac[3],lwipdev.mac[4],lwipdev.mac[5] );
			//解析出通过DHCP获取到的IP地址
			lwipdev.ip[3]= ( uint8_t ) ( ip>>24 );
			lwipdev.ip[2]= ( uint8_t ) ( ip>>16 );
			lwipdev.ip[1]= ( uint8_t ) ( ip>>8 );
			lwipdev.ip[0]= ( uint8_t ) ( ip );
			printf ( "通过DHCP获取到IP地址..............%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3] );
			//解析通过DHCP获取到的子网掩码地址
			lwipdev.netmask[3]= ( uint8_t ) ( netmask>>24 );
			lwipdev.netmask[2]= ( uint8_t ) ( netmask>>16 );
			lwipdev.netmask[1]= ( uint8_t ) ( netmask>>8 );
			lwipdev.netmask[0]= ( uint8_t ) ( netmask );
			printf ( "通过DHCP获取到子网掩码............%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3] );
			//解析出通过DHCP获取到的默认网关
			lwipdev.gateway[3]= ( uint8_t ) ( gw>>24 );
			lwipdev.gateway[2]= ( uint8_t ) ( gw>>16 );
			lwipdev.gateway[1]= ( uint8_t ) ( gw>>8 );
			lwipdev.gateway[0]= ( uint8_t ) ( gw );
			printf ( "通过DHCP获取到的默认网关..........%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3] );
			break;
		}
		else if ( lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES ) //通过DHCP服务获取IP地址失败,且超过最大尝试次数
		{
			lwipdev.dhcpstatus=0XFF;//DHCP失败.
			//使用静态IP地址
			IP4_ADDR ( & ( lwip_netif.ip_addr ),lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3] );
			IP4_ADDR ( & ( lwip_netif.netmask ),lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3] );
			IP4_ADDR ( & ( lwip_netif.gw ),lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3] );
			printf ( "DHCP服务超时,使用静态IP地址!\r\n" );
			printf ( "网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n",lwipdev.mac[0],lwipdev.mac[1],lwipdev.mac[2],lwipdev.mac[3],lwipdev.mac[4],lwipdev.mac[5] );
			printf ( "静态IP地址........................%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3] );
			printf ( "子网掩码..........................%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3] );
			printf ( "默认网关..........................%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3] );
			break;
		}
		delay_ms ( 250 ); //延时250ms
	}
	lwip_comm_dhcp_delete();//删除DHCP任务
}
#endif
