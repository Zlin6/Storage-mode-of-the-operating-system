#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define memorysize 65536 
#define blocksize 256
#define blocknum 256
//设置最大段长为1024，即2^10，最大段数为64，即2^6 
#define seg_max 64 
#define seg_maxsize 1024

//默认内存大小为65536B （0~65535），即64KB，物理块(页框)大小=页面大小为256，所以物理块长度（个数）为256  
enum STATUS {FREE,OCCUPIED  //内存块状态 
}block[blocknum]; //内存块数组  
//无寄存器，页表不占用内存地址空间，相当于快表 
typedef struct {
	int frame;//物理页框号 
}pagetableentry;
pagetableentry* pagetable; //页表 或段页式的页表 

//无寄存器，段表不占用内存地址空间，相当于快表 
typedef struct {
    int base;  // 段的基址
    int limit; // 段长
} segmenttableentry;
segmenttableentry* segtable;   // 段表

//段页表 
typedef struct{
	pagetableentry* pagetable2;//相当于页表寄存器 ，占用内存地址空间 
	int  pagetableL;//页表长 
	int  pageblock;//页表始值 
	
}segpagetableentry;
segpagetableentry* segpagetable; //段页式中的段表 



int allocatememory(int prosize){
	//初始化页表和内存块
	int i;
	for(i=0;i<blocknum;i++){
		block[i]=FREE;
	} 
	int numpages;
	//查看页表/页面长度 
	if(prosize%blocksize==0){
		numpages=prosize/blocksize;
	}
	else{
		numpages=prosize/blocksize+1;
	}
	//分配 
	pagetable = (pagetableentry*)malloc(sizeof(pagetableentry)*numpages);
	
	for(i=0;i<numpages;i++){
		
		pagetable[i].frame=rand()%256;
		while(block[pagetable[i].frame]==OCCUPIED){
			pagetable[i].frame=rand()%256;
		} 
		block[pagetable[i].frame]=OCCUPIED;
	}
	printf("\t页面大小为256，页表长度为%d,分配成功！\n\n",numpages);
	return numpages;
}
void showpagetable(int pp){
	printf("\n\t页表展示\n");
	printf("\t _______________\n");
	printf("\t|Page\t|Frame\t|\n");
	int i;
	for(i=0;i<pp;i++){
		printf("\t|%d\t|%d\t|\n",i,pagetable[i].frame);
		if(i==pp-1){
			
			printf("\t|_______|_______|\n");
		}
	}
}

void pagetrasla(int A,int L,int prosize){

	int p=A/blocksize;// 页号 
	int d=A%blocksize;//页内地址 
	if(p>=L)  printf("\t页号大于等于页表长度，越界中断\n"); 
	else if(A>=prosize) printf("\t逻辑地址大于所给进程长度，越界中断\n");
	else{
		int kh=pagetable[p].frame;//块号 
		int Wads = blocksize*kh+ d;
		printf("\t物理地址=内存块大小x页帧(物理块号)+偏移量\n"); 
		printf("\t页号为：%d\n\t页内地址为：%d\n\t物理块号为：%d\n\t物理地址为：%d\n",p,d,kh,Wads); 
	} 
} 
void showmemory(){
	printf("\n\t内存状态\n");
	printf("\t ________________\n");
	printf("\t|Frame\t|Status\t |\n");
	int i;
	for(i=0;i<blocknum;i++){
		char ff[]="FREE";
		char ss[]="OCCUPIED";
		
		if(block[i]==0) {
			printf("\t|%d\t|%s\t |\n",i,ff);
		}
		else if(block[i]==1){
			printf("\t|%d\t|%s|\n",i,ss);
		}
		
		if(i==blocknum-1){
			
			printf("\t|_______|________|\n");
		}
	}
	
	
}
void weishitu();
void pagemenu() {
	int id1;
	int prosize;
	int aa,pp;
	int log;
	printf("\t-------------------------------分页式地址换算---------------------------------\n"); 
	printf("\t请输入进程大小(内存上限65536)，系统将为你分配内存：\n");
	printf("\t");scanf("%d",&prosize);
	pp=allocatememory(prosize);
	
	while(1){
		printf("\t------------------------------------------------------------------------------\n"); 
		printf("\n\t请输入选项：\n");
		printf("\t1查看页表\n");
		printf("\t2输入逻辑地址，查找物理地址\n");
		printf("\t3查看内存块占用情况\n");
		printf("\t4返回\n");
		printf("\t------------------------------------------------------------------------------\n\t"); 
		scanf("%d",&aa); 
		switch(aa){
			case 1:
				showpagetable(pp);
				break;
			case 2:
				printf("\t请输入逻辑地址：\n\t");
				scanf("%d",&log); 
				pagetrasla(log,pp,prosize);
				break;
			case 3:
				showmemory();
				weishitu();
				break;
			case 4:
			    free(pagetable);
			    
			    system("cls");
				return; 
			default:
				printf("\t输入错误，请重试\n");
				break;
		}
		
	}
}





int  allocatememory2(int prosize){
	//10+16=26
//	segtable = (segmenttableentry*)malloc(sizeof(segmenttableentry)*se) 
	//每个进程最多64个段，每个段最大长度为1024
	int dc=0;
	int jdq=0;//计段器 
	int aa[64]={0};//计段长 
	//求段长和段表长度 
	while(dc<prosize){
		if(jdq>=64) {
			printf("\t进程过大，内存分配失败\n"); return -1;
		}
		aa[jdq]=rand()%1024+1; //段长决定分配的段表长度 
		dc=dc+aa[jdq];
		if(dc>=prosize) {
			dc=prosize;
			aa[jdq]=aa[jdq]-(dc-prosize);
			break;
		}
		jdq++; 
		
	}
	segtable = (segmenttableentry*)malloc(sizeof(segmenttableentry)*(jdq+1));
	int i;
	//创建段表  
	for(i=0;i<=jdq;i++ ){
		segtable[i].limit=aa[i];
		if(i==0){
			segtable[i].base=0+rand()%1024;
		}
		else{
			//基址看自己想怎么调 
			segtable[i].base=segtable[i-1].base+segtable[i-1].limit+rand()%256;
		}
		
		if(segtable[i].base>memorysize){
			printf("\t内存过小，内存分配失败\n");
			return 0; 
		}
	} 
	jdq=jdq+1;
	printf("\t段表长度为%d,分配成功!\n\n",jdq);
	return  jdq;
	
} 
void showsegtable(int pp){
	printf("\n\t段表展示:\n");
	printf("\t ________________________\n");
	printf("\t|段号\t|段长\t|基址\t|\n"); 
	int i;

	for(i=0;i<pp;i++)
	{
		printf("\t|%d\t|%d\t|%d\t|\n",i,segtable[i].limit,segtable[i].base);
		if(i==pp-1){
			printf("\t|_______|_______|_______|\n");
		}
	}
}
void segtrasla(int ls,int la,int pp,int prosize ){
	if(ls>=pp) {
		printf("\t段号大于等于段表长度，越界中断\n"); 
		return;
	}
	else if (la>=segtable[ls].limit){
		printf("\t段内地址大于等于段长，越界中断\n"); 
		return; 
	}
	int wads=segtable[ls].base+la;
	printf("\t物理地址=段表基址+段内地址\n");
	printf("\t物理地址为:%d\n",wads); 
	
}
//void showsegmemory(){
//	
//}
//void segmentpage(int sn,int pn,int pd){
//	
//}
void segmentmenu(){
	int prosize;
	int aa,pp;
	int ls,la;
	printf("\t-------------------------------分段式地址换算---------------------------------\n"); 
	printf("\t请输入进程大小(内存上限65536)，系统将为你分配内存：\n\t");
	scanf("%d",&prosize);
	pp=allocatememory2(prosize);
	
	while(1){
		printf("\t------------------------------------------------------------------------------\n");
		printf("\n\t请输入选项：\n");
		printf("\t1查看段表\n");
		printf("\t2输入逻辑地址，查找物理地址\n");
		printf("\t3返回\n");
		printf("\t------------------------------------------------------------------------------\n\t"); 
		scanf("%d",&aa); 
		switch(aa){
			case 1:
				showsegtable(pp);
				break;
			case 2:
				printf("\t请输入逻辑地址(段号和段内地址)：\n");
				printf("\t段号：");
				scanf("%d",&ls);
				printf("\t段内地址："); 
				scanf("%d",&la);
				segtrasla(ls,la,pp,prosize);
				break;
			case 3:
				free(segtable);
			    
			    system("cls");
				return; 
			
			default:
				printf("\t输入错误，请重试\n");
				break;
		}
		
	}
	
}
//有关判断的递归 
//void judge(enum STATUS block[],int length){
//	if(block[length]==FREE&&)
//	else if(a==0&&)
//}
int allocatememory3(int prosize){
	int dc=0;
	int jdq=0;//段表最大长度为64 
	int aa[64]={0};
	//初始化
	int i;
	for(i=0;i<blocknum;i++){
		block[i]=FREE;
	} 
	while(dc<prosize){
		if(jdq>=64){
			printf("\t进程过大，内存分配失败\n");
			return 0; 
		} 
		aa[jdq]=rand()%1024+1;  //每个段长最大为1024 
		dc=dc+aa[jdq];
		if(dc>=prosize){
			dc=prosize;
			aa[jdq]=aa[jdq]-(dc-prosize);
			break; 
		} 
		jdq++;
	}
	jdq=jdq+1;//计段器从0开始，所以段表长度+1
	
	segpagetable =(segpagetableentry*)malloc(sizeof(segpagetableentry)*jdq);
	
	//创建段表 
	for(i=0;i<jdq;i++){
		if(aa[i]%blocksize==0){
			segpagetable[i].pagetableL=aa[i]/blocksize;//页面大小为256， 
		} 
		else{
			segpagetable[i].pagetableL=aa[i]/blocksize+1;//页表长度 
		}
		
		segpagetable[i].pageblock=rand()%blocknum;//页表始值 
		segpagetable[i].pagetable2 =(pagetableentry*)malloc(sizeof(pagetableentry)*segpagetable[i].pagetableL); 
		
		
		int ui=segpagetable[i].pagetableL;//页表长 
		int uu=0;
		int sss=0;
		while(1){
			
			uu=segpagetable[i].pageblock;//始值 
			if(ui==1){
				if(block[uu]==FREE){
					block[uu]=OCCUPIED;
					
					break;
				}
				else{
					segpagetable[i].pageblock=rand()%blocknum;//内存条件不满足 
				}
			}
			else if(ui==2)
			{
				if(block[uu]==FREE&&block[uu+1]==FREE){
					block[uu]=OCCUPIED;
					block[uu+1]=OCCUPIED;
					
					break;
				}
				else{
					segpagetable[i].pageblock=rand()%blocknum;//内存条件不满足 
				}
			}
			else if(ui==3){
				if(block[uu]==FREE&&block[uu+1]==FREE&&block[uu+2]==FREE){
					block[uu]=OCCUPIED;
					block[uu+1]=OCCUPIED;
					block[uu+2]=OCCUPIED;
					
					break;
				}
				else{
					segpagetable[i].pageblock=rand()%blocknum;//内存条件不满足 
				}
			}
			else if(ui==4){
				if(block[uu]==FREE&&block[uu+1]==FREE&&block[uu+2]==FREE&&block[uu+3]==FREE){
					block[uu]=OCCUPIED;
					block[uu+1]=OCCUPIED;
					block[uu+2]=OCCUPIED;
					block[uu+3]=OCCUPIED;
					
					break;
				}
				else{
					segpagetable[i].pageblock=rand()%blocknum;//内存条件不满足 
				}
			}
			sss++;
			if(sss==1000) {
				printf("\t内存所剩不足以存储进程，分配失败！\n");
				return;
			} 
			
		}
		
		//创建页表
		
		int j;
		int qw;
		for(j=0;j<segpagetable[i].pagetableL;j++){
			segpagetable[i].pagetable2[j].frame=rand()%blocknum;
			qw=0;
			while(block[segpagetable[i].pagetable2[j].frame]==OCCUPIED){
				segpagetable[i].pagetable2[j].frame=rand()%blocknum;
				qw++;
				if(qw==1000){
					printf("\t内存所剩不足以存储进程，分配失败！\n");
					return;
				}
			}
			block[segpagetable[i].pagetable2[j].frame]=OCCUPIED;
		}
	}
	printf("\t已分配段表长度为%d,注意每个段表项中页表长度最大应为4，内存分配成功！\n",jdq);
	return jdq;
} 
void showpagetable2(int tt){
	printf("\n\t页表展示:\n");
	printf("\t _______________\n");
	printf("\t|Page\t|Frame\t|\n");
	int pp;
	int i,j;
	for(j=0;j<tt;j++){
		pp=segpagetable[j].pagetableL;
		printf("\t|_______|_______|\n");
		printf("\t|第%d段页表\t|\n",j); 
		for(i=0;i<pp;i++){
			
			printf("\t|%d\t|%d\t|\n",i,segpagetable[j].pagetable2[i].frame);
			if((i==pp-1)&&(j==tt-1)){
				printf("\t|_______|_______|\n");
			}
		}
	}
}

void showsegtable2(int pp){
	printf("\n\t段表展示:\n");
	printf("\t _________________________\n");
	printf("\t|段号\t|页表长度|页表始值|\n"); 
	int i;
	for(i=0;i<pp;i++)
	{
		printf("\t|%d\t|%d\t |%d\t  |\n",i,segpagetable[i].pagetableL,segpagetable[i].pageblock);
		if(i==pp-1){
			printf("\t|_______|________|________|\n");
		}
	}
	
}
void segpagetrasla(int ls,int la,int pp,int prosize ){
	int p=la/blocksize;
	int d=la%blocksize;
	if(ls>=pp) {
		printf("\t段号大于等于段表长度，越界中断\n"); 
		return;
	}
	else if (p>=segpagetable[ls].pagetableL){
		printf("\t所给地址页号大于等于页表长度，越界中断\n"); 
		return; 
	}
	else{
		int uy=segpagetable[ls].pagetable2[p].frame;
		printf("\t指定页框：%d\n",uy);
		int wads=(segpagetable[ls].pagetable2[p].frame)*blocksize+d;
		printf("\t物理地址=段号所在页表的逻辑地址的页号对应的页帧x页帧大小（物理块大小）+逻辑地址的偏移量\n"); 
		printf("\t段号：%d\n\t页号：%d\n\t页内偏移量：%d\n\t物理地址为:%d\n\n",ls,p,d,wads); 
	}

	
}
void showmemory2(){
	printf("\n\t内存状态\n");
	printf("\t ________________\n");
	printf("\t|Frame\t|Status\t |\n");
	int i;
	for(i=0;i<blocknum;i++){
		char ff[]="FREE";
		char ss[]="OCCUPIED";
		
		if(block[i]==0) {
			printf("\t|%d\t|%s\t |\n",i,ff);
		}
		else if(block[i]==1){
			printf("\t|%d\t|%s|\n",i,ss);
		}
		
		if(i==blocknum-1){
			
			printf("\t|_______|________|\n");
		}
	}
	
}
void weishitu(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD originalAttrs;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttrs = consoleInfo.wAttributes;
	printf("\n\t位示图表示内存状态：（0为未占用，1为已占用）\n");
	printf("\t    0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15\n");
	printf("\t   ________________________________________________\n");
	int i;
	int j;
	int ss=0;
	for(j=0;j<16;j++){
		if(j>9){
			printf("\t%d|",j);
		}
		else{
			printf("\t%d |",j);
		}
		
		for(i=j*16;i<j*16+16;i++){		
			if(block[i]==1){
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				printf(" %d ",block[i]);
				SetConsoleTextAttribute(hConsole, originalAttrs);
				 ss++;
				 
			}
			else if(block[i]==0){
				 
				 printf(" %d ",block[i]);
			}
			
		} 
		printf("|\n");	
	}
	printf("\t  |________________________________________________|\n");
	printf("\t内存块已占用%d个\n",ss);
} 
void segpagemenu(){
	int prosize;
	int aa,pp;
	int ls,la;
	printf("\t-------------------------------段页式地址换算---------------------------------\n"); 
	printf("\t请输入进程大小(内存上限65536)，系统将为你分配内存：\n\t");
	scanf("%d",&prosize);
	pp=allocatememory3(prosize);//段表长度 
	while(1){
		printf("\t------------------------------------------------------------------------------\n"); 
		printf("\n\t请输入选项：\n");
		printf("\t1查看段表\n");
		printf("\t2查看页表\n");
		printf("\t3输入逻辑地址，查找物理地址\n");
		printf("\t4查看内存块占用情况\n");
		printf("\t5返回\n");
		printf("\t------------------------------------------------------------------------------\n\t");
		scanf("%d",&aa); 
		switch(aa){
			case 1:
				showsegtable2(pp);
				break;
			case 2:
				showpagetable2(pp);
				break;
			case 3:
				printf("\t请输入逻辑地址(段号和段内逻辑地址)：\n");
				printf("\t段号：");
				scanf("%d",&ls);
				printf("\t段内逻辑地址："); 
				scanf("%d",&la);
				segpagetrasla(ls,la,pp,prosize);
				break;
			case 4:
				showmemory2();
				weishitu();
				break; 
			case 5:
				free(pagetable);
				free(segpagetable);
				 
			    system("cls");
				return; 
			default:
				printf("\t输入错误，请重试\n");
				break;
		}
		
	}
}
int main()
{
	int id;
	while(1)
	{
		printf("             -------------------------基本地址换算过程演示------------------------------\n");
		printf("             |                                                                         |\n");
		printf("             |                                                                         |\n");
		printf("             |                            1.分页式                                     |\n");
		printf("             |                            2.分段式                                     |\n");
		printf("             |                            3.段页式                                     |\n");
		printf("             |                            4.运行结束                                   |\n");
		printf("             |                                                                         |\n");
		printf("             |                                                                         |\n");
		printf("             ---------------------------------------------------------------------------\n\t");
		printf("\t请输入你的选择：");
		scanf("%d",&id); 
		switch(id)
		{
			case 1:
				pagemenu();
            	break;
            case 2:
				segmentmenu();
				break;
			case 3:
				segpagemenu();
            	break;
            case 4:
            	free(pagetable);
            	free(segtable);
            	free(segpagetable);
            	return 0;
            default:
            	printf("\t\t输入有误，请重新输入\n"); break;
				 
		}
	 } 

	
}
