#include<iostream>
#include<algorithm>
#include<ctime>
#include<windows.h>
#include<fstream>
#include<conio.h>
#include<cstring>
#include<cstdio>
#include<iomanip>
#include<sstream>

using namespace std;



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

bool CheckIsFloat(char* s){
	if(!CheckEmptyAndSpace(s)) return false;
	bool point=false;
	for(int i=0;s[i]!='\0';i++){
		if(s[i]=='.'){
			if(point||s[i+1]=='\0'||i==0) return false;
			else point=true;
		}
		else if(!isdigit(s[i])) return false;
	}
	return true;
}

void Chuanhoa(char* s){//Chuan hoa chuoi
	int len=strlen(s);
	int i=0,j=len-1;
	while(i<len&&s[i]==' ') ++i;
	while(j>=0&&j>i&&s[j]==' ')--j;
	int cnt=0,index=-1;
	for(int k=i;k<=j;k++){
		if(s[k]==' '){
			if(cnt==0){
				s[++index]=s[k];
			}
			++cnt;
		}
		else{
			cnt=0;
			s[++index]=s[k];
		}
	}
	s[++index]='\0';
	return;
}

int CompareTime(int ad, int am, int ay,int bd,int bm,int by)
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
			else return 0;
		}
	}
}

bool ValidDate(int dd,int mm,int yy){
	 if(yy>=1900 && yy<=9999)
	    {
	        //check month
	        if(mm>=1 && mm<=12)
	        {
	            //check days
	            if((dd>=1 && dd<=31) && (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12));
	            else if((dd>=1 && dd<=30) && (mm==4 || mm==6 || mm==9 || mm==11));
	            else if((dd>=1 && dd<=28) && (mm==2));
	            else if(dd==29 && mm==2 && (yy%400==0 ||(yy%4==0 && yy%100!=0)));
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
	    
	   	time_t t=time(0);
	   	tm* currenttime=localtime(&t);
	   	if(CompareTime(dd,mm,yy,currenttime->tm_mday,currenttime->tm_mon+1,currenttime->tm_year+1900)>0) return false;
	   	//Kiem tra thoi gian nhap la tuong lai
	    return true;
}

void DrawRectangle(int col,int row,int width,int height){
	for(int j=row;j-row<height;j++){
		for(int i=col;i-col<width;i++){
			gotoxy(i,j);
			cout<<" ";
		}
	}
}

void Vehopcochu(int col,int row,int docaocuahop,int dodaicuahop,int maucuahop, int maucuachu,char * text,bool canletrai){
	SetBGColor(maucuahop);
	DrawRectangle(col,row,dodaicuahop,docaocuahop);
	TextColor(maucuachu);
	if(!canletrai) gotoxy(col,row+docaocuahop/2);
	else gotoxy(col+dodaicuahop/2-strlen(text)/2,row+docaocuahop/2);
	cout<<text;
}
void DrawAnnouncement(const char* announce){
	int row=1,col=75;
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



const int MAX_VATTU=100;
const int MAX_NHANVIEN=1000;
const double MAX_TONKHO=1e9;

enum Phai{Nam,Nu};

struct Thoigian{
	int ngay,thang,nam;
};

struct Vattu{
	char MAVT[10];
	char tenvt[30];
	char donvitinh[30];
	double soluongton;
};


struct CTHoadon{
	char MAVT[10];
	double soluong;
	int dongia;
	int vat;
	int trangthai;//1: khach mua,0 khach tra hang
};


struct DSCTHoadon{
	CTHoadon cthd[20];
	int n=0;
};

struct Hoadon{
	char sohoadon[20];
	Thoigian ngaylaphoadon;
	char loaihoadon;
	
	DSCTHoadon dscthd;
};

struct NodeHD{
	Hoadon hoadon;
	NodeHD* next=NULL;
};

struct Nhanvien{
	int MANV;
	char ho[30];
	char ten[30];
	Phai gioitinh;
	//con tro danh sach hoa don
	NodeHD* dshd=NULL;//Danh sach lien ket don
};

struct NodeNV{
	Nhanvien nhanvien;
	NodeNV* left=NULL;
	NodeNV* right=NULL;
};



/////////////////////// Du lieu
NodeNV* nhanvientree=NULL;//Cay nhi phan tim kiems
NodeNV* sapxepnhanvien[MAX_NHANVIEN];
int soluongnhanvien=0;
Vattu* danhsachvattu[MAX_VATTU];
int soluongvattu=0;

Vattu* Timkiemvattu(Vattu* danhsachvattu[],int soluongvattu,char* mavt){
	for(int i=0;i<soluongvattu;i++){
		if(stricmp(danhsachvattu[i]->MAVT,mavt)==0) return danhsachvattu[i];
	}
	return NULL;
}
/////
NodeNV* CreateNode(int manv,char * ho,char* ten,Phai gioitinh){//Tao  nhan vien
	NodeNV* newnode=new NodeNV;
	newnode->nhanvien.MANV=manv;
	strcpy(newnode->nhanvien.ho,ho);
	strcpy(newnode->nhanvien.ten,ten);
	newnode->nhanvien.gioitinh=gioitinh;
	return newnode;
}
/////////////////////////////////////////Thuat toan cua vattu
NodeNV* Timkiemnhanvien(NodeNV* tree,int manv){//Tim kiem nhan vien
	NodeNV* re=tree;
	while(re!=NULL){
		if(manv<re->nhanvien.MANV) re=re->left;
		else if(manv>re->nhanvien.MANV) re=re->right;
		else if(manv==re->nhanvien.MANV) return re;
	}
	return NULL;
}

NodeNV* Chennhanvien(NodeNV* &tree,NodeNV* nhanvienmoi){//Tim kiem doc gia
	if(tree==NULL) {
		tree=nhanvienmoi;
		return nhanvienmoi;
	}
	NodeNV* last=NULL;
	NodeNV* vitrichen=tree;
	while(vitrichen!=NULL){
		last=vitrichen;
		if(nhanvienmoi->nhanvien.MANV<vitrichen->nhanvien.MANV) vitrichen=vitrichen->left;
		else if(nhanvienmoi->nhanvien.MANV>vitrichen->nhanvien.MANV) vitrichen=vitrichen->right;
		else if(nhanvienmoi->nhanvien.MANV==vitrichen->nhanvien.MANV) return NULL;//da co trong cay
	}
	if(nhanvienmoi->nhanvien.MANV<last->nhanvien.MANV) last->left=nhanvienmoi;
	else last->right=nhanvienmoi;
	
	return nhanvienmoi;
}

NodeNV* GetMinNode(NodeNV* root){//Lay nut be trai nhat
		while(root->left!=NULL) root=root->left;
		return root;
}

NodeNV* Xoanhanvien(NodeNV* &root,NodeNV* canxoa){//Ham nay se khong xoa vung nho vung nho du lieu se duoc tra lai
		if(root==NULL) return root;
		
		NodeNV* renode;
		NodeNV* nextnode;
		
		if(canxoa->nhanvien.MANV<root->nhanvien.MANV) {
			nextnode=root->left;
			renode=Xoanhanvien(nextnode,canxoa);
			root->left=nextnode;
		}
		else if(canxoa->nhanvien.MANV>root->nhanvien.MANV) {
			nextnode=root->right;
			renode=Xoanhanvien(nextnode,canxoa);
			root->right=nextnode;
		}
		else{//Truong hop bang
			if(root->right==NULL||root->left==NULL){//nut nay chua mot nut la hoac khong chu nut la nao, truong hop khong can xoay cay
				NodeNV* tmp=root->right?root->right:root->left;
				
				if(tmp==NULL){//Truong hop nay khong can cap nhat lai cay
					NodeNV* tmp=root;
					root=NULL;
					return tmp;
				}
				else{//Do cao giam co the anh huong toi do cao cua nut cha
					*root=*tmp;//xoa root, dua node len cho root
	//				root->left=NULL;
	//				root->right=NULL;
	//				s
					//Xoa vung nho cua tmp
					renode=tmp;
				}
				
			}
			else { //Lay nut cuc trai
				NodeNV* tmp=GetMinNode(root->right);
	//			Node* tmp=GetMinValueNode(root->right);//Nut cuc trai cua cay con ben phai
	//			root->material=tmp->material;//Lay noi dung cu nut cuc trai
				//Sap chep key  va data cua nut cuc trai 
				root->nhanvien=tmp->nhanvien;
//				strcpy(root->key,tmp->key); 
//				root->data=tmp->data;
				
				
				nextnode=root->right;
				renode=Xoanhanvien(nextnode,tmp);//Xoa nut cuc trai;
				root->right=nextnode;
			}
		}
		return renode;
	}

////////////////////////////////////////////

int maucoban=10;
int mauchon=100;
int maucuachu=120;
int maunen=30;

char label[4][50]={"Vat tu","Nhan vien","Quan ly","Thoat"};
int chonmenu=0;
int startrow=10,startcol=20;
void Vemenu(){
	SetBGColor(0);
	system("cls");
	for(int i=0;i<4;i++){
		if(i==chonmenu) Vehopcochu(startcol,startrow+i*3+i*3,3,30,mauchon,maucuachu,label[i],true);
		else Vehopcochu(startcol,startrow+i*3+i*3,3,30,maucoban,maucuachu,label[i],true);
	}
	
}


///////////////////////Vattu
void Sapxepvattutheoten(Vattu* danhsachvattu[],int l,int r){
	//Quicksort
	if(l<=r){
		char key[30];//key la ten vat tu
		strcpy(key,danhsachvattu[(l+r)/2]->tenvt);
		int i=l;
		int j=r;
		while(stricmp(danhsachvattu[i]->tenvt,key)<0) ++i;
		while(stricmp(danhsachvattu[j]->tenvt,key)>0) --j;
		
		if(i<=j){
			swap(danhsachvattu[i],danhsachvattu[j]);
			++i;
			--j;
		}
		if(l<j){
			Sapxepvattutheoten(danhsachvattu,l,j);
		}
		if(r>i){
			Sapxepvattutheoten(danhsachvattu,i,r);
		}
	}
}

void Sapxepnhanvientheohoten(NodeNV* dsnv[],int l,int r){
	if (l <= r)
	{
		// Create a Key/Pivot Element
		string key=string(dsnv[(l+r)/2]->nhanvien.ten)+string(dsnv[(l+r)/2]->nhanvien.ho);
		// Create temp Variables to loop through array
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp((string(dsnv[i]->nhanvien.ten)+string(dsnv[i]->nhanvien.ho)).c_str(),key.c_str())<0)
				i++;
			while (stricmp((string(dsnv[j]->nhanvien.ten)+string(dsnv[j]->nhanvien.ho)).c_str(),key.c_str())>0)
				j--;
 
			if (i <= j)
			{
				swap(dsnv[i],dsnv[j]);
				i++;
				j--;
			}
		}
 
		// Recursion to the smaller partition in the array after sorted above
		// Reference Gi?i Thu?t Ð? Quy :: STDIO.VN
		if (l < j)
			Sapxepnhanvientheohoten(dsnv, l, j);
		if (r > i)
			Sapxepnhanvientheohoten(dsnv, i, r);
	}
	
}

void Indanhsachvattu(){
	SetBGColor(0);
	system("cls");
	Sapxepvattutheoten(danhsachvattu,0,soluongvattu-1);
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	TextColor(255);
	SetBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA VT";
	gotoxy(startcol+10,startrow-3);
	cout<<"|TEN VT";
	gotoxy(startcol+40,startrow-3);
	cout<<"|DON VI TINH";
	gotoxy(startcol+70,startrow-3);
	cout<<"|SO LUONG TON";
	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	
	for(int i=0;i<soluongvattu;i++){
		gotoxy(startcol,startrow+i);
		cout<<danhsachvattu[i]->MAVT;
		gotoxy(startcol+10,startrow+i);
		cout<<danhsachvattu[i]->tenvt;
		gotoxy(startcol+40,startrow+i);
		cout<<danhsachvattu[i]->donvitinh;
		gotoxy(startcol+70,startrow+i);
		cout<<danhsachvattu[i]->soluongton;
	}
}

void Themvattu(){
	char field[4][30];//field[0]: mavt, field[1]: tenvt, field[2]: don vi tinh, field[3]: so luong
	for(int i=0;i<4;i++) field[i][0]='\0';
	int width[4]={9,29,29,9};//kha nang chua ki tu toi da cua field
	int row=15;
	int printpos[4]={30,40,70,100};//Vi tri in
	int chisonhap=0;//Vi tri nhap vao
	char label[4][50]={"Ma va tu","Ten vat tu","Don vi tinh","So luong"};

	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,90,5);
	for(int i=0;i<4;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
	}

	while(true){//Nhap vao 
		int type=getch();
		if(type==9){//Phim tab tro ve menu vattu
			return;
		}
		else if(type==224){//Mui ten len xuon
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
		else if(type==8){//Backspace xoa 1 chu cua field hien tai
			int vitrixoa=strlen(field[chisonhap])-1;
			if(vitrixoa<0) continue;
			
			field[chisonhap][vitrixoa]='\0';
			
			//Ve 1 o den len cho vua xoa
			SetBGColor(mauchon);
			DrawRectangle(printpos[chisonhap]+vitrixoa,row,1,1);
		}
		else if(type==13){
			//Luu du lieu
			if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])||!CheckEmptyAndSpace(field[2])) {
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			if(!CheckIsFloat(field[3])) {
				DrawAnnouncement("Nhap vao so luong ton khong hop le");
				continue;
			}
			
			bool flag=false;
			for(int i=0;i<soluongvattu;i++){
				if(stricmp(danhsachvattu[i]->MAVT,field[0])==0){
					DrawAnnouncement("Ma vat tu da ton tai");
					flag=true;
					break;
				}
			}
			if(flag) continue;
			//Hop le , luu vao mang
			Vattu* vattumoi=new Vattu;
			Chuanhoa(field[0]),Chuanhoa(field[1]),Chuanhoa(field[1]);
			strcpy(vattumoi->MAVT,field[0]);
			strcpy(vattumoi->tenvt,field[1]);
			strcpy(vattumoi->donvitinh,field[2]);
			vattumoi->soluongton=atof(field[3]);
			
			danhsachvattu[soluongvattu++]=vattumoi;
			
			DrawAnnouncement("Luu thanh cong");
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
	//Ve khung nhap
}

void Hieuchinhvattu(){//Khong duoc sua ma vattu
	char kiemtra[10];
	
	int vitrisuadoi=-1;
	kiemtra[0]='\0';
	
	char field[2][30];//field[0]: tenvt, field[1]: don vi tinh
	for(int i=0;i<2;i++) field[i][0]='\0';
	int width[2]={29,29};//kha nang chua ki tu toi da cua field
	int row=15;
	int printpos[2]={20,50};//Vi tri in
	int chisonhap=0;//Vi tri nhap vao
	
	char label[2][50]={"Ten vat tu","Don vi tinh"};


	SetBGColor(maunen);
	DrawRectangle(50-1,row-3,20,5);
	Vehopcochu(50,row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);
	Vehopcochu(50,row-2,1,strlen("Nhap vao ma vat tu"),mauchon,maucuachu,"Nhap vao ma vat tu",false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(vitrisuadoi==-1){//Nhap vao ma vat tu
			if(type==13){
				//Tim kiem vat tu
				bool f=false;
				for(int i=0;i<soluongvattu;i++){
					if(stricmp(kiemtra,danhsachvattu[i]->MAVT)==0){
						vitrisuadoi=i;//Vi tri can sua doi
						
						///Ve khung chinh sua thong tin
						f=true;
						break;
						
					}
				}
				if(!f)DrawAnnouncement("Khong tim thay vat tu");
				else {
					DrawAnnouncement("Tim thay vat tu");
					strcpy(field[0],danhsachvattu[vitrisuadoi]->tenvt);
					strcpy(field[1],danhsachvattu[vitrisuadoi]->donvitinh);
					
					SetBGColor(maunen);
					DrawRectangle(printpos[0]-1,row-3,70,5);
						for(int i=0;i<2;i++){
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
					DrawRectangle(50+vitrixoa,row,1,1);
				}
				else{
					//Them chu vao field
					if(sizeof(kiemtra)-1==strlen(kiemtra))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(kiemtra);
					kiemtra[vitrithemchu]=(char)type;
					kiemtra[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(50,row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);//Ve lai
					
				}
			}
		}
		else{//vattusuadoi!=NULL chinh sua vattu 
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
						chisonhap=min(chisonhap+1,1);
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
				else if(type==13){
					//Luu du lieu
					if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])) {
						DrawAnnouncement("Nhap vao khong hop le");
						continue;
					}
					
					//Hop le , luu vao mang
					Chuanhoa(field[0]),Chuanhoa(field[1]);
					strcpy(danhsachvattu[vitrisuadoi]->tenvt,field[0]);
					strcpy(danhsachvattu[vitrisuadoi]->donvitinh,field[1]);
					
					
					DrawAnnouncement("Luu thanh cong");
					return;
				}
				else if(type==27){
					//Xoa vat tu dich qua trai
					for(int i=vitrisuadoi;i<soluongvattu-1;i++){
						danhsachvattu[i]=danhsachvattu[i+1];
					}
					--soluongvattu;
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

void Quanlyvattu(){//Cac chuc nang trong menu vattu
	//Quan ly vat tu khong co menu chon phu
	
	//Xoa man hinh
	Indanhsachvattu();//In danh sach
	//
	while(true){//Chon chuc nang
		int type=getch();
		if(type==9){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(type==0){//Bat phim F1,F2
			type=getch();
			if(type==59){//Them vat tu, phim F1
				Themvattu();
			}
			else if(type==60){//Hieu chinh vat tu, phim F2
				Hieuchinhvattu();	
			}
			
			//Cap nhat, ve lai danh sach
			Indanhsachvattu();
		}
	}
	
}

////////////////////////
void Indanhsachnhanvien(){
	SetBGColor(0);
	system("cls");
	
	TextColor(255);
	SetBGColor(0);
	int startcol=10,startrow=10;
	
	
	gotoxy(startcol,startrow-3);
	cout<<"|MA NV";
	gotoxy(startcol+10,startrow-3);
	cout<<"|HO";
	gotoxy(startcol+40,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+70,startrow-3);
	cout<<"|GIOI TINH";
	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	
	
	Sapxepnhanvientheohoten(sapxepnhanvien,0,soluongnhanvien-1);
	for(int i=0;i<soluongnhanvien;i++){
		gotoxy(startcol,startrow+i);
		cout<<sapxepnhanvien[i]->nhanvien.MANV;
		gotoxy(startcol+10,startrow+i);
		cout<<sapxepnhanvien[i]->nhanvien.ho;
		gotoxy(startcol+40,startrow+i);
		cout<<sapxepnhanvien[i]->nhanvien.ten;
		gotoxy(startcol+70,startrow+i);
		if(sapxepnhanvien[i]->nhanvien.gioitinh==Nam)
		cout<<"Nam";
		else cout<<"Nu";
	}
}

void Themnhanvien(){
	char field[4][30];//field[0]: manv, field[1]: ho, field[2]: ten, field[3]: gioi tinh
	for(int i=0;i<4;i++) field[i][0]='\0';
	int width[4]={9,29,29,9};//kha nang chua ki tu toi da cua field
	int row=15;
	int printpos[4]={20,30,60,90};//Vi tri in
	int chisonhap=0;//Vi tri nhap vao
	char label[4][50]={"Ma nv","Ho","Ten","Gioi tinh"};
	
	Phai tmpgt=Nam;
	strcpy(field[3],"Nam");
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,95,5);
	for(int i=0;i<4;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),maucoban,maucuachu,label[i],false);
	}

	while(true){//Nhap vao 
		int type=getch();
		if(type==9){//Phim tab tro ve menu vattu
			return;
		}
		else if(type==224){//Mui ten len xuon
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
		else if(type==13){
			//Luu du lieu
			if(!CheckEmptyAndSpace(field[1])||!CheckEmptyAndSpace(field[2])) {
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			if(!CheckFullOfNumber(field[0])){
				DrawAnnouncement("Nhap vao ma nhan vien khong hop le");
				continue;
			}
			
			NodeNV* timkiem=Timkiemnhanvien(nhanvientree,atoi(field[0]));
			if(timkiem!=NULL){
				DrawAnnouncement("Ma nhan vien da ton tai");
				continue;
			}	
			
			NodeNV* newnode=CreateNode(atoi(field[0]),field[1],field[2],tmpgt);
			Chennhanvien(nhanvientree,newnode);
			sapxepnhanvien[soluongnhanvien++]=newnode;
			
			DrawAnnouncement("Luu thanh cong");
			return;
			
		}
		else if(chisonhap==3){
			if(type==(int)'1'){
				tmpgt=Nu;
				strcpy(field[3],"Nu");
				Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
			}
			else if(type==(int)'0'){
				tmpgt=Nam;
				strcpy(field[3],"Nam");
				Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
			}
		}
		else if(type==8){//Backspace xoa 1 chu cua field hien tai,khong phai field 3
			int vitrixoa=strlen(field[chisonhap])-1;
			if(vitrixoa<0) continue;
			
			field[chisonhap][vitrixoa]='\0';
			
			//Ve 1 o den len cho vua xoa
			SetBGColor(mauchon);
			DrawRectangle(printpos[chisonhap]+vitrixoa,row,1,1);
		}
		else{//Khong phai field 3
			//Them chu vao field
			if(width[chisonhap]==strlen(field[chisonhap]))  continue;//khong nhap da nhap so luong toi da roi
			
			int vitrithemchu=strlen(field[chisonhap]);
			field[chisonhap][vitrithemchu]=(char)type;
			field[chisonhap][vitrithemchu+1]='\0';//Phai co ky tu ket thuc
			Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
			
		}
		
	}
}

void Hieuchinhnhanvien(){
	char kiemtra[10];
	kiemtra[0]='\0';
	NodeNV* nhanviencansua=NULL;
	
	char field[3][30];//field[0]: ho, field[1]: ten, field[2]: gioit tinh
	for(int i=0;i<3;i++) field[i][0]='\0';
	int width[3]={29,29,9};//kha nang chua ki tu toi da cua field
	int row=15;
	int printpos[3]={10,40,70};//Vi tri in
	int chisonhap=0;//Vi tri nhap vao
	
	char label[4][50]={"Ho","Ten","Gioi tinh"};
	
	Phai tmpgt;
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,30,5);
	Vehopcochu(printpos[0],row,1,sizeof(kiemtra)-1,mauchon,maucuachu,kiemtra,false);
	Vehopcochu(printpos[0],row-2,1,strlen("Nhap ma nv"),mauchon,maucuachu,"Nhap ma nv",false);//Ve lai
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(nhanviencansua==NULL){//Nhap vao ma nhan vien
			if(type==13){
				if(!CheckFullOfNumber(kiemtra)) {
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				//Tim kiem nha vien
				nhanviencansua=Timkiemnhanvien(nhanvientree,atoi(kiemtra));
				if(nhanviencansua==NULL)DrawAnnouncement("Khong tim nhan vien");
				else {
					DrawAnnouncement("Tim thay nhan vien");
					strcpy(field[0],nhanviencansua->nhanvien.ho);
					strcpy(field[1],nhanviencansua->nhanvien.ten);
					if(nhanviencansua->nhanvien.gioitinh==Nam) strcpy(field[2],"Nam"),tmpgt=Nam;
					else strcpy(field[2],"Nu"),tmpgt=Nu;
					
					SetBGColor(maunen);
					DrawRectangle(printpos[0]-1,row-3,73,5);
						for(int i=0;i<3;i++){
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
		else{//nhanviensuadoi!=NULL
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
						chisonhap=min(chisonhap+1,2);
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
				}
				else if(type==13){
					if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])) continue;
					strcpy(nhanviencansua->nhanvien.ho,field[0]);
					strcpy(nhanviencansua->nhanvien.ten,field[1]);
					nhanviencansua->nhanvien.gioitinh=tmpgt;
		
					DrawAnnouncement("Luu thanh cong");
					return;
				}
				else if(type==27){
					//Xoa nhan vien
					if(nhanviencansua->nhanvien.dshd!=NULL){
						DrawAnnouncement("Khong the xoa nhan vien nay");
						continue;
					}
					NodeNV* vitrixoa=Xoanhanvien(nhanvientree,nhanviencansua);
			
			
					//dich qua trai
					int i=soluongnhanvien-1;
					NodeNV* last=NULL;
					while(sapxepnhanvien[i]!=vitrixoa){
						NodeNV* tmp=sapxepnhanvien[i];
						sapxepnhanvien[i]=last;
						last=tmp;
						--i;
					}
					sapxepnhanvien[i]=last;
					--soluongnhanvien;
					delete vitrixoa;
					
				
					DrawAnnouncement("Xoa thanh cong");
					return;
				}
				else if(chisonhap==2){
					if(type==(int)'1'){
						tmpgt=Nu;
						strcpy(field[2],"Nu");
						Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);
					}
					else if(type==(int)'0'){
						tmpgt=Nam;
						strcpy(field[2],"Nam");
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
void Quanlynhanvien(){
	Indanhsachnhanvien();
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==0){
			type=getch();
			if(type==59){
				Themnhanvien();
			}
			else if(type==60){
				Hieuchinhnhanvien();
			}
			Indanhsachnhanvien();
		}
	}
}

void Inchitiethoadon(DSCTHoadon hoadon){
	int row=10,col=60;//Vi tri bat dau in nhung hoa don da lam
	TextColor(255);
	SetBGColor(0);
	DrawRectangle(col,row,70,20);
	
	
	gotoxy(col,row-3);
	cout<<"|MA VT";
	gotoxy(col+10,row-3);
	cout<<"|TEN VT";
	gotoxy(col+40,row-3);
	cout<<"|SO LUONG";
	gotoxy(col+60,row-3);
	cout<<"|DON GIA";
	gotoxy(col+70,row-3);
	cout<<"|VAT";
	gotoxy(col+73,row-3);
	cout<<"|TRANG THAI";
	
	int cnt=90;
	gotoxy(col,row-2);
	while(cnt--) cout<<'_';
	
	for(int i=0;i<hoadon.n;i++){
		Vattu* vt=Timkiemvattu(danhsachvattu,soluongvattu,hoadon.cthd[i].MAVT);
		gotoxy(col,row+i);
		cout<<hoadon.cthd[i].MAVT;
		gotoxy(col+10,row+i);
		if(vt==NULL) cout<<"Vat tu  da bi xoa";
		else cout<<vt->tenvt;
		gotoxy(col+40,row+i);
		cout<<fixed<<setprecision(3)<<hoadon.cthd[i].soluong;
		gotoxy(col+60,row+i);
		cout<<hoadon.cthd[i].dongia;
		gotoxy(col+70,row+i);
		cout<<hoadon.cthd[i].vat;
		gotoxy(col+73,row+i);
		if(hoadon.cthd[i].trangthai==0) cout<<"Khach da tra hang";
		else cout<<"Khach da mua";
	}
}

void Lamhoadon(){
	SetBGColor(0);
	system("cls");
	
	int row=10;
	
	char field[6][30];//Ma hoa don, ma nhan vien, ngay, thang ,name, loai hoa don
	for(int i=0;i<6;i++) field[i][0]='\0';
	int width[6]={19,9,2,2,4,15};//kha nang chua ki tu toi da cua field
	int chisonhap=0;//Vi tri nhap vao
	char label[6][50]={"So hoa don","Ma nv","dd","mm","yyyy","Loai hoa don"};
	int printpos[6]={10,30,40,43,46,51};
	
	
	bool hoadonnhap=true;//Co phai hoa don nhap hay k
	strcpy(field[5],"Hoa don nhap");
	//Nhap vao so hoa don, ma nhan vien, ngay thang nam truoc
	
	NodeNV* nhanvienthanhtoan=NULL;
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,70,5);
	for(int i=0;i<6;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),mauchon,maucuachu,label[i],false);
	}
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==13){
				if(!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])||!CheckFullOfNumber(field[4])||!CheckEmptyAndSpace(field[0])){
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				if(!ValidDate(atoi(field[2]),atoi(field[3]),atoi(field[4]))){
					DrawAnnouncement("Ngay thang nhap vao khong hop le");
					continue;
				}
				//Kiem tra ma hoa don da co chua
				bool f=false;
				for(int i=0;i<soluongnhanvien;i++){
					NodeHD* first=sapxepnhanvien[i]->nhanvien.dshd;
					while(first!=NULL){
						if(stricmp(first->hoadon.sohoadon,field[0])==0){
							f=true;
							break;
						}
						first=first->next;
					}
					if(f){
						break;
					}
				}
				if(f){
					DrawAnnouncement("Ma hoa don da ton tai");
					continue;//Khong tim thay hoa quay lai nhap
				}
				
				nhanvienthanhtoan=Timkiemnhanvien(nhanvientree,atoi(field[1]));//Tim kiem ma nhan vien
				if(nhanvienthanhtoan==NULL){//Khong tim thay ma nhan vien
					DrawAnnouncement("Khong tim thay nhan vien");
					continue;//Quay lai nhap tiep
				}
				else break;///Thoat vong lap chuyen qua them hoa don
		}
		else if(type==224){
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
		else if(chisonhap==5){
			if(type==(int)'x'){//Chuyen thanh hoa don xuat
				strcpy(field[5],"Hoa don xuat");
				hoadonnhap=false;
				Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
			}
			else if(type==(int)'n'){//Chuyen thanh hoa don nhap
				strcpy(field[5],"Hoa don nhap");
				hoadonnhap=true;
				Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
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
	
	SetBGColor(0);
	system("cls");
	//
	//Them chi tiet hoa don an F1 de thanh toan hoa don
	char field_2[4][30];//Ma vat tu, soluong, dong gia, vat
	for(int i=0;i<4;i++) field_2[i][0]='\0';
	int width_2[4]={9,9,9,2};//kha nang chua ki tu toi da cua field
	chisonhap=0;//Vi tri nhap vao
	char label_2[4][50]={"Ma vat tu","So luong","Don gia","Vat"};
	int printpos_2[4]={10,20,30,40};
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,40,5);
	for(int i=0;i<4;i++){
		if(chisonhap==i) Vehopcochu(printpos_2[i],row,1,width_2[i],mauchon,maucuachu,field_2[i],false);
		else Vehopcochu(printpos_2[i],row,1,width_2[i],maucoban,maucuachu,field_2[i],false);
		Vehopcochu(printpos_2[i],row-2,1,strlen(label_2[i]),mauchon,maucuachu,label_2[i],false);
	}
	
	DSCTHoadon hoadon;
	Vattu* hieuchinh[20];
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==224){
			type=getch();
					if(type==75){// qua trai
						//Ve lai field_2 cu
						Vehopcochu(printpos_2[chisonhap],row,1,width_2[chisonhap],maucoban,maucuachu,field_2[chisonhap],false);
						chisonhap=max(chisonhap-1,0);
						Vehopcochu(printpos_2[chisonhap],row,1,width_2[chisonhap],mauchon,maucuachu,field_2[chisonhap],false);
					}
					else if(type==77) //qua phai
					{
						Vehopcochu(printpos_2[chisonhap],row,1,width_2[chisonhap],maucoban,maucuachu,field_2[chisonhap],false);
						chisonhap=min(chisonhap+1,3);
						Vehopcochu(printpos_2[chisonhap],row,1,width_2[chisonhap],mauchon,maucuachu,field_2[chisonhap],false);
					}
		
		}
		else if(type==13){
				//Them thanh toan
				if(!CheckEmptyAndSpace(field_2[0])){
					DrawAnnouncement("Nhap vao ma vat tu khong hop le");
					continue;
				}
				if(!CheckIsFloat(field_2[1])||!CheckFullOfNumber(field_2[2])||!CheckFullOfNumber(field_2[3])){
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				if(hoadon.n==20){
					DrawAnnouncement("So hoa don da dat toi da");
					continue;
				}
				Vattu* timkiem=Timkiemvattu(danhsachvattu,soluongvattu,field_2[0]);
				if(timkiem==NULL){
					DrawAnnouncement("Khong tim thay vat tu nay");
					continue;
				}
				bool f=false;
				for(int i=0;i<hoadon.n;i++){
					if(stricmp(field_2[0],hoadon.cthd[i].MAVT)==0){
						DrawAnnouncement("Vat tu da co trong hoa don");
						f=true;
						break;
					}	
				}
				if(f) continue;//Vat tu da co trong hoa don
				hieuchinh[hoadon.n]=timkiem;//Con tro tro toi vat tu vua moi nhap vao
				
				///Kiem tra doi voi hoa don nhap
				if(hoadonnhap){
					if((double)hieuchinh[hoadon.n]->soluongton+atoi(field_2[1])>MAX_TONKHO){//So luong nhap vao lon hon sua chua cua kho
						DrawAnnouncement("So luong nhan vao vuot qua suc chua cua kho");
						continue;
					}
				}
				
				///Kiem tra doi voi hoa don xuat
				if(!hoadonnhap){
					if(atoi(field_2[1])>hieuchinh[hoadon.n]->soluongton){
						char a[100]="So luong ton kho cua vat tu nay la: ";
						stringstream f;
						f<<hieuchinh[hoadon.n]->soluongton;
						char tmp[10];
						f>>tmp;
						strcat(a,tmp);
						DrawAnnouncement(a);
						continue;
					}
				}
				
				
				//Luu du lieu thanh toan
				strcpy(hoadon.cthd[hoadon.n].MAVT,field_2[0]);
				hoadon.cthd[hoadon.n].soluong=atoi(field_2[1]);
				hoadon.cthd[hoadon.n].dongia=atoi(field_2[2]);
				hoadon.cthd[hoadon.n].vat=atoi(field_2[3]);
				hoadon.cthd[hoadon.n].trangthai=1;//Danh dau la da mua
				
				
				hoadon.n++;
				
				Inchitiethoadon(hoadon);
				
				DrawAnnouncement("Them hoa don thanh cong");
		}
		else if(type==0){
			type=getch();
			if(type==59){
				//Thanh toan hoa don
				if(hoadonnhap){//Hoa don nhap
					for(int i=0;i<hoadon.n;i++){
						hieuchinh[i]->soluongton+=hoadon.cthd[i].soluong;
						hoadon.cthd[i].trangthai=1;
						
					}
				}
				else{//Hoa don xuat
					for(int i=0;i<hoadon.n;i++){
						hieuchinh[i]->soluongton-=hoadon.cthd[i].soluong;
						hoadon.cthd[i].trangthai=1;
					}
				}

				//Luu du lieu vao danh sach cua nhan vien
				NodeHD* hoadonmoi=new NodeHD;
				if(hoadonnhap)
					hoadonmoi->hoadon.loaihoadon='N';
				else
					hoadonmoi->hoadon.loaihoadon='X';
				strcpy(hoadonmoi->hoadon.sohoadon,field[0]);
				hoadonmoi->hoadon.ngaylaphoadon.ngay=atoi(field[2]);
				hoadonmoi->hoadon.ngaylaphoadon.thang=atoi(field[3]);
				hoadonmoi->hoadon.ngaylaphoadon.nam=atoi(field[4]);
				hoadonmoi->hoadon.dscthd=hoadon;
				
				hoadonmoi->next=nhanvienthanhtoan->nhanvien.dshd;
				nhanvienthanhtoan->nhanvien.dshd=hoadonmoi;
				
				DrawAnnouncement("Thanh toan thanh cong");
				return;
			}
		}
		else if(type==8){//Backspace xoa 1 chu cua field_2 hien tai
					int vitrixoa=strlen(field_2[chisonhap])-1;
					if(vitrixoa<0) continue;
					
					field_2[chisonhap][vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos_2[chisonhap]+vitrixoa,row,1,1);
				}
		else{
					//Them chu vao field_2
					if(width_2[chisonhap]==strlen(field_2[chisonhap]))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(field_2[chisonhap]);
					field_2[chisonhap][vitrithemchu]=(char)type;
					field_2[chisonhap][vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos_2[chisonhap],row,1,width_2[chisonhap],mauchon,maucuachu,field_2[chisonhap],false);//Ve lai
					
		}
			
	}
}

void Trahang(){
	SetBGColor(0);
	system("cls");
	char field[4][30];//field[0]: ma vat, field[1]: ngay, field[2]: thang, field[3]: nam
	for(int i=0;i<4;i++) field[i][0]='\0';
	int width[4]={9,2,2,4};//kha nang chua ki tu toi da cua field
	int row=10;
	int printpos[4]={10,20,30,36};//Vi tri in
	int chisonhap=0;//Vi tri nhap vao
	
	char label[4][50]={"Ma vt","Ngay tra","Thang","Nam"};
	
	char sohoadon[20];
	sohoadon[0]='\0';
	NodeHD* hd=NULL;
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,30,5);
	Vehopcochu(printpos[0],row,1,sizeof(sohoadon)-1,maucoban,maucuachu,sohoadon,false);
	Vehopcochu(printpos[0],row-2,1,strlen("Nhap vao ma hoa don"),mauchon,maucuachu,"Nhap vao ma hoa don",false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(hd==NULL){
			if(type==13){
				
				//Tim kiem hoa don
				bool f=false;
					
				for(int i=0;i<soluongnhanvien;i++){
					NodeHD* first=sapxepnhanvien[i]->nhanvien.dshd;
					while(first!=NULL){
						if(stricmp(sohoadon,first->hoadon.sohoadon)==0) {
							hd=first;
							f=true;
							break;
						}
						first=first->next;
					}
					if(f) break;
				}
				
				/////////
				if(!f){ 
					DrawAnnouncement("Khong tim thay hoa don");
					continue;
				}
				else{
					if(hd->hoadon.loaihoadon!='X'){
						hd=NULL;
						DrawAnnouncement("Hoa don nhap vao phai la hoa don xuat");
						continue;
					}
					DrawAnnouncement("Tim thay hoa don");
					SetBGColor(0);
					system("cls");
					Inchitiethoadon(hd->hoadon.dscthd);
					
					SetBGColor(maunen);
					DrawRectangle(printpos[0]-1,row-3,40,5);
					for(int i=0;i<4;i++){
						if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
						else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
						Vehopcochu(printpos[i],row-2,1,strlen(label[i]),mauchon,maucuachu,label[i],false);
					}
				}
			}
			if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(sohoadon)-1;
					if(vitrixoa<0) continue;
					
					sohoadon[vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(printpos[0]+vitrixoa,row,1,1);
			}
			else{
					//Them chu vao field
					if(sizeof(sohoadon)-1==strlen(sohoadon))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(sohoadon);
					sohoadon[vitrithemchu]=(char)type;
					sohoadon[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(printpos[0],row,1,sizeof(sohoadon)-1,mauchon,maucuachu,sohoadon,false);//Ve lai
					
			}
		}
		else{//hd!=NULL
			if(type==13){
				if(!CheckEmptyAndSpace(field[0])||!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])){
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				if(!ValidDate(atoi(field[1]),atoi(field[2]),atoi(field[3]))){
					DrawAnnouncement("Ngay tra nhap vao khong hop le");
					continue;
				}
				if(!CompareTime(atoi(field[1]),atoi(field[2]),atoi(field[3]),hd->hoadon.ngaylaphoadon.ngay,hd->hoadon.ngaylaphoadon.thang,hd->hoadon.ngaylaphoadon.nam)<0){
					DrawAnnouncement("Nhap vao khong hop le");//Kiem tra ngay nhap vao lon hon ngay mua
					continue;
				}
				
				//Kiem tra tra trong vong 3 ngay
				tm ngaytra={0,0,0,atoi(field[1]),atoi(field[2])-1,atoi(field[3])-1900};
				tm ngaymua={0,0,0,hd->hoadon.ngaylaphoadon.ngay,hd->hoadon.ngaylaphoadon.thang-1,hd->hoadon.ngaylaphoadon.nam-1900};
		 		time_t tngaytra={mktime(&ngaytra)};
		 		time_t tngaymua={mktime(&ngaymua)};
		 						
		 		int differ=difftime(tngaytra,tngaymua) / (60 * 60 * 24);
		 		if(differ>3){
		 			DrawAnnouncement("Chi cho tra hang trong vong 3 ngay");
		 			continue;
				 }
				//Tim kiem ma vat tu trong danh sach cthd
				int vitritrongcthd=-1;
				for(int i=0;i<hd->hoadon.dscthd.n;i++){
						if(stricmp(hd->hoadon.dscthd.cthd[i].MAVT,field[0])==0&&hd->hoadon.dscthd.cthd[i].trangthai==1){
							vitritrongcthd=i;
							break;
						}
				}
				if(vitritrongcthd==-1){
					DrawAnnouncement("Vat tu nay khong co trong danh sach hoac da duoc tra roi");
					continue;
				}
				Vattu* vattuduoctra=Timkiemvattu(danhsachvattu,soluongvattu,field[0]);
				if(vattuduoctra==NULL){
					DrawAnnouncement("Vat tu nay da bi xoa");
					continue;
				}
				else{
					///Tra hang
					vattuduoctra->soluongton+=hd->hoadon.dscthd.cthd[vitritrongcthd].soluong;
					hd->hoadon.dscthd.cthd[vitritrongcthd].trangthai=0;//Danh lai la da tra hang
					DrawAnnouncement("Tra hang thanh cong");
					return;
				}
			 }
			else if(type==224){//Mui ten len xuon
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
			else if(type==8){//Backspace xoa 1 chu cua field hien tai,khong phai field 3
				int vitrixoa=strlen(field[chisonhap])-1;
				if(vitrixoa<0) continue;
				
				field[chisonhap][vitrixoa]='\0';
				
				//Ve 1 o den len cho vua xoa
				SetBGColor(mauchon);
				DrawRectangle(printpos[chisonhap]+vitrixoa,row,1,1);
			}
			else{
				if(width[chisonhap]==strlen(field[chisonhap]))  continue;//khong nhap da nhap so luong toi da roi
				
				int vitrithemchu=strlen(field[chisonhap]);
				field[chisonhap][vitrithemchu]=(char)type;
				field[chisonhap][vitrithemchu+1]='\0';//Phai co ky tu ket thuc
				Vehopcochu(printpos[chisonhap],row,1,width[chisonhap],mauchon,maucuachu,field[chisonhap],false);//Ve lai
				
			}
		}
	}
}


void Inhoadon(){
	SetBGColor(0);
	system("cls");
	NodeHD* hoadon=NULL;
	char sohoadon[20];
	int col=40,row=10;
	sohoadon[0]='\0';
	
	
	SetBGColor(maunen);
	DrawRectangle(col-1,row-3,30,5);
	Vehopcochu(col,row,1,sizeof(sohoadon)-1,maucoban,maucuachu,sohoadon,false);
	Vehopcochu(col,row-2,1,strlen("Nhap vao ma hoa don"),mauchon,maucuachu,"Nhap vao ma hoa don",false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
			if(type==13){
				//Tim kiem hoa don
				if(!CheckEmptyAndSpace(sohoadon)){
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				bool f=false;
					
				for(int i=0;i<soluongnhanvien;i++){
					NodeHD* first=sapxepnhanvien[i]->nhanvien.dshd;
					while(first!=NULL){
						if(stricmp(sohoadon,first->hoadon.sohoadon)==0) {
							hoadon=first;
							f=true;
							break;
						}
						first=first->next;
					}
					if(f) break;
				}
				
				/////////
				if(!f){ 
					DrawAnnouncement("Khong tim thay hoa don");
					continue;
				}
				else{
					DrawAnnouncement("Tim thay hoa don");
					SetBGColor(0);
					system("cls");
					TextColor(255);
					SetBGColor(0);
					
					gotoxy(col,row-8);
					if(hoadon->hoadon.loaihoadon=='X') cout<<"--HON DON XUAT--";
					else cout<<"--HOA DON NHAP--";
					gotoxy(col,row-7);
					cout<<"So hoa don: "<<hoadon->hoadon.sohoadon;
					gotoxy(col,row-6);
					cout<<"Ngay lap hoa don: "<<hoadon->hoadon.ngaylaphoadon.ngay<<'/'<<hoadon->hoadon.ngaylaphoadon.thang<<'/'<<hoadon->hoadon.ngaylaphoadon.nam;
					Inchitiethoadon(hoadon->hoadon.dscthd);
					break;
				}
			}
			if(type==8){//Backspace xoa 1 chu cua field hien tai
					int vitrixoa=strlen(sohoadon)-1;
					if(vitrixoa<0) continue;
					
					sohoadon[vitrixoa]='\0';
					
					//Ve 1 o den len cho vua xoa
					SetBGColor(mauchon);
					DrawRectangle(col+vitrixoa,row,1,1);
			}
			else{
					//Them chu vao field
					if(sizeof(sohoadon)-1==strlen(sohoadon))  continue;//khong nhap da nhap so luong toi da roi
					
					int vitrithemchu=strlen(sohoadon);
					sohoadon[vitrithemchu]=(char)type;
					sohoadon[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
					Vehopcochu(col,row,1,sizeof(sohoadon)-1,mauchon,maucuachu,sohoadon,false);//Ve lai
					
		}
	}
	//
	while(true){
		int type=getch();
		if(type==9) return;
	}
}

void Lietkehoadoncuanhanvientrongkhoangthoigian(){
	SetBGColor(0);
	system("cls");
	char field[7][30];//field[0]: ma nhan vien, field[1]: ngay, field[2]: thang, field[3]: nam, den field[4], field[5],field[6] ngay thang nam
	for(int i=0;i<7;i++) field[i][0]='\0';
	int width[7]={9,2,2,4,2,2,4};//kha nang chua ki tu toi da cua field
	int row=10;
	int startcol=40;
	int printpos[7]={10,20,23,26,36,39,42};//Vi tri in
	int chisonhap=0;//Vi tri nhap vao
	char label[7][50]={"Ma NV","dd","mm","yyyy","dd","mm","yyyy"};
	
	
	SetBGColor(maunen);
	DrawRectangle(printpos[0]-1,row-3,60,5);
	for(int i=0;i<7;i++){
		if(chisonhap==i) Vehopcochu(printpos[i],row,1,width[i],mauchon,maucuachu,field[i],false);
		else Vehopcochu(printpos[i],row,1,width[i],maucoban,maucuachu,field[i],false);
		Vehopcochu(printpos[i],row-2,1,strlen(label[i]),mauchon,maucuachu,label[i],false);
	}
	
	NodeNV* nhanvienthanhtoan=NULL;
	NodeHD* hd[100];
	int soluongin=0;
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==13){
				if(!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])||!CheckFullOfNumber(field[4])||!CheckFullOfNumber(field[0])||!CheckFullOfNumber(field[5])||!CheckFullOfNumber(field[6])){
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				if(!ValidDate(atoi(field[1]),atoi(field[2]),atoi(field[3]))||!ValidDate(atoi(field[4]),atoi(field[5]),atoi(field[6]))){
					DrawAnnouncement("Ngay thang nhap vao khong hop le");
					continue;
				}
				if(CompareTime(atoi(field[1]),atoi(field[2]),atoi(field[3]),atoi(field[4]),atoi(field[5]),atoi(field[6]))>0){//Ngay dau tien lon hon ngay ngay sau
					DrawAnnouncement("Ngay thang nhap vao khong hop le");
					continue;
				}

				nhanvienthanhtoan=Timkiemnhanvien(nhanvientree,atoi(field[0]));//Tim kiem ma nhan vien
				if(nhanvienthanhtoan==NULL){//Khong tim thay ma nhan vien
					DrawAnnouncement("Khong tim thay nhan vien");
					continue;//Quay lai nhap tiep
				}
				else {
					DrawAnnouncement("Tim thay nhan vien");
					break;
					
				}
		}
		else if(type==224){
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
						chisonhap=min(chisonhap+1,6);
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
		NodeHD* nodehd=nhanvienthanhtoan->nhanvien.dshd;
			while(nodehd!=NULL){
						if(CompareTime(atoi(field[1]),atoi(field[2]),atoi(field[3]),nodehd->hoadon.ngaylaphoadon.ngay,nodehd->hoadon.ngaylaphoadon.thang,nodehd->hoadon.ngaylaphoadon.nam)<=0&&
						CompareTime(atoi(field[4]),atoi(field[5]),atoi(field[6]),nodehd->hoadon.ngaylaphoadon.ngay,nodehd->hoadon.ngaylaphoadon.thang,nodehd->hoadon.ngaylaphoadon.nam)>=0)
						hd[soluongin++]=nodehd;
						nodehd=nodehd->next;
						
			}
			SetBGColor(0);
			system("cls");
			TextColor(255);
			SetBGColor(0);
			
			gotoxy(startcol+12,row-7);
			cout<<"TU NGAY: "<<field[1]<<'/'<<field[2]<<'/'<<field[3]<<"    DEN NGAY: "<<field[4]<<'/'<<field[5]<<'/'<<field[6];
			
			gotoxy(startcol,row-5);
			cout<<"Ho ten nhan vien lap:"<<nhanvienthanhtoan->nhanvien.ho<<' '<<nhanvienthanhtoan->nhanvien.ten;
			
			gotoxy(startcol,row-3);
			cout<<"|SO HOA DON";
			gotoxy(startcol+20,row-3);
			cout<<"|NGAY LAP HOA DON";
			gotoxy(startcol+30,row-3);
			cout<<"|LOAI HOA DON";
			gotoxy(startcol+50,row-3);
			cout<<"|TRI GIA HOA DON";
			
			int cnt=70;
			gotoxy(startcol,row-2);
			while(cnt--) cout<<'_';
					
					//In chi tiet hoa don
			for(int i=0;i<soluongin;i++){
				gotoxy(startcol,row+i);
				cout<<hd[i]->hoadon.sohoadon;
				gotoxy(startcol+20,row+i);
				cout<<hd[i]->hoadon.ngaylaphoadon.ngay<<"/"<<hd[i]->hoadon.ngaylaphoadon.thang<<"/"<<hd[i]->hoadon.ngaylaphoadon.nam;
				gotoxy(startcol+30,row+i);
				if(hd[i]->hoadon.loaihoadon=='N') cout<<"Hoa don nhap";
				else cout<<"Hoa don xuat";
				gotoxy(startcol+50,row+i);
				double sum=0;
				for(int j=0;j<hd[i]->hoadon.dscthd.n;j++){
					sum+=hd[i]->hoadon.dscthd.cthd[j].soluong*hd[i]->hoadon.dscthd.cthd[j].dongia*(hd[i]->hoadon.dscthd.cthd[j].vat/100.f+1);
				}
			cout<<sum<<endl;
	}
	while(true){
		int type=getch();
		if(type==9) return;
	}
	
}

void Thongketheonam(){
	SetBGColor(0);
	system("cls");
	double doanhthu[13]{0};
	char nam[5];
	nam[0]='\0';
	int col=30;
	int row=10;
	
	SetBGColor(maunen);
	DrawRectangle(col-1,row-3,15,5);
	Vehopcochu(col,row,1,sizeof(nam)-1,maucoban,maucuachu,nam,false);
	Vehopcochu(col,row-2,1,strlen("Nhap vao nam"),mauchon,maucuachu,"Nhap vao nam",false);
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==13){
			if(!CheckFullOfNumber(nam)){
				DrawAnnouncement("Nhap vao khong hop le");
				continue;
			}
			for(int i=0;i<soluongnhanvien;i++){
				NodeHD* hd=sapxepnhanvien[i]->nhanvien.dshd;
				while(hd!=NULL){
					if(hd->hoadon.ngaylaphoadon.nam==atoi(nam)&&hd->hoadon.loaihoadon=='X')//Hao don xuat
					for(int j=0;j<hd->hoadon.dscthd.n;j++){
						doanhthu[hd->hoadon.ngaylaphoadon.thang]+=hd->hoadon.dscthd.cthd[j].soluong*hd->hoadon.dscthd.cthd[j].dongia*(hd->hoadon.dscthd.cthd[j].vat/100.f+1);
					}
					hd=hd->next;
				}
			}
			break;
		}
		else if(type==8){//Backspace xoa 1 chu cua field hien tai
			int vitrixoa=strlen(nam)-1;
			if(vitrixoa<0) continue;
					
			nam[vitrixoa]='\0';
					
			//Ve 1 o den len cho vua xoa
			SetBGColor(mauchon);
			DrawRectangle(col+vitrixoa,row,1,1);
		}
		else{
			if(sizeof(nam)-1==strlen(nam))  continue;//khong nhap da nhap so luong toi da roi
					
			int vitrithemchu=strlen(nam);
			nam[vitrithemchu]=(char)type;
			nam[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
			Vehopcochu(col,row,1,4,mauchon,maucuachu,nam,false);//Ve lai
		}
	}
	//In 
	TextColor(255);
	SetBGColor(0);
	system("cls");
	gotoxy(col+10,row-5);
	cout<<"BANG THONG KE DOANH THU NAM "<<nam;
	gotoxy(col,row-3);
	cout<<"|THANG";
	gotoxy(col+10,row-3);
	cout<<"|DOANH THU";
	gotoxy(col,row-2);
	int cnt=30;
	while(cnt--) cout<<'_';
	for(int i=1;i<=12;i++){
		gotoxy(col,row+i);
		cout<<i;
		gotoxy(col+10,row+i);
		cout<<doanhthu[i];
	}
	while(true){
		int type=getch();
		if(type==9) return;
	}
}

void Quanly(){
	int startcol=60,startrow=5;
	char label[5][50]={"Lam hoa don","Tra hang","In hoa don","Liet ke hoa don cua 1 nhan vien","Bang thong ke doanh thu"};
	int chonmenu=0;
	for(int i=0;i<5;i++){
		if(i==chonmenu) Vehopcochu(startcol,startrow+i*3+i*3,3,30,mauchon,maucuachu,label[i],true);
		else Vehopcochu(startcol,startrow+i*3+i*3,3,30,maucoban,maucuachu,label[i],true);
	}
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		else if(type==13){
			if(chonmenu==0){
				Lamhoadon();
				return;
			}
			else if(chonmenu==1){
				Trahang();
				return;
			}
			else if(chonmenu==2){
				Inhoadon();
				return;
			}
			else if(chonmenu==3){
				Lietkehoadoncuanhanvientrongkhoangthoigian();
				return;	
			}
			else if(chonmenu==4){
				Thongketheonam();
				return;
			}
		}
		else if(type==224){//Bat phim mui ten di chuyen len xuong
			type=getch();
			if(type==72){
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
				chonmenu=max(chonmenu-1,0);
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
			}
			else if(type==80){
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
				chonmenu=min(chonmenu+1,4);
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
			}
		}
		
	}
}

void GhiFileVattu(){
	fstream f;
	f.open("Vattu.txt",ios::out|ios::binary|ios::trunc);
	for(int i=0;i<soluongvattu;i++){
		f.write(reinterpret_cast<char*>(&(*danhsachvattu[i])),sizeof(Vattu));
	}
	f.close();
}
void DocFileVattu(){
	fstream f;
	f.open("Vattu.txt",ios::in|ios::binary);
	if(!f) return;
	Vattu tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(Vattu));
		if(f.eof()) break;
		else{
			danhsachvattu[soluongvattu]=new Vattu;
			*danhsachvattu[soluongvattu]=tmp;
			++soluongvattu;
		}
	}
}
void GhiFileNhanvien(){
	fstream f;
	f.open("Nhanvien.txt",ios::out|ios::trunc);
	f<<soluongnhanvien<<endl;
	for(int i=0;i<soluongnhanvien;i++){
		f<<sapxepnhanvien[i]->nhanvien.MANV<<endl<<sapxepnhanvien[i]->nhanvien.ho<<endl<<sapxepnhanvien[i]->nhanvien.ten<<endl;
		if(sapxepnhanvien[i]->nhanvien.gioitinh==Nam) f<<'B'<<endl;
		else f<<'G'<<endl;
		
		NodeHD* dshd=sapxepnhanvien[i]->nhanvien.dshd;
		while(dshd!=NULL){
			f<<dshd->hoadon.sohoadon<<' '<<dshd->hoadon.dscthd.n<<' '<<dshd->hoadon.loaihoadon<<' '<<dshd->hoadon.ngaylaphoadon.ngay<<' '<<dshd->hoadon.ngaylaphoadon.thang<<' '<<dshd->hoadon.ngaylaphoadon.nam<<endl;
			for(int j=0;j<dshd->hoadon.dscthd.n;j++){
				f<<dshd->hoadon.dscthd.cthd[j].MAVT<<' '<<dshd->hoadon.dscthd.cthd[j].soluong<<' '<<dshd->hoadon.dscthd.cthd[j].dongia<<' '<<dshd->hoadon.dscthd.cthd[j].vat<<' '<<dshd->hoadon.dscthd.cthd[j].trangthai<<endl; 
			}	
			dshd=dshd->next;
		}
		f<<"/"<<endl;
	}
}

void DocFileNhanvien(){
	fstream f;
	f.open("Nhanvien.txt",ios::in);
	if(!f) return;
	f>>soluongnhanvien;
	for(int i=0;i<soluongnhanvien;i++){
		sapxepnhanvien[i]=new NodeNV;
		f>>sapxepnhanvien[i]->nhanvien.MANV;
		f.ignore();
		f.getline(sapxepnhanvien[i]->nhanvien.ho,256);
		f.getline(sapxepnhanvien[i]->nhanvien.ten,256);
		char gt;
		f>>gt;
		if(gt=='B')sapxepnhanvien[i]->nhanvien.gioitinh=Nam;
		else sapxepnhanvien[i]->nhanvien.gioitinh=Nu;
		
		
		char sohoadon[20];
		f>>sohoadon;
		while(stricmp(sohoadon,"/")!=0){
			NodeHD * hd=new NodeHD;
			f>>hd->hoadon.dscthd.n>>hd->hoadon.loaihoadon>>hd->hoadon.ngaylaphoadon.ngay>>hd->hoadon.ngaylaphoadon.thang>>hd->hoadon.ngaylaphoadon.nam;
			strcpy(hd->hoadon.sohoadon,sohoadon);
			for(int j=0;j<hd->hoadon.dscthd.n;j++){
				f>>hd->hoadon.dscthd.cthd[j].MAVT>>hd->hoadon.dscthd.cthd[j].soluong>>hd->hoadon.dscthd.cthd[j].dongia>>hd->hoadon.dscthd.cthd[j].vat>>hd->hoadon.dscthd.cthd[j].trangthai; 	
			}	
			hd->next=sapxepnhanvien[i]->nhanvien.dshd;
			sapxepnhanvien[i]->nhanvien.dshd=hd;
			f>>sohoadon;
		}
		
		Chennhanvien(nhanvientree,sapxepnhanvien[i]);
	}
}
int main(){
	DocFileVattu();
	DocFileNhanvien();
	system("mode 1000");
	Vemenu();
	while(true){
		int type=getch();
		if(type==13){//Phim enter thuc hien chuc nang
			if(chonmenu==0){
				Quanlyvattu();
				Vemenu();//Sau khi thuc hien cai gi do thi tro ve menu chinh
			}
			if(chonmenu==1){
				Quanlynhanvien();
				Vemenu();
			}
			if(chonmenu==2){
				Quanly();
				Vemenu();
			}
			if(chonmenu==3){
				GhiFileVattu();
				GhiFileNhanvien();
				break;
			}
		}
		if(type==224){//Bat phim mui ten di chuyen len xuong
			type=getch();
			if(type==72){
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
				chonmenu=max(chonmenu-1,0);
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
			}
			else if(type==80){
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
				chonmenu=min(chonmenu+1,3);
				Vehopcochu(startcol,startrow+chonmenu*3+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
			}
		}
	}
}


