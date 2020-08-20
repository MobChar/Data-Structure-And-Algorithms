#include<windows.h>
#include<iostream>
#include <cmath>
#include<conio.h>
#include<algorithm>
#include<string>
#include<ctime>
#include <unistd.h>
#include<sstream>
#include<cstring>
#include<fstream>

using namespace std;

const int MAX_MAYBAY=300;

struct Thoigian{
	int ngay,thang,nam;
	int gio,phut;
};
enum Phai{Nam,Nu
};

struct Danhsachve{
	void Khoitao(int Soday,int Sodong){
		soday=Soday;
		sodong=Sodong;
		sove= new string*[soday];
		cmnd=new string*[soday];					
		for(int i=0;i<soday;i++)
		{
				sove[i]=new string[sodong];
				cmnd[i]=new string[sodong];
		}
		for(int i=0;i<soday;i++){
			for(int j=0;j<sodong;j++){
				
				//Tinh toan so ve
				sove[i][j]=((char)((int)'A'+i));
				char tmp[10];
				itoa(j+1,tmp,10);
				string s(tmp);
				int countnumber=(int)log10(soday*sodong)+1;
				countnumber-=sove[i][j].size()+s.size();
				while(countnumber-->0) {
					sove[i][j]+="0";
				}
//				while(sove[j][j].size()+s.size()<countnumber) {
//					sove[i][j]+="0";
//					cout<<sove[i][j].size()+s.size()<<endl;
//				}
				sove[i][j]+=s;
				
				cmnd[i][j]="";
			}
		}
		sovetrong=soday*sodong;
								  // A B C D
								/////0/0/0/0
								/////1/1/1/1
								/////2/2/2/2
								/////3/3/3/3
								/////4/4/4/4
		
	}
	void Test(){
		for(int i=0;i<soday;i++){
			for(int j=0;j<sodong;j++){
				cout<<sove[i][j]<<' ';
			}
			cout<<endl;
		}
	}
	int sovetrong;
	int soday,sodong;
	string** sove;//mang con tro
	string** cmnd;//=="" thi chua co ai dat ve 
};

struct Maybay{
	char sohieumaybay[15];
	char loaimaybay[40];
	int soday,sodong;
};

struct HanhKhach{
	char cmnd[13];
	char ho[40];
	char ten[40];
	Phai gioitinh;
};

struct Chuyenbay{
	char macb[15];
	char sohieumaybay[15];
	char sanbayden[40];
	Thoigian thoidiemxuatphat;
	
	Danhsachve danhsachve;
	
	int trangthai;
};

struct NodeHK{
	HanhKhach hanhkhach;
	NodeHK* left=NULL;
	NodeHK* right=NULL;
};

struct NodeCB{
	Chuyenbay chuyenbay;
	NodeCB* next=NULL;
};



Maybay* danhsachmaybay[MAX_MAYBAY];
int soluongmaybay=0;
NodeHK* hanhkhachtree=NULL;//Cay nhi phan tim kiem
NodeCB* danhsachchuyenbay=NULL;//Danh sach lien ket don


int mauchon=5;
int maucoban=100;
int maucuachu=120;
int maunen=2;

//int DD=0,MM=0,YYYY=0,hh=0,mm=0;//Khoang thoi gian cap nhat lan truoc

int Timkiemmaybay(Maybay* danhsachmaybay[],int soluongmaybay,char sohieumaybay[]){
	for(int i=0;i<soluongmaybay;i++){
		if(strcmp(danhsachmaybay[i]->sohieumaybay,sohieumaybay)==0){
			return i;//Tim thay may bay co so hieu la so hieu may bay tra ve vi tri cua may bay trong mang
		}
	}
	return -1;//Khong tim thay tra ve -1
}
void InsertFirst_Danhsachchuyenbay(NodeCB* danhsachchuyenbay,NodeCB* chuyenbaymoi){//Chen vao danh sach lien ket don chuyen bay
	chuyenbaymoi->next=danhsachchuyenbay;
	danhsachchuyenbay=chuyenbaymoi;
}
NodeHK* Timkiemhanhkhach(NodeHK* cayhanhkhach,char cmnd[]){//Tim kiem hanh  khach dua vao cmnd, Neu tim thay tra ve node hanh khach do khong thi tra ve null
	NodeHK* re=cayhanhkhach;
	while(re!=NULL){
		if(stricmp(cmnd,re->hanhkhach.cmnd)<0) re=re->left;//Tim kiem o ben trai
		else if(stricmp(cmnd,re->hanhkhach.cmnd)>0) re=re->right;//Tim kiem o ben phai
		else return re;
	}
	return NULL;
}

void Themhanhkhachvaocay(NodeHK* &cayhanhkhach,NodeHK* hanhkhachmoi){
	if(cayhanhkhach==NULL) {
		cayhanhkhach=hanhkhachmoi;
		return;
	}
	NodeHK* last=NULL;
	NodeHK* vitrichen=cayhanhkhach;
	while(vitrichen!=NULL){
		last=vitrichen;
		if(stricmp(hanhkhachmoi->hanhkhach.cmnd,vitrichen->hanhkhach.cmnd)<0) vitrichen=vitrichen->left;
		else if(stricmp(hanhkhachmoi->hanhkhach.cmnd,vitrichen->hanhkhach.cmnd)>0) vitrichen=vitrichen->right;
		else return;//Truong hop da co trong cay thi khong chen vo nua
	}
	if(stricmp(hanhkhachmoi->hanhkhach.cmnd,vitrichen->hanhkhach.cmnd)<0) last->left=hanhkhachmoi;
	else last->right=hanhkhachmoi;
}

NodeCB* Timkiemchuyenbay(NodeCB* danhsachchuyenbay,char machuyenbay[]){
	while(danhsachchuyenbay!=NULL){
		if(stricmp(danhsachchuyenbay->chuyenbay.macb,machuyenbay)==0) return danhsachchuyenbay;
		danhsachchuyenbay=danhsachchuyenbay->next;
	}
	return NULL;
}
void InsertOrder_chuyenbay(NodeCB* &danhsachchuyenbay,NodeCB* chuyenbaymoi){//Danh sachh chuyen luon co san thu tu theo ma chuyen bay
	if(danhsachchuyenbay==NULL){
		danhsachchuyenbay=chuyenbaymoi;
		return;
	}
	NodeCB* p=danhsachchuyenbay;
	NodeCB* last=danhsachchuyenbay;
	while(p!=NULL&&stricmp(p->chuyenbay.macb,p->chuyenbay.macb)<0){//Tim node dau tien lon hon hoac ban chuyenbaymoi
		last=p;
		p=p->next;
	}
	//Chen
	chuyenbaymoi->next=last->next;//Chuyen bay moi tro toi thang tiep theo cua last
	last->next=chuyenbaymoi;//last tro toi chuyen bay moi
	
}

void Xoachuyenbay(NodeCB* &danhsachchuyenbay,NodeCB* chuyenbaycanxoa){
	if(chuyenbaycanxoa==NULL||danhsachchuyenbay==NULL) return;
	if(chuyenbaycanxoa==danhsachchuyenbay){//Xoa phan tu o dau
		danhsachchuyenbay=chuyenbaycanxoa->next;
		return;
	}
	NodeCB* p=danhsachchuyenbay;//Con tro duyet ho danh sach chuyen bay
	while(p->next!=NULL&&p->next!=chuyenbaycanxoa){
		p=p->next;
	}
	if(p->next==NULL) return;//Khong tim thay thang can xoa
	p->next=chuyenbaycanxoa->next;
}




void gotoxy (int column, int line){	
	COORD coord; 	
	coord.X = column;  	
	coord.Y = line; 	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void TextColor (int color){	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}

void SetBGColor(int color){
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	color <<= 4;
	wAttributes &= 0xff0f;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

bool CheckEmptyAndSpace(char* s){
	//Tra ve false neu chuoi trong hoac toan la dau cach, true truong hop con lai
	if(strlen(s)==0) return false;
	for(int i=0;s[i]!='\0';i++){
		if(s[i]=='/') return false;
	}
	for(int i=0;s[i]!='\0';i++){
		if(s[i]!=' ') return true;
	}
	return false;
}

bool CheckFullOfNumber(char* s){
	if(!CheckEmptyAndSpace(s)) return false;
	//Chuoi chi bao gom so thi tra ve true, false truong hop con lai
	for(int i=0;s[i]!='\0';i++){
		if(!isdigit(s[i])) return false;
	}
	return true;
}



void DrawRectangle(int col,int row,int width,int height){//Ve hinh chu nhat
	for(int j=row;j-row<height;j++){
		for(int i=col;i-col<width;i++){
			gotoxy(i,j);
			cout<<" ";
		}
	}
}


bool ValidTime(int DD,int MM,int YY,int hh,int mm){//Kiem tra thoi gian nhap vao
	if(hh>23||mm>59||hh<0||mm<0) return false;
	 if(YY>=1900 && YY<=9999)
	    {
	        //check month
	        if(MM>=1 && MM<=12)
	        {
	            //check days
	            if((DD>=1 && DD<=31) && (MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10 || MM==12));
	            else if((DD>=1 && DD<=30) && (MM==4 || MM==6 || MM==9 || MM==11));
	            else if((DD>=1 && DD<=28) && (MM==2));
	            else if(DD==29 && MM==2 && (YY%400==0 ||(YY%4==0 && YY%100!=0)));
	            else
	                return false;
	        }
	        else
	        {
	            return false;
	        }
	    }
	    else
	    {
	       return false;
	    }
	    return true;
}

bool ValidTime(int DD,int MM,int YY){//Kiem tra thoi gian nhap vao
	 if(YY>=1900 && YY<=9999)
	    {
	        //check month
	        if(MM>=1 && MM<=12)
	        {
	            //check days
	            if((DD>=1 && DD<=31) && (MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10 || MM==12));
	            else if((DD>=1 && DD<=30) && (MM==4 || MM==6 || MM==9 || MM==11));
	            else if((DD>=1 && DD<=28) && (MM==2));
	            else if(DD==29 && MM==2 && (YY%400==0 ||(YY%4==0 && YY%100!=0)));
	            else
	                return false;
	        }
	        else
	        {
	            return false;
	        }
	    }
	    else
	    {
	       return false;
	    }
	    return true;
}

int CompareTime(int ad, int am, int ay,int ahh,int amm,int bd,int bm,int by,int bhh,int bmm)
{
	if(ay!=by)
	{
		if(ay>by)return 1;
		else return -1;
	}
	else
	{
		if(am!=bm)
		{
			if(am>bm) return 1;
			else return -1;
		}
		else
		{
			if(ad!=bd)
			{
				if(ad>bd) return 1;
				else return -1;
			}
			else {
				//Ngay thang nam deu bang nhau
				if(ahh*60+amm>bhh*60+bmm)//So sanh bang phut
				return 1;
				else if(ahh*60+amm<bhh*60+bmm)
				return -1;
				else return 0;
			}
		}
	}
}


void Vehopcochu(int col,int row,int docaocuahop,int dodaicuahop,int maucuahop, int maucuachu,char * text,bool canletrai){//Ve hop co doan text ben trong
	SetBGColor(maucuahop);
	DrawRectangle(col,row,dodaicuahop,docaocuahop);
	TextColor(maucuachu);
	if(!canletrai) gotoxy(col,row+docaocuahop/2);
	else gotoxy(col+dodaicuahop/2-strlen(text)/2,row+docaocuahop/2);
	cout<<text;
}

void DrawAnnouncement(const char* announce){//Thong bao
	int row=0,col=75;
	TextColor(114);
	gotoxy(col,row);
	cout<<announce;
	double playtime=1.f;
	double starttime=(double)clock()/CLOCKS_PER_SEC;
	while(true){
		if((double)clock()/CLOCKS_PER_SEC-starttime>=playtime) break;
	}
	SetBGColor(0);
	DrawRectangle(col,row,strlen(announce),1);
}

bool Capnhatdanhsachchuyenbay(){//Nhap vao ngay thang nam de cap nhat danh sach chuyen bay danh dau cai nao la hoan tat	
	char field[5][40];//Ngay thang nam gio phut
	char label[5][50]={"DD","MM","YYYY","hh","mm"};
	int width[5]={2,2,4,2,2};
	int printpos[5]={20,23,26,31,34};
	
	
	int chisonhap=0;
	int row=10;
	
	for(int i=0;i<5;i++) field[i][0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-5,45,7);
	for(int i=0;i<5;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
	}
	gotoxy(printpos[0],row-4);
	cout<<"Nhap vao ngay gio hien tai de cap nhat";
	
	
	while(true){
		int type=getch();
		if(type==9){//Phim tab thoat khoi them may bay
			return false;
		}
		else if(type==13){//Phim Enter
			if(!CheckFullOfNumber(field[0])||!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])||!CheckFullOfNumber(field[4])){
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			if(!ValidTime(atoi(field[0]),atoi(field[1]),atoi(field[2]),atoi(field[3]),atoi(field[4]))){
				DrawAnnouncement("Thoi gian nhap vao khong hop le");
				continue;
			}
//			if(CompareTime(DD,MM,YYYY,hh,mm,atoi(field[0]),atoi(field[1]),atoi(field[2]),atoi(field[3]),atoi(field[4]))>0){
//				DrawAnnouncement("Ban dang nhap vao thoi diem qua khu ???");
//				continue;
//			}
//			DD=atoi(field[0]);
//			MM=atoi(field[1]);
//			YYYY=atoi(field[2]);
//			hh=atoi(field[3]);
//			mm=atoi(field[4]);
			//Cap nhat danh sach chuyen bay
			NodeCB* p=danhsachchuyenbay;
			while(p!=NULL){//Neu chuyen bay da bi huy,hoan tat thi khong cap nhat
				if((p->chuyenbay.trangthai==1||p->chuyenbay.trangthai==2)&&CompareTime(atoi(field[0]),atoi(field[1]),atoi(field[2]),atoi(field[3]),atoi(field[4]),p->chuyenbay.thoidiemxuatphat.ngay,p->chuyenbay.thoidiemxuatphat.thang,p->chuyenbay.thoidiemxuatphat.nam,p->chuyenbay.thoidiemxuatphat.gio,p->chuyenbay.thoidiemxuatphat.phut)>0)
				p->chuyenbay.trangthai=3;//Da hoan tat chuyen bay
				p=p->next;
			}
			DrawAnnouncement("Cap nhat danh sach chuyen bay thanh cong");
			return true;
		}
		else if(type==224){//Bat phim mui ten
			type=getch();
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,120,field[chisonhap],false);
				
			if(type==75) chisonhap=max(chisonhap-1,0);
			if(type==77) chisonhap=min(chisonhap+1,4);
				
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,120,field[chisonhap],false);
		}
		else if(type==8){//Xoa mot ki tu cua chuoi dang nhap vao
				int nextindex=max((int)strlen(field[chisonhap])-1,0);
				field[chisonhap][nextindex]='\0';
				SetBGColor(mauchon);
				DrawRectangle(printpos[chisonhap]+nextindex,row,1,1);
				continue;
			}
		else {//Them mot ki tu cua chuoi dang nhap vao
			int nextindex=strlen(field[chisonhap]);
			if(nextindex>=width[chisonhap]) continue;
			
			field[chisonhap][nextindex]=(char)type;
			field[chisonhap][nextindex+1]='\0';
			
			gotoxy(printpos[chisonhap],row);
			TextColor(maucuachu);
			cout<<field[chisonhap];
		}
	}
}

//////////////////////////////////Chuc nang
void Indanhsachmaybay(Maybay* danhsachmaybay[],int soluongmaybay){//Khong can sap xep danh sach may bay
	SetBGColor(0);
	system("cls");
	
	TextColor(255);
	SetBGColor(0);
	
	int row=10,col=40;
	gotoxy(col,row-3);
	cout<<"|SO HIEU MB";
	gotoxy(col+15,row-3);
	cout<<"|LOAI MAY BAY";
	gotoxy(col+55,row-3);
	cout<<"|DAY";
	gotoxy(col+60,row-3);
	cout<<"|DONG";
	
	int cnt=80;
	gotoxy(col,row-2);
	while(cnt--) cout<<'_';
	
	for(int i=0;i<soluongmaybay;i++){
		gotoxy(col,row+i);
		cout<<danhsachmaybay[i]->sohieumaybay;
		gotoxy(col+15,row+i);
		cout<<danhsachmaybay[i]->loaimaybay;
		gotoxy(col+55,row+i);
		cout<<danhsachmaybay[i]->soday;
		gotoxy(col+60,row+i);
		cout<<danhsachmaybay[i]->sodong;
	}
}

void Themmaybay(){
	char field[4][40];//So hieu may bay, loai may bay, so day ,so dong
	char label[4][50]={"So hieu mb","Loai may bay","Day","Dong"};
	int width[4]={14,39,3,3};
	int printpos[4]={40,45,95,99};
	
	
	int chisonhap=0;
	int row=5;
	
	for(int i=0;i<4;i++) field[i][0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,70,5);
	for(int i=0;i<4;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
	}
	
	
	while(true){
		int type=getch();
		if(type==9){//Phim tab thoat khoi them may bay
			return;
		}
		else if(type==13){//Phim Enter
			if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])){
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			
			if(atoi(field[2])<0||atoi(field[2])>5){
				DrawAnnouncement("So day phai >0 va <=5");
				continue;
			}
			if(atoi(field[3])<0||atoi(field[3])>20){
				DrawAnnouncement("So dong phai >0 va <=20");
				continue;
			}

			//Kiem tra da ton tai ma may bay chua
			int vitrimaybay=Timkiemmaybay(danhsachmaybay,soluongmaybay,field[0]);
			if(vitrimaybay==-1){//Khong tim thay may bay co so hieu la field[0]
				
				//Toa may bay moit luu vao mang con tro may bay
				Maybay* maybaymoi=new Maybay;
				//Dua du lieu cho may bay moi
				strcpy(maybaymoi->sohieumaybay,field[0]);
				strcpy(maybaymoi->loaimaybay,field[1]);
				maybaymoi->soday=atoi(field[2]);
				maybaymoi->sodong=atoi(field[3]);
				
				//dua vao mang con tro
				danhsachmaybay[soluongmaybay++]=maybaymoi;
				
				DrawAnnouncement("Them may bay thanh cong");
				return;//Thoat khoi chuc nang them may bay
			}
			else {//Ma may da ton tai
				DrawAnnouncement("Ma may bay da ton tai");
				continue;
			}
		}
		else if(type==224){//Bat phim mui ten
			type=getch();
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,120,field[chisonhap],false);
				
			if(type==75) chisonhap=max(chisonhap-1,0);
			if(type==77) chisonhap=min(chisonhap+1,3);
				
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,120,field[chisonhap],false);
		}
		else if(type==8){//Xoa mot ki tu cua chuoi dang nhap vao
				int nextindex=max((int)strlen(field[chisonhap])-1,0);
				field[chisonhap][nextindex]='\0';
				SetBGColor(mauchon);
				DrawRectangle(printpos[chisonhap]+nextindex,row,1,1);
				continue;
			}
		else {//Them mot ki tu cua chuoi dang nhap vao
			int nextindex=strlen(field[chisonhap]);
			if(nextindex>=width[chisonhap]) continue;
			
			field[chisonhap][nextindex]=(char)type;
			field[chisonhap][nextindex+1]='\0';
			
			gotoxy(printpos[chisonhap],row);
			TextColor(maucuachu);
			cout<<field[chisonhap];
		}
	}
	
	
}

void Hieuchinhmaybay(){//Chi duoc sua doi loai may bay
	char field[4][40];//So hieu may bay, loai may bay, so day ,so dong
	char label[4][50]={"So hieu mb","Loai may bay","So day","So dong"};//Chi cho hieu chinh loai may bay
	int width[4]={14,39,3,3};
	int printpos[4]={40,55,95,99};
	
	int chisonhap=1;
	int vitrisuadoi=-1;
	int row=5;
	char kiemtra[15];
	kiemtra[0]='\0';
	
	for(int i=0;i<4;i++) field[i][0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,20,5);
	Vehopcochu(printpos[0],row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);
	Vehopcochu(printpos[0],row-2,1,strlen("Nhap vao ma mb"),maucoban,maucuachu,"Nhap vao ma mb",false);

	
	
    while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(vitrisuadoi==-1){//Nhap vao ma vat tu
			if(type==13){
				//Tim kiem vat tu
				if(!CheckEmptyAndSpace(kiemtra)) {
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				vitrisuadoi=Timkiemmaybay(danhsachmaybay,soluongmaybay,kiemtra);
				if(vitrisuadoi==-1)DrawAnnouncement("Khong tim thay may bay");
				else {
					DrawAnnouncement("Tim thay may bay");
	
				
					
					strcpy(field[0],danhsachmaybay[vitrisuadoi]->sohieumaybay);//Dua de lieu ra ngoai de xem
					strcpy(field[1],danhsachmaybay[vitrisuadoi]->loaimaybay);
					itoa(danhsachmaybay[vitrisuadoi]->soday,field[2],10);
					itoa(danhsachmaybay[vitrisuadoi]->sodong,field[3],10);
					
					SetBGColor(maunen);
					DrawRectangle(printpos[0]-1,row-3,70,5);
						for(int i=0;i<4;i++){
							if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
							else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
							Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
						}
				}
			}
			else{
				if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(kiemtra)-1;
					if(vitrixoa<0) continue;
					
					kiemtra[vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[0]+vitrixoa,row,1,1);
				}
				else{
					//Them chu vao field
					if(sizeof(kiemtra)-1==strlen(kiemtra))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(kiemtra);
					kiemtra[vitrithemchu]=(char)type;
					kiemtra[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[0],row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);//Ve lai
					
				}
			}
		}
		else{//vitrisuadoi!=-1 da tim thay may bay, Khong co bat phim di chuyen
				 if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(field[chisonhap])-1;
					if(vitrixoa<0) continue;
					
					field[chisonhap][vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[chisonhap]+vitrixoa,row,1,1);
				}
				else if(type==13){
					//Luu du lieu
					if(!CheckEmptyAndSpace(field[1])) {
						DrawAnnouncement("Nhap vao khong hop le");
						continue;
					}
					
					//Hop le , luu vao mang
					strcpy(danhsachmaybay[vitrisuadoi]->loaimaybay,field[1]);
					
					
					DrawAnnouncement("Luu thanh cong");
					return;
				}
				else if(type==27){
					//Xoa vat tu dich qua trai
					for(int i=vitrisuadoi;i<soluongmaybay-1;i++){
						danhsachmaybay[i]=danhsachmaybay[i+1];
					}
					--soluongmaybay;
					DrawAnnouncement("Xoa thanh cong");
					return;
				}
				else{
					//Them chu vao field
					if(width[chisonhap]==strlen(field[chisonhap]))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(field[chisonhap]);
					field[chisonhap][vitrithemchu]=(char)type;
					field[chisonhap][vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
					
				}
		}
	}
	
	
}
void Quanlymaybay(){
	Indanhsachmaybay(danhsachmaybay,soluongmaybay);
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==0){
			type=getch();
			if(type==59){
				//Them may bay
				Themmaybay();
				Indanhsachmaybay(danhsachmaybay,soluongmaybay);
			}
			else if(type==60){
				//Hieu chinh, xoa may bay
				Hieuchinhmaybay();
				Indanhsachmaybay(danhsachmaybay,soluongmaybay);
			}
		}
	}
}


void Themchuyenbay(){
	//Xoa man hinh
	SetBGColor(0);
	system("cls");
	
	//Trang thai khoi tao dau tien cua may bay la 0
	char field[8][40];//Ma chuyen bay,So hieu may bay,San bay den, date, month, year, hour, minute,
	char label[8][50]={"Ma chuyen bay","So hieu may bay","San bay den","DD","MM","YYYY","hh","mm"};//Chi cho hieu chinh loai may bay
	int printpos[8]={20,35,50,90,93,96,101,104};
	int width[8]={14,14,39,2,2,4,2,2};
	
	int chisonhap=0;
	int vitrisuadoi=-1;
	int row=5;
	
	for(int i=0;i<8;i++) field[i][0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,100,5);
	for(int i=0;i<8;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
	}
	
	
	while(true){
		int type=getch();
		if(type==9){//Phim tab thoat khoi them may bay
			return;
		}
		else if(type==13){//Phim Enter
			//Ma chuyen bay,So hieu may bay,San bay den, date, month, year, hour, minute,
			if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])||!CheckEmptyAndSpace(field[2])||!CheckFullOfNumber(field[3])||!CheckFullOfNumber(field[4])||!CheckFullOfNumber(field[5])||!CheckFullOfNumber(field[6])||!CheckFullOfNumber(field[7])){
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			if(!ValidTime(atoi(field[3]),atoi(field[4]),atoi(field[5]),atoi(field[6]),atoi(field[7]))){
				DrawAnnouncement("Thoi gia nhap vao khong hop le");
				continue;
			}
			
			NodeCB* kiemtra=danhsachchuyenbay;
			bool f=false;//Khong tim ma chuyen bay
			while(kiemtra!=NULL){
				if(stricmp(kiemtra->chuyenbay.macb,field[0])==0){
					DrawAnnouncement("Ma chuyen bay da ton tai");
					f=true;
					break;
				}
				kiemtra=kiemtra->next;	
			}
			if(f){//Ma chuyen bay da ton tai nhap lai
				continue;
			}
			
			int vitrimaybay=Timkiemmaybay(danhsachmaybay,soluongmaybay,field[1]);
			if(vitrimaybay==-1){//Khong tim thay may bay nhap lai
				DrawAnnouncement("Khong tim thay may bay");
				continue;
			}
			
			NodeCB* chuyenbaymoi=new NodeCB;
			strcpy(chuyenbaymoi->chuyenbay.macb,field[0]);
			strcpy(chuyenbaymoi->chuyenbay.sohieumaybay,field[1]);
			strcpy(chuyenbaymoi->chuyenbay.sanbayden,field[2]);
			chuyenbaymoi->chuyenbay.trangthai=1;//Con ve
			chuyenbaymoi->chuyenbay.thoidiemxuatphat.ngay=atoi(field[3]);
			chuyenbaymoi->chuyenbay.thoidiemxuatphat.thang=atoi(field[4]);
			chuyenbaymoi->chuyenbay.thoidiemxuatphat.nam=atoi(field[5]);
			chuyenbaymoi->chuyenbay.thoidiemxuatphat.gio=atoi(field[6]);
			chuyenbaymoi->chuyenbay.thoidiemxuatphat.phut=atoi(field[7]);
			chuyenbaymoi->chuyenbay.danhsachve.Khoitao(danhsachmaybay[vitrimaybay]->soday,danhsachmaybay[vitrimaybay]->sodong);
			
			
			//Dua chuyen bay moi vao danh sach lien ket don insert_order
			InsertOrder_chuyenbay(danhsachchuyenbay,chuyenbaymoi);
			
			
			DrawAnnouncement("Them chuyen bay thanh cong");
			return;
			
			
		}
		else if(type==224){//Bat phim mui ten
			type=getch();
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,120,field[chisonhap],false);
				
			if(type==75) chisonhap=max(chisonhap-1,0);
			if(type==77) chisonhap=min(chisonhap+1,7);
				
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,120,field[chisonhap],false);
		}
		else if(type==8){//Xoa mot ki tu cua chuoi dang nhap vao
				int nextindex=max((int)strlen(field[chisonhap])-1,0);
				field[chisonhap][nextindex]='\0';
				SetBGColor(mauchon);
				DrawRectangle(printpos[chisonhap]+nextindex,row,1,1);
				continue;
			}
		else {//Them mot ki tu cua chuoi dang nhap vao
			int nextindex=strlen(field[chisonhap]);
			if(nextindex>=width[chisonhap]) continue;
			
			field[chisonhap][nextindex]=(char)type;
			field[chisonhap][nextindex+1]='\0';
			
			gotoxy(printpos[chisonhap],row);
			TextColor(maucuachu);
			cout<<field[chisonhap];
		}
	}
}
void Hieuchinhchuyenbay(){//Chi duoc sua doi loai may bay
	char field[6][40];//Ngay, thang ,nam ,gio ,phut, trang thai
	char label[6][50]={"DD","MM","YYYY","hh","mm","Trang thai"};//Chi cho hieu chinh loai may bay
	int width[6]={2,2,4,2,2,15};
	int printpos[6]={30,33,36,41,44,47};
	
	int chisonhap=0;
	int row=5;
	char kiemtra[15];//Nhap vao ma chuyen bay
	kiemtra[0]='\0';
	//
	int trangthaichuyenbay=1;//1 Con ve, 0 huy chuyen,2 het ve, 3 hoan tat
	
	NodeCB* hieuchinhchuyenbay=NULL;//Chua tim thay chuyen bay
	
	for(int i=0;i<6;i++) field[i][0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,35,5);
	Vehopcochu(printpos[0],row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);
	Vehopcochu(printpos[0],row-2,1,strlen("Nhap vao ma chuyen bay"),maucoban,maucuachu,"Nhap vao ma chuyen bay",false);

	
	
    while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(hieuchinhchuyenbay==NULL){//Nhap vao ma chuyen bay
			if(type==13){
				if(!CheckEmptyAndSpace(kiemtra)) {
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				
				hieuchinhchuyenbay=Timkiemchuyenbay(danhsachchuyenbay,kiemtra);
				if(hieuchinhchuyenbay==NULL){
					DrawAnnouncement("Khong tim thay chuyen bay nay");
					continue;
				}
				
				itoa(hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.ngay,field[0],10);
				itoa(hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.thang,field[1],10);
				itoa(hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.nam,field[2],10);
				itoa(hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.gio,field[3],10);
				itoa(hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.phut,field[4],10);
	
				if(hieuchinhchuyenbay->chuyenbay.trangthai==0) strcpy(field[5],"Huy chuyen");
				else if(hieuchinhchuyenbay->chuyenbay.trangthai==1) strcpy(field[5],"Con ve");
				else if(hieuchinhchuyenbay->chuyenbay.trangthai==2) strcpy(field[5],"Het ve");
				else strcpy(field[5],"Hoan tat");
				trangthaichuyenbay=1;//Con ve
			
				SetBGColor(maunen);
				DrawRectangle(printpos[0]-1,row-3,70,5);
				for(int i=0;i<6;i++){
					if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
					else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
					Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
				}
			}
			else{
				if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(kiemtra)-1;
					if(vitrixoa<0) continue;
					
					kiemtra[vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[0]+vitrixoa,row,1,1);
				}
				else{
					//Them chu vao field
					if(sizeof(kiemtra)-1==strlen(kiemtra))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(kiemtra);
					kiemtra[vitrithemchu]=(char)type;
					kiemtra[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[0],row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);//Ve lai
					
				}
			}
		}
		else{//dang hieu chinh chuyen bay
				if(type==13){
					//Luu du lieu
					if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])||!CheckFullOfNumber(field[4])){
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
					}
					if(!ValidTime(atoi(field[0]),atoi(field[1]),atoi(field[2]),atoi(field[3]),atoi(field[4]))){
						DrawAnnouncement("Thoi gia nhap vao khong hop le");
						continue;
					}
					
					hieuchinhchuyenbay->chuyenbay.trangthai=trangthaichuyenbay;
					hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.ngay=atoi(field[0]);
					hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.thang=atoi(field[1]);
					hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.nam=atoi(field[2]);
					hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.gio=atoi(field[3]);
					hieuchinhchuyenbay->chuyenbay.thoidiemxuatphat.phut=atoi(field[4]);

					
					
					DrawAnnouncement("Sua doi chuyen bay thanh cong");
					return;
				}
				if(type==224){//Mui ten len xuon
					type=getch();
					if(type==75){// qua trai
						//Ve lai field cu
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,maucuachu,field[chisonhap],false);
						chisonhap=max(chisonhap-1,0);
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					else if(type==77) //qua phai
					{
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,maucuachu,field[chisonhap],false);
						chisonhap=min(chisonhap+1,5);
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
				}
				else if(type==27){
					//Xoa chuyen bay
					Xoachuyenbay(danhsachchuyenbay,hieuchinhchuyenbay);
					DrawAnnouncement("Xoa chuyen bay thanh cong");
					return;
				}
				else if(chisonhap==5){
//					if(type==(int)'1'){
//						trangthaichuyenbay=1;
//						strcpy(field[5],"Con ve");
//						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
//					} Dang het ve chuyen ve con ve thi khong hop le nenn bo
					if(type==(int)'2'){
						trangthaichuyenbay=2;
						strcpy(field[5],"Het ve");
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					else if(type==(int)'3'){
						trangthaichuyenbay=3;
						strcpy(field[5],"Hoan tat");
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					else if(type==(int)'0'){
						trangthaichuyenbay=0;
						strcpy(field[5],"Huy chuyen");
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					
				}
				else if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(field[chisonhap])-1;
					if(vitrixoa<0) continue;
					
					field[chisonhap][vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[chisonhap]+vitrixoa,row,1,1);
				}
				else{
					//Them chu vao field
					if(width[chisonhap]==strlen(field[chisonhap]))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(field[chisonhap]);
					field[chisonhap][vitrithemchu]=(char)type;
					field[chisonhap][vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
					
				}
		}
	}
	
	
}
void Quanlychuyenbay(){
	int chooser=0;
	char label[2][50]={"Them chuyen bay","Hieu chinh chuyen bay"};
	int row=10,col=50;
	for(int i=0;i<2;i++){
		if(chooser==i) Vehopcochu(col,row+5*i+2*i,5,30,mauchon,maucuachu,label[i],true);
		else Vehopcochu(col,row+5*i+2*i,5,30,maucoban,maucuachu,label[i],true);
	}
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==13){
			if(chooser==0)
			{
				SetBGColor(0);
				system("cls");
				Themchuyenbay();
				return;
			}
			else if(chooser==1){
				SetBGColor(0);
				system("cls");
				Hieuchinhchuyenbay();
				return;
			}
		}
		else if(type==224){
			type=getch();
			if(type==72){
				Vehopcochu(col,row+5*chooser+2*chooser,5,30,maucoban,maucuachu,label[chooser],true);
				chooser=max(0,chooser-1);
				Vehopcochu(col,row+5*chooser+2*chooser,5,30,mauchon,maucuachu,label[chooser],true);
			}
			else if(type=80){
				
				Vehopcochu(col,row+5*chooser+2*chooser,5,30,maucoban,maucuachu,label[chooser],true);
				chooser=min(1,chooser+1);
				Vehopcochu(col,row+5*chooser+2*chooser,5,30,mauchon,maucuachu,label[chooser],true);
			}
		}
		
	}
}

void Inthongtinhanhkhach(int col,int row,NodeHK* hanhkhach){
		int width=20;
		int height=10;
		if(hanhkhach==NULL){
			SetBGColor(0);
			DrawRectangle(col,row,width,height);
			return;
		}
		TextColor(255);
		SetBGColor(0);
		gotoxy(col,row);
		cout<<"CMND: "<<hanhkhach->hanhkhach.cmnd;
		gotoxy(col,row+1);
		cout<<"Ho: "<<hanhkhach->hanhkhach.ho;
		gotoxy(col,row+2);
		cout<<"Ten: "<<hanhkhach->hanhkhach.ten;
		gotoxy(col,row+3);
		if(hanhkhach->hanhkhach.gioitinh==Nam) cout<<"Phai: Nam";
		else cout<<"Phai Nu";
}
void Datve_Huyve(){
	if(!Capnhatdanhsachchuyenbay()) return;//Cap nhat lai danh sach chuyen bay
	
	SetBGColor(0);
	system("cls");
	//Nhap vao thong tin
	char field[4][40];//CMND, ho , ten ,phai
	char label[4][50]={"CMND","Ho","Ten","Gioi tinh"};//Chi cho hieu chinh loai may bay
	int width[4]={12,39,39,15};
	int printpos[4]={30,45,85,125};
	
	for(int i=0;i<4;i++) field[i][0]='\0';
	
	int chisonhap=0;
	int row=5;
	Phai tmpgioitinh=Nam;//Nam;
	strcpy(field[3],"Nam");
	
	
	//Nhap vao so CMND
	char cmnd[13];
	cmnd[0]='\0';
	
	
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,50,5);
	Vehopcochu(printpos[0],row,1,sizeof(cmnd)-1,mauchon,maucuachu,cmnd,false);
	Vehopcochu(printpos[0],row-2,1,strlen("Nhap vao cmnd, chua co thi nhan F1"),maucoban,maucuachu,"Nhap vao cmnd, chua co thi nhan F1",false);
	
	bool cocmnd=true;//0 nhap vao cmdn, 1 them thong tin
	NodeHK* hanhkhach=NULL;//Chua tim thay hanh khach
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(cocmnd==true){//Nhap vao ma vat tu
			if(type==13){
				//Tim kiem cmnd
				//Neu tim thay thi ket thuc vong lap nay qua vong lap huy ve
				hanhkhach=Timkiemhanhkhach(hanhkhachtree,cmnd);
				if(hanhkhach==NULL){
					DrawAnnouncement("Khong tim thay hanh khach");
					continue;
				}
				else {
					DrawAnnouncement("Tim thay hanh khach");
					Inthongtinhanhkhach(40,10,hanhkhach);
					
					gotoxy(10,9);
					cout<<"Nhan bat ky de tiep tuc";
					
					getch();
					break;
				}
				
			}
			else if(type==0){
				type=getch();
				if(type==59)//F1 chuyen qua them thong tin
				{
					cocmnd=false;//Chuyen qua them thong tin
					SetBGColor(0);
					system("cls");
					
					SetBGColor(maunen);
					DrawRectangle(printpos[0]-1,row-3,120,5);
					for(int i=0;i<4;i++){
						if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
						else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
						Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
					}
					continue;
				}
			}
			else if(type==8){//Backspace xoa 1 chu cua cmnd
					int vitrixoa=strlen(cmnd)-1;
					if(vitrixoa<0) continue;
					
					cmnd[vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[0]+vitrixoa,row,1,1);
			}
			else{
					//Them chu vao field
					if(sizeof(cmnd)-1==strlen(cmnd))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(cmnd);
					cmnd[vitrithemchu]=(char)type;
					cmnd[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[0],row,1,sizeof(cmnd)-1,mauchon,maucuachu,cmnd,false);//Ve lai
					
			}
		}
		else{//cocmnd==false
				if(type==13){
					//Luu du lieu
					if(!CheckFullOfNumber(field[0])||strlen(field[0])!=12||!CheckEmptyAndSpace(field[1])||!CheckEmptyAndSpace(field[2])){
						DrawAnnouncement("Nhap vao khong hop le !");
						continue;
					}
					NodeHK* timkiem=Timkiemhanhkhach(hanhkhachtree,field[0]);
					if(timkiem!=NULL){
						DrawAnnouncement("So CMND da ton tai");
						continue;
					}
					//Luu du lieu
					NodeHK* hanhkhachmoi=new NodeHK;
					strcpy(hanhkhachmoi->hanhkhach.cmnd,field[0]);
					strcpy(hanhkhachmoi->hanhkhach.ho,field[1]);
					strcpy(hanhkhachmoi->hanhkhach.ten,field[2]);
					hanhkhachmoi->hanhkhach.gioitinh=tmpgioitinh;
					
					Themhanhkhachvaocay(hanhkhachtree,hanhkhachmoi);
					DrawAnnouncement("Them hanh khach thanh cong");
					
					
					SetBGColor(0);
					system("cls");
					SetBGColor(maunen);
					DrawRectangle(printpos[0]-1,row-3,20,5);
					Vehopcochu(printpos[0],row,1,sizeof(cmnd)-1,mauchon,maucuachu,cmnd,false);
					Vehopcochu(printpos[0],row-2,1,strlen("Nhap vao cmnd, chua co thi nhan F1"),maucoban,maucuachu,"Nhap vao ma cmnd,chua co thi nhan F1",false);
					
					cocmnd=true;//Bat dau nhap lai so cmnd
					continue;
				}
				if(type==224){//Mui ten len xuon
					type=getch();
					if(type==75){// qua trai
						//Ve lai field cu
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,maucuachu,field[chisonhap],false);
						chisonhap=max(chisonhap-1,0);
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					else if(type==77) //qua phai
					{
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,maucuachu,field[chisonhap],false);
						chisonhap=min(chisonhap+1,3);
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
				}
				else if(chisonhap==3){
					if(type==(int)'1'){
						tmpgioitinh=Nu;
						strcpy(field[3],"Nu");
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					if(type==(int)'0'){
						tmpgioitinh=Nam;
						strcpy(field[3],"Nam");
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
				}
				 else if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(field[chisonhap])-1;
					if(vitrixoa<0) continue;
					
					field[chisonhap][vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[chisonhap]+vitrixoa,row,1,1);
				}
				else{
					//Them chu vao field
					if(width[chisonhap]==strlen(field[chisonhap]))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(field[chisonhap]);
					field[chisonhap][vitrithemchu]=(char)type;
					field[chisonhap][vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
					
				}
		}
	}
	
	//Nhap voo ma chuyen bay
	char machuyenbay[15];
	machuyenbay[0]='\0';
	NodeCB* chuyenbay=NULL;
	
	SetBGColor(0);
	system("cls");
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,20,5);
	Vehopcochu(printpos[0],row,1,sizeof(machuyenbay)-1,mauchon,maucuachu,machuyenbay,false);
	Vehopcochu(printpos[0],row-2,1,strlen("Nhap vao ma chuyen bay"),maucoban,maucuachu,"Nhap vao ma chuyen bay",false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==13){
			chuyenbay=Timkiemchuyenbay(danhsachchuyenbay,machuyenbay);
			if(chuyenbay==NULL){
				DrawAnnouncement("Khong tim thay chuyen bay");
				continue;
			}
			if(chuyenbay->chuyenbay.trangthai==3){
				DrawAnnouncement("Chuyen bay nay da hoan tat");
				continue;
			}
			if(chuyenbay->chuyenbay.trangthai==0){
				DrawAnnouncement("Chuyen bay da bi huy");
				continue;
			}
			//Tim thay chuyen bay
				break;//Thoat vong lap nay bat dau dat ve, huy ve
		}
		else if(type==8){//Backspace xoa 1 chu cua ma chuyen bay
			int vitrixoa=strlen(machuyenbay)-1;
			if(vitrixoa<0) continue;
					
			machuyenbay[vitrixoa]='\0';
					
			//Ve 1 o den len cho vua xoa
			SetBGColor(mauchon);
			DrawRectangle(printpos[0]+vitrixoa,row,1,1);
		}
		else{
			//Them chu vao field
			if(sizeof(machuyenbay)-1==strlen(machuyenbay))  continue;//khong nhap da nhap so luong toi da roi
					
			int vitrithemchu=strlen(machuyenbay);
			machuyenbay[vitrithemchu]=(char)type;
			machuyenbay[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
			Vehopcochu(printpos[0],row,1,sizeof(machuyenbay)-1,mauchon,maucuachu,machuyenbay,false);//Ve lai
					
		}
	}
	
	SetBGColor(0);
	system("cls");
	
	int w=(140-chuyenbay->chuyenbay.danhsachve.sodong-1)/chuyenbay->chuyenbay.danhsachve.sodong;
	int h=(40-chuyenbay->chuyenbay.danhsachve.soday-1)/chuyenbay->chuyenbay.danhsachve.soday;
	
	int startrow=2,startcol=2;//Vi tri bat dau tin
	int chon_hang=0,chon_cot=0;
	//In trang thai cac ve
	for(int i=0;i<chuyenbay->chuyenbay.danhsachve.soday;i++){
		for(int j=0;j<chuyenbay->chuyenbay.danhsachve.sodong;j++){
			if(i==chon_hang&&j==chon_cot){
				NodeHK* hk=Timkiemhanhkhach(hanhkhachtree,(char*)chuyenbay->chuyenbay.danhsachve.cmnd[i][j].c_str());
				Inthongtinhanhkhach(145,10,hk);
				
				SetBGColor(mauchon);
			}
			else SetBGColor(maucoban);
			DrawRectangle(startcol+j*w+j*2,startrow+i*h+i*2,w,h);
			
			if(chuyenbay->chuyenbay.danhsachve.cmnd[i][j]!="")
			Vehopcochu(startcol+j*w+j*2,startrow+i*h+i*2,1,w,14,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[i][j].c_str(),true);
			else Vehopcochu(startcol+j*w+j*2,startrow+i*h+i*2,1,w,2,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[i][j].c_str(),true);
		}
	}
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==0){
			type=getch();
			if(type==59)//F1,dat ve
			{
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]!=""){//Kiem tra ghe do phai trong
					DrawAnnouncement("Cho nay da duoc dat roi!!!");
					continue;
				}
				//
				//Kiem tra tre moi chuyen bay moi hanh khach chi mua duoc 1 ve
				bool f=false;
				for(int i=0;i<chuyenbay->chuyenbay.danhsachve.soday;i++){
					for(int j=0;j<chuyenbay->chuyenbay.danhsachve.sodong;j++){
							if(stricmp((char*)chuyenbay->chuyenbay.danhsachve.cmnd[i][j].c_str(),hanhkhach->hanhkhach.cmnd)==0){
								f=true;
								break;	
							}			
					}
					if(f==true) break;
				}
				if(f==true){
					DrawAnnouncement("Ban da dat ve trong chuyen bay nay roi");
					continue;
				}
				//Ve lai o
	
				
				chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]=hanhkhach->hanhkhach.cmnd;
				chuyenbay->chuyenbay.danhsachve.sovetrong--;//Gian so luong ve trong
				if(chuyenbay->chuyenbay.danhsachve.sovetrong==0){
					chuyenbay->chuyenbay.trangthai=2;//Het ve
				}
				
				SetBGColor(mauchon);
				DrawRectangle(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,w,h);
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]!="")
				Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,14,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				else Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,2,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				NodeHK* hk=Timkiemhanhkhach(hanhkhachtree,(char*)chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot].c_str());
				Inthongtinhanhkhach(145,10,hk);
				
				DrawAnnouncement("Dat ve thanh cong");
				continue;
				
			
			}
			else if(type==60){
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]==""){//Kiem tra ghe trong
					DrawAnnouncement("Khong the huy cho nay!!!");
					continue;
				}
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]!=hanhkhach->hanhkhach.cmnd){//Chi co nguoi dat ve moi duoc huy ve
					DrawAnnouncement("Ban khong phai la nguoi dat cho nay!!!");
					continue;
				}
				//Ve lai o	
				chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]="";//Huy ve
				chuyenbay->chuyenbay.danhsachve.sovetrong++;//Tang so luong ve trong
				
				SetBGColor(mauchon);
				DrawRectangle(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,w,h);
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]!="")
				Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,14,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				else Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,2,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				NodeHK* hk=Timkiemhanhkhach(hanhkhachtree,(char*)chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot].c_str());
				Inthongtinhanhkhach(145,10,hk);
				
				DrawAnnouncement("Huy ve thanh cong");
				continue;
			}
		}
		else if(type==224){//Mui ten len xuon
				type=getch();
				SetBGColor(maucoban);
				DrawRectangle(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,w,h);
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]!="")
				Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,14,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				else Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,2,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				
				
				if(type==75) chon_cot=max(chon_cot-1,0);
				if(type==77) chon_cot=min(chon_cot+1,chuyenbay->chuyenbay.danhsachve.sodong-1);
						
				if(type==72) chon_hang=max(chon_hang-1,0);
				if(type==80) chon_hang=min(chon_hang+1,chuyenbay->chuyenbay.danhsachve.soday-1);
				
				NodeHK* hk=Timkiemhanhkhach(hanhkhachtree,(char*)chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot].c_str());
				Inthongtinhanhkhach(145,10,hk);
				
				SetBGColor(mauchon);
				DrawRectangle(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,w,h);
				if(chuyenbay->chuyenbay.danhsachve.cmnd[chon_hang][chon_cot]!="")
				Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,14,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
				else Vehopcochu(startcol+chon_cot*w+chon_cot*2,startrow+chon_hang*h+chon_hang*2,1,w,2,maucuachu,(char*)chuyenbay->chuyenbay.danhsachve.sove[chon_hang][chon_cot].c_str(),true);
			
			}
	}
}



void Indanhsachhanhkhachcuamotchuyenbay(){//Cai nay khong can cap nhat danh sach chuyen bay
	int col=20;
	int row=10;
	char kiemtra[15];//Nhap vao ma chuyen bay
	kiemtra[0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(col-1,row-3,30,5);
	Vehopcochu(col,row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);
	Vehopcochu(col,row-2,1,strlen("Nhap vao ma chuyen bay"),maucoban,maucuachu,"Nhap vao ma chuyen bay",false);

	
	
    while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==13){
			NodeCB* p=danhsachchuyenbay;
			while(p!=NULL){
				if(stricmp(p->chuyenbay.macb,kiemtra)==0) break;
				p=p->next;
			}
			
			if(p==NULL) {
				DrawAnnouncement("Khong tim thay chuyen bay");
				continue;
			}
			
			//In ra danh sach
			SetBGColor(0);
			system("cls");
			
			TextColor(255);
			SetBGColor(0);
			gotoxy(col+45,row-7);
			cout<<"DANH SACH HANH KHACH THUOC CHUYEN BAY "<<kiemtra;
			gotoxy(col+45,row-5);
			cout<<"Ngay khoi hanh: "<<p->chuyenbay.thoidiemxuatphat.ngay<<'/'<<p->chuyenbay.thoidiemxuatphat.thang<<'/'<<p->chuyenbay.thoidiemxuatphat.nam<<' '<<p->chuyenbay.thoidiemxuatphat.gio<<':'<<p->chuyenbay.thoidiemxuatphat.phut<<"   Noi den:"<<p->chuyenbay.sanbayden;
			int k=1;
			
			
			gotoxy(col,row-3);
			cout<<"|STT";
			gotoxy(col+4,row-3);
			cout<<"|SO VE";
			gotoxy(col+15,row-3);
			cout<<"|CMND";
			gotoxy(col+28,row-3);
			cout<<"|HO TEN";
			gotoxy(col+109,row-3);
			cout<<"|GIOI TINH";
			
			int cnt=120;
			gotoxy(col,row-2);
			while(cnt--) cout<<"_";
			
			for(int i=0;i<p->chuyenbay.danhsachve.soday;i++){
				for(int j=0;j<p->chuyenbay.danhsachve.sodong;j++){
					if(p->chuyenbay.danhsachve.cmnd[i][j]!=""){//Hanh khach da dat ve
						//In ra hanh khach
						NodeHK* hk=Timkiemhanhkhach(hanhkhachtree,(char*)p->chuyenbay.danhsachve.cmnd[i][j].c_str());
						
						gotoxy(col,row+k);
						cout<<k;
						gotoxy(col+4,row+k);
						cout<<p->chuyenbay.danhsachve.sove[i][j];
						gotoxy(col+15,row+k);
						cout<<hk->hanhkhach.cmnd;
						gotoxy(col+28,row+k);
						cout<<hk->hanhkhach.ho<<' '<<hk->hanhkhach.ten;
						gotoxy(col+109,row+k);
						if(hk->hanhkhach.gioitinh==Nam) cout<<"Nam";
						else cout<<"Nu";
						
						++k;
					}
				}
			}
			while(true){
				type=getch();
				if(type==9) return;
			}
			
		}
		else if(type==8){//Backspace xoa 1 chu cua field hien tai
				int vitrixoa=strlen(kiemtra)-1;
				if(vitrixoa<0) continue;
					
				kiemtra[vitrixoa]='\0';
					
				//Ve 1 o den len cho vua xoa
				SetBGColor(mauchon);
				DrawRectangle(col+vitrixoa,row,1,1);
		}
		else{
					//Them chu vao field
				if(sizeof(kiemtra)-1==strlen(kiemtra))  continue;//khong nhap da nhap so luong toi da roi
					
				int vitrithemchu=strlen(kiemtra);
				kiemtra[vitrithemchu]=(char)type;
				kiemtra[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
				Vehopcochu(col,row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);//Ve lai
					
		}
	}
		
}

void Indanhsachmaybaykhoihanhtrongngay(){
	if(!Capnhatdanhsachchuyenbay()) return;
	SetBGColor(0);
	system("cls");
	char field[4][40];//So hieu may bay, loai may bay, so day ,so dong
	char label[4][50]={"DD","MM","YYYY","Diem den"};
	int width[4]={2,2,4,39};
	int printpos[4]={20,23,26,31};
	
	
	int chisonhap=0;
	int row=5;
	
	for(int i=0;i<4;i++) field[i][0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,100,7);
	for(int i=0;i<4;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
	}
	
	
	while(true){
		int type=getch();
		if(type==9){//Phim tab thoat
			return;
		}
		else if(type==13){//Phim Enter
			//Kiem tra
			if(!CheckFullOfNumber(field[0])||!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])){
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			if(!ValidTime(atoi(field[0]),atoi(field[1]),atoi(field[2]))){
				DrawAnnouncement("Ngay thang nhap vao khong hop le");
				continue;
			}
			int k=0;
			//Tim tat ca chuyen bay, in ra so luong ve trong va gio khoi hanh
			SetBGColor(0);
			system("cls");
			
			TextColor(255);
			SetBGColor(0);
			
			gotoxy(printpos[0],row-3);
			cout<<"|MA CHUYEN BAY";
			gotoxy(printpos[0]+15,row-3);
			cout<<"|GIO KHOI HANH";
			gotoxy(printpos[0]+30,row-3);
			cout<<"|SO VE TRONG";
			
			int cnt=45;
			gotoxy(printpos[0],row-2);
			while(cnt--) cout<<'_';
			
			NodeCB* p=danhsachchuyenbay;
			while(p!=NULL){
				if(p->chuyenbay.thoidiemxuatphat.ngay==atoi(field[0])&&p->chuyenbay.thoidiemxuatphat.thang==atoi(field[1])&&p->chuyenbay.thoidiemxuatphat.nam==atoi(field[2])&&stricmp(field[3],p->chuyenbay.sanbayden)==0&&p->chuyenbay.trangthai==1){
					//Chuyen bay dung thoi gian dung noi den, trong trang con ve 
					//In ra
					gotoxy(printpos[0],row+k);
					cout<<p->chuyenbay.macb;
					gotoxy(printpos[0]+15,row+k);
					cout<<p->chuyenbay.thoidiemxuatphat.gio<<':'<<p->chuyenbay.thoidiemxuatphat.phut;
					gotoxy(printpos[0]+30,row+k);
					cout<<p->chuyenbay.danhsachve.sovetrong;
					
					++k;
				}
				
				p=p->next;
			}
			
			while(true){
				type=getch();
				if(type==9) return;
			}
			
			
		}
		else if(type==224){//Bat phim mui ten
			type=getch();
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],maucoban,120,field[chisonhap],false);
				
			if(type==75) chisonhap=max(chisonhap-1,0);
			if(type==77) chisonhap=min(chisonhap+1,3);
				
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,120,field[chisonhap],false);
		}
		else if(type==8){//Xoa mot ki tu cua chuoi dang nhap vao
				int nextindex=max((int)strlen(field[chisonhap])-1,0);
				field[chisonhap][nextindex]='\0';
				SetBGColor(mauchon);
				DrawRectangle(printpos[chisonhap]+nextindex,row,1,1);
				continue;
			}
		else {//Them mot ki tu cua chuoi dang nhap vao
			int nextindex=strlen(field[chisonhap]);
			if(nextindex>=width[chisonhap]) continue;
			
			field[chisonhap][nextindex]=(char)type;
			field[chisonhap][nextindex+1]='\0';
			
			gotoxy(printpos[chisonhap],row);
			TextColor(maucuachu);
			cout<<field[chisonhap];
		}
	}
	
}

void Indanhsachveconcuachuyenbay(){
	int col=10;
	int row=5;
	char kiemtra[15];//Nhap vao ma chuyen bay
	kiemtra[0]='\0';
	
	SetBGColor(maunen);
	DrawRectangle(col-1,row-3,25,5);
	Vehopcochu(col,row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);
	Vehopcochu(col,row-2,1,strlen("Nhap vao ma chuyen bay"),maucoban,maucuachu,"Nhap vao ma chuyen bay",false);

	
	
    while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==13){
			NodeCB* p=danhsachchuyenbay;
			while(p!=NULL){
				if(stricmp(p->chuyenbay.macb,kiemtra)==0) break;
				p=p->next;
			}
			
			if(p==NULL) {
				DrawAnnouncement("Khong tim thay chuyen bay");
				continue;
			}
			if(p->chuyenbay.trangthai!=1){
				DrawAnnouncement("Khong the in ve con cua chuyen bay nay vi co the chuyen bay da het ve, bi huy hoac da hoan thanh");
				continue;
			}
			SetBGColor(0);
			system("cls");
			TextColor(255);
			SetBGColor(0);
			
			gotoxy(35,7);
			cout<<"DANH SACH VE TRONG CUA MAY BAY "<<p->chuyenbay.macb;
			//In het danh sach ve
			for(int i=0;i<p->chuyenbay.danhsachve.soday;i++){
				for(int j=0;j<p->chuyenbay.danhsachve.sodong;j++){
					if(p->chuyenbay.danhsachve.cmnd[i][j]==""){
						gotoxy(20+j*3+j,10+2*i);
						cout<<p->chuyenbay.danhsachve.sove[i][j];
					}
				}
			}
			while(true){
				type=getch();
				if(type==9){
					return;
				}
			}
			
			
			
		}
		else if(type==8){//Backspace xoa 1 chu cua field hien tai
				int vitrixoa=strlen(kiemtra)-1;
				if(vitrixoa<0) continue;
					
				kiemtra[vitrixoa]='\0';
					
				//Ve 1 o den len cho vua xoa
				SetBGColor(mauchon);
				DrawRectangle(col+vitrixoa,row,1,1);
		}
		else{
					//Them chu vao field
				if(sizeof(kiemtra)-1==strlen(kiemtra))  continue;//khong nhap da nhap so luong toi da roi
					
				int vitrithemchu=strlen(kiemtra);
				kiemtra[vitrithemchu]=(char)type;
				kiemtra[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
				Vehopcochu(col,row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);//Ve lai
					
		}
	}
}

const int SOLUONGMENU=7;
const int MENU_BOX_HEIGHT=3;
const int MENU_BOX_WIDTH=50;
char label[SOLUONGMENU][50]={"Quan ly may bay","Quan ly chuyen bay","Dat ve/Huy ve","In danh sach hanh khach cua mot chuyen bay","Danh sach chuyen bay khoi hanh trong ngay","In danh sach chuyen bay con cua chuyen bay","Quit"};
int chonmenu=0;
int row=2,col=110;//Vi tri bat dau in

void Vemenu(){
	SetBGColor(0);
	system("cls");
	for(int i=0;i<SOLUONGMENU;i++){
		if(chonmenu==i){
			Vehopcochu(col,row+i*MENU_BOX_HEIGHT+i*3,MENU_BOX_HEIGHT,MENU_BOX_WIDTH,mauchon,maucuachu,label[i],true);
		}
		else Vehopcochu(col,row+i*MENU_BOX_HEIGHT+i*3,MENU_BOX_HEIGHT,MENU_BOX_WIDTH,maucoban,maucuachu,label[i],true);
	}
}

void Ghimaybay(){
	fstream f;
	f.open("Maybay.txt",ios::out|ios::binary|ios::trunc);
	for(int i=0;i<soluongmaybay;i++){
		f.write(reinterpret_cast<char*>(&(*danhsachmaybay[i])),sizeof(Maybay));
	}
}

void Docmaybay(){
	fstream f;
	f.open("Maybay.txt",ios::in|ios::binary);
	if(!f){
		soluongmaybay=0;
		return;
	}
	Maybay tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(Maybay));
		if(f.eof()) break;
		danhsachmaybay[soluongmaybay]=new Maybay;
		*danhsachmaybay[soluongmaybay]=tmp;
		++soluongmaybay;
	}
}

void Duyetvaghihanhkhach(fstream& f,NodeHK* cayhanhkhach){
	if(cayhanhkhach!=NULL){
		Duyetvaghihanhkhach(f,cayhanhkhach->left);
		f.write(reinterpret_cast<char*>(&(cayhanhkhach->hanhkhach)),sizeof(HanhKhach));//Chi hi hanh khach thoi
		Duyetvaghihanhkhach(f,cayhanhkhach->right);
	}
}
void Ghihanhkhach(){
	fstream f;
	f.open("Hanhkhach.txt",ios::out|ios::binary|ios::trunc);
	Duyetvaghihanhkhach(f,hanhkhachtree);
}
void Dochanhkhach(){
	fstream f;
	f.open("Hanhkhach.txt",ios::in|ios::binary);
	if(!f){
		return;
	}
	HanhKhach tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(HanhKhach));
		if(f.eof()) break;
		else{
			//Toa mot node moi dua vao cay
			NodeHK* hanhkhachmoi=new NodeHK;
			hanhkhachmoi->hanhkhach=tmp;
			Themhanhkhachvaocay(hanhkhachtree,hanhkhachmoi);
		}
		
	}
}

void Ghichuyenbay(){
	fstream f;
	f.open("Chuyenbay.txt",ios::out|ios::trunc);
	NodeCB* cb=danhsachchuyenbay;
	while(cb!=NULL){
		f<<cb->chuyenbay.macb<<endl;
		f<<cb->chuyenbay.sohieumaybay<<endl;
		f<<cb->chuyenbay.sanbayden<<endl;
		f<<cb->chuyenbay.thoidiemxuatphat.ngay<<' '<<cb->chuyenbay.thoidiemxuatphat.thang<<' '<<cb->chuyenbay.thoidiemxuatphat.nam<<' '<<cb->chuyenbay.thoidiemxuatphat.gio<<' '<<cb->chuyenbay.thoidiemxuatphat.phut<<endl;
		f<<cb->chuyenbay.trangthai<<endl;
		f<<cb->chuyenbay.danhsachve.soday<<' '<<cb->chuyenbay.danhsachve.sodong<<endl;
		f<<(cb->chuyenbay.danhsachve.soday*cb->chuyenbay.danhsachve.sodong)-cb->chuyenbay.danhsachve.sovetrong<<endl;
		for(int i=0;i<cb->chuyenbay.danhsachve.soday;i++){
			for(int j=0;j<cb->chuyenbay.danhsachve.sodong;j++){
				if(cb->chuyenbay.danhsachve.cmnd[i][j]!="") f<<i*cb->chuyenbay.danhsachve.sodong+j<<' '<<cb->chuyenbay.danhsachve.cmnd[i][j]<<endl;
			}
		}
		cb=cb->next;
	}
}

void Docchuyenbay(){
	fstream f;
	f.open("Chuyenbay.txt",ios::in);
	if(!f) return;
	while(true){
		NodeCB* cb=new NodeCB;
		f>>cb->chuyenbay.macb;
		if(f.eof()) break;
		f>>cb->chuyenbay.sohieumaybay;
		f>>cb->chuyenbay.sanbayden;
		f>>cb->chuyenbay.thoidiemxuatphat.ngay>>cb->chuyenbay.thoidiemxuatphat.thang>>cb->chuyenbay.thoidiemxuatphat.nam>>cb->chuyenbay.thoidiemxuatphat.gio>>cb->chuyenbay.thoidiemxuatphat.phut;
		f>>cb->chuyenbay.trangthai;
		int soday, sodong;
		f>>soday>>sodong;
		cb->chuyenbay.danhsachve.Khoitao(soday,sodong);
		int sovekhongtrong;
		f>>sovekhongtrong;
		for(int i=0;i<sovekhongtrong;i++){
			int vitri;
			string cmnd;
			f>>vitri>>cmnd;
			cb->chuyenbay.danhsachve.cmnd[vitri/sodong][vitri%sodong]=cmnd;
		}
		cb->chuyenbay.danhsachve.sovetrong-=sovekhongtrong;
		InsertOrder_chuyenbay(danhsachchuyenbay,cb);
		
	}
}


int main(){
	Docmaybay();
	Dochanhkhach();
	Docchuyenbay();
	system("mode 1000");
	Vemenu();
	//Thuc thi
	while(true){
		int type=getch();
		if(type==13){
			if(chonmenu==0){
				Quanlymaybay();//
			}
			else if(chonmenu==1){
				Quanlychuyenbay();
			}
			else if(chonmenu==2){
				Datve_Huyve();
			}
			else if(chonmenu==3){
				Indanhsachhanhkhachcuamotchuyenbay();
			}
			else if(chonmenu==4){
				Indanhsachmaybaykhoihanhtrongngay();
			}
			else if(chonmenu==5){
				Indanhsachveconcuachuyenbay();
			}
			else if(chonmenu==6){
				Ghimaybay();
				Ghichuyenbay();
				Ghihanhkhach();
				break;
			}
			Vemenu();
		}
		if(type==224){
			type=getch();
			if(type==72){
				Vehopcochu(col,row+chonmenu*MENU_BOX_HEIGHT+chonmenu*3,MENU_BOX_HEIGHT,MENU_BOX_WIDTH,maucoban,maucuachu,label[chonmenu],true);
				chonmenu=max(0,chonmenu-1);
				Vehopcochu(col,row+chonmenu*MENU_BOX_HEIGHT+chonmenu*3,MENU_BOX_HEIGHT,MENU_BOX_WIDTH,mauchon,maucuachu,label[chonmenu],true);
			}
			else if(type=80){
				Vehopcochu(col,row+chonmenu*MENU_BOX_HEIGHT+chonmenu*3,MENU_BOX_HEIGHT,MENU_BOX_WIDTH,maucoban,maucuachu,label[chonmenu],true);
				chonmenu=min(SOLUONGMENU-1,chonmenu+1);
				Vehopcochu(col,row+chonmenu*MENU_BOX_HEIGHT+chonmenu*3,MENU_BOX_HEIGHT,MENU_BOX_WIDTH,mauchon,maucuachu,label[chonmenu],true);
			}
		}
	}
}
