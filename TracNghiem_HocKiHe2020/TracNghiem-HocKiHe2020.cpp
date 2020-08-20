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
#define LEFT 75
#define RIGHT 77
#define ESC 27


using namespace std;



///////////////////////////////////////////////////////////////// BAT DAU KHAI BAO ////////////////////////////////////////////////////////////////////

const int MAX_MH=300,MAX_LOP=500;

struct MonHoc{
	char maMH[15];
	char tenMH[50];
};


struct ChiTietThi{//Struct phu
	int quesID;
	char answer;
};

struct DiemThi{
	char maMH[15];
	float diem;
	
	int soLuongCau,thoiGianThi;
	ChiTietThi* chiTiet[50];
};

struct NodeDT{
	DiemThi diemThi;
	NodeDT* next=NULL;
};



struct SinhVien{
	char maSV[11];
	char ho[30];
	char ten[30];
	char phai[4];
	
	char password[20];
	NodeDT* dsDT=NULL;//Con tro tro den danh sach lien ket don diem thi

};

struct NodeSV{
	SinhVien sinhVien;
	NodeSV* next=NULL;
};


struct Lop{
	char maLop[12];
	char tenLop[100];
	char nienKhoa[10];
	
	NodeSV* dsSV=NULL;//Con tro tro den danh sach lien ket don sinh vien
	
	
};

struct CauHoiThi{
	int id;
	char maMH[15];
	
	char noiDung[100];
	
	char cauA[100];
	char cauB[100];
	char cauC[100];
	char cauD[100];
	
	char dapAn;
};

struct NodeCHT{
	CauHoiThi  cauHoiThi;
	NodeCHT* left=NULL;
	NodeCHT* right=NULL;
};



struct Data{
	MonHoc* dsMH[MAX_MH]={NULL};//Mang con tro
	Lop* dsLop[MAX_LOP]={NULL};//Mang con tro
	NodeCHT * dsCHT=NULL;//Cay nhi phan tim kiem
	
	int countMH=0;
	int countLop=0;
};



/////////////////////////////////////////////////////////// KET THUC KHAI BAO //////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////// BAT DAU THUAT TOAN /////////////////////////////////////////////////////////////////////
bool isEmpty(char* s){
	//Tra ve false neu chuoi trong hoac toan trong
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

bool isInteger(char* s){
	if(isEmpty(s)) return false;
	//Chuoi chi bao gom so thi tra ve true, false truong hop con lai
	for(int i=0;s[i]!='\0';i++){
		if(!isdigit(s[i])) return false;
	}
	return true;
}

/*-------------------------------diem thi-----------------------------------------*/
NodeDT* createNode_DiemThi(char ma_MH[15], float diem_thi){
	NodeDT* node=new NodeDT;
	strcpy(node->diemThi.maMH,ma_MH);
	node->diemThi.diem=diem_thi;
	node->next=NULL;
	return node;
}
NodeDT* insertHead_DiemThi(NodeDT* &head,NodeDT* node){//Them 'node' vao dau danh sach cua 'head'
    if(head == NULL){
        head = node;
    }else{
        node->next = head; 
        head = node; 
    }
    return head;
}



/*-----------------------------------sinh vien--------------------------------*/


NodeSV* createNode_SinhVien(char ma_SV[10], char ho[30], char ten[30], char phai[3], char password[30]){
	NodeSV* node=new NodeSV;
	strcpy(node->sinhVien.maSV,ma_SV);
	strcpy(node->sinhVien.ho,ho);
	strcpy(node->sinhVien.ten,ten);
	strcpy(node->sinhVien.phai,phai);
	strcpy(node->sinhVien.password,password);
	//Con tro danh sach diem thi = NULL vi moi tao Sinh Vien nen khong co diem thi
	node->sinhVien.dsDT=NULL;
	node->next=NULL;
	
	return node;
}

NodeSV* insertHead_SinhVien(NodeSV* &head, NodeSV* node){//Them 'node' vao dau danh sach cua 'head'
    if(head == NULL){
        head = node;
    }else{
        node->next = head; 
        head = node; 
    }
    return head;
}

/*-------------------------------lop--------------------------------------*/

Lop* add_Lop(Lop* dsLop[],int& sl, char ma_lop[10], char ten_lop[100], char nien_khoa[10])
{
	Lop* lop=new Lop;
	strcpy(lop->maLop,ma_lop);
	strcpy(lop->tenLop,ten_lop);
	strcpy(lop->nienKhoa, nien_khoa);
	lop->dsSV=NULL;
	
	//Them lop vao mang con tro dsLop
	dsLop[sl++]=lop;
	
	return lop;
}

void delete_Lop(Lop* dsLop[],int& sl, int index){
		delete dsLop[index];
	for(int i=index;i<sl-1;i++){
		dsLop[i]=dsLop[i+1];
	}

	--sl;
}

void modify_Lop(Lop* dsLop[],int index, char ten_lop[100], char nien_khoa[10]){
	strcpy(dsLop[index]->tenLop,ten_lop);
	strcpy(dsLop[index]->nienKhoa,nien_khoa);
}

/*-----------------------------------mon hoc--------------------------------------*/
MonHoc* add_MonHoc(MonHoc* dsMH[],int& sl, char ma_MH[15], char ten_MH[50])
{
	if(sl==MAX_MH) return NULL;


	MonHoc* mon_hoc=new MonHoc;
	strcpy(mon_hoc->maMH,ma_MH);
	strcpy(mon_hoc->tenMH,ten_MH);
	
	if(sl==0){
		dsMH[sl++]=mon_hoc;
	}
	else{
		//Insert order
		int index=sl-1;
		while(index>=0&&stricmp(dsMH[index]->maMH,ma_MH)>0){
			dsMH[index+1]=dsMH[index];
			--index;
		}
		dsMH[index+1]=mon_hoc;
		++sl;
	}
	return mon_hoc;
}

void delete_MonHoc(MonHoc* dsMH[],int& sl, int index){
	
	for(int i=index;i<sl-1;i++){
		dsMH[i]=dsMH[i+1];
	}
	--sl;
}

void modify_MonHoc(MonHoc* dsMH[],int index, char ten_mon_hoc[100]){
	strcpy(dsMH[index]->tenMH,ten_mon_hoc);
}

/*-----------------------------------de thi --------------------------------------*/
NodeCHT* createNode_CauHoiThi(int id, char ma_MH[15],	char noi_dung[100], char cau_A[100], char cau_B[100], char cau_C[100], char cau_D[100], char dap_an){//Tao  nhan vien
	NodeCHT* node=new NodeCHT;
	node->cauHoiThi.id=id;
	strcpy(node->cauHoiThi.maMH,ma_MH);
	strcpy(node->cauHoiThi.noiDung,noi_dung);
	strcpy(node->cauHoiThi.cauA,cau_A);
	strcpy(node->cauHoiThi.cauB,cau_B);
	strcpy(node->cauHoiThi.cauC,cau_C);
	strcpy(node->cauHoiThi.cauD,cau_D);
	node->cauHoiThi.dapAn=dap_an;
	
	return node;
}

NodeCHT* insertNode_CauHoiThi(NodeCHT* &tree, NodeCHT* node){
	if(tree==NULL) {
		tree=node;
		return node;
	}
	NodeCHT* last=NULL;//node truoc vitrichen 
	NodeCHT* vitrichen=tree;
	while(vitrichen!=NULL){
		last=vitrichen;
		if(node->cauHoiThi.id<vitrichen->cauHoiThi.id) vitrichen=vitrichen->left;
		else if(node->cauHoiThi.id>vitrichen->cauHoiThi.id) vitrichen=vitrichen->right;
		else if(node->cauHoiThi.id==vitrichen->cauHoiThi.id) return NULL;//da co trong cay
	}
	if(node->cauHoiThi.id<last->cauHoiThi.id) last->left=node;
	else last->right=node;
	
	return node;
}

NodeCHT* search(NodeCHT* root, int key) 
{ 
    // Base Cases: root is null or key is present at root 
    if (root == NULL || root->cauHoiThi.id== key) 
       return root; 
     
    // Key is greater than root's key 
    if (root->cauHoiThi.id < key) 
       return search(root->right, key); 
  
    // Key is smaller than root's key 
    return search(root->left, key); 
} 

void timKiemCauHoiThiTheoMaMonHoc(NodeCHT* dsCHT,NodeCHT* dsThi[],char* ma_mon_hoc, int& so_luong_cau_thuc_te)
{
	
    if ( dsCHT == NULL )
        return;
    else if(stricmp(dsCHT->cauHoiThi.maMH,ma_mon_hoc)==0)
    {
    	dsThi[so_luong_cau_thuc_te++]=dsCHT;	
	}
   timKiemCauHoiThiTheoMaMonHoc( dsCHT->left,dsThi,ma_mon_hoc,so_luong_cau_thuc_te);
   timKiemCauHoiThiTheoMaMonHoc( dsCHT->right,dsThi,ma_mon_hoc,so_luong_cau_thuc_te );
}



///////////////////////////////////////////////////////// KET THUC THUAT TOAN ////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////BAT DAU DO HOA////////////////////////////////////////////////////////////////////////////
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

void DrawRectangle(int col,int row,int width,int height){
	for(int j=row;j-row<height;j++){
		for(int i=col;i-col<width;i++){
			gotoxy(i,j);
			cout<<" ";
		}
	}
}

void drawListItem(int col, int row, char* prefix, char* text, int color){
	TextColor(color);
	gotoxy(col,row);
	cout<<prefix<<" "<<text;
}

void DrawTextBox(int col,int row,int docaocuahop,int dodaicuahop,int maucuahop, int maucuachu,char * text,bool canletrai){
	SetBGColor(maucuahop);
	DrawRectangle(col,row,dodaicuahop,docaocuahop);
	TextColor(maucuachu);
	if(!canletrai) gotoxy(col,row+docaocuahop/2);
	else gotoxy(col+dodaicuahop/2-strlen(text)/2,row+docaocuahop/2);
	cout<<text;
}

void DrawTextBox_Float(int col,int row,int docaocuahop,int dodaicuahop,int maucuahop, int maucuachu,float val){
	SetBGColor(maucuahop);
	TextColor(maucuachu);
	cout<<fixed<<setprecision(1)<<val;
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

struct ExamBox{
	CauHoiThi* cauHoiThi;
	int row,col;
	int index=0;//index so thu tu cua cau
	char choosen='A';
	
	void drawSelf(int press_type){
			if( press_type==UP) {
				choosen=(((choosen-A)+3)%4)+A;
			}
			if( press_type==DOWN) {
				choosen=(((choosen-A)+1)%4)+A;
			}
		
		redraw(true);
	}
	
	void redraw(bool high_light){
		char tmp[20];
		itoa(index,tmp,10);
		strcat(tmp,")");
		if(high_light==true)
		drawListItem(col,row,tmp,cauHoiThi->noiDung,PURPLE);//YEL
		else
		drawListItem(col,row,tmp,cauHoiThi->noiDung,WHITE);
		
		if(choosen=='A') drawListItem(col,row+2,"\tA.",cauHoiThi->cauA,GREEN);
		else drawListItem(col,row+2,"\tA.",cauHoiThi->cauA,WHITE);
		if(choosen=='B') drawListItem(col,row+3,"\tB.",cauHoiThi->cauB,GREEN);
		else drawListItem(col,row+3,"\tB.",cauHoiThi->cauB,WHITE);
		if(choosen=='C') drawListItem(col,row+4,"\tC.",cauHoiThi->cauC,GREEN);
		else drawListItem(col,row+4,"\tC.",cauHoiThi->cauC,WHITE);
		if(choosen=='D') drawListItem(col,row+5,"\tD.",cauHoiThi->cauD,GREEN);
		else drawListItem(col,row+5,"\tD.",cauHoiThi->cauD,WHITE);
	}
	
	void drawAnswer(){
		char tmp1[20];
		itoa(index,tmp1,10);
		strcat(tmp1,")");
		drawListItem(col,row,tmp1,cauHoiThi->noiDung,WHITE);
		
		char tmp[200]="";
		strcpy(tmp,cauHoiThi->cauA);
		if(cauHoiThi->dapAn=='A') strcat(tmp," (*)");
		if(choosen=='A') drawListItem(col,row+2,"\tA.",tmp,GREEN);
		else drawListItem(col,row+2,"\tA.",tmp,WHITE);
		strcpy(tmp,cauHoiThi->cauB);
		if(cauHoiThi->dapAn=='B') strcat(tmp," (*)");
		if(choosen=='B') drawListItem(col,row+3,"\tB.",tmp,GREEN);
		else drawListItem(col,row+3,"\tB.",tmp,WHITE);
		strcpy(tmp,cauHoiThi->cauC);
		if(cauHoiThi->dapAn=='C') strcat(tmp," (*)");
		if(choosen=='C') drawListItem(col,row+4,"\tC.",tmp,GREEN);
		else drawListItem(col,row+4,"\tC.",tmp,WHITE);
		strcpy(tmp,cauHoiThi->cauD);
		if(cauHoiThi->dapAn=='D') strcat(tmp," (*)");
		if(choosen=='D') drawListItem(col,row+5,"\tD.",tmp,GREEN);
		else drawListItem(col,row+5,"\tD.",tmp,WHITE);
	}
	
	bool checkAnswer(){
		return choosen==cauHoiThi->dapAn;
	}
};

struct TextBox{
	TextBox(int row2, int col2, int max_char=20,char *label2=""){
		row=row2;
		col=col2;
		maxChar=max_char;
		label=label2;
		field=new char[maxChar];
		strcpy(field,"\0");
		
		redraw();
//		DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,"",false);
	}
	int row,col,maxChar;
	char* field,*label;
	char* getText(){
		return field;
	}
	void drawSelf(int press_type){
		if(press_type==BACK_SPACE){//Backspace xoa 1 chu cua field hien tai
			int vitrixoa=strlen(field)-1;
			if(vitrixoa<0) return;
			field[vitrixoa]='\0';
					
			//Ve 1 o den len cho vua xoa
			SetBGColor(GREEN);
			DrawRectangle(col+vitrixoa,row,1,1);
		}
		else if(isprint(press_type)){
			if(maxChar-1==strlen(field))  return;//khong nhap da nhap so luong toi da roi
					
			int vitrithemchu=strlen(field);
			field[vitrithemchu]=(char)press_type;
			field[vitrithemchu+1]='\0';//Phai co ky tu ket thuc
			redraw();
//			DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,field,false);//Ve lai
		}
	}
	void redraw(){
		DrawTextBox(col-strlen(label),row,1,strlen(label),BLACK,WHITE,label,false);
		DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,field,false);
	}
	
};

struct ComboBox{
	ComboBox(int row2, int col2, char combo_text[][30],int size2,int max_char=5){
		row=row2;
		col=col2;
		maxChar=max_char;
		size=size2;
		
		choosen=0;
		for(int i=0;i<size;i++)
		strcpy(comboText[i],combo_text[i]);
		DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,comboText[choosen],false);
	}
	char comboText[10][30];
	int choosen=0;
	int size;
	int row,col,maxChar;
	char* getText(){
		return comboText[choosen];
	}
	void drawSelf(int press_type){
		if(press_type==BACK_SPACE){
			choosen=++choosen%size;
			DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,comboText[choosen],false);//Ve lai
		}
		else if(isprint(press_type)){
			choosen=++choosen%size;
			DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,comboText[choosen],false);//Ve lai
		}
	}
	void redraw(){
		DrawTextBox(col,row,1,maxChar-1,GREEN,WHITE,comboText[choosen],false);
	}
};
////////////////////////////////////////////////////////KET THUC DO HOA////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////// BAT DAU HAM XU LY MENU//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////	Doc ghi file
void writeMonHoc(MonHoc* dsMH[], int& sl){
	fstream f;
	f.open("MonHoc.txt",ios::out|ios::binary|ios::trunc);
	for(int i=0;i<sl;i++){
		f.write(reinterpret_cast<char*>(&(*dsMH[i])),sizeof(MonHoc));
	}
	f.close();
}
void readMonHoc(MonHoc* dsMH[], int& sl){
	fstream f;
	f.open("MonHoc.txt",ios::in|ios::binary);
	if(!f) return;//Chua co file return luon
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

void writeInorder_CauHoiThi(NodeCHT* node,fstream& f) 
{ 
    if (node == NULL) 
        return; 
    writeInorder_CauHoiThi(node->left,f); 
  	f.write(reinterpret_cast<char*>(&(node->cauHoiThi)),sizeof(CauHoiThi)); 
  	writeInorder_CauHoiThi(node->right,f); 
} 

void writeCauHoiThi(NodeCHT* dsCHT){
	fstream f;
	f.open("CauHoiThi.txt",ios::out|ios::binary|ios::trunc);
	writeInorder_CauHoiThi(dsCHT,f);
	f.close();
}
void readCauHoiThi(NodeCHT* & dsCHT){
	fstream f;
	f.open("CauHoiThi.txt",ios::in|ios::binary);
	if(!f) return;
	CauHoiThi tmp;
	while(true){
		f.read(reinterpret_cast<char*>(&tmp),sizeof(CauHoiThi));
		if(f.eof()) break;
		else{
			NodeCHT* node=new NodeCHT;
			node->cauHoiThi=tmp;
			insertNode_CauHoiThi(dsCHT,node);
		}
	}
}


void writeDiemThi(NodeSV* sv){
	fstream f;
	f.open(string(sv->sinhVien.maSV)+".txt",ios::out|ios::trunc);
	NodeDT* dt=sv->sinhVien.dsDT;
	while(dt!=NULL){
		f<<dt->diemThi.maMH<<endl;
		f<<dt->diemThi.diem<<endl;
		f<<dt->diemThi.thoiGianThi<<endl;
		f<<dt->diemThi.soLuongCau<<endl;
		for(int i=0;i<dt->diemThi.soLuongCau;i++){
			f<<dt->diemThi.chiTiet[i]->quesID<<' '<<dt->diemThi.chiTiet[i]->answer<<endl;
		}
		dt=dt->next;
	}
}

void readDiemThi(NodeDT* & dsDT,string ma_sv){
	fstream f;
	f.open(string(ma_sv)+".txt",ios::in);
	if(!f) return;
	DiemThi tmp;
	while(true){
	
		f>>tmp.maMH;
		if(f.eof())break;
		f>>tmp.diem;
		f>>tmp.thoiGianThi;
		f>>tmp.soLuongCau;
		for(int i=0;i<tmp.soLuongCau;i++){
			tmp.chiTiet[i]=new ChiTietThi;
			f>>tmp.chiTiet[i]->quesID>>tmp.chiTiet[i]->answer;
		}

			
		NodeDT* node=new NodeDT;
		node->diemThi=tmp;
		insertHead_DiemThi(dsDT,node);
	}
}

void writeLop(Lop* dsLop[], int& sl){
	fstream f1;
	f1.open("Lop.txt",ios::out|ios::trunc);
	f1<<sl<<endl;
	for(int i=0;i<sl;i++){
		f1<<dsLop[i]->maLop<<endl;
		
		fstream f;
		f.open(string(dsLop[i]->maLop)+".txt",ios::out|ios::trunc);
		f<<dsLop[i]->maLop<<endl;
		f<<dsLop[i]->tenLop<<endl;
		f<<dsLop[i]->nienKhoa<<endl;
		
		NodeSV* sv=dsLop[i]->dsSV;
		while(sv!=NULL){
			f<<sv->sinhVien.maSV<<endl;
			f<<sv->sinhVien.ho<<endl;
			f<<sv->sinhVien.ten<<endl;
			f<<sv->sinhVien.phai<<endl;
			f<<sv->sinhVien.password<<endl;
			writeDiemThi(sv);
			sv=sv->next;
		}
	}
}

void readLop(Lop* dsLop[], int& sl){
	fstream f;
	f.open("Lop.txt",ios::in);
	if(!f) return;
	f>>sl;
	f.ignore();
	for(int i=0;i<sl;i++){
		dsLop[i]=new Lop;
		f.getline(dsLop[i]->maLop,100);
		
		fstream f1;
		f1.open(string(dsLop[i]->maLop)+".txt",ios::in);
		f1.getline(dsLop[i]->maLop,100);
		f1.getline(dsLop[i]->tenLop,100);
		f1.getline(dsLop[i]->nienKhoa,100);
		SinhVien sv;
//		cout<<dsLop[i]->maLop<<endl;
//		cout<<dsLop[i]->tenLop<<endl;
//		cout<<dsLop[i]->nienKhoa<<endl;
		while(true){
			f1.getline(sv.maSV,100);
			if(f1.eof()) break;
			f1.getline(sv.ho,100);
			f1.getline(sv.ten,100);
			f1.getline(sv.phai,100);
			f1.getline(sv.password,100);
			
			
			
			NodeSV* node=new NodeSV;
			node->sinhVien=sv;
			readDiemThi(node->sinhVien.dsDT,sv.maSV);
			insertHead_SinhVien(dsLop[i]->dsSV,node);
		}
		
	}
}




//////////////////////////////////////////////////////////////////
//Thong so menu chinh
	int maucoban=10;
	int mauchon=100;
	int maucuachu=120;
	int maunen=30;

	char label[8][50]={"Quan Ly Lop","Sinh Vien","Mon hoc","Them cau hoi thi","Thi trac nghiem","Bang diem","Chi tiet thi","Luu va thoat"};
	int chonmenu=0;
	int startrow=1,startcol=20;
	void Vemenu(){
		SetBGColor(0);
		system("cls");
		for(int i=0;i<8;i++){
			if(i==chonmenu) 	DrawTextBox(startcol,startrow+i*3+i,3,30,mauchon,maucuachu,label[i],true);
			else 	DrawTextBox(startcol,startrow+i*3+i,3,30,maucoban,maucuachu,label[i],true);
		}
	}
	
/*----------------------------------- Quan ly lop --------------------------------------*/
void inDanhSachLop(Lop* dsLop[],int& sl){
	SetBGColor(0);
	system("cls");

	
	DrawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them      F2: Hieu chinh     F2->Esc: Xoa",false);
		
		
	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	TextColor(WHITE);
	SetBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA LOP";
	gotoxy(startcol+15,startrow-3);
	cout<<"|TEN LOP";
	gotoxy(startcol+70,startrow-3);
	cout<<"|NIEN KHOA";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	
	for(int i=0;i<sl;i++){
		gotoxy(startcol,startrow+i);
		cout<<dsLop[i]->maLop;
		gotoxy(startcol+15,startrow+i);
		cout<<dsLop[i]->tenLop;
		gotoxy(startcol+70,startrow+i);
		cout<<dsLop[i]->nienKhoa;
	}
}

void themLop(Lop* dsLop[],int& sl){


	int choosen=0;
	SetBGColor(maunen);
	DrawRectangle(15,13,135,10);
	TextBox tb[3]={	TextBox(15,30,12,"Ma lop: "), TextBox(17,30,100,"Ten lop: "), TextBox(19,30,10,"Nien khoa: ")};// ma lop, ten lop , nien khoa

	while(true){//Nhap vao 
		int press_type=getch();
		if(press_type==TAB){//Phim tab tro ve menu quan ly Lop
			return;
		}
		else if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+2)%3;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%3;
			}
		}
		else if(press_type==ENTER){
			//Kiem tra du lieu 
			if(isSpace(tb[0].getText())){
				DrawAnnouncement("Ma lop khong duoc nhap khoang trong");
				continue;
			}
			if(isEmpty(tb[1].getText())||isEmpty(tb[2].getText())){
				DrawAnnouncement("Ten lop va nien khoa khong duoc de trong");
				continue;
			}
			bool f=false;
			for(int i=0;i<sl;i++){
				if(stricmp(dsLop[i]->maLop,tb[0].getText())==0){
					DrawAnnouncement("Ma lop da ton tai");
					f=true;
					break;
				}
			}
			if(f) continue;
			add_Lop(dsLop,sl,tb[0].getText(),tb[1].getText(),tb[2].getText());
			DrawAnnouncement("Them lop thanh cong");
			return;
		}
		else tb[choosen].drawSelf(press_type);
	
		
	}
	//Ve khung nhap
}

void hieuChinhLop(Lop* dsLop[],int& sl){//Khong duoc sua ma lop


	int choosen=0;
	TextBox* tb[2];


	SetBGColor(maunen);
	DrawRectangle(50,13,40,5);
	
	//Nhap vao ma lop
	int vitrisuadoi=-1;
	TextBox input_ma_lop = TextBox(15,70,12,"Nhap vao ma lop: ");	
	
	
	while(true){//Vong lap nhap vao ma lop
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		else if(press_type==ENTER){
				//Tim kiem ma lop xem co ton tai ma lop khong
				bool f=false;
				for(int i=0;i<sl;i++){
					if(stricmp(input_ma_lop.getText(),dsLop[i]->maLop)==0){
						vitrisuadoi=i;//Vi tri can sua doi
						
						///Chuyen trang thai di den hieu chinh lop
						f=true;
						break;
						
					}
				}
				if(!f)DrawAnnouncement("Khong tim thay lop");
				else {
					DrawAnnouncement("Tim thay lop");
					
					
					inDanhSachLop(dsLop,sl);
					
					SetBGColor(maunen);
					DrawRectangle(15,15-3,140,7);
						
					tb[0]=new TextBox(15,30,100,"Ten lop: ");
					tb[1]=new TextBox(17,30,10,"Nien khoa: ");
					
					break;
				}
			}
		else input_ma_lop.drawSelf(press_type);
	
	}
	
	
		//Vong lap hieu chinh ma lop
	while(true){
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+1)%2;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%2;
			}
		}
		else if(press_type==ESC){
			//Xoa lop
			if(dsLop[vitrisuadoi]->dsSV!=NULL){
				DrawAnnouncement("Khong the xoa lop nay vi co du lieu sinh vien");
				continue;
			}
			delete_Lop(dsLop,sl,vitrisuadoi);
			DrawAnnouncement("Xoa lop thanh cong");
			return;
		}
		else if(press_type==ENTER){
			if(isEmpty(tb[0]->getText())||isEmpty(tb[1]->getText())){
				DrawAnnouncement("Ten lop va nien khoa khong duoc de trong");
				continue;
			}
			modify_Lop(dsLop,vitrisuadoi,tb[0]->getText(),tb[1]->getText());
			DrawAnnouncement("Hieu chinh lop thanh cong");
			return;
		}
		else tb[choosen]->drawSelf(press_type);
	}
}



//Ham goi tat ca chuc nang cua lop
void quanLyLop(Lop* dsLop[],int& sl){
	inDanhSachLop(dsLop,sl);
	Lop* tmp[MAX_LOP];//Danh sach sach lop theo nien khoa
	int cnt=0;
	
	TextBox nien_khoa(5,80,10,"Nhap vao nien khoa: ");//Nhap vao nien khoa de in ra
	while(true){//Chon chuc nang
		int type=getch();
		if(type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(type==0){//Bat phim F1,F2
			type=getch();
			if(type==F1){//Them lop 
				themLop(dsLop,sl);
			}
			else if(type==F2){//Hieu chinh lop
				hieuChinhLop(dsLop,sl);	
			}
			
			
			//Cap nhat, ve lai danh sach
			if(stricmp(nien_khoa.getText(),"")==0){
				inDanhSachLop(dsLop,sl);
				nien_khoa.redraw();
			}
			else{
				cnt=0;
				for(int i=0;i<sl;i++){
					if(stricmp(dsLop[i]->nienKhoa,nien_khoa.getText())==0){
						tmp[cnt++]=dsLop[i];
					}
				}
				
				inDanhSachLop(tmp,cnt);
				nien_khoa.redraw();
			}
		
		}
		else {
			nien_khoa.drawSelf(type);
			if(stricmp(nien_khoa.getText(),"")==0){
				inDanhSachLop(dsLop,sl);
				nien_khoa.redraw();
			}
			else{
				cnt=0;
				for(int i=0;i<sl;i++){
					if(stricmp(dsLop[i]->nienKhoa,nien_khoa.getText())==0){
						tmp[cnt++]=dsLop[i];
					}
				}
				
				inDanhSachLop(tmp,cnt);
				nien_khoa.redraw();
			}
		}
	}
}

/*----------------------------------- Quan sinh vien --------------------------------------*/
void inDanhSachSinhVien(Lop* dsLop){
	SetBGColor(0);
	system("cls");

	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	TextColor(WHITE);
	SetBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA SV";
	gotoxy(startcol+12,startrow-3);
	cout<<"|HO";
	gotoxy(startcol+40,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+60,startrow-3);
	cout<<"|PHAI";
	gotoxy(startcol+70,startrow-3);
	cout<<"|PASSWORD";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	NodeSV* node=dsLop->dsSV;
	int i=0;
	while(node!=NULL)
	{
		gotoxy(startcol,startrow+i);
		cout<<node->sinhVien.maSV;
		gotoxy(startcol+12,startrow+i);
		cout<<node->sinhVien.ho;
		gotoxy(startcol+40,startrow+i);
		cout<<node->sinhVien.ten;
		gotoxy(startcol+60,startrow+i);
		cout<<node->sinhVien.phai;
		gotoxy(startcol+70,startrow+i);
		cout<<node->sinhVien.password;
		node=node->next;
		i++;
	}
}
void quanLySinhVien(Lop* dsLop[],int& sl){
	SetBGColor(0);
	system("cls");
	
	DrawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them  ",false);
		
	SetBGColor(maunen);
	DrawRectangle(30,18,60,4);
	TextBox ma_lop(20,60,12,"Nhap vao ma lop: ");
	int index=-1;
	while(true){
		//Vong lap nhap ma lop
		int type=getch();
		if(type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(type==ENTER){
			//Vong lap kiem tra ma lop co ton tai khong
			bool f=false;
			for(int i=0;i<sl;i++){
				if(stricmp(dsLop[i]->maLop,ma_lop.getText())==0){
					DrawAnnouncement("Tim thay lop");
					f=true;
					index=i;
					break;//Den vong lap in danh sach sinh vien
					
				}
			}
			if(f) break;
			DrawAnnouncement("Khong thay lop");
		}
		else ma_lop.drawSelf(type);
	}
	
	inDanhSachSinhVien(dsLop[index]);
	while(true){
		int press_type=getch();
		if(press_type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(press_type==0){//Bat phim F1
			press_type=getch();
			if(press_type==F1){//Them sinh vien
				char combo_text[2][30]={{"NAM"},{"NU"}};
				SetBGColor(maunen);
				DrawRectangle(38,9,60,12);
				TextBox tb[4]={	TextBox(10,60,11,"Ma sinh vien: "), TextBox(12,60,30,"Ho: "), TextBox(14,60,30,"Ten: "),TextBox(16,60,20,"Password: ")};// ma sv, ho, ten, password 
				ComboBox phai(18,60,combo_text,2);//phai
				int choosen=0;
				//Nhap sinh vien
				while(true){
					press_type=getch();
					if(press_type==TAB){//An nut tab thoat khoi chuc nang
						return;
					}
					if(press_type==DOUBLE_KEY){
						press_type=getch();
						if( press_type==LEFT) {
							choosen=(choosen+4)%5;
						}
						if( press_type==RIGHT) {
							choosen=(choosen+1)%5;
						}
					}
					else if(press_type==ENTER){
						bool f=false;
						for(int i=0;i<sl;i++){
								NodeSV* head=dsLop[i]->dsSV;
								while(head!=NULL){
									if(stricmp(head->sinhVien.maSV,tb[0].getText())==0){
										DrawAnnouncement("Da ton tai ma sinh vien nay");//Mot sinh vien thi chi duoc o mot lop
										f=true;
										break;
									}
										head=head->next;
								}
								if(f) break;
						}
						if(f) continue;
						if(isSpace(tb[0].getText())){
							DrawAnnouncement("Ma sinh vien khong the chu khoang trong");
							continue;
						}
						if(isEmpty(tb[1].getText())||isEmpty(tb[2].getText())||isEmpty(tb[3].getText())){
							DrawAnnouncement("Ho ten password khong duoc de trong");
							continue;
						}
						NodeSV* node=createNode_SinhVien(tb[0].getText(),tb[1].getText(),tb[2].getText(),phai.getText(),tb[3].getText());
						insertHead_SinhVien(dsLop[index]->dsSV,node);
						DrawAnnouncement("Them sinh vien thanh cong");
						break;
					}
					else {
						if(choosen==4) phai.drawSelf(press_type);
						else tb[choosen].drawSelf(press_type);
					}
				}
				inDanhSachSinhVien(dsLop[index]);
			}
		}
	}
	
	

}
/*----------------------------------- Quan ly mon hoc --------------------------------------*/
void inDanhSachMonHoc(MonHoc* dsMH[],int& sl){
	SetBGColor(0);
	system("cls");
	
	DrawTextBox(10,0,1,20,BLACK,WHITE,"F1: Them      F2: Hieu chinh     F2->Esc: Xoa",false);

	int startrow=10,startcol=30;
	//Chinh mau chu thanh chu trang
	TextColor(WHITE);
	SetBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA MON HOC";
	gotoxy(startcol+30,startrow-3);
	cout<<"|TEN MON HOC";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	//
	
	
	for(int i=0;i<sl;i++){
		gotoxy(startcol,startrow+i);
		cout<<dsMH[i]->maMH;
		gotoxy(startcol+30,startrow+i);
		cout<<dsMH[i]->tenMH;
	}
}

void themMonHoc(MonHoc* dsMH[],int& sl){



	int choosen=0;
	SetBGColor(maunen);
	DrawRectangle(30,8,90,6);
	TextBox tb[2]={	TextBox(9,50,15,"Ma mon hoc: "), TextBox(11,50,50,"Ten mon hoc: ")};// ma mon hoc, ten mon hoc

	while(true){//Nhap vao 
		int press_type=getch();
		if(press_type==TAB){//Phim tab tro ve menu quan ly Lop
			return;
		}
		else if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+1)%2;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%2;
			}
		}
		else if(press_type==ENTER){
			bool f=false;
			for(int i=0;i<sl;i++){
				if(stricmp(dsMH[i]->maMH,tb[0].getText())==0){
					f=true;
					DrawAnnouncement("Ma mon hoc nay da ton tai");
					break;
				}
			}
			if(f) continue;
			if(isSpace(tb[0].getText())){
				DrawAnnouncement("Ma mon hoc khong duoc chua khoang trang");
				continue;
			}
			if(isEmpty(tb[1].getText())){
				DrawAnnouncement("Ten mon hoc khong duoc de trong");
				continue;
			}
			add_MonHoc(dsMH,sl,tb[0].getText(),tb[1].getText());
			DrawAnnouncement("Them lop mon hoc thanh cong");
			return;
		}
		else tb[choosen].drawSelf(press_type);
	
		
	}
	//Ve khung nhap
}

void hieuChinhMonHoc(NodeCHT* dsCHT, MonHoc* dsMH[],int& sl){//Khong duoc ma mon hoc
	SetBGColor(maunen);
	DrawRectangle(28,13,50,5);
	
	//Nhap vao ma lop
	int vitrisuadoi=-1;
	TextBox input_ma_mon_hoc = TextBox(15,50,15,"Nhap vao ma mon hoc: ");

	int choosen=0;//Vi tri nhap vao khi hieu chinh
	TextBox* tb;



	
	
	
	
	while(true){//Vong lap nhap vao ma mon hoc
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		else if(press_type==ENTER){
				//Tim kiem ma lop xem co ton tai ma mon hoc
				bool f=false;
				for(int i=0;i<sl;i++){
					if(stricmp(input_ma_mon_hoc.getText(),dsMH[i]->maMH)==0){
						vitrisuadoi=i;//Vi tri can sua doi
						
						///Chuyen trang thai di den hieu chinh mon hoc
						f=true;
						break;
						
					}
				}
				if(!f)DrawAnnouncement("Khong tim thay mon hoc");
				else {
					DrawAnnouncement("Tim thay mon hoc");
					
					inDanhSachMonHoc(dsMH,sl);
					
					SetBGColor(maunen);
					DrawRectangle(20,12,80,5);
						
					tb=new TextBox(15,40,50,"Ten mon hoc: ");
				
					
					break;
				}
			}
		else input_ma_mon_hoc.drawSelf(press_type);
	
	}
	
	
		//Vong lap hieu chinh ten mon hoc
	while(true){
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		else if(press_type==ESC){
			NodeCHT* dsThi[1000];
			int slch=0;
			timKiemCauHoiThiTheoMaMonHoc(dsCHT, dsThi,dsMH[vitrisuadoi]->maMH, slch);
			if(slch>0){
				DrawAnnouncement("Khong the xoa mon hoc nay vi co cau hoi cho mon hoc nay");
				continue;
			}
			//Xoa mon hoc
			delete_MonHoc(dsMH,sl,vitrisuadoi);
			DrawAnnouncement("Xoa mon hoc thanh cong");
			return;
		}
		else if(press_type==ENTER){
			if(isEmpty(tb->getText())){
				DrawAnnouncement("Ten mon hoc khong duoc de trong");
				continue;
			}
			modify_MonHoc(dsMH,vitrisuadoi,tb->getText());
			DrawAnnouncement("Hieu chinh mon hoc thanh cong");
			return;
		}
		else tb->drawSelf(press_type);
	}
}



//Ham goi tat ca chuc nang cua lop
void quanLyMonHoc(NodeCHT* dsCHT, MonHoc* dsMH[],int& sl){
	inDanhSachMonHoc(dsMH,sl);
	while(true){//Chon chuc nang
		int type=getch();
		if(type==TAB){//An nut tab thoat khoi chuc nang
			return;
		}
		else if(type==0){//Bat phim F1,F2
			type=getch();
			if(type==F1){
				themMonHoc(dsMH,sl);
			}
			else if(type==F2){
				hieuChinhMonHoc(dsCHT, dsMH,sl);	
			}
			inDanhSachMonHoc(dsMH,sl);
		
		}
	}
}


/*----------------------------------- Them cau hoi thi --------------------------------------*/
void themCauHoiThi(NodeCHT* &dsCHT, MonHoc* dsMH[], int& sl){
	SetBGColor(0);
	system("cls");
	
	SetBGColor(maunen);
	DrawRectangle(28,12,40,5);
	
	int vitrisuadoi=-1;
	TextBox input_ma_mon_hoc = TextBox(15,50,15,"Nhap vao ma mon hoc: ");

	


	//Them cau hoi thi
	int choosen=0;
	TextBox* tb[5];
	ComboBox* dap_an;



	
	
	
	
	while(true){//Vong lap nhap vao ma mon hoc
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		else if(press_type==ENTER){
				//Tim kiem ma lop xem co ton tai ma mon hoc
				bool f=false;
				for(int i=0;i<sl;i++){
					if(stricmp(input_ma_mon_hoc.getText(),dsMH[i]->maMH)==0){
						vitrisuadoi=i;//Vi tri can sua doi
						
						///Chuyen trang thai di den hieu chinh mon hoc
						f=true;
						break;
						
					}
				}
				if(!f)DrawAnnouncement("Khong tim thay mon hoc");
				else {
					DrawAnnouncement("Tim thay mon hoc");
					
				
					
					SetBGColor(0);
					system("cls");
					SetBGColor(maunen);
					DrawRectangle(12,6,140,16);
						
					tb[0]=new TextBox(8,40,100,"Noi dung: ");
					tb[1]=new TextBox(12,40,100,"Cau A: ");
					tb[2]=new TextBox(14,40,100,"Cau B: ");
					tb[3]=new TextBox(16,40,100,"Cau C: ");
					tb[4]=new TextBox(18,40,100,"Cau D: ");
					
					char arr[4][30]={{"A"},{"B"},{"C"},{"D"}};		
					dap_an=new ComboBox(20,40,arr,4);
					
					
					break;
				}
			}
		else input_ma_mon_hoc.drawSelf(press_type);
	
	}
	
//	SetBGColor(0);
//	system("cls");
//	SetBGColor(maunen);
//	DrawRectangle(12,6,140,16);
//	for(int i=0;i<5;i++) tb[i]->redraw();	
//	dap_an->redraw();

	while(true){
		int press_type=getch();
		if(press_type==TAB){
			return;
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
		else if(press_type==ENTER){
			if(isEmpty(tb[0]->getText())||isEmpty(tb[1]->getText())||isEmpty(tb[2]->getText())||isEmpty(tb[3]->getText())||isEmpty(tb[4]->getText())){
				DrawAnnouncement("Noi dung va dap an khong duoc de trong");
				continue;
			}
			NodeCHT* new_node=NULL;
			do{
				new_node=createNode_CauHoiThi(rand()%1000000,dsMH[vitrisuadoi]->maMH,tb[0]->getText(),tb[1]->getText(),tb[2]->getText(),tb[3]->getText(),tb[4]->getText(),dap_an->getText()[0]);
			}while(insertNode_CauHoiThi(dsCHT,new_node)==NULL);
			DrawAnnouncement("Them cau hoi thi thanh cong");
		}
		else {
			if(choosen==5){
				dap_an->drawSelf(press_type);
			}
			else
			tb[choosen]->drawSelf(press_type);
		}
	}
}


///////////////////////////////////////////////////////// BAT DAU XU LY LUONG////////////////////////////////////////////////////////////////////


void batDauDemThoiGian(int time_in_second,int* f,int *ended)
{
	int hour,minute,second,tmp;
	char hour_t[4], minute_t[4], second_t[4];
	char time_text[20]="";
	
	
	
	while(*ended==0&&time_in_second>0){
		tmp=time_in_second;
		hour=tmp/(60*60);
		tmp%=(60*60);
		minute=tmp/60;
		tmp%=60;
		second=tmp;
		
		
		itoa(hour,hour_t,10);
		itoa(minute,minute_t,10);
		itoa(second,second_t,10);
		
		if(hour<10) {
			hour_t[1]=hour_t[0];
			hour_t[0]='0';
		}
		if(minute<10) {
			minute_t[1]=minute_t[0];
			minute_t[0]='0';
		}
		if(second<10) {
			second_t[1]=second_t[0];
			second_t[0]='0';
		}
		
		hour_t[2]='\0';
		minute_t[2]='\0';
		second_t[2]='\0';
		
		
		
		strcpy(time_text,"");
		
		strcat(time_text,hour_t);
		strcat(time_text,":");
		strcat(time_text,minute_t);
		strcat(time_text,":");
		strcat(time_text,second_t);
		
		
		
		while(*f!=1);
		*f=0;
		DrawTextBox(2,0,1,16,GREEN,WHITE,time_text,true);
		*f=1;
		sleep(1);
		time_in_second--;
	}
	*ended=1;
	DrawTextBox(10,20,3,30,maucoban,maucuachu,"Het gio",true);//Ve menu ve co ban
	
}


bool batDauThi(NodeCHT* dsCHT,SinhVien* sinh_vien,MonHoc* mon_hoc,int so_luong_cau,int time_in_minute){
	int so_luong_cau_thuc_te=0;
	NodeCHT* dsThi[1000];
	
	timKiemCauHoiThiTheoMaMonHoc(dsCHT,dsThi,mon_hoc->maMH,so_luong_cau_thuc_te);

	if(so_luong_cau_thuc_te<so_luong_cau){
		char tmp1[10];
		itoa(so_luong_cau_thuc_te,tmp1,10);
		char tmp2[100]="So luong cau hoi cua mon nay la: ";
		strcat(tmp2,tmp1);
		DrawAnnouncement(tmp2);
		return false;	
	}
	
	//Random  sort
	for(int i=0;i<so_luong_cau_thuc_te;i++){
		int index=rand()%so_luong_cau_thuc_te;
		swap(dsThi[i],dsThi[index]);
	}
	
	SetBGColor(0);
	system("cls");
	DrawTextBox(20,0,1,20,BLACK,WHITE,"F1: Trang truoc      F2: Trang sau     Tab: Nop bai",false);
	
	DrawAnnouncement("Bat dau lam bai");
	
	const int COUNT_QUES_PAGE=4;
	int page_index=0;
	int page_count=ceil(so_luong_cau*1./COUNT_QUES_PAGE);
	int current_page_ques_count=min(so_luong_cau,COUNT_QUES_PAGE);
	
	int choosen=0;
	ExamBox current_ques[so_luong_cau];
	int row=5;
	for(int i=0;i<so_luong_cau;i++) {
		current_ques[i].cauHoiThi=&dsThi[i]->cauHoiThi;
	}
	for(int i=page_index*COUNT_QUES_PAGE;i<page_index*COUNT_QUES_PAGE+current_page_ques_count;i++){
					current_ques[i].col=3;
					current_ques[i].row=row+7*(i-page_index*COUNT_QUES_PAGE);
					current_ques[i].index=i+1;
					current_ques[i].cauHoiThi=&dsThi[i]->cauHoiThi;
					current_ques[i].redraw(choosen==i?true:false);
				
	}

	
	

	
	int f=1;
	int ended=0;
	//Vong lap bat dau thi
	thread timer(batDauDemThoiGian,time_in_minute*60,&f,&ended);
	

	while(!ended){
		int press_type=getch();
		if(press_type==TAB){
			ended=1;
		}
		else if(press_type==0){//Bat phim F1,F2
			press_type=getch();
			if(press_type==F1){//
			
				while(f!=1);
				f=0;
				page_index=max(page_index-1,0);
				SetBGColor(0);
				system("cls");
				DrawTextBox(20,0,1,20,BLACK,WHITE,"F1: Trang truoc      F2: Trang sau     Tab: Nop bai",false);
				
				choosen=page_index*COUNT_QUES_PAGE;
				current_page_ques_count=min(so_luong_cau-page_index*COUNT_QUES_PAGE,COUNT_QUES_PAGE);
				for(int i=page_index*COUNT_QUES_PAGE;i<page_index*COUNT_QUES_PAGE+current_page_ques_count;i++){
					current_ques[i].col=3;
					current_ques[i].row=row+7*(i-page_index*COUNT_QUES_PAGE);
					current_ques[i].index=i+1;
					current_ques[i].cauHoiThi=&dsThi[i]->cauHoiThi;
					current_ques[i].redraw(choosen==i?true:false);
				
				}
			
				f=1;
			
			}
			else if(press_type==F2){
				page_index=min(page_count-1,page_index+1);
				SetBGColor(0);
				system("cls");
				DrawTextBox(20,0,1,20,BLACK,WHITE,"F1: Trang truoc      F2: Trang sau     Tab: Nop bai",false);
				while(f!=1);
				f=0;
				choosen=page_index*COUNT_QUES_PAGE;
				current_page_ques_count=min(so_luong_cau-page_index*COUNT_QUES_PAGE,COUNT_QUES_PAGE);
				for(int i=page_index*COUNT_QUES_PAGE;i<page_index*COUNT_QUES_PAGE+current_page_ques_count;i++){
					current_ques[i].col=3;
					current_ques[i].row=row+7*(i-page_index*COUNT_QUES_PAGE);
					current_ques[i].index=i+1;
					current_ques[i].cauHoiThi=&dsThi[i]->cauHoiThi;
					current_ques[i].redraw(choosen==i?true:false);
					current_ques[i].index=i;
				}
			
				f=1;
			}
		}
		else if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				while(f!=1);
				f=0;
				current_ques[choosen].redraw(false);
				choosen=page_index*COUNT_QUES_PAGE+((choosen+current_page_ques_count-1)%current_page_ques_count);
				current_ques[choosen].redraw(true);
				f=1;
			}
			if( press_type==RIGHT) {
				while(f!=1);
				f=0;
				current_ques[choosen].redraw(false);
				choosen=page_index*COUNT_QUES_PAGE+((choosen+1)%current_page_ques_count);
				current_ques[choosen].redraw(true);
				f=1;
			}
			else
			{
					while(f!=1);
					f=0;
					current_ques[choosen].drawSelf(press_type);
					f=1;
			}
		}
		else{
			while(f!=1);
			f=0;
			current_ques[choosen].drawSelf(press_type);
			f=1;
		}
	
	}
	int cnt=0;
	for(int i=0;i<so_luong_cau;i++){
		cnt+=current_ques[i].checkAnswer();
	}
	float diem=round((cnt*10./so_luong_cau)*10)/10;
	if(sinh_vien!=NULL){//Neu SV==NULL thi hien tai la GV
		NodeDT* dt=createNode_DiemThi(mon_hoc->maMH,diem);
		dt->diemThi.soLuongCau=so_luong_cau;
		dt->diemThi.thoiGianThi=time_in_minute;
		for(int i=0;i<so_luong_cau;i++){
			dt->diemThi.chiTiet[i]=new ChiTietThi;
			dt->diemThi.chiTiet[i]->quesID=current_ques[i].cauHoiThi->id;
			dt->diemThi.chiTiet[i]->answer=current_ques[i].choosen;
		}
		insertHead_DiemThi(sinh_vien->dsDT,dt);
	}
	
	timer.join();
	char tmp3[100]="So cau dung: ";
	char tmp4[20]="";
	itoa(cnt,tmp4,10);
	strcat(tmp3,tmp4);
	strcat(tmp3,"/");
	itoa(so_luong_cau,tmp4,10);
	strcat(tmp3,tmp4);
	DrawTextBox(10,20,3,30,maucoban,maucuachu,tmp3,true);
	DrawTextBox(10,22,3,30,maucoban,maucuachu,"Diem: ",true);
	DrawTextBox_Float(18,22,3,30,maucoban,maucuachu,diem);
	
	
	getch();
	
	return true;
	

}





void thiTracNghiem(NodeCHT* &dsCHT,SinhVien* sinh_vien,MonHoc* dsMH[],int& sl){
	SetBGColor(0);
	system("cls");
	int vitrisuadoi=-1;
	
	SetBGColor(maunen);
	DrawRectangle(20,8,40,15);
	int choosen=0;
	TextBox tb[3]={TextBox(10,40,15,"Ma mon hoc: "),TextBox(12,40,3,"So cau thi: "),TextBox(14,40,3,"So phut thi: ")};



	
	
	
	
	while(true){//Vong lap nhap vao ma mon hoc
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+2)%3;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%3;
			}
		}
		else if(press_type==ENTER){
				//Tim kiem ma lop xem co ton tai ma mon hoc
				bool f=false;
				for(int i=0;i<sl;i++){
					if(stricmp(tb[0].getText(),dsMH[i]->maMH)==0){
						vitrisuadoi=i;//Vi tri can sua doi
						
						f=true;
						break;
						
					}
				}
				if(!f)DrawAnnouncement("Khong tim thay mon hoc");
				else {
					DrawAnnouncement("Tim thay mon hoc");
					
					if((!(isInteger(tb[1].getText())&&isInteger(tb[2].getText())))||strcmp(tb[1].getText(),"0")==0||strcmp(tb[2].getText(),"0")==0){
						DrawAnnouncement("So cau, so phut thi phai la so nguyen duong >0");
						continue;
					}
					
					if(sinh_vien!=NULL){//Truong hop khong phai giao vien
						bool f=false;
						NodeDT* dt=sinh_vien->dsDT;
						while(dt!=NULL){
							if(stricmp(dt->diemThi.maMH,dsMH[vitrisuadoi]->maMH)==0){
								DrawAnnouncement("Sinh vien da thi mon nay roi");
								f=true;
								break;
							}
							if(f) break;
							dt=dt->next;
						}
						if(f) continue;
					}
					
					
					int num=atoi(tb[1].getText());
					int minute=atoi(tb[2].getText());	
					
					if(batDauThi(dsCHT,sinh_vien,dsMH[vitrisuadoi],num,minute)) return;
					else{
						SetBGColor(0);
						system("cls");
						SetBGColor(maunen);
						DrawRectangle(20,8,40,15);
						tb[0].redraw();
						tb[1].redraw();
						tb[2].redraw();
					}
				}
				
			}
		else tb[choosen].drawSelf(press_type);
	
	}
	
	
	
}



/*----------------------------------------------In bang diem----------------------------------------*/
void inBangDiem(Lop* dsLop[],int& slLop, MonHoc* dsMH[], int& slMH){
	SetBGColor(0);
	system("cls");
	SetBGColor(maunen);
	DrawRectangle(20,9,55,5);
	
	int vi_tri_mh=-1,vi_tri_lop=-1;
	TextBox tb[2]={TextBox(10,40,12,"Ma lop: "),TextBox(12,40,15,"Ma mon hoc:")};//lop, mon hoc
	int choosen=0;
	
	



	
	
	
	
	
	while(true){//Vong lap nhap vao ma mon hoc
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+1)%2;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%2;
			}
		}
		else if(press_type==ENTER){
				//Tim kiem ma lop xem co ton tai ma mon hoc
				bool f1=false,f2=false;
				for(int i=0;i<slLop;i++){
					if(stricmp(tb[0].getText(),dsLop[i]->maLop)==0){
						vi_tri_lop=i;//Vi tri can sua doi
						
						///Chuyen trang thai di den hieu chinh lop
						f1=true;
						
					}
				}
				for(int i=0;i<slMH;i++){
					if(stricmp(tb[1].getText(),dsMH[i]->maMH)==0){
						vi_tri_mh=i;//Vi tri can sua doi
						
						///Chuyen trang thai di den hieu chinh mon hoc
						f2=true;
					}
				}
				if(!f2)DrawAnnouncement("Khong tim thay mon hoc");
				else if(!f1)DrawAnnouncement("Khong tim thay lop");
				else {
					DrawAnnouncement("In bang diem");
					break;
					
				}
			}
		else tb[choosen].drawSelf(press_type);
	}
	
	
	SetBGColor(0);
	system("cls");
	int startrow=10,startcol=30;
	TextColor(WHITE);
	SetBGColor(0);
	
	//Khung cua bang
	gotoxy(startcol,startrow-3);
	cout<<"|MA SV";
	gotoxy(startcol+20,startrow-3);
	cout<<"|HO SV";
	gotoxy(startcol+40,startrow-3);
	cout<<"|TEN SV";
	gotoxy(startcol+70,startrow-3);
	cout<<"|DIEM";

	
	int cnt=100;
	gotoxy(startcol,startrow-2);
	while(cnt--) cout<<'_';
	char* ma_mon_hoc=dsMH[vi_tri_mh]->maMH;
	NodeSV* node=dsLop[vi_tri_lop]->dsSV;
	int i=0;
	while(node!=NULL){
	
			gotoxy(startcol,startrow+i);
			cout<<node->sinhVien.maSV;
			gotoxy(startcol+20,startrow+i);
			cout<<node->sinhVien.ho;
			gotoxy(startcol+40,startrow+i);
			cout<<node->sinhVien.ten;
			
			
			bool f=false;
			NodeDT* dt=node->sinhVien.dsDT;
			while(dt!=NULL){
				if(stricmp(dt->diemThi.maMH,ma_mon_hoc)==0){	
				
					gotoxy(startcol+70,startrow+i);
					cout<<fixed<<setprecision(1)<<dt->diemThi.diem;
					f=true;
				}	
				
				dt=dt->next;
			}
			if(!f){
				gotoxy(startcol+70,startrow+i);
				cout<<"Chua thi";
			}
		i++;
		node=node->next;
	}
	
	getch();
	
	
}


void chiTietThi(NodeCHT* dsCHT,Lop* dsLop[],int& slLop, MonHoc* dsMH[], int& slMH){//Khong duoc ma mon hoc

	SetBGColor(0);
	system("cls");
	
	SetBGColor(maunen);
	DrawRectangle(20,8,40,6);
	

	int vi_tri_mh=-1;
	NodeSV *sinh_vien;
	TextBox tb[2]={TextBox(10,40,12,"Ma sinh vien: "),TextBox(12,40,15,"Ma mon hoc: ")};//ma sv, ma mon hoc
	int choosen=0;
	





	
	
	
	while(true){//Vong lap nhap vao ma mon hoc
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+1)%2;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%2;
			}
		}
		else if(press_type==ENTER){
				//Tim kiem ma lop xem co ton tai ma mon hoc
				bool f1=false,f2=false;
				for(int i=0;i<slLop;i++){
					NodeSV* head=dsLop[i]->dsSV;
					while(head!=NULL){
						if(stricmp(head->sinhVien.maSV,tb[0].getText())==0){
							sinh_vien=head;
							f1=true;
							break;
						}
						head=head->next;
					}
					if(f1) break;
				}
				for(int i=0;i<slMH;i++){
					if(stricmp(tb[1].getText(),dsMH[i]->maMH)==0){
						vi_tri_mh=i;
						
						///Chuyen trang thai di den hieu chinh mon hoc
						f2=true;
					}
				}
				if(!f2)DrawAnnouncement("Khong tim thay mon hoc");
				else if(!f1)DrawAnnouncement("Khong tim thay sinh vien");
				else {
					DrawAnnouncement("In chi tiet thi");
					break;
					
				}
			}
		else tb[choosen].drawSelf(press_type);
	}
	
	
	//Lay NodeDT
	NodeDT* ket_qua=sinh_vien->sinhVien.dsDT;
	while(ket_qua!=NULL&&stricmp(ket_qua->diemThi.maMH,dsMH[vi_tri_mh]->maMH)!=0){
		ket_qua=ket_qua->next;
	}
	if(ket_qua==NULL){
		DrawAnnouncement("Sinh vien chua thi mon nay");
		return;
	}
	
	//In thong tin ket qua thi
	
	SetBGColor(0);
	system("cls");
	DrawTextBox(15,0,1,10,GREEN,WHITE,(char*)("So cau: "+to_string(ket_qua->diemThi.soLuongCau)).c_str(),false);
	DrawTextBox(30,0,1,10,GREEN,WHITE,(char*)("Thoi gian lam: "+to_string(ket_qua->diemThi.thoiGianThi)).c_str(),false);
	DrawTextBox(50,0,1,10,GREEN,WHITE,"Diem thi: ",false);
	DrawTextBox_Float(60,0,1,10,GREEN,WHITE,ket_qua->diemThi.diem);
	
	int so_luong_cau=ket_qua->diemThi.soLuongCau;
	const int COUNT_QUES_PAGE=4;
	int page_index=0;
	int page_count=ceil(so_luong_cau*1./COUNT_QUES_PAGE);
	int current_page_ques_count=min(so_luong_cau,COUNT_QUES_PAGE);
	
	
	ExamBox current_ques[so_luong_cau];
	int row=5;
	for(int i=0;i<so_luong_cau;i++) {
		current_ques[i].cauHoiThi=&search(dsCHT,ket_qua->diemThi.chiTiet[i]->quesID)->cauHoiThi;//Tim kiem cau hoi theo id 
		current_ques[i].choosen=ket_qua->diemThi.chiTiet[i]->answer;
	}
	for(int i=page_index*COUNT_QUES_PAGE;i<page_index*COUNT_QUES_PAGE+current_page_ques_count;i++){
					current_ques[i].col=3;
					current_ques[i].row=row+7*(i-page_index*COUNT_QUES_PAGE);
					current_ques[i].index=i+1;
					current_ques[i].drawAnswer();
	}

	
	
	while(true){
		int press_type=getch();
		if(press_type==TAB){
			return;
		}
		else if(press_type==0){//Bat phim F1,F2
			press_type=getch();
			if(press_type==F1){//
				page_index=max(page_index-1,0);
				SetBGColor(0);
				system("cls");
			
				choosen=page_index*COUNT_QUES_PAGE;
				current_page_ques_count=min(so_luong_cau-page_index*COUNT_QUES_PAGE,COUNT_QUES_PAGE);
				for(int i=page_index*COUNT_QUES_PAGE;i<page_index*COUNT_QUES_PAGE+current_page_ques_count;i++){
					current_ques[i].col=3;
					current_ques[i].row=row+7*(i-page_index*COUNT_QUES_PAGE);
					current_ques[i].index=i+1;
					current_ques[i].drawAnswer();
				}
			
			
			
			}
			else if(press_type==F2){
				page_index=min(page_count-1,page_index+1);
				SetBGColor(0);
				system("cls");
			
				choosen=page_index*COUNT_QUES_PAGE;
				current_page_ques_count=min(so_luong_cau-page_index*COUNT_QUES_PAGE,COUNT_QUES_PAGE);
				for(int i=page_index*COUNT_QUES_PAGE;i<page_index*COUNT_QUES_PAGE+current_page_ques_count;i++){
					current_ques[i].col=3;
					current_ques[i].row=row+7*(i-page_index*COUNT_QUES_PAGE);
					current_ques[i].index=i+1;
					current_ques[i].drawAnswer();
				}
			
			}
		}
		
	}
}

void drawMenu(){
	//Data
	NodeSV* sinh_vien=NULL;
	Data data;
	
	readMonHoc(data.dsMH,data.countMH);
	readCauHoiThi(data.dsCHT);
	readLop(data.dsLop,data.countLop);



	
	//Login 
	int choosen=0;
	TextBox tb[2]={	TextBox(5,15,20,"Tai khoan: "),	TextBox(7,15,20,"Mat khau: ")};	
	while(true){
		int press_type=getch();
		if(press_type==DOUBLE_KEY){
			press_type=getch();
			if( press_type==LEFT) {
				choosen=(choosen+1)%2;
			}
			if( press_type==RIGHT) {
				choosen=(choosen+1)%2;
			}
		}
		else if(press_type==ENTER){
			bool f=false;
			for(int i=0;i<data.countLop;i++){
				NodeSV* head=data.dsLop[i]->dsSV;
				while(head!=NULL){
					if(stricmp(head->sinhVien.maSV,tb[0].getText())==0&&stricmp(head->sinhVien.password,tb[1].getText())==0){
						sinh_vien=head;
						f=true;
						break;
					}
						head=head->next;
				}
				if(f) break;
			}
			if(stricmp(tb[0].getText(),"gv")==0&&stricmp(tb[1].getText(),"gv")==0){
				break;
			}
			if(f){
				DrawAnnouncement("Dang nhap thanh cong");
				break;
			}
			else{
				DrawAnnouncement("Tai khoan hoac mat khau khong chinh xac");
			}
			//Di toi menu chinh
		}
		else tb[choosen].drawSelf(press_type);	//Bat phim in duoc nhu a b c d e f 12 3  {} 
		
	}
	
	
	//Menu

	
	if(sinh_vien==NULL){
		Vemenu();
		while(true){
			int type=getch();
			if(type==ENTER){//Phim enter thuc hien chuc nang
				if(chonmenu==0){
					quanLyLop(data.dsLop,data.countLop);
					Vemenu();//Sau khi thuc hien cai gi do thi tro ve menu chinh
				}
				if(chonmenu==1){
					quanLySinhVien(data.dsLop,data.countLop);
					Vemenu();
				}
				if(chonmenu==2){
					quanLyMonHoc(data.dsCHT,data.dsMH,data.countMH);
					Vemenu();
				}
				if(chonmenu==3){
					themCauHoiThi(data.dsCHT,data.dsMH,data.countMH);
					Vemenu();
				}
				if(chonmenu==4){
					thiTracNghiem(data.dsCHT,&sinh_vien->sinhVien,data.dsMH,data.countMH);
					Vemenu();
				}
				if(chonmenu==5){
					inBangDiem(data.dsLop,data.countLop,data.dsMH,data.countMH);
					Vemenu();
				}
				if(chonmenu==6){
					chiTietThi(data.dsCHT,data.dsLop,data.countLop,data.dsMH,data.countMH);
					Vemenu();
				}
				if(chonmenu==7){
					writeMonHoc(data.dsMH,data.countMH);
					writeCauHoiThi(data.dsCHT);
					writeLop(data.dsLop,data.countLop);
					return;
				}
			}
			if(type==224){//Bat phim mui ten di chuyen len xuong
				type=getch();
				if(type==UP){
					DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
					chonmenu=max(chonmenu-1,0);
					DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
				}
				else if(type==DOWN){
					DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
					chonmenu=min(chonmenu+1,7);
					DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
				}
			}
		}
	}
	else{
		char* label[2]={"Thi trac nghiem","Luu va thoat"};
		SetBGColor(0);
		system("cls");
		DrawTextBox(startcol,startrow,3,30,mauchon,maucuachu,label[0],true);
		DrawTextBox(startcol,startrow+4,3,30,maucoban,maucuachu,label[1],true);	
		while(true){
			int type=getch();
			if(type==ENTER){
				if(chonmenu==0){
					thiTracNghiem(data.dsCHT,&sinh_vien->sinhVien,data.dsMH,data.countMH);
//					Vemenu();
				}
				if(chonmenu==1){
//					writeMonHoc(data.dsMH,data.countMH);
//					writeCauHoiThi(data.dsCHT);
					writeLop(data.dsLop,data.countLop);
					return;
				}
				SetBGColor(0);
				system("cls");
				DrawTextBox(startcol,startrow,3,30,mauchon,maucuachu,label[0],true);
				DrawTextBox(startcol,startrow+4,3,30,maucoban,maucuachu,label[1],true);	
			}
		
			if(type==224){//Bat phim mui ten di chuyen len xuong
					type=getch();
					if(type==UP){
						DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
						chonmenu=max(chonmenu-1,0);
						DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
					}
					else if(type==DOWN){
						DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,maucoban,maucuachu,label[chonmenu],true);//Ve menu ve co ban
						chonmenu=min(chonmenu+1,1);
						DrawTextBox(startcol,startrow+chonmenu+chonmenu*3,3,30,mauchon,maucuachu,label[chonmenu],true);//Ve menu tiep theo
					}
			}
		}
	}
	
	
}





/////////////////////////////////////////////////////// KET THUC XU LY MENU/////////////////////////////////////////////////////////////////////////



int main(){
	system("mode 1000");
	srand((unsigned)time(0));
	drawMenu();
//	SetBGColor(10);
//	gotoxy(10,3);
//	cout<<" "<<endl;	
//	DrawTextBox(10,10,20,60,GREEN,WHITE,"aaa",true);
//	DrawTextBox_Float(10,10,5,30,GREEN,WHITE,1.4223);
}
