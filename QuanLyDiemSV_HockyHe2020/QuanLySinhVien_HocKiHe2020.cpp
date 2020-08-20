#include<iostream>
#include<cstring>
#include<thread>
#include <Windows.h>
#include <cstdlib>
#include<conio.h>
#include <unistd.h>
#include<cctype>
#include <ctime> 
#include<cmath>
#include<iomanip>
#include<string>
#include<fstream>

#define GREY 120
#define RED 100
#define YELLOW 30
#define BLACK 0
#define WHITE 15
#define GREEN 10
#define PURPLE 5
#define A 65
#define B 66
#define C 67
#define D 68
#define UP 72
#define DOWN 80
#define DOUBLE_KEY 224
#define BACK_SPACE 8
#define ENTER 13
#define TAB 9
#define F1 59
#define F2 60
#define F3 61
#define F4 62
#define LEFT 75
#define RIGHT 77
#define ESC 27

using namespace std;

const int MAX_MH=300;

struct MonHoc{
	char  maMH[10];
	char tenMH[60];
	float soTCLT,soTCTH;
};

struct SinhVien{
	char maSV[12];
	char ho[50];
	char ten[50];
	char phai[4];
	char sdt[12];
	
	char maLop[15];
	int namNhapHoc;
};


struct DangKi{
	char maSV[12];
	float diem;
};

struct NodeSV{
	SinhVien sinhVien;
	NodeSV* next=NULL;
};

struct NodeDK{
	DangKi dangKi;
	NodeDK* next=NULL;
};


struct LopTinChi{
	int maLopTC;
	char maMH[10];
	char nienKhoa[10];
	unsigned int hocKi;
	unsigned int nhom;
	unsigned int soSVMAX, soSVMIN;
	
	NodeDK* dsDK=NULL;
};

struct NodeLTC{
	LopTinChi lopTinChi;
	NodeLTC* left=NULL;
	NodeLTC* right=NULL;
};


struct DuLieu{
	MonHoc* dsMH[MAX_MH];//Mang con tro
	unsigned int soLuongMH=0;
	
	NodeLTC* dsLTC=NULL;//Cay nhi phan tim kiem
	
	NodeSV* dsSV=NULL;//Danh sach lien ket don
	
	
};

/*------------------------------------------------------------------------DO HOA-----------------------------------------------------------------*/
void gotoxy (int column, int line){	
	COORD coord; 	
	coord.X = column;  	
	coord.Y = line; 	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void textColor (int color){	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}

void setBGColor(int color){
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

void drawRectangle(int col,int row,int width,int height){
	for(int j=row;j-row<height;j++){
		for(int i=col;i-col<width;i++){
			gotoxy(i,j);
			cout<<" ";
		}
	}
}

void drawTextBox(int col,int row,int docaocuahop,int dodaicuahop,int maucuahop, int maucuachu,char * text,bool canletrai){
	setBGColor(maucuahop);
	drawRectangle(col,row,dodaicuahop,docaocuahop);
	textColor(maucuachu);
	if(!canletrai) gotoxy(col,row+docaocuahop/2);
	else gotoxy(col+dodaicuahop/2-strlen(text)/2,row+docaocuahop/2);
	cout<<text;
}



void drawAlert(const char* announce){
	int row=1,col=75;
	textColor(114);
	gotoxy(col,row);
	cout<<announce;
	double playtime=1.f;
	double starttime=(double)clock()/CLOCKS_PER_SEC;
	while(true){
		if((double)clock()/CLOCKS_PER_SEC-starttime>=playtime) break;
	}
	setBGColor(0);
	drawRectangle(col,row,strlen(announce),1);
}

bool isEmpty(char* s){

	if(strlen(s)==0) return true;
	return false;
}

bool isSpace(char* s){
	if(strlen(s)==0) return true;
	for(int i=0;s[i]!='\0';i++){
		if(s[i]==' ') return true;
	}
	return false;
}

bool isFloat(char* s){
	if(isEmpty(s)) return false;
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


bool isInteger(char* s){
	if(isEmpty(s)) return false;
	//Chuoi chi bao gom so thi tra ve true, false truong hop con lai
	for(int i=0;s[i]!='\0';i++){
		if(!isdigit(s[i])) return false;
	}
	return true;
}


struct TextField{
	TextField(int input_row, int input_col, int max_char=20,char *input_label=""){
		row=input_row;
		col=input_col;
		maxChar=max_char;
		label=input_label;
		text=new char[maxChar];
		strcpy(text,"\0");
		
		draw();
	}
	int row,col,maxChar;
	char* text,*label;
	char* getText(){
		return text;
	}
	void setText(char* s){
		strcpy(text,s);
		text[maxChar-1]='\0';
		
		draw();
	}
	void update(int input_type){
		if(input_type==BACK_SPACE){
			int vitrixoa=strlen(text)-1;
			if(vitrixoa<0) return;
			text[vitrixoa]='\0';
					
			//Ve 1 o den len cho vua xoa
			setBGColor(PURPLE);
			drawRectangle(col+vitrixoa,row,1,1);
		}
		else if(isprint(input_type)){
			if(maxChar-1==strlen(text))  return;//khong nhap da nhap so luong toi da roi
					
			int vitrithemchu=strlen(text);
			text[vitrithemchu]=(char)input_type;
			text[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
			draw();
		}
	}
	void draw(){
		drawTextBox(col-strlen(label),row,1,strlen(label),BLACK,WHITE,label,false);
		drawTextBox(col,row,1,maxChar-1,PURPLE,WHITE,text,false);
	}
	
};



/*------------------------------------------------------------------------THUAT TOAN----------------------------------------------------------------*/
NodeLTC* getMinNode(NodeLTC* root){//Lay nut be trai nhat
		if(root==NULL) return NULL;
		while(root->left!=NULL) root=root->left;
		return root;
}

NodeLTC* getMaxNode(NodeLTC* root){//Lay nut ben phai nhat
		if(root==NULL) return NULL;
		while(root->right!=NULL) root=root->right;
		return root;
}

NodeLTC* create_LTC(int ma_LTC,char ma_MH[10], char nien_khoa[10], unsigned int hoc_ki, unsigned int nhom, unsigned int so_sv_min, unsigned int so_sv_max){//Tao node LTC
	NodeLTC* new_node=new NodeLTC;
	new_node->lopTinChi.maLopTC=ma_LTC;
	strcpy(new_node->lopTinChi.maMH,ma_MH);
	strcpy(new_node->lopTinChi.nienKhoa,nien_khoa);
	new_node->lopTinChi.hocKi=hoc_ki;
	new_node->lopTinChi.nhom=nhom;
	new_node->lopTinChi.soSVMAX=so_sv_max;
	new_node->lopTinChi.soSVMIN=so_sv_min;
	new_node->left=NULL;
	new_node->right=NULL;
	
	return new_node;
}



NodeLTC* searchValue_LTC(NodeLTC* node, char ma_MH[10], char nien_khoa[10], unsigned int hoc_ki, unsigned int nhom){ 
    if (node == NULL) 
        return NULL; 
   
    if(stricmp(node->lopTinChi.maMH,ma_MH)==0&&stricmp(node->lopTinChi.nienKhoa, nien_khoa)==0&&node->lopTinChi.hocKi==hoc_ki&&node->lopTinChi.nhom==nhom){
    	return node;
	}
	NodeLTC* ret=searchValue_LTC(node->left, ma_MH, nien_khoa, hoc_ki, nhom); 
	if(ret!=NULL) return ret;
	else return searchValue_LTC(node->right, ma_MH, nien_khoa, hoc_ki, nhom); 
} 

NodeLTC* searchMH_LTC(NodeLTC* node, char ma_MH[10]){ 
    if (node == NULL) 
        return NULL; 
   
    if(stricmp(node->lopTinChi.maMH,ma_MH)==0){
    	return node;
	}
	NodeLTC* ret=searchMH_LTC(node->left, ma_MH); 
	if(ret!=NULL) return ret;
	else return searchMH_LTC(node->right, ma_MH); 
} 


NodeLTC* insert_LTC(NodeLTC* & root, NodeLTC* node ){
	if(root==NULL) {
		root=node;
		return node;
	}
	NodeLTC* last=NULL;
	NodeLTC* vitrichen=root;
	while(vitrichen!=NULL){
		last=vitrichen;
		if(node->lopTinChi.maLopTC<vitrichen->lopTinChi.maLopTC) vitrichen=vitrichen->left;
		else if(node->lopTinChi.maLopTC>vitrichen->lopTinChi.maLopTC) vitrichen=vitrichen->right;
		else if(node->lopTinChi.maLopTC==vitrichen->lopTinChi.maLopTC) return NULL;
	}
	if(node->lopTinChi.maLopTC<last->lopTinChi.maLopTC) last->left=node;
	else last->right=node;
	
	return node;
}


NodeLTC* search_LTC(NodeLTC* root, int ma_lop_TC) 
{ 
    if (root == NULL || root->lopTinChi.maLopTC== ma_lop_TC) 
       return root; 
    
    if (root->lopTinChi.maLopTC < ma_lop_TC) 
       return search_LTC(root->right, ma_lop_TC); 

    return search_LTC(root->left, ma_lop_TC); 
} 



NodeLTC* delete_LTC(NodeLTC* &root,NodeLTC* canxoa){//Ham nay se khong xoa vung nho vung nho du lieu se duoc tra lai
		if(root==NULL) return root;
		
		NodeLTC* renode;//Nut tra ve, nut bo ra khoi cai cay
		NodeLTC* nextnode;//Cay 
		
		if(canxoa->lopTinChi.maLopTC<root->lopTinChi.maLopTC) {
			nextnode=root->left;
			renode=delete_LTC(nextnode,canxoa);//
			root->left=nextnode;
		}
		else if(canxoa->lopTinChi.maLopTC>root->lopTinChi.maLopTC) {
			nextnode=root->right;	
			renode=delete_LTC(nextnode,canxoa);
			root->right=nextnode;
		}
		else{//Truong hop bang
			if(root->right==NULL||root->left==NULL){//nut nay chua mot nut la hoac khong chu nut la nao, truong hop khong can xoay cay
				NodeLTC* tmp=root->right?root->right:root->left;
				
				if(tmp==NULL){//Truong hop nay khong can cap nhat lai cay
					NodeLTC* tmp=root;
					root=NULL;
					return tmp;
				}
				else{//Do cao giam co the anh huong toi do cao cua nut cha
					*root=*tmp;//xoa root, dua node len cho root
					renode=tmp;
				}
				
			}
			else { //Lay nut cuc trai
				NodeLTC* tmp=getMinNode(root->right);//Nut cuc trai cua cay con ben phai
				//Sap chep key  va data cua nut cuc trai 
				root->lopTinChi=tmp->lopTinChi;
				
				nextnode=root->right;
				renode=delete_LTC(nextnode,tmp);//Xoa nut cuc trai;
				root->right=nextnode;
			}
		}
		return renode;
}

void delete_MH(MonHoc* dsMH[],unsigned int& sl, int index){
	MonHoc* tmp= dsMH[index];
	for(int i=index;i<sl-1;i++){
		dsMH[i]=dsMH[i+1];
	}
	
	delete tmp;
	--sl;
}

int compare_SV(NodeSV* sv1, NodeSV* sv2){
	if(stricmp(sv1->sinhVien.maLop, sv2->sinhVien.maLop)==0){
		return stricmp(sv1->sinhVien.maSV,sv2->sinhVien.maSV);
	}
	else return stricmp(sv1->sinhVien.maLop, sv2->sinhVien.maLop);
}

void insertOrder_SV(NodeSV* &head_ref, NodeSV* new_node) 
{ 	
    NodeSV* current; 
    /* Special case for the head end */
    if (head_ref == NULL || compare_SV(head_ref,new_node)>=0) { 
        new_node->next = head_ref; 
        head_ref = new_node; 
    } 
    else { 
        /* Locate the node before the 
 point of insertion */
        current = head_ref; 
        while (current->next != NULL  && compare_SV(current->next,new_node)<0) { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
} 


void insertHead_DK(NodeLTC*& ltc, NodeDK* dk){
	dk->next=ltc->lopTinChi.dsDK;
	ltc->lopTinChi.dsDK=dk;
}



/*-------------------------------------------------------------LOP TIN CHI-------------------------------------------*/
void printInorder_LTC(NodeLTC* node,int startrow, int startcol, int& i) 
{ 
    if (node == NULL) 
        return; 
 
    printInorder_LTC(node->left,startrow, startcol,i); 
    
   	gotoxy(startcol,startrow+i);
	cout<<node->lopTinChi.maLopTC;
	gotoxy(startcol+15,startrow+i);
	cout<<node->lopTinChi.maMH;
	gotoxy(startcol+40,startrow+i);
	cout<<node->lopTinChi.nienKhoa;
	gotoxy(startcol+60,startrow+i);
	cout<<node->lopTinChi.hocKi;
	gotoxy(startcol+80,startrow+i);
	cout<<node->lopTinChi.nhom;
	gotoxy(startcol+90,startrow+i);
	cout<<node->lopTinChi.soSVMIN;
	gotoxy(startcol+100,startrow+i);
	cout<<node->lopTinChi.soSVMAX;
 
 	 i++;  
  	printInorder_LTC(node->right,startrow, startcol,i); 
} 
void inDanhSachLTC(NodeLTC* root){
	setBGColor(0);
	system("cls");
	
	drawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them      F2: Hieu chinh     F2->Esc: Xoa      Tab: Thoat        ENTER: Luu",false);
		
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	textColor(WHITE);
	setBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA LOP TC";
	gotoxy(startcol+15,startrow-3);
	cout<<"|MA MH";
	gotoxy(startcol+40,startrow-3);
	cout<<"|NIEN KHOA";
	gotoxy(startcol+60,startrow-3);
	cout<<"|HOC KY";
	gotoxy(startcol+80,startrow-3);
	cout<<"|NHOM";
	gotoxy(startcol+90,startrow-3);
	cout<<"|SV MIN";
	gotoxy(startcol+100,startrow-3);
	cout<<"|SV MAX";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	int i=0;
	printInorder_LTC(root , startrow, startcol,i);
}




void themLTC(NodeLTC* &root,MonHoc* dsMH[], unsigned int& slMH){
	


	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(35,13,50,15);
	TextField fields[6]={TextField(15,60,10,"Ma mon hoc: "), TextField(17,60,10,"Nien khoa : "), TextField(19,60,2,"Hoc ki: ")
	, TextField(21,60,3,"Nhom: "),TextField(23,60,4,"So sv min: "),TextField(25,60,4,"So sv max: ")};

	while(true){//Nhap vao 
		int input_type=getch();
		if(input_type==TAB){//Phim tab tro ve menu quan ly mon hoc
			return;
		}
		else if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+5)%6;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%6;
			}
		}
		else if(input_type==ENTER){
			//Kiem tra du lieu 
			if(isEmpty(fields[1].getText())||!isInteger(fields[2].getText())||!isInteger(fields[3].getText())||!isInteger(fields[4].getText())||!isInteger(fields[5].getText()))
			{
				drawAlert("Nien khoa khong duoc trong hoc ki, nhom, so sinh vien min, max la so nguyen duong");
				continue;
			}
			bool f=false;
			for(int i=0;i<slMH;i++){
				if(stricmp(dsMH[i]->maMH,fields[0].getText())==0){
					f=true;
					break;
				}
			}
			
			if(!f){
				drawAlert("Khong tim thay ma mon hoc");
				continue;
			}
			if(atoi(fields[4].getText())>atoi(fields[5].getText())){
				drawAlert("So luong sinh vien min <= sinh vien max");
				continue;
			}
			if(searchValue_LTC(root,fields[0].getText(),fields[1].getText(),atoi(fields[2].getText()),atoi(fields[3].getText()))!=NULL){
				drawAlert("Da co lop tin chi cho du lieu nay roi");
				continue;
			}
			
			while(true){
				int ma_LTC=rand()%1000000;
				NodeLTC* new_node=create_LTC(ma_LTC,fields[0].getText(),fields[1].getText(),atoi(fields[2].getText()),atoi(fields[3].getText()),atoi(fields[4].getText()),atoi(fields[5].getText()));
				if(insert_LTC(root, new_node)!=NULL) break;
			}
			drawAlert("Them lop tin chi thanh cong");
			return;
			
		}
		else fields[choosen].update(input_type);
	
		
	}
}


void hieuChinhLTC(NodeLTC* &root,MonHoc* dsMH[], unsigned int& slMH){//Khong duoc sua ma lop


	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(48,13,40,5);
	

	NodeLTC* ltc=NULL;
	TextField input_ltc = TextField(15,75,10,"Nhap vao ma lop tin chi: ");	
	
	
	while(true){//Vong lap nhap vao ma lop tc
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		else if(input_type==ENTER){
				//Tim kiem ma lop xem co ton tai  lop tin chi
				if(!isInteger(input_ltc.getText())){
					drawAlert("Ma lop tin chi nhap vao la so nguyen duong");
					continue;
				}
				int id=atoi(input_ltc.getText());
				ltc=search_LTC(root,id);
				if(ltc==NULL){
					drawAlert("Khong tim thay lop tin chi");
					continue;
				}
				else break;
				
			}
		else input_ltc.update(input_type);
	
	}
		setBGColor(YELLOW);
	drawRectangle(45,13,50,15);
	TextField fields[6]={TextField(15,75,10,"Ma mon hoc: "), TextField(17,75,10,"Nien khoa : "), TextField(19,75,2,"Hoc ki: ")
	, TextField(21,75,3,"Nhom: "),TextField(23,75,4,"So sv min: "),TextField(25,75,4,"So sv max: ")};
	
	fields[0].setText(ltc->lopTinChi.maMH);
	fields[1].setText(ltc->lopTinChi.nienKhoa);
	fields[2].setText((char*)to_string(ltc->lopTinChi.nhom).c_str());
	fields[3].setText((char*)to_string(ltc->lopTinChi.hocKi).c_str());
	fields[4].setText((char*)to_string(ltc->lopTinChi.soSVMIN).c_str());
	fields[5].setText((char*)to_string(ltc->lopTinChi.soSVMAX).c_str());
	
	//Hieu lop chung chi
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+5)%6;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%6;
			}
		}
		else if(input_type==ESC){
			//Xoa ltc
			if(ltc->lopTinChi.dsDK!=NULL){
				drawAlert("Khong the xoa lop tin chi nay vi da co sinh vien dang ki");
				continue;
			}
			NodeLTC* ret=delete_LTC(root,ltc);
			delete ret;
			drawAlert("Xoa lop tin chi thanh cong");
			return;
		}
		else if(input_type==ENTER){
			if(isEmpty(fields[1].getText())||!isInteger(fields[2].getText())||!isInteger(fields[3].getText())||!isInteger(fields[4].getText())||!isInteger(fields[5].getText()))
			{
				drawAlert("Nien khoa khong duoc trong hoc ki, nhom, So sinh vien min, max la so nguyen duong");
				continue;
			}
			bool f=false;
			for(int i=0;i<slMH;i++){
				if(stricmp(dsMH[i]->maMH,fields[0].getText())==0){
					f=true;
					break;
				}
			}
			if(!f){
				drawAlert("Khong tim thay ma mon hoc");
				continue;
			}
			
			NodeLTC* t=searchValue_LTC(root,fields[0].getText(),fields[1].getText(),atoi(fields[2].getText()),atoi(fields[3].getText()));
			if(ltc!=t&&t!=NULL){
				drawAlert("Da co lop tin chi cho du lieu nay roi");
				continue;
			}
			
			//Chinh sua ltc
			strcpy(ltc->lopTinChi.maMH,fields[0].getText());
			strcpy(ltc->lopTinChi.nienKhoa,fields[1].getText());
			ltc->lopTinChi.hocKi=atoi(fields[2].getText());
			ltc->lopTinChi.nhom=atoi(fields[3].getText());
			ltc->lopTinChi.soSVMAX=atoi(fields[5].getText());
			ltc->lopTinChi.soSVMIN=atoi(fields[4].getText());
			
			drawAlert("Chinh sua lop tin chi thanh cong!");
			return;
			
		}
		else fields[choosen].update(input_type);
		
	}
}





void quanLyLTC(NodeLTC* &root, MonHoc* dsMH[], unsigned int &slMH){
	inDanhSachLTC(root);
	while(true){//Chon chuc nang
		int input_type=getch();
		if(input_type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(input_type==0){//Bat phim F1,F2
			input_type=getch();
			if(input_type==F1){//Them ltc
				themLTC(root,dsMH,slMH);
			}
			else if(input_type==F2){//Hieu chinh ltc
				hieuChinhLTC(root,dsMH,slMH);	
			}
			inDanhSachLTC(root);
		}
	}
			
}


/*-----------------------------------------------------MON HOC----------------------------------------------------*/
void quickSortTenMH_MH(MonHoc* dsMH[],int l,int r){//Quick sort
	if (l <= r)
	{
	
		char key[60];//Key la ten mon hoc
		strcpy(key,dsMH[(l+r)/2]->tenMH);
	
	
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp(dsMH[i]->tenMH,key)<0)
				i++;
			while (stricmp(dsMH[j]->tenMH,key)>0)
				j--;
 
			if (i <= j)
			{
				swap(dsMH[i],dsMH[j]);
				i++;
				j--;
			}
		}
 
	
		if (l < j)
			quickSortTenMH_MH(dsMH, l, j);
		if (r > i)
			quickSortTenMH_MH(dsMH, i, r);
	}
	
}
void inDanhSachMH(MonHoc* dsMH[], unsigned int &slMH){
	//Sap xep mon hoc theo ten mon hoc
	quickSortTenMH_MH(dsMH,0,slMH-1);
	
	setBGColor(0);
	system("cls");
	
	drawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them      F2: Hieu chinh     F2->Esc: Xoa          Tab: Thoat         ENTER: Luu",false);
		
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	textColor(WHITE);
	setBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA MON HOC";
	gotoxy(startcol+15,startrow-3);
	cout<<"|TEN MON HOC";
	gotoxy(startcol+50,startrow-3);
	cout<<"|SO TIN CHI LY THUYET";
	gotoxy(startcol+82,startrow-3);
	cout<<"|SO TIN CHI THUC HANH";


	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	
	for(int i=0;i<slMH;i++){
		gotoxy(startcol,startrow+i);
		cout<<dsMH[i]->maMH;
		gotoxy(startcol+15,startrow+i);
		cout<<dsMH[i]->tenMH;
		gotoxy(startcol+50,startrow+i);
		cout<<dsMH[i]->soTCLT;
		gotoxy(startcol+82,startrow+i);
		cout<<fixed<<setprecision(1)<<dsMH[i]->soTCTH;

	}
	
	

}

void themMH(MonHoc* dsMH[], unsigned int& slMH){
	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(15,13,120,11);
	TextField fields[4]={TextField(15,50,10,"Ma mon hoc: "), TextField(17,50,60,"Ten mon hoc : "), TextField(19,50,5,"So tin chi ly thuyet: ")
	, TextField(21,50,5,"So tin chi thuc hanh: ")};

	while(true){//Nhap vao 
		int input_type=getch();
		if(input_type==TAB){//Phim tab tro ve menu quan ly mon hoc
			return;
		}
		else if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+3)%4;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%4;
			}
		}
		else if(input_type==ENTER){
			//Kiem tra du lieu 
			if(isEmpty(fields[1].getText())||!isFloat(fields[2].getText())||!isFloat(fields[3].getText()))
			{
				drawAlert("Ten mon hoc khong duoc de trong so tin chi ly thuyet, thuc hanh phai la so thuc");
				continue;
			}
			if(isSpace(fields[0].getText())){
				drawAlert("Ma mon hoc khong duoc chua khoang trang");
				continue;
			}
			bool f=false;
			for(int i=0;i<slMH;i++){
				if(stricmp(dsMH[i]->maMH,fields[0].getText())==0){
					f=true;
					break;
				}
			}
			if(f){
				drawAlert("Da co ma mon hoc nay roi");
				continue;
			}
			
			MonHoc* mon_hoc=new MonHoc;
			strcpy(mon_hoc->maMH,fields[0].getText());
			strcpy(mon_hoc->tenMH,fields[1].getText());
			mon_hoc->soTCLT=stof(string(fields[2].getText()));
			mon_hoc->soTCTH=stof(string(fields[3].getText()));
			
			dsMH[slMH++]=mon_hoc;
			
			drawAlert("Them mon hoc thanh cong");
			return;
			
			
		}
		else fields[choosen].update(input_type);
	
		
	}
}

void hieuChinhMH(NodeLTC* root,MonHoc* dsMH[], unsigned int& slMH){//Khong duoc sua ma lop


	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(47,13,40,5);
	

	int vi_tri_mh=-1;
	TextField input_mh = TextField(15,70,10,"Nhap vao ma mon hoc: ");	
	
	
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		else if(input_type==ENTER){
			bool f=false;
			for(int i=0;i<slMH;i++){
				if(stricmp(dsMH[i]->maMH,input_mh.getText())==0){
					vi_tri_mh=i;
					f=true;
					break;
				}
			}
			if(f){
				drawAlert("Tim thay mon hoc");
				break;
			}		
			else{
				drawAlert("Khong tim thay mon hoc");
				continue;
			}
		}
		else input_mh.update(input_type);
	
	}
	setBGColor(YELLOW);
	drawRectangle(20,13,90,9);
	TextField fields[3]={TextField(15,50,60,"Ten mon hoc: "), TextField(17,50,5,"So tin chi ly thuyet : "), TextField(19,50,5,"So tin chi thuc hanh: ")};
	fields[0].setText(dsMH[vi_tri_mh]->tenMH);
	fields[1].setText((char*)to_string(dsMH[vi_tri_mh]->soTCLT).c_str());
	fields[2].setText((char*)to_string(dsMH[vi_tri_mh]->soTCTH).c_str());
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+2)%3;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%3;
			}
		}
		else if(input_type==ESC){
			if(searchMH_LTC(root,dsMH[vi_tri_mh]->maMH)!=NULL){
				drawAlert("Khong the xoa mon hoc nay vi da co lop tin chi");
				continue;
			}
			else{
				delete_MH(dsMH,slMH,vi_tri_mh);
				drawAlert("Xoa mon hoc thanh cong");
				return;
			}
		}
		else if(input_type==ENTER){
			if(isEmpty(fields[0].getText())||!isFloat(fields[1].getText())||!isFloat(fields[2].getText()))
			{
				drawAlert("Ten mon hoc khong duoc trong. so tin chi ly thuyet, thuc hanh phai la so thuc");
				continue;
			}
			
			strcpy(dsMH[vi_tri_mh]->tenMH,fields[0].getText());
			dsMH[vi_tri_mh]->soTCLT=stof(string(fields[1].getText()));
			dsMH[vi_tri_mh]->soTCTH=stof((fields[2].getText()));
		
			drawAlert("Hieu chinh mon hoc thanh cong");
			return;
			
		}
		else fields[choosen].update(input_type);
		
	}
}


void quanLyMH(NodeLTC* root,MonHoc* dsMH[], unsigned int &slMH){
	inDanhSachMH(dsMH, slMH);
	while(true){//Chon chuc nang
		int input_type=getch();
		if(input_type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(input_type==0){//Bat phim F1,F2
			input_type=getch();
			if(input_type==F1){//Them mon hoc
				themMH(dsMH,slMH);
			}
			else if(input_type==F2){//Hieu chinh mon hoc
				hieuChinhMH(root,dsMH,slMH);	
			}
			inDanhSachMH(dsMH, slMH);
		}
	}
			
}

/*------------------------------------------------------------------SINH VIEN ---------------------------*/
void inDSSVTheoLop(NodeSV* dsSV){
	setBGColor(0);
	system("cls");
	
	int startrow=10,startcol=30;
	
	setBGColor(YELLOW);
	drawRectangle(50,13,40,5);
	
	TextField input_lop = TextField(15,70,15,"Nhap vao ma lop: ");	
	
	NodeSV* print_pos=NULL;
	
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		else if(input_type==ENTER){
			//Tim kiem ma lop tren danh sach sinh vien
			NodeSV* node=dsSV;
		
			while(node!=NULL&&stricmp(node->sinhVien.maLop,input_lop.getText())<0){
				node=node->next;
			}
			if(node!=NULL){		
				if(stricmp(node->sinhVien.maLop,input_lop.getText())==0){
					print_pos=node;
					drawAlert("Tim thay lop");
					break;//Toi vong lap in sinh vien
				}
			}
			

			drawAlert("Khong tim thay lop nay");
			continue;
		}
		else input_lop.update(input_type);
	}
	
	setBGColor(0);
	system("cls");
	textColor(WHITE);
		
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA SINH VIEN";
	gotoxy(startcol+15,startrow-3);
	cout<<"|HO";
	gotoxy(startcol+40,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+62,startrow-3);
	cout<<"|PHAI";
	gotoxy(startcol+82,startrow-3);
	cout<<"|SDT";
	gotoxy(startcol+94,startrow-3);
	cout<<"|Nam nhap hoc";


	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';

	int i=0;
	while(print_pos!=NULL&&stricmp(print_pos->sinhVien.maLop,input_lop.getText())==0){
		gotoxy(startcol,startrow+i);
		cout<<print_pos->sinhVien.maSV;
		gotoxy(startcol+15,startrow+i);
		cout<<print_pos->sinhVien.ho;
		gotoxy(startcol+40,startrow+i);
		cout<<print_pos->sinhVien.ten;
		gotoxy(startcol+62,startrow+i);
		cout<<print_pos->sinhVien.phai;
		gotoxy(startcol+82,startrow+i);
		cout<<print_pos->sinhVien.sdt;
		gotoxy(startcol+94,startrow+i);
		cout<<print_pos->sinhVien.namNhapHoc;
		
		print_pos=print_pos->next;
		++i;
	}
	
	getch();
}

void themSV(NodeSV*& dsSV){//Khong duoc sua ma lop
	setBGColor(0);
	system("cls");

	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(50,13,45,5);
	
	
	TextField input_lop = TextField(15,70,15,"Nhap vao ma lop: ");	
	
	
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		else if(input_type==ENTER){
			if(isSpace(input_lop.getText())){
				drawAlert("Ma lop khong duoc rong, khong duoc chua khoang trang");
				continue;
			}
			//Tim kiem ma lop tren danh sach sinh vien
			NodeSV* node=dsSV;
			while(node!=NULL&&stricmp(node->sinhVien.maLop,input_lop.getText())<0){
				node=node->next;
			}
			if(node!=NULL){
				if(stricmp(node->sinhVien.maLop,input_lop.getText())==0){
					drawAlert("Da co sinh vien thuoc ma lop nay");
					break;//Toi vong lap nhap sinh vien
				}
				node=node->next;
			}
			

			drawAlert("Chua co sinh vien thuoc ma lop nay");
			break;
		}
		else input_lop.update(input_type);
	
	}
	
	SinhVien dsSV_tmp[10];
	int sl_them=0;
	
	
	int startrow=10,startcol=30;
	
	
	
	while(true){
				//In danh sach sinh vien dang nhap
				setBGColor(0);
				system("cls");
				textColor(WHITE);
				
				drawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them      F2: Hieu chinh     F2->Esc: Xoa            Tab: Thoat            ENTER: Luu",false);
					
				//Khung cua bang
				gotoxy(startcol,startrow-3);
				cout<<"|STT";
				gotoxy(startcol+5,startrow-3);
				cout<<"|MA SINH VIEN";
				gotoxy(startcol+20,startrow-3);
				cout<<"|HO";
				gotoxy(startcol+45,startrow-3);
				cout<<"|TEN";
				gotoxy(startcol+67,startrow-3);
				cout<<"|PHAI";
				gotoxy(startcol+87,startrow-3);
				cout<<"|SDT";
				gotoxy(startcol+99,startrow-3);
				cout<<"|Nam nhap hoc";
			
			
				
				int cnt=100;
				gotoxy(startcol,startrow-2);
				while(cnt--) cout<<'_';
			
				int i=0;
				for(int i=0;i<sl_them;i++){
					gotoxy(startcol,startrow+i);
					cout<<i+1;
					gotoxy(startcol+5,startrow+i);
					cout<<dsSV_tmp[i].maSV;
					gotoxy(startcol+20,startrow+i);
					cout<<dsSV_tmp[i].ho;
					gotoxy(startcol+45,startrow+i);
					cout<<dsSV_tmp[i].ten;
					gotoxy(startcol+67,startrow+i);
					cout<<dsSV_tmp[i].phai;
					gotoxy(startcol+87,startrow+i);
					cout<<dsSV_tmp[i].sdt;
					gotoxy(startcol+99,startrow+i);
					cout<<dsSV_tmp[i].namNhapHoc;
				}
				
		int press_type=getch();
		if(press_type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(press_type==ENTER){
			for(int i=0;i<sl_them;i++){
				NodeSV* new_node=new NodeSV;
				new_node->sinhVien=dsSV_tmp[i];
				insertOrder_SV(dsSV,new_node);
			}
			drawAlert("Luu du lieu sinh vien thanh cong");
			return;
		}
		else if(press_type==0){//Bat phim F1
			press_type=getch();
			if(press_type==F1){//Them sinh vien
			
				setBGColor(YELLOW);
				drawRectangle(38,9,68,13);
				TextField fields[6]={	TextField(10,60,12,"Ma sinh vien: "), 	TextField(12,60,50,"Ho: "), 	TextField(14,60,50,"Ten: "),	TextField(16,60,4,"Phai: "),
					TextField(18,60,12,"So dien thoai: "),		TextField(20,60,5,"Nam nhap hoc: ")}; 
				int choosen=0;
				//Nhap sinh vien
				while(true){
					press_type=getch();
					if(press_type==TAB){//An nut tab thoat khoi chuc nang
						break;
					}
					if(press_type==DOUBLE_KEY){
						press_type=getch();
						if( press_type==LEFT) {
							choosen=(choosen+5)%6;
						}
						if( press_type==RIGHT) {
							choosen=(choosen+1)%6;
						}
					}
					else if(press_type==ENTER){
						NodeSV* node=dsSV;
						bool f=false;
						while(node!=NULL){
							if(stricmp(node->sinhVien.maSV,fields[0].getText())==0){
								f=true;
								break;
							}
							node=node->next;
						}
						if(f) {
							drawAlert("Da ton tai ma sinh vien trong danh sach sinh vien");
							continue;
						}
						
						f=false;
						for(int i=0;i<sl_them;i++)
						{
							if(stricmp(fields[0].getText(),dsSV_tmp[i].maSV)==0){
								drawAlert("Ban da nhap ma sinh vien nay roi");
								f=true;
								break;
							}
						}
						if(f) continue;
						
						if(isSpace(fields[0].getText())||isEmpty(fields[1].getText())||isEmpty(fields[2].getText())){
									drawAlert("Ma sinh vien khong duoc chua khoang trang, ho va ten khong duoc de trong");
									continue;
						}
						
						if(strcmp(fields[3].getText(),"NAM")!=0&&strcmp(fields[3].getText(),"NU")!=0){
							drawAlert("Phai la NAM hoac NU");
							continue;
						}
						if(!isInteger(fields[4].getText())||!isInteger(fields[4].getText())){
							drawAlert("So dien thoai, ngay nhap hoc phai la so");
							continue;
						}
						
						strcpy(	dsSV_tmp[sl_them].maLop,input_lop.getText());
						strcpy(	dsSV_tmp[sl_them].maSV,fields[0].getText());
						strcpy(	dsSV_tmp[sl_them].ho,fields[1].getText());
						strcpy(	dsSV_tmp[sl_them].ten,fields[2].getText());
						strcpy(	dsSV_tmp[sl_them].phai,fields[3].getText());
						strcpy(	dsSV_tmp[sl_them].sdt,fields[4].getText());
						dsSV_tmp[sl_them].namNhapHoc=atoi(fields[5].getText());
						
						++sl_them;
						
						drawAlert("Them sinh vien vao danh sach them");
						break;
					}
					else fields[choosen].update(press_type);

				}
			}
			if(press_type==F2){
				bool f=false;
				//Nhap lieu dong de hieu chinh
				setBGColor(YELLOW);
				drawRectangle(40,2,60,3);
				TextField dong=TextField(3,70,10,"Nhap dong de hieu chinh: ");
				int index=-1;
				while(true){//Vong lap hieu chinh
					press_type=getch();
					if(press_type==TAB) {
						f=true;
						break;
					}
					else if(press_type==ENTER){
						if(!isInteger(dong.getText())){
							drawAlert("Dong nhap vao phai la so nguyen");
							continue;
						}
						if(atoi(dong.getText())>sl_them){
							drawAlert("Dong nhap vao khong hop le");
							continue;
						}
						index=atoi(dong.getText());
						break;
						
					}
					else dong.update(press_type);
					
				}
				if(f) continue;
				
						setBGColor(YELLOW);
						drawRectangle(38,9,68,13);
						TextField fields[6]={	TextField(10,60,12,"Ma sinh vien: "), 	TextField(12,60,50,"Ho: "), 	TextField(14,60,50,"Ten: "),	TextField(16,60,4,"Phai: "),
						TextField(18,60,12,"So dien thoai: "),		TextField(20,60,5,"Nam nhap hoc: ")};  
						int choosen=0;
						
						fields[0].setText(dsSV_tmp[index-1].maSV);
						fields[1].setText(dsSV_tmp[index-1].ho);
						fields[2].setText(dsSV_tmp[index-1].ten);
						fields[3].setText(dsSV_tmp[index-1].phai);
						fields[4].setText(dsSV_tmp[index-1].sdt);
						fields[5].setText((char*)to_string(dsSV_tmp[index-1].namNhapHoc).c_str());
						
						while(true){//Vong lap sua thong tin va xoa
							press_type=getch();
							if(press_type==TAB){//An nut tab thoat khoi chuc nang
								break;
							}
							else if(press_type==DOUBLE_KEY){
								press_type=getch();
								if( press_type==LEFT) {
									choosen=(choosen+5)%6;
								}
								if( press_type==RIGHT) {
									choosen=(choosen+1)%6;
								}
							}
							else if(press_type==ESC){
								for(int i=index-1;i<sl_them-1;i++){
									dsSV_tmp[i]=dsSV_tmp[i+1];
								}
								--sl_them;
								drawAlert("Xoa sinh vien thanh cong");
								break;
							}
							else if(press_type==ENTER){
								NodeSV* node=dsSV;
								bool f=false;
								while(node!=NULL){
									if(stricmp(node->sinhVien.maSV,fields[0].getText())==0){
										f=true;
										break;
									}
									node=node->next;
								}
								if(f) {
									drawAlert("Da ton tai ma sinh vien trong danh sach sinh vien");
									continue;
								}
						
								
								f=false;
								for(int i=0;i<sl_them;i++)
								{
									if(i!=index-1&&stricmp(fields[0].getText(),dsSV_tmp[i].maSV)==0){
										drawAlert("Ban da nhap ma sinh vien nay roi");
										f=true;
										break;
									}
								}
								if(f) continue;
								
								if(isSpace(fields[0].getText())||isEmpty(fields[1].getText())||isEmpty(fields[2].getText())){
									drawAlert("Ma sinh vien khong duoc chua khoang trang, ho va ten khong duoc de trong");
									continue;
								}
								
								if(strcmp(fields[3].getText(),"NAM")!=0&&strcmp(fields[3].getText(),"NU")!=0){
									drawAlert("Phai la NAM hoac NU");
									continue;
								}
								if(!isInteger(fields[4].getText())||!isInteger(fields[4].getText())){
									drawAlert("So dien thoai, ngay nhap hoc phai la so");
									continue;
								}
								
								strcpy(	dsSV_tmp[index-1].maLop,input_lop.getText());
								strcpy(	dsSV_tmp[index-1].maSV,fields[0].getText());
								strcpy(	dsSV_tmp[index-1].ho,fields[1].getText());
								strcpy(	dsSV_tmp[index-1].ten,fields[2].getText());
								strcpy(	dsSV_tmp[index-1].phai,fields[3].getText());
								strcpy(	dsSV_tmp[index-1].sdt,fields[4].getText());
								dsSV_tmp[index-1].namNhapHoc=atoi(fields[5].getText());
								
								drawAlert("Sua danh sach thanh cong");
								break;
							}
							else fields[choosen].update(press_type);
		
						}
				
			}
		}
		
	}
	


}

void quanLySV(NodeSV*& dsSV){
	
	int choosen=0;
	char label[2][50]={"Them sinh vien","In DSSV theo lop"};
	int startrow=10,startcol=70;
	
	for(int i=0;i<2;i++){
		if(i==choosen) drawTextBox(startcol,startrow+i*3+i*3,3,30,RED,120,label[i],true);
		else drawTextBox(startcol,startrow+i*3+i*3,3,30,GREEN,120,label[i],true);
	}
	while(true){
		int input_type=getch();
		if(input_type==ENTER){//Phim enter thuc hien chuc nang
			if(choosen==0){
				themSV(dsSV);
				return;
			}
			if(choosen==1){
				inDSSVTheoLop(dsSV);
				return;
			}
		}
		if(input_type==DOUBLE_KEY){//Bat phim mui ten di chuyen len xuong
			input_type=getch();
			if(input_type==UP){
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,GREEN,120,label[choosen],true);//Ve menu ve co ban
				choosen=max(choosen-1,0);
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,RED,120,label[choosen],true);//Ve menu tiep theo
			}
			else if(input_type==DOWN){
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,GREEN,120,label[choosen],true);//Ve menu ve co ban
				choosen=min(choosen+1,1);
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,RED,120,label[choosen],true);//Ve menu tiep theo
			}
		}
	}
}

/*------------------------------------------------------------------DANG KI LOP TIN CHI---------------------------*/
void locLTC(NodeLTC* node, char nien_khoa[10], unsigned int hoc_ki, NodeLTC* out[],int& count){ 
    if (node == NULL) 
        return ;
		
	locLTC(node->left, nien_khoa, hoc_ki, out, count);  
   
    if(stricmp(node->lopTinChi.nienKhoa, nien_khoa)==0&&node->lopTinChi.hocKi==hoc_ki){
    	out[count++]=node;
	}

	locLTC(node->right, nien_khoa, hoc_ki, out, count); 
} 
void dangKiLTC(NodeLTC* root,MonHoc* dsMH[],unsigned int& slMH,NodeSV* dsSV ){
	setBGColor(0);
	system("cls");
	
	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(50,13,40,9);
	TextField fields[3]={TextField(15,70,12,"Ma sinh vien: "), TextField(17,70,10,"Nien khoa : "), TextField(19,70,2,"Hoc ki: ")};
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+2)%3;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%3;
			}
		}
		else if(input_type==ENTER){
			bool f=false;
			NodeSV* node=dsSV;
			while(node!=NULL){
				if(stricmp(node->sinhVien.maSV,fields[0].getText())==0){
					f=true;
					break;
				}
				node=node->next;
			}	
			if(!f){
				drawAlert("Khong tim thay sinh vien");
				continue;
			}		
			if(!isInteger(fields[2].getText())){
				drawAlert("Hoc ki phai la so nguyen duong");
				continue;
			}
			if(isEmpty(fields[1].getText())){
				drawAlert("Nien khoa khong duoc trong");
				continue;
			}
			break;
		}
		else fields[choosen].update(input_type);
		
	}
	
	//Bat doc loc cac ltc
	int count=0;
	NodeLTC* dsLTC[100];
	
	locLTC(root,fields[1].getText(),atoi(fields[2].getText()),dsLTC,count);
	
	int count_left[100];//So luong slot con lai
	for(int i=0;i<count;i++){
		int tmp=0;//So luong da dang ki mon hoc nay
		NodeDK* node=dsLTC[i]->lopTinChi.dsDK;
		while(node!=NULL){
			tmp++;
			node=node->next;
		}
		count_left[i]=dsLTC[i]->lopTinChi.soSVMAX-tmp;
	}
	
	
	
	setBGColor(0);
	system("cls");
	
	drawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them vao dang ki      F2->Enter: Xoa dang ki             Tab: Thoat              ENTER: Luu dang ki",false);
	
		
	int startrow=10,startcol=10;
	//Chinh mau chu thanh chu trang
	textColor(WHITE);
	setBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA LOP TC";
	gotoxy(startcol+15,startrow-3);
	cout<<"|MA MH";
	gotoxy(startcol+30,startrow-3);
	cout<<"|TEN MON HOC";
	gotoxy(startcol+80,startrow-3);
	cout<<"|NHOM";
	gotoxy(startcol+85,startrow-3);
	cout<<"|SO LUONG CON LAI";
	
	
	for(int i=0;i<count;i++){
	
		gotoxy(startcol,startrow+i);
		cout<<dsLTC[i]->lopTinChi.maLopTC;
		gotoxy(startcol+15,startrow+i);
		cout<<dsLTC[i]->lopTinChi.maMH;
		gotoxy(startcol+30,startrow+i);
		for(int j=0;i<slMH;j++){
			if(stricmp(dsMH[j]->maMH,dsLTC[i]->lopTinChi.maMH)==0){
					cout<<dsMH[j]->tenMH;
					break;
			}
		}
		gotoxy(startcol+80,startrow+i);
		cout<<dsLTC[i]->lopTinChi.nhom;
		gotoxy(startcol+85,startrow+i);
		cout<<count_left[i];
	
	 
 	}
 	
 	NodeLTC* dsDK[15];
 	int count_DSDK=0;
 	//Ve danh sach dang ki
	int startrow_dk=5,  startcol_dk=130;
	setBGColor(0);
	drawRectangle(startcol_dk-5,startrow_dk-5,40,20);
	drawTextBox(startcol_dk,1,1,20,GREEN,WHITE,"DANH SACH DANG KI", true);
	textColor(WHITE);
	gotoxy(startcol_dk,startrow_dk-3);
	cout<<"|MA MH";
	gotoxy(startcol_dk+15,startrow_dk-3);
	cout<<"|TEN MH";
	for(int i=0;i<count_DSDK;i++){
		gotoxy(startcol_dk, startrow_dk+i);
		cout<<dsDK[i]->lopTinChi.maMH;
		for(int j=0;j<slMH;j++){
			if(stricmp(dsMH[j]->maMH,dsDK[i]->lopTinChi.maMH)==0){
					gotoxy(startcol_dk+15,startrow_dk+i);
					cout<<dsMH[j]->tenMH;
					break;
			}
		}
	}
 	

	choosen=0;
	setBGColor(GREEN);
	drawRectangle(startcol-5,startrow+choosen,2,1);
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==UP) {
				setBGColor(BLACK);
				drawRectangle(startcol-5,startrow+choosen,2,1);
				choosen=(choosen-1+count)%count;
				setBGColor(GREEN);
				drawRectangle(startcol-5,startrow+choosen,2,1);
			}
			if( input_type==DOWN) {
				setBGColor(BLACK);
				drawRectangle(startcol-5,startrow+choosen,2,1);
				choosen=(choosen+1)%count;
				setBGColor(GREEN);
				drawRectangle(startcol-5,startrow+choosen,2,1);
			}
		}
		else if(input_type==ENTER){
			if(count_DSDK==0){
				drawAlert("Ban chua dang ki lop tin chi nao");
				continue;
			}
			//Luu danh sach dang ki
			for(int i=0;i<count_DSDK;i++){
				//Insert head
				NodeDK* dk=new NodeDK;
				strcpy(dk->dangKi.maSV,fields[0].getText());
				dk->dangKi.diem=0;
				
				insertHead_DK(dsDK[i],dk);
			}
			drawAlert("Dang ki thanh cong");
			return;
		}
		else if(input_type==0){//Bat phim F1,F2
			input_type=getch();
			if(input_type==F1){
				//Them vao danh sach dang ki
				if(count_left[choosen]<=0){
					drawAlert("So sinh vien dang ki trong lop tin chi nay da dat toi da");
					continue;
				}
				if(count_DSDK==15){
					drawAlert("So luong danh sach dang ki da dat toi da");
					continue;
				}
				bool f=false;
				for(int i=0;i<count_DSDK;i++){
					if(dsDK[i]==dsLTC[choosen]){
						drawAlert("Lop tin chi nay da o trong danh sach dang ki");
						f=true;
						break;
					}
				}
				if(f) continue;
				
				//Kiem tra xem sinh vien da dang ki mon chua
				f=false;
				NodeDK* node=dsLTC[choosen]->lopTinChi.dsDK;
				while(node!=NULL){
					if(stricmp(node->dangKi.maSV,fields[0].getText())==0){
						drawAlert("SInh vien da dang ki lop tin chi nay roi");
						f=true;
						break;
					}
					node=node->next;
				}
				if(f) continue;
				
				
				dsDK[count_DSDK++]=dsLTC[choosen];
					//Ve danh sach dang ki
				setBGColor(0);
				drawRectangle(startcol_dk-5,startrow_dk-5,40,20);
				drawTextBox(startcol_dk,1,1,20,GREEN,WHITE,"DANH SACH DANG KI", true);
				
				textColor(WHITE);
				gotoxy(startcol_dk,startrow_dk-3);
				cout<<"|MA MH";
				gotoxy(startcol_dk+15,startrow_dk-3);
				cout<<"|TEN MH";
				for(int i=0;i<count_DSDK;i++){
					gotoxy(startcol_dk, startrow_dk+i);
					cout<<dsDK[i]->lopTinChi.maMH;
					for(int j=0;j<slMH;j++){
						if(stricmp(dsMH[j]->maMH,dsDK[i]->lopTinChi.maMH)==0){
								gotoxy(startcol_dk+15,startrow_dk+i);
								cout<<dsMH[j]->tenMH;
								break;
						}
					}
				}
 	
				
				
				drawAlert("Them vao danh sach dang ki");
			}
			else if(input_type==F2){//MODE XOA 
				if(count_DSDK==0){
					drawAlert("Khong co du lieu de xoa");
					continue;
				}
				
				choosen=0;
				
				setBGColor(RED);
				drawRectangle(startcol_dk-5,startrow_dk+choosen,2,1);
				textColor(WHITE);
				while(true){
					input_type=getch();
					if(input_type==TAB){
						break;
					}
					if(input_type==DOUBLE_KEY){
						input_type=getch();
						if( input_type==UP) {
							setBGColor(BLACK);
							drawRectangle(startcol_dk-5,startrow_dk+choosen,2,1);
							choosen=(choosen-1+count_DSDK)%count_DSDK;
							setBGColor(RED);
							drawRectangle(startcol_dk-5,startrow_dk+choosen,2,1);
						}
						if( input_type==DOWN) {
							setBGColor(BLACK);
							drawRectangle(startcol_dk-5,startrow_dk+choosen,2,1);
							choosen=(choosen+1)%count_DSDK;
							setBGColor(RED);
							drawRectangle(startcol_dk-5,startrow_dk+choosen,2,1);
						}
					}
					else if(input_type==ENTER){
						for(int i=choosen;i<count_DSDK-1;i++){
							dsDK[i]=dsDK[i+1];
						}
						--count_DSDK;
						choosen=0;
						//Ve danh sach dang ki
						setBGColor(0);
						drawRectangle(startcol_dk-5,startrow_dk-5,40,20);
						drawTextBox(startcol_dk,1,1,20,GREEN,WHITE,"DANH SACH DANG KI", true);
						
						textColor(WHITE);
						gotoxy(startcol_dk,startrow_dk-3);
						cout<<"|MA MH";
						gotoxy(startcol_dk+15,startrow_dk-3);
						cout<<"|TEN MH";
						for(int i=0;i<count_DSDK;i++){
							gotoxy(startcol_dk, startrow_dk+i);
							cout<<dsDK[i]->lopTinChi.maMH;
							for(int j=0;j<slMH;j++){
								if(stricmp(dsMH[j]->maMH,dsDK[i]->lopTinChi.maMH)==0){
										gotoxy(startcol_dk+15,startrow_dk+i);
										cout<<dsMH[j]->tenMH;
										break;
								}
							}
						}
						
						drawAlert("Xoa dang ki thanh cong");
						break;
 	
					}
				}
			}
		}
	}
}

void inDSSVDangKiLTC(NodeLTC* root, NodeSV* dsSV ){
		
	setBGColor(0);
	system("cls");
	
	
	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(40,13,50,12);
	TextField fields[4]={TextField(15,60,10,"Ma mon hoc: "), TextField(17,60,10,"Nien khoa : "), TextField(19,60,2,"Hoc ki: ")
	, TextField(21,60,3,"Nhom: ")};
	NodeLTC* ltc=NULL;
	while(true){//Nhap vao 
		int input_type=getch();
		if(input_type==TAB){//Phim tab tro ve menu quan ly mon hoc
			return;
		}
		else if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+3)%4;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%4;
			}
		}
		else if(input_type==ENTER){
			//Kiem tra du lieu 
			if(isEmpty(fields[1].getText())||!isInteger(fields[2].getText())||!isInteger(fields[3].getText()))
			{
				drawAlert("Nien khoa khong duoc trong. Hoc ki, nhom la so nguyen duong");
				continue;
			}
			
			ltc=searchValue_LTC(root,fields[0].getText(),fields[1].getText(),atoi(fields[2].getText()),atoi(fields[3].getText()));
			//4 du lieu phai giong nhau thi chi co duy nhat 1 lop tin chi
			if(ltc==NULL){
				drawAlert("Khong tim thay lop tin chi");
				continue;
			}
			break;
			
		}
		else fields[choosen].update(input_type);
	
		
	}
	
	setBGColor(0);
	system("cls");
	
	
	
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	textColor(WHITE);
	setBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA SINH VIEN";
	gotoxy(startcol+25,startrow-3);
	cout<<"|HO ";
	gotoxy(startcol+70,startrow-3);
	cout<<"|TEN";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	NodeDK* dk=ltc->lopTinChi.dsDK;

		
	
	int i=0;
	while(dk!=NULL){
		NodeSV* node=dsSV;
		while(node!=NULL){
			if(stricmp(node->sinhVien.maSV,dk->dangKi.maSV)==0){				
				break;
			}
		else node=node->next;
		}
		if(node==NULL){
			drawAlert("Du lieu sinh cua sinh vien khong ton tai");
			continue;
		}
   		gotoxy(startcol,startrow+i);
		cout<<node->sinhVien.maSV;
		gotoxy(startcol+25,startrow+i);
		cout<<node->sinhVien.ho;
		gotoxy(startcol+70,startrow+i);
		cout<<node->sinhVien.ten;
		dk=dk->next;
		++i;
	}
	
	getch();
	
}

void nhapDiemLTC(NodeLTC* root,  NodeSV* dsSV){
	setBGColor(0);
	system("cls");
	
	
	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(40,13,50,12);
	TextField fields[4]={TextField(15,60,10,"Ma mon hoc: "), TextField(17,60,10,"Nien khoa : "), TextField(19,60,2,"Hoc ki: ")
	, TextField(21,60,3,"Nhom: ")};
	NodeLTC* ltc=NULL;
	while(true){//Nhap vao 
		int input_type=getch();
		if(input_type==TAB){//Phim tab tro ve menu quan ly mon hoc
			return;
		}
		else if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+3)%4;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%4;
			}
		}
		else if(input_type==ENTER){
			//Kiem tra du lieu 
			if(isEmpty(fields[1].getText())||!isInteger(fields[2].getText())||!isInteger(fields[3].getText()))
			{
				drawAlert("Nien khoa khong duoc trong. Hoc ki, nhom la so nguyen duong");
				continue;
			}
			
			ltc=searchValue_LTC(root,fields[0].getText(),fields[1].getText(),atoi(fields[2].getText()),atoi(fields[3].getText()));
			//4 du lieu phai giong nhau thi chi co duy nhat 1 lop tin chi
			if(ltc==NULL){
				drawAlert("Khong tim thay lop tin chi");
				continue;
			}
			
			//Kiem tra so luong sinh vien dang ki >= so luong dang ki min thi minh moi nhap diem duoc
			int cnt=0;
			NodeDK* dk=ltc->lopTinChi.dsDK;
			while(dk!=NULL){
				dk=dk->next;
				++cnt;
			}
			if(cnt<ltc->lopTinChi.soSVMIN){
				drawAlert("So luong sinh vien chua dat yeu cau");
				continue;
			}
			break;
			
		}
		else fields[choosen].update(input_type);
	
		
	}
	
	setBGColor(0);
	system("cls");
	
	
	
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	textColor(WHITE);
	setBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|STT";
	gotoxy(startcol+5,startrow-3);
	cout<<"|MA SINH VIEN";
	gotoxy(startcol+25,startrow-3);
	cout<<"|HO ";
	gotoxy(startcol+70,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+90,startrow-3);
	cout<<"|DIEM";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	NodeDK* dk=ltc->lopTinChi.dsDK;
	TextField* nhap_diem[100];
	choosen=0;
		
	
	int i=0;
	while(dk!=NULL){
		NodeSV* node=dsSV;
		while(node!=NULL){
			if(stricmp(node->sinhVien.maSV,dk->dangKi.maSV)==0){				
				break;
			}
		else node=node->next;
		}
		if(node==NULL){
			drawAlert("Du lieu sinh cua sinh vien khong ton tai");
			continue;
		}
		gotoxy(startcol,startrow+i);
		cout<<i+1;
   		gotoxy(startcol+5,startrow+i);
		cout<<node->sinhVien.maSV;
		gotoxy(startcol+25,startrow+i);
		cout<<node->sinhVien.ho;
		gotoxy(startcol+70,startrow+i);
		cout<<node->sinhVien.ten;
		
		nhap_diem[i]=new TextField(startrow+i,startcol+90,4,"");
		nhap_diem[i]->setText((char*)to_string(dk->dangKi.diem).c_str());
		++i;
		dk=dk->next;
	}	
	dk=ltc->lopTinChi.dsDK;
	while(true){
		int input_type=getch();
		if(input_type==TAB) return;
		else if(input_type==ENTER){
			if(!isFloat(nhap_diem[choosen]->getText())||atof(nhap_diem[choosen]->getText())>10){
				drawAlert("Diem chi nhap so thuc <=10");
				continue;
			}
			
			
			for(int j=0;j<i;j++){
				dk->dangKi.diem=stof(string(nhap_diem[j]->getText()));//stof chuyen  string ve float
				dk=dk->next;
			}
			
			drawAlert("Nhap diem thanh cong");
			return;
		
		}
		else if(input_type==DOUBLE_KEY){
			input_type=getch();
			if(input_type==UP){
				if(!isFloat(nhap_diem[choosen]->getText())||atof(nhap_diem[choosen]->getText())>10){
					drawAlert("Diem chi nhap so thuc");
					continue;
				}
				choosen=max(0,choosen-1);
			}
			else if(input_type==DOWN){
				if(!isFloat(nhap_diem[choosen]->getText())||atof(nhap_diem[choosen]->getText())>10){
					drawAlert("Diem chi nhap so thuc");
					continue;
				}
				choosen=min(choosen+1,i-1);
			}
		}
		else nhap_diem[choosen]->update(input_type);
	}
	
}

void inDiemLTC(NodeLTC* root,MonHoc* dsMH[],unsigned int& slMH  ,NodeSV* dsSV){
	setBGColor(0);
	system("cls");
	
	
	int choosen=0;
	setBGColor(YELLOW);
	drawRectangle(40,13,50,12);
	TextField fields[4]={TextField(15,60,10,"Ma mon hoc: "), TextField(17,60,10,"Nien khoa : "), TextField(19,60,2,"Hoc ki: ")
	, TextField(21,60,3,"Nhom: ")};
	NodeLTC* ltc=NULL;
	while(true){//Nhap vao 
		int input_type=getch();
		if(input_type==TAB){//Phim tab tro ve menu quan ly mon hoc
			return;
		}
		else if(input_type==DOUBLE_KEY){
			input_type=getch();
			if( input_type==LEFT) {
				choosen=(choosen+3)%4;
			}
			if( input_type==RIGHT) {
				choosen=(choosen+1)%4;
			}
		}
		else if(input_type==ENTER){
			//Kiem tra du lieu 
			if(isEmpty(fields[1].getText())||!isInteger(fields[2].getText())||!isInteger(fields[3].getText()))
			{
				drawAlert("Nien khoa khong duoc trong. Hoc ki, nhom la so nguyen duong");
				continue;
			}
			
			ltc=searchValue_LTC(root,fields[0].getText(),fields[1].getText(),atoi(fields[2].getText()),atoi(fields[3].getText()));
			//4 du lieu phai giong nhau thi chi co duy nhat 1 lop tin chi
			if(ltc==NULL){
				drawAlert("Khong tim thay lop tin chi");
				continue;
			}
			break;
			
		}
		else fields[choosen].update(input_type);
	
		
	}
	
	setBGColor(0);
	system("cls");
	for(int i=0;i<slMH;i++){
		if(stricmp(dsMH[i]->maMH,fields[0].getText())==0){
			drawTextBox(60,1,1,20,BLACK,WHITE,(char*)("BANG DIEM MON HOC: "+string(dsMH[i]->tenMH)).c_str(),false);
			break;
		}
	}

	drawTextBox(60,2,1,20,BLACK,WHITE,(char*)("Nien khoa: "+string(fields[1].getText())).c_str(),false);
	drawTextBox(85,2,1,20,BLACK,WHITE,(char*)("Hoc ki: "+string(fields[2].getText())).c_str(),false);
	drawTextBox(105,2,1,20,BLACK,WHITE,(char*)("Nhom: "+string(fields[3].getText())).c_str(),false);
	
	
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	textColor(WHITE);
	setBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"STT";
	gotoxy(startcol+5,startrow-3);
	cout<<"|MA SINH VIEN";
	gotoxy(startcol+25,startrow-3);
	cout<<"|HO ";
	gotoxy(startcol+70,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+90,startrow-3);
	cout<<"|DIEM";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	NodeDK* dk=ltc->lopTinChi.dsDK;
	TextField* nhap_diem[100];
	choosen=0;
		
	
	int i=0;
	while(dk!=NULL){
		NodeSV* node=dsSV;
		while(node!=NULL){
			if(stricmp(node->sinhVien.maSV,dk->dangKi.maSV)==0){				
				break;
			}
		else node=node->next;
		}
		if(node==NULL){
			drawAlert("Du lieu sinh cua sinh vien khong ton tai");
			continue;
		}
		gotoxy(startcol,startrow+i);
		cout<<i+1;
   		gotoxy(startcol+5,startrow+i);
		cout<<node->sinhVien.maSV;
		gotoxy(startcol+25,startrow+i);
		cout<<node->sinhVien.ho;
		gotoxy(startcol+70,startrow+i);
		cout<<node->sinhVien.ten;
		gotoxy(startcol+90,startrow+i);
		cout<<dk->dangKi.diem;
		++i;
		dk=dk->next;
	}	
	
	getch();
	
}

void travelInDiemLop(NodeLTC* node,MonHoc* dsMH[], int slMH, char ma_SV[12], float& sum1, int& sum2){
	if (node == NULL) 
        return ; 
	travelInDiemLop(node->left,dsMH, slMH, ma_SV, sum1, sum2); 
	NodeDK* dk=node->lopTinChi.dsDK;
	while(dk!=NULL){
		if(stricmp(dk->dangKi.maSV,ma_SV)==0){
			//sinh vien co tham gia ltc nay
			//Lay thong tin mon hoc
			for(int i=0;i<slMH;i++){
				if(stricmp(dsMH[i]->maMH,node->lopTinChi.maMH)==0){
					sum1+=(dsMH[i]->soTCLT+dsMH[i]->soTCTH)*(dk->dangKi.diem);
					sum2+=dsMH[i]->soTCLT+dsMH[i]->soTCTH;
				
					break;
				}
			}
			break;
		}
		dk=dk->next;
	}
	travelInDiemLop(node->right,dsMH, slMH, ma_SV, sum1, sum2); 
}

void inDiemLop(NodeLTC* root,  NodeSV* dsSV, MonHoc* dsMH[], int slMH){
	setBGColor(0);
	system("cls");
	
	int startrow=10,startcol=30;
	
	setBGColor(YELLOW);
	drawRectangle(50,13,40,5);
	
	TextField input_lop = TextField(15,70,15,"Nhap vao ma lop: ");	
	//Danh sach sinh vien co thu tu theo ma lop+ ma sv
	//Duyet ds dv tim kiem ma lop
	
	//ma lop+ ma sv
	
	//N17DCCN N17DCCN N18DCCN
	
	NodeSV* print_pos=NULL;//Vi tri dau tien minh tim thay ma lop
	
	while(true){
		int input_type=getch();
		if(input_type==TAB){
			return;
		}
		else if(input_type==ENTER){
			//Tim kiem ma lop tren danh sach sinh vien
			NodeSV* node=dsSV;
		
			while(node!=NULL&&stricmp(node->sinhVien.maLop,input_lop.getText())<0){
				node=node->next;
			}
			if(node!=NULL){		//Tim thay ma lop
				if(stricmp(node->sinhVien.maLop,input_lop.getText())==0){
					print_pos=node;
					drawAlert("Tim thay lop");
					break;//Toi vong lap in sinh vien
				}
				//> bo
			}
			

			drawAlert("Khong tim thay lop nay");
			continue;
		}
		else input_lop.update(input_type);
	}
	
	setBGColor(0);
	system("cls");
	textColor(WHITE);
	
	drawTextBox(60,1,1,20,BLACK,WHITE,"BANG THONG KE DIEM TRUNG BINH KHOA HOC",false);
	drawTextBox(75,2,1,20,BLACK,WHITE,(char*)("Lop "+string(input_lop.getText())).c_str(),false);
			
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|STT";
	gotoxy(startcol+5,startrow-3);
	cout<<"|MA SINH VIEN";
	gotoxy(startcol+25,startrow-3);
	cout<<"|HO";
	gotoxy(startcol+70,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+90,startrow-3);
	cout<<"|DIEM TB";
	
	//D17CQCN01-N 

	//MA LOP D17CQCN01-N D17CQCN01-N D17CQCN03-N D17CQCN03-N
	//MA SV  n17dccn012  n17dccn013  n17dccn014     n17dccn015
	
	
	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';

	int i=0;
	while(print_pos!=NULL&&stricmp(print_pos->sinhVien.maLop,input_lop.getText())==0){
		//Duyet lay diem
		float sum1=0;
		int sum2=0;//tong so tinchi=0 0/0=nan
		travelInDiemLop(root,dsMH,slMH,print_pos->sinhVien.maSV,sum1,sum2);
		
		gotoxy(startcol,startrow+i);
		cout<<i+1;
		gotoxy(startcol+5,startrow+i);
		cout<<print_pos->sinhVien.maSV;
		gotoxy(startcol+25,startrow+i);
		cout<<print_pos->sinhVien.ho;
		gotoxy(startcol+70,startrow+i);
		cout<<print_pos->sinhVien.ten;
		gotoxy(startcol+90,startrow+i);
//			cout<<"Sum1: "<<sum1<<"SUm2: "<<sum2<<endl;
		if(sum2==0) {
			cout<<"Chua hoc";
		}
		else cout<<fixed<<setprecision(2)<<sum1/sum2;
		
		print_pos=print_pos->next;
		++i;
	}
	getch();
}


void quanLyDKLTC(NodeLTC* root,MonHoc* dsMH[],unsigned int& slMH, NodeSV* dsSV ){
	int choosen=0;
	char label[4][50]={"Dang ki lop tin chi","In DSSV theo lop tin chi","Nhap diem lop tin chi","In bang diem lop tin chi"};
	int startrow=10,startcol=70;
	
	for(int i=0;i<4;i++){
		if(i==choosen) drawTextBox(startcol,startrow+i*3+i*3,3,30,RED,120,label[i],true);
		else drawTextBox(startcol,startrow+i*3+i*3,3,30,GREEN,120,label[i],true);
	}
	while(true){
		int input_type=getch();
		if(input_type==ENTER){//Phim enter thuc hien chuc nang
			if(choosen==0){
				dangKiLTC(root, dsMH, slMH, dsSV);
				return;
			}
			if(choosen==1){
				inDSSVDangKiLTC(root,dsSV);
				return;
			}
			if(choosen==2){
				nhapDiemLTC(root,dsSV);
				return;
			}
			if(choosen==3){
				inDiemLTC(root,dsMH, slMH, dsSV);
				return;
			}
		}
		if(input_type==DOUBLE_KEY){//Bat phim mui ten di chuyen len xuong
			input_type=getch();
			if(input_type==UP){
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,GREEN,120,label[choosen],true);//Ve menu ve co ban
				choosen=max(choosen-1,0);
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,RED,120,label[choosen],true);//Ve menu tiep theo
			}
			else if(input_type==DOWN){
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,GREEN,120,label[choosen],true);//Ve menu ve co ban
				choosen=min(choosen+1,3);
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,RED,120,label[choosen],true);//Ve menu tiep theo
			}
		}
	}
}

/*------------------------------------------------------------------DOC GHI FILE--------------------------------------------------*/
void writeMonHoc(MonHoc* dsMH[],unsigned int& sl){
	fstream f;
	f.open("MonHoc.txt",ios::out|ios::binary|ios::trunc);
	for(int i=0;i<sl;i++){
		f.write(reinterpret_cast<char*>(&(*dsMH[i])),sizeof(MonHoc));//GHi tung cuc mon hoc
	}
	f.close();
}
void readMonHoc(MonHoc* dsMH[], unsigned int& sl){
	fstream f;
	f.open("MonHoc.txt",ios::in|ios::binary);
	if(!f) return;//Khong co file nay
	MonHoc tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(MonHoc));
		if(f.eof()) break;
		else{
			dsMH[sl]=new MonHoc;
			*dsMH[sl]=tmp;
			++sl;
		}
	}
}


void writeSinhVien(NodeSV* dsSV){
	fstream f;
	f.open("SinhVien.txt",ios::out|ios::binary|ios::trunc);
	NodeSV* sv=dsSV;
	while(sv!=NULL){
		f.write(reinterpret_cast<char*>(&(sv->sinhVien)),sizeof(SinhVien));
		sv=sv->next;
	}
	f.close();
}
void readSinhVien(NodeSV* &dsSV){
	fstream f;
	f.open("SinhVien.txt",ios::in|ios::binary);
	if(!f) return;
	SinhVien tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(SinhVien));
		if(f.eof()) break;
		else{
			NodeSV* sv=new NodeSV;
			sv->sinhVien=tmp;
			insertOrder_SV(dsSV,sv);
		}
	}
}


void writeDSDK(NodeDK* dsDK, string file_name){
	fstream f;
	f.open(file_name,ios::out|ios::binary|ios::trunc);
	NodeDK* dk=dsDK;
	while(dk!=NULL){
		f.write(reinterpret_cast<char*>(&(dk->dangKi)),sizeof(DangKi));
		dk=dk->next;
	}
}

void readDSDK(NodeLTC* ltc, string file_name){
	fstream f;
	f.open(file_name,ios::in|ios::binary);
	if(!f) return;
	DangKi tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(DangKi));
		if(f.eof()) break;
		else{
			NodeDK* dk=new NodeDK;
			dk->dangKi=tmp;
			insertHead_DK(ltc,dk);
		}
	}
}



void writeInorder_LTC(NodeLTC* node,fstream& f) 
{ 
    if (node == NULL) 
        return; 
    writeInorder_LTC(node->left,f); 
    
    f<<node->lopTinChi.maLopTC<<' ';
    f<<node->lopTinChi.maMH<<' ';
    f<<node->lopTinChi.nienKhoa<<' ';
    f<<node->lopTinChi.hocKi<<' ';
    f<<node->lopTinChi.nhom<<' ';
    f<<node->lopTinChi.soSVMIN<<' ';
    f<<node->lopTinChi.soSVMAX;
    f<<endl;
  	writeDSDK(node->lopTinChi.dsDK,to_string(node->lopTinChi.maLopTC)+".txt");
  	
  	writeInorder_LTC(node->right,f); 
} 

void writeLopTinChi(NodeLTC* root){
	fstream f;
	f.open("LopTinChi.txt",ios::out|ios::trunc);
	writeInorder_LTC(root,f);
	f.close();
	
}

void readLopTinChi(NodeLTC* & root){
	fstream f;
	f.open("LopTinChi.txt",ios::in);
	if(!f) return;
	LopTinChi tmp;
	while(true){
		f>>tmp.maLopTC;
		if(f.eof()) break;
		f>>tmp.maMH;
    	f>>tmp.nienKhoa;
    	f>>tmp.hocKi;
    	f>>tmp.nhom;
    	f>>tmp.soSVMIN;
    	f>>tmp.soSVMAX;
    	
    	NodeLTC* new_node=new NodeLTC;
    	new_node->lopTinChi=tmp;
    	readDSDK(new_node,to_string(tmp.maLopTC)+".txt");
    	insert_LTC(root,new_node);
	}
	f.close();
}

/*------------------------------------------------------------------MAIN------------------------------------------------------*/



int maucoban=GREEN;//mau xanh la cay
int mauchon=RED;//Mau do
int maucuachu=GREY;


char label[6][50]={"Lop tin chi","Mon hoc","Sinh Vien","Quan ly lop tin chi","In bang diem theo lop","Luu va thoat"};
int startrow=10,startcol=20;

int choosen=0;

void drawMenu(){
	setBGColor(0);
	system("cls");
	for(int i=0;i<6;i++){
		if(i==choosen) drawTextBox(startcol,startrow+i*3+i*3,3,30,mauchon,maucuachu,label[i],true);
		else drawTextBox(startcol,startrow+i*3+i*3,3,30,maucoban,maucuachu,label[i],true);
	}
	
}




int main(){
	srand(0);
	system("mode 1000");
	//Du lieu
	DuLieu duLieu;
	
	readMonHoc(duLieu.dsMH,duLieu.soLuongMH);
	readSinhVien(duLieu.dsSV);
	readLopTinChi(duLieu.dsLTC);
	
	drawMenu();
	getch();
	
	while(true){
		int input_type=getch();
		if(input_type==ENTER){//Phim enter thuc hien chuc nang
			if(choosen==0){
				quanLyLTC(duLieu.dsLTC,duLieu.dsMH,duLieu.soLuongMH);
				drawMenu();
			}
			if(choosen==1){
				quanLyMH(duLieu.dsLTC,duLieu.dsMH,duLieu.soLuongMH);
				drawMenu();
			}
			if(choosen==2){
				quanLySV(duLieu.dsSV);
				drawMenu();
			}
			if(choosen==3){
				quanLyDKLTC(duLieu.dsLTC,duLieu.dsMH, duLieu.soLuongMH, duLieu.dsSV);
				drawMenu();
			}
			if(choosen==4){
				inDiemLop(duLieu.dsLTC, duLieu.dsSV,duLieu.dsMH,duLieu.soLuongMH);
				drawMenu();
			}
			if(choosen==5){
				writeSinhVien(duLieu.dsSV);
				writeMonHoc(duLieu.dsMH,duLieu.soLuongMH);
				writeLopTinChi(duLieu.dsLTC);
				return 0;
			}
		}
		if(input_type==DOUBLE_KEY){//Bat phim mui ten di chuyen len xuong
			input_type=getch();
			if(input_type==UP){
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,maucoban,maucuachu,label[choosen],true);//Ve menu ve co ban
				choosen=max(choosen-1,0);
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,mauchon,maucuachu,label[choosen],true);//Ve menu tiep theo
			}
			else if(input_type==DOWN){
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,maucoban,maucuachu,label[choosen],true);//Ve menu ve co ban
				choosen=min(choosen+1,5);
				drawTextBox(startcol,startrow+choosen*3+choosen*3,3,30,mauchon,maucuachu,label[choosen],true);//Ve menu tiep theo
			}
		}
	}
//gotoxy(40,5);
//
//	textColor(PURPLE);
//	setBGColor(GREEN);
//	cout<<"Hello";
//	drawTextBox(10,10,20,50,GREEN,WHITE,"HELLO",true);
//	drawAlert("HELLO");
}
