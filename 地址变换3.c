#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define memorysize 65536 
#define blocksize 256
#define blocknum 256
//�������γ�Ϊ1024����2^10��������Ϊ64����2^6 
#define seg_max 64 
#define seg_maxsize 1024

//Ĭ���ڴ��СΪ65536B ��0~65535������64KB�������(ҳ��)��С=ҳ���СΪ256����������鳤�ȣ�������Ϊ256  
enum STATUS {FREE,OCCUPIED  //�ڴ��״̬ 
}block[blocknum]; //�ڴ������  
//�޼Ĵ�����ҳ��ռ���ڴ��ַ�ռ䣬�൱�ڿ�� 
typedef struct {
	int frame;//����ҳ��� 
}pagetableentry;
pagetableentry* pagetable; //ҳ�� ���ҳʽ��ҳ�� 

//�޼Ĵ������α�ռ���ڴ��ַ�ռ䣬�൱�ڿ�� 
typedef struct {
    int base;  // �εĻ�ַ
    int limit; // �γ�
} segmenttableentry;
segmenttableentry* segtable;   // �α�

//��ҳ�� 
typedef struct{
	pagetableentry* pagetable2;//�൱��ҳ��Ĵ��� ��ռ���ڴ��ַ�ռ� 
	int  pagetableL;//ҳ�� 
	int  pageblock;//ҳ��ʼֵ 
	
}segpagetableentry;
segpagetableentry* segpagetable; //��ҳʽ�еĶα� 



int allocatememory(int prosize){
	//��ʼ��ҳ����ڴ��
	int i;
	for(i=0;i<blocknum;i++){
		block[i]=FREE;
	} 
	int numpages;
	//�鿴ҳ��/ҳ�泤�� 
	if(prosize%blocksize==0){
		numpages=prosize/blocksize;
	}
	else{
		numpages=prosize/blocksize+1;
	}
	//���� 
	pagetable = (pagetableentry*)malloc(sizeof(pagetableentry)*numpages);
	
	for(i=0;i<numpages;i++){
		
		pagetable[i].frame=rand()%256;
		while(block[pagetable[i].frame]==OCCUPIED){
			pagetable[i].frame=rand()%256;
		} 
		block[pagetable[i].frame]=OCCUPIED;
	}
	printf("\tҳ���СΪ256��ҳ����Ϊ%d,����ɹ���\n\n",numpages);
	return numpages;
}
void showpagetable(int pp){
	printf("\n\tҳ��չʾ\n");
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

	int p=A/blocksize;// ҳ�� 
	int d=A%blocksize;//ҳ�ڵ�ַ 
	if(p>=L)  printf("\tҳ�Ŵ��ڵ���ҳ���ȣ�Խ���ж�\n"); 
	else if(A>=prosize) printf("\t�߼���ַ�����������̳��ȣ�Խ���ж�\n");
	else{
		int kh=pagetable[p].frame;//��� 
		int Wads = blocksize*kh+ d;
		printf("\t�����ַ=�ڴ���Сxҳ֡(������)+ƫ����\n"); 
		printf("\tҳ��Ϊ��%d\n\tҳ�ڵ�ַΪ��%d\n\t������Ϊ��%d\n\t�����ַΪ��%d\n",p,d,kh,Wads); 
	} 
} 
void showmemory(){
	printf("\n\t�ڴ�״̬\n");
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
	printf("\t-------------------------------��ҳʽ��ַ����---------------------------------\n"); 
	printf("\t��������̴�С(�ڴ�����65536)��ϵͳ��Ϊ������ڴ棺\n");
	printf("\t");scanf("%d",&prosize);
	pp=allocatememory(prosize);
	
	while(1){
		printf("\t------------------------------------------------------------------------------\n"); 
		printf("\n\t������ѡ�\n");
		printf("\t1�鿴ҳ��\n");
		printf("\t2�����߼���ַ�����������ַ\n");
		printf("\t3�鿴�ڴ��ռ�����\n");
		printf("\t4����\n");
		printf("\t------------------------------------------------------------------------------\n\t"); 
		scanf("%d",&aa); 
		switch(aa){
			case 1:
				showpagetable(pp);
				break;
			case 2:
				printf("\t�������߼���ַ��\n\t");
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
				printf("\t�������������\n");
				break;
		}
		
	}
}





int  allocatememory2(int prosize){
	//10+16=26
//	segtable = (segmenttableentry*)malloc(sizeof(segmenttableentry)*se) 
	//ÿ���������64���Σ�ÿ������󳤶�Ϊ1024
	int dc=0;
	int jdq=0;//�ƶ��� 
	int aa[64]={0};//�ƶγ� 
	//��γ��Ͷα��� 
	while(dc<prosize){
		if(jdq>=64) {
			printf("\t���̹����ڴ����ʧ��\n"); return -1;
		}
		aa[jdq]=rand()%1024+1; //�γ���������Ķα��� 
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
	//�����α�  
	for(i=0;i<=jdq;i++ ){
		segtable[i].limit=aa[i];
		if(i==0){
			segtable[i].base=0+rand()%1024;
		}
		else{
			//��ַ���Լ�����ô�� 
			segtable[i].base=segtable[i-1].base+segtable[i-1].limit+rand()%256;
		}
		
		if(segtable[i].base>memorysize){
			printf("\t�ڴ��С���ڴ����ʧ��\n");
			return 0; 
		}
	} 
	jdq=jdq+1;
	printf("\t�α���Ϊ%d,����ɹ�!\n\n",jdq);
	return  jdq;
	
} 
void showsegtable(int pp){
	printf("\n\t�α�չʾ:\n");
	printf("\t ________________________\n");
	printf("\t|�κ�\t|�γ�\t|��ַ\t|\n"); 
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
		printf("\t�κŴ��ڵ��ڶα��ȣ�Խ���ж�\n"); 
		return;
	}
	else if (la>=segtable[ls].limit){
		printf("\t���ڵ�ַ���ڵ��ڶγ���Խ���ж�\n"); 
		return; 
	}
	int wads=segtable[ls].base+la;
	printf("\t�����ַ=�α��ַ+���ڵ�ַ\n");
	printf("\t�����ַΪ:%d\n",wads); 
	
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
	printf("\t-------------------------------�ֶ�ʽ��ַ����---------------------------------\n"); 
	printf("\t��������̴�С(�ڴ�����65536)��ϵͳ��Ϊ������ڴ棺\n\t");
	scanf("%d",&prosize);
	pp=allocatememory2(prosize);
	
	while(1){
		printf("\t------------------------------------------------------------------------------\n");
		printf("\n\t������ѡ�\n");
		printf("\t1�鿴�α�\n");
		printf("\t2�����߼���ַ�����������ַ\n");
		printf("\t3����\n");
		printf("\t------------------------------------------------------------------------------\n\t"); 
		scanf("%d",&aa); 
		switch(aa){
			case 1:
				showsegtable(pp);
				break;
			case 2:
				printf("\t�������߼���ַ(�κźͶ��ڵ�ַ)��\n");
				printf("\t�κţ�");
				scanf("%d",&ls);
				printf("\t���ڵ�ַ��"); 
				scanf("%d",&la);
				segtrasla(ls,la,pp,prosize);
				break;
			case 3:
				free(segtable);
			    
			    system("cls");
				return; 
			
			default:
				printf("\t�������������\n");
				break;
		}
		
	}
	
}
//�й��жϵĵݹ� 
//void judge(enum STATUS block[],int length){
//	if(block[length]==FREE&&)
//	else if(a==0&&)
//}
int allocatememory3(int prosize){
	int dc=0;
	int jdq=0;//�α���󳤶�Ϊ64 
	int aa[64]={0};
	//��ʼ��
	int i;
	for(i=0;i<blocknum;i++){
		block[i]=FREE;
	} 
	while(dc<prosize){
		if(jdq>=64){
			printf("\t���̹����ڴ����ʧ��\n");
			return 0; 
		} 
		aa[jdq]=rand()%1024+1;  //ÿ���γ����Ϊ1024 
		dc=dc+aa[jdq];
		if(dc>=prosize){
			dc=prosize;
			aa[jdq]=aa[jdq]-(dc-prosize);
			break; 
		} 
		jdq++;
	}
	jdq=jdq+1;//�ƶ�����0��ʼ�����Զα���+1
	
	segpagetable =(segpagetableentry*)malloc(sizeof(segpagetableentry)*jdq);
	
	//�����α� 
	for(i=0;i<jdq;i++){
		if(aa[i]%blocksize==0){
			segpagetable[i].pagetableL=aa[i]/blocksize;//ҳ���СΪ256�� 
		} 
		else{
			segpagetable[i].pagetableL=aa[i]/blocksize+1;//ҳ���� 
		}
		
		segpagetable[i].pageblock=rand()%blocknum;//ҳ��ʼֵ 
		segpagetable[i].pagetable2 =(pagetableentry*)malloc(sizeof(pagetableentry)*segpagetable[i].pagetableL); 
		
		
		int ui=segpagetable[i].pagetableL;//ҳ�� 
		int uu=0;
		int sss=0;
		while(1){
			
			uu=segpagetable[i].pageblock;//ʼֵ 
			if(ui==1){
				if(block[uu]==FREE){
					block[uu]=OCCUPIED;
					
					break;
				}
				else{
					segpagetable[i].pageblock=rand()%blocknum;//�ڴ����������� 
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
					segpagetable[i].pageblock=rand()%blocknum;//�ڴ����������� 
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
					segpagetable[i].pageblock=rand()%blocknum;//�ڴ����������� 
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
					segpagetable[i].pageblock=rand()%blocknum;//�ڴ����������� 
				}
			}
			sss++;
			if(sss==1000) {
				printf("\t�ڴ���ʣ�����Դ洢���̣�����ʧ�ܣ�\n");
				return;
			} 
			
		}
		
		//����ҳ��
		
		int j;
		int qw;
		for(j=0;j<segpagetable[i].pagetableL;j++){
			segpagetable[i].pagetable2[j].frame=rand()%blocknum;
			qw=0;
			while(block[segpagetable[i].pagetable2[j].frame]==OCCUPIED){
				segpagetable[i].pagetable2[j].frame=rand()%blocknum;
				qw++;
				if(qw==1000){
					printf("\t�ڴ���ʣ�����Դ洢���̣�����ʧ�ܣ�\n");
					return;
				}
			}
			block[segpagetable[i].pagetable2[j].frame]=OCCUPIED;
		}
	}
	printf("\t�ѷ���α���Ϊ%d,ע��ÿ���α�����ҳ�������ӦΪ4���ڴ����ɹ���\n",jdq);
	return jdq;
} 
void showpagetable2(int tt){
	printf("\n\tҳ��չʾ:\n");
	printf("\t _______________\n");
	printf("\t|Page\t|Frame\t|\n");
	int pp;
	int i,j;
	for(j=0;j<tt;j++){
		pp=segpagetable[j].pagetableL;
		printf("\t|_______|_______|\n");
		printf("\t|��%d��ҳ��\t|\n",j); 
		for(i=0;i<pp;i++){
			
			printf("\t|%d\t|%d\t|\n",i,segpagetable[j].pagetable2[i].frame);
			if((i==pp-1)&&(j==tt-1)){
				printf("\t|_______|_______|\n");
			}
		}
	}
}

void showsegtable2(int pp){
	printf("\n\t�α�չʾ:\n");
	printf("\t _________________________\n");
	printf("\t|�κ�\t|ҳ����|ҳ��ʼֵ|\n"); 
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
		printf("\t�κŴ��ڵ��ڶα��ȣ�Խ���ж�\n"); 
		return;
	}
	else if (p>=segpagetable[ls].pagetableL){
		printf("\t������ַҳ�Ŵ��ڵ���ҳ���ȣ�Խ���ж�\n"); 
		return; 
	}
	else{
		int uy=segpagetable[ls].pagetable2[p].frame;
		printf("\tָ��ҳ��%d\n",uy);
		int wads=(segpagetable[ls].pagetable2[p].frame)*blocksize+d;
		printf("\t�����ַ=�κ�����ҳ����߼���ַ��ҳ�Ŷ�Ӧ��ҳ֡xҳ֡��С��������С��+�߼���ַ��ƫ����\n"); 
		printf("\t�κţ�%d\n\tҳ�ţ�%d\n\tҳ��ƫ������%d\n\t�����ַΪ:%d\n\n",ls,p,d,wads); 
	}

	
}
void showmemory2(){
	printf("\n\t�ڴ�״̬\n");
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
	printf("\n\tλʾͼ��ʾ�ڴ�״̬����0Ϊδռ�ã�1Ϊ��ռ�ã�\n");
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
	printf("\t�ڴ����ռ��%d��\n",ss);
} 
void segpagemenu(){
	int prosize;
	int aa,pp;
	int ls,la;
	printf("\t-------------------------------��ҳʽ��ַ����---------------------------------\n"); 
	printf("\t��������̴�С(�ڴ�����65536)��ϵͳ��Ϊ������ڴ棺\n\t");
	scanf("%d",&prosize);
	pp=allocatememory3(prosize);//�α��� 
	while(1){
		printf("\t------------------------------------------------------------------------------\n"); 
		printf("\n\t������ѡ�\n");
		printf("\t1�鿴�α�\n");
		printf("\t2�鿴ҳ��\n");
		printf("\t3�����߼���ַ�����������ַ\n");
		printf("\t4�鿴�ڴ��ռ�����\n");
		printf("\t5����\n");
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
				printf("\t�������߼���ַ(�κźͶ����߼���ַ)��\n");
				printf("\t�κţ�");
				scanf("%d",&ls);
				printf("\t�����߼���ַ��"); 
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
				printf("\t�������������\n");
				break;
		}
		
	}
}
int main()
{
	int id;
	while(1)
	{
		printf("             -------------------------������ַ���������ʾ------------------------------\n");
		printf("             |                                                                         |\n");
		printf("             |                                                                         |\n");
		printf("             |                            1.��ҳʽ                                     |\n");
		printf("             |                            2.�ֶ�ʽ                                     |\n");
		printf("             |                            3.��ҳʽ                                     |\n");
		printf("             |                            4.���н���                                   |\n");
		printf("             |                                                                         |\n");
		printf("             |                                                                         |\n");
		printf("             ---------------------------------------------------------------------------\n\t");
		printf("\t���������ѡ��");
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
            	printf("\t\t������������������\n"); break;
				 
		}
	 } 

	
}
