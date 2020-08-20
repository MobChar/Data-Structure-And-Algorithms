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

#define PI 3.14
#define SONGAYMUONTOIDA (int)7

const int MAX_MATHE=1e9;
const int SOLUONGTOIDA=10000;


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

void DrawRectangle(int col,int row,int width,int height){
	for(int j=row;j-row<height;j++){
		for(int i=col;i-col<width;i++){
			gotoxy(i,j);
			cout<<" ";
		}
	}
}



struct Thoigian{
	int ngay,thang, nam;
};


struct Danhmucsach{
	char masach[10];
	int trangthai;
	char vitri[30];
};

struct NodeDMS{
	Danhmucsach danhmucsach;
	NodeDMS* next=NULL;
};


struct Dausach{
	char isbn[6];
	char tensach[30];
	char tacgia[30];
	char theloai[30];
	int sotrang;
	int namxuatban;
	
	//Con tro danh muc sach
	NodeDMS* dms=NULL;
};


struct DSdausach{
	Dausach* dausach[SOLUONGTOIDA];//Danh sach tuyen tinh la mot mang con tro
	int soluong;//so luong dau scah
};




enum Phai{Nam,Nu
};

struct Muontra{
	char masach[10];
	Thoigian ngaymuon,ngaytra;
	int trangthai;//0 la dang muon, 1 la da tra, 2 la lam mat sach
};

struct NodeMT{//Danh sach lien ket kep
	Muontra muontra;
	NodeMT* left=NULL;
	NodeMT* right=NULL;
};


struct Thedocgia{
	int mathe;
	char ho[30];
	char ten[30];
	Phai gioitinh;
	int trangthai;//0 la bi khoa, 1 la dang hoat dong
	
	NodeMT* First=NULL;//Danh sach muon tra
	NodeMT* Last=NULL;
	
};

struct NodeTDG{//Cay nhi phan tim kiem
	Thedocgia thedocgia;
	NodeTDG* left=NULL;
	NodeTDG* right=NULL;
};

NodeTDG* sapxepdocgia[SOLUONGTOIDA];

int soluongdocgia=0;
NodeTDG* thedocgia=NULL;//Cay nhi phan tim kiem
DSdausach dsdausach;

void InsertFirst(NodeMT* &first,NodeMT* &last,NodeMT* newnode){
	if(first==NULL) first=newnode,last=newnode;
	else{
		newnode->right=first;
		first->left=newnode;
		first=newnode;
	}
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

NodeTDG* CreateNode(int mathe,char* ho,char* ten,Phai gioitinh,int trangthai){
	NodeTDG* newnode=new NodeTDG;
	newnode->thedocgia.mathe=mathe;
	strcpy(newnode->thedocgia.ho,ho);
	strcpy(newnode->thedocgia.ten,ten);
	newnode->thedocgia.gioitinh=gioitinh;
	newnode->thedocgia.trangthai=trangthai;
	
	return newnode;
}
//
NodeTDG* Timkiemdocgia(NodeTDG* tree,int mathedocgia){
	NodeTDG* re=tree;
	while(re!=NULL){
		if(mathedocgia<re->thedocgia.mathe) re=re->left;
		else if(mathedocgia>re->thedocgia.mathe) re=re->right;
		else if(mathedocgia==re->thedocgia.mathe) return re;
	}
	return NULL;
}

NodeTDG* Chendocgia(NodeTDG* &tree,NodeTDG* thedocgiamoi){
	if(tree==NULL) {
		tree=thedocgiamoi;
		return thedocgiamoi;
	}
	NodeTDG* last=NULL;
	NodeTDG* vitrichen=tree;
	while(vitrichen!=NULL){
		last=vitrichen;
		if(thedocgiamoi->thedocgia.mathe<vitrichen->thedocgia.mathe) vitrichen=vitrichen->left;
		else if(thedocgiamoi->thedocgia.mathe>vitrichen->thedocgia.mathe) vitrichen=vitrichen->right;
		else if(thedocgiamoi->thedocgia.mathe==vitrichen->thedocgia.mathe) return NULL;//da co trong cay
	}
	if(thedocgiamoi->thedocgia.mathe<last->thedocgia.mathe) last->left=thedocgiamoi;
	else last->right=thedocgiamoi;
	
	return thedocgiamoi;
}

NodeTDG* GetMinNode(NodeTDG* root){
		while(root->left!=NULL) root=root->left;
		return root;
}

NodeTDG* Xoadocgia(NodeTDG* &root,NodeTDG* canxoa){//Ham nay se khong xoa vung nho vung nho du lieu se duoc tra lai
		if(root==NULL) return root;
		
		NodeTDG* renode;
		NodeTDG* nextnode;
		
		if(canxoa->thedocgia.mathe<root->thedocgia.mathe) {
			nextnode=root->left;
			renode=Xoadocgia(nextnode,canxoa);
			root->left=nextnode;
		}
		else if(canxoa->thedocgia.mathe>root->thedocgia.mathe) {
			nextnode=root->right;
			renode=Xoadocgia(nextnode,canxoa);
			root->right=nextnode;
		}
		else{//Truong hop bang
			if(root->right==NULL||root->left==NULL){//nut nay chua mot nut la hoac khong chu nut la nao, truong hop khong can xoay cay
				NodeTDG* tmp=root->right?root->right:root->left;
				
				if(tmp==NULL){//Truong hop nay khong can cap nhat lai cay
					NodeTDG* tmp=root;
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
				NodeTDG* tmp=GetMinNode(root->right);
	//			Node* tmp=GetMinValueNode(root->right);//Nut cuc trai cua cay con ben phai
	//			root->material=tmp->material;//Lay noi dung cu nut cuc trai
				//Sap chep key  va data cua nut cuc trai 
				root->thedocgia=tmp->thedocgia;
//				strcpy(root->key,tmp->key); 
//				root->data=tmp->data;
				
				
				nextnode=root->right;
				renode=Xoadocgia(nextnode,tmp);//Xoa nut cuc trai;
				root->right=nextnode;
			}
		}
		return renode;
	}





void Sapxepdocgiatheohoten(NodeTDG* docgia[],int l,int r){
	if (l <= r)
	{
		// Create a Key/Pivot Element
		string key=string(docgia[(l+r)/2]->thedocgia.ten)+string(docgia[(l+r)/2]->thedocgia.ho);
		// Create temp Variables to loop through array
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp((string(docgia[i]->thedocgia.ten)+string(docgia[i]->thedocgia.ho)).c_str(),key.c_str())<0)
				i++;
			while (stricmp((string(docgia[j]->thedocgia.ten)+string(docgia[j]->thedocgia.ho)).c_str(),key.c_str())>0)
				j--;
 
			if (i <= j)
			{
				swap(docgia[i],docgia[j]);
				i++;
				j--;
			}
		}
 
		// Recursion to the smaller partition in the array after sorted above
		// Reference Gi?i Thu?t Ð? Quy :: STDIO.VN
		if (l < j)
			Sapxepdocgiatheohoten(docgia, l, j);
		if (r > i)
		Sapxepdocgiatheohoten(docgia, i, r);
	}
	
}

void Sapxepdocgiatheomadocgia(NodeTDG* docgia[],int l,int r){
	if (l <= r)
	{
		// Create a Key/Pivot Element
		int key=docgia[(l+r)/2]->thedocgia.mathe;
		
 
		// Create temp Variables to loop through array
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (docgia[i]->thedocgia.mathe<key)
				i++;
			while (docgia[j]->thedocgia.mathe>key)
				j--;
 
			if (i <= j)
			{
				swap(docgia[i],docgia[j]);
				i++;
				j--;
			}
		}
 
		// Recursion to the smaller partition in the array after sorted above
		// Reference Gi?i Thu?t Ð? Quy :: STDIO.VN
		if (l < j)
			Sapxepdocgiatheomadocgia(docgia, l, j);
		if (r > i)
		Sapxepdocgiatheomadocgia(docgia, i, r);
	}
	
}

//	Muontra * dsmt[1000];
//	Thedocgia* docgiamuonsachquahan[1000];
//	Dausach* dausach[1000];
//	int songayquahan[1000];

void Sapxepdocgiatheoquahan(int songayquahan[],Muontra* dsmt[], Thedocgia* docgiamuonsachquahan[], Dausach* dausach[],int l,int r){
		if (l <= r)
	{
		// Create a Key/Pivot Element
		int key=songayquahan[(l+r)/2];
		
 
		// Create temp Variables to loop through array
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (songayquahan[i]>key)
				i++;
			while (songayquahan[j]<key)
				j--;
 
			if (i <= j)
			{
				swap(songayquahan[i],songayquahan[j]);
				swap(dsmt[i],dsmt[j]);
				swap(docgiamuonsachquahan[i],docgiamuonsachquahan[j]);
				swap(dausach[i],dausach[j]);
				i++;
				j--;
			}
		}
 
		// Recursion to the smaller partition in the array after sorted above
		// Reference Gi?i Thu?t Ð? Quy :: STDIO.VN
		if (l < j)
			Sapxepdocgiatheoquahan(songayquahan,dsmt,docgiamuonsachquahan,dausach, l, j);
		if (r > i)
		Sapxepdocgiatheoquahan(songayquahan,dsmt,docgiamuonsachquahan,dausach, i, r);
	}
}

void Sapxepdausach(DSdausach& dsdausach,int l,int r){
	if (l <= r)
	{
		// Create a Key/Pivot Element
		string keytheloai(dsdausach.dausach[(l+r)/2]->theloai);
		string keyten(dsdausach.dausach[(l+r)/2]->tensach);
	
 
		// Create temp Variables to loop through array
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp(dsdausach.dausach[i]->theloai,keytheloai.c_str())<0||(((stricmp(dsdausach.dausach[i]->theloai,keytheloai.c_str())==0)&&(stricmp(dsdausach.dausach[i]->tensach,keyten.c_str())<0))))
				i++;
			while (stricmp(dsdausach.dausach[j]->theloai,keytheloai.c_str())>0||(((stricmp(dsdausach.dausach[j]->theloai,keytheloai.c_str())==0)&&(stricmp(dsdausach.dausach[j]->tensach,keyten.c_str())>0)))){
				--j;
			}
 		
			if (i <= j)
			{
				swap(dsdausach.dausach[i],dsdausach.dausach[j]);
				i++;
				j--;
			}
		}
 
		// Recursion to the smaller partition in the array after sorted above
		// Reference Gi?i Thu?t Ð? Quy :: STDIO.VN
		if (l < j)
			Sapxepdausach(dsdausach, l, j);
		if (r > i)
		Sapxepdausach(dsdausach, i, r);
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







Dausach* Timkiemdausachbangisbn(DSdausach& ds,char* isbn){
	for(int i=0;i<ds.soluong;i++){
		if(stricmp(ds.dausach[i]->isbn,isbn)==0) return ds.dausach[i];
	}	
	return NULL;
}

Dausach* Timkiemdausachbangmasach(DSdausach& ds,char* masach){
	for(int i=0;i<ds.soluong;i++){
		NodeDMS* dms=ds.dausach[i]->dms;
		while(dms!=NULL){
			if(strcmp(dms->danhmucsach.masach,masach)==0) return ds.dausach[i];
			dms=dms->next;
		}
	}
	return NULL;
}

Danhmucsach* Timkiemdanhmucsach(DSdausach& ds,char* masach){
	for(int i=0;i<ds.soluong;i++){
		NodeDMS* dms=dsdausach.dausach[i]->dms;
		while(dms!=NULL){
			if(stricmp(dms->danhmucsach.masach,masach)==0) return &dms->danhmucsach;
			dms=dms->next;
		}
	}
	return NULL;
}

void NextString(char* cur,int len){
	int add=1;
	int i=len-2;//Ki tu cuoi la ki tu ket thuc
	while(add!=0){
		int val=(cur[i]-'0')+add;
		cur[i]=(val%10)+'0';
		if(val>9) add=1;
		else add=0;
		--i;
		if(i==-1){
			cout<<"He thong da vuot qua gioi han ma vat tu"<<endl;
			break;
		}
	}
}

char masach[10];

//////////////Docgia;
bool printbyname;

void Themdocgia(){
	char field[4][30];//ho, ten
	field[0][0]='\0';
	field[1][0]='\0';
	int chisofield=0;
	int col[4]={37,67,97,112};//noi bat dau ve 
	int width[]={29,29,3,19};
	int row=30;
	
	Phai tmpphai=Nam;
	int tmptrangthai=1;
	strcpy(field[2],"Nam");
	strcpy(field[3],"The dang hoat dong");
	
	char lable[4][20]={"Ho:","Ten:","Gioi tinh:","Trang thai the:"};
	//Ve 
	SetBGColor(38);
	DrawRectangle(35,row-3,100,5);
	
	for(int i=0;i<4;i++){
		if(i==chisofield){
			Vehopcochu(col[i],row,1,width[i],30,120,field[i],false);
		}
		else{
			Vehopcochu(col[i],row,1,width[i],0,120,field[i],false);
		}
		Vehopcochu(col[i],row-2,1,strlen(lable[i]),0,120,lable[i],false);
	}
	////
	
	while(true){
		int type=getch();
		if(type==9) {
			return;
		}
		if(type==13){
			//Luu tru va thoat
			
			if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])) {
				DrawAnnouncement("Du lieu nhap vao khong hop le");
				continue;
			}
		
			///////////////////////////////////////////Rong
			
			int newmathe=rand()%MAX_MATHE;
			while(Timkiemdocgia(thedocgia,newmathe)!=NULL){
				newmathe=rand()%MAX_MATHE;
			}
			
			Chuanhoa(field[0]);
			Chuanhoa(field[1]);
			NodeTDG* newnode=CreateNode(newmathe,field[0],field[1],tmpphai,tmptrangthai);
			sapxepdocgia[soluongdocgia++]=newnode;
			Chendocgia(thedocgia,newnode);
			
			DrawAnnouncement("Luu du lieu thanh cong");
			
			if(printbyname) Sapxepdocgiatheohoten(sapxepdocgia,0,soluongdocgia-1);
			else Sapxepdocgiatheomadocgia(sapxepdocgia,0,soluongdocgia-1);
			return;
			
		}
		else if(type==224){
				type=getch();
				if(type!=75&&type!=77) continue;
				
				Vehopcochu(col[chisofield],row,1,width[chisofield],0,120,field[chisofield],false);
				
				if(type==75) chisofield=max(chisofield-1,0);
				if(type==77) chisofield=min(chisofield+1,3);
				
				Vehopcochu(col[chisofield],row,1,width[chisofield],30,120,field[chisofield],false);

			}
		else if(chisofield==2||chisofield==3){
				if(chisofield==2){
					if(type==(int)'1') {
						strcpy(field[chisofield],"Nu");
						tmpphai=Nu;
						
					Vehopcochu(col[chisofield],row,1,width[chisofield],30,120,field[chisofield],false);
					}
					if(type==(int)'0') {
						strcpy(field[chisofield],"Nam");
						tmpphai=Nam;
					
					Vehopcochu(col[chisofield],row,1,width[chisofield],30,120,field[chisofield],false);
					}
				}
				if(chisofield==3){
					if(type==(int)'0'){
						tmptrangthai=0;
						strcpy(field[chisofield],"The bi khoa");
						Vehopcochu(col[chisofield],row,1,width[chisofield],30,120,field[chisofield],false);
					}
					if(type==(int)'1'){
						tmptrangthai=1;
						strcpy(field[chisofield],"The dang hoat dong");
						Vehopcochu(col[chisofield],row,1,width[chisofield],30,120,field[chisofield],false);
					}
					
				}	
			}
		else{
			if(type==8){
				int nextindex=max((int)strlen(field[chisofield])-1,0);
				field[chisofield][nextindex]='\0';
				SetBGColor(30);
				DrawRectangle(col[chisofield]+nextindex,row,1,1);
				continue;
			}
			int nextindex=strlen(field[chisofield]);
			if(nextindex>=width[chisofield]) continue;
			
			field[chisofield][nextindex]=(char)type;
			field[chisofield][nextindex+1]='\0';
			
			gotoxy(col[chisofield],row);
			TextColor(120);
			cout<<field[chisofield];
		}
	}
}
void Inquanlydocgia(int startcol, int startrow){
	//Sap xep lai doc gia
	if(printbyname) Sapxepdocgiatheohoten(sapxepdocgia,0,soluongdocgia-1);
	else Sapxepdocgiatheomadocgia(sapxepdocgia,0,soluongdocgia-1);
	
	//Xoa man hinh
	SetBGColor(0);
	system("cls");
	//Doi mau chu
	TextColor(30);
	SetBGColor(0);
	//In huong dan
	char huongdan[200]={"F1:Them doc gia     F2:Hieu chinh doc gia    F3:In theo ten/theo ma doc gia    Esc: Xoa doc gia trong menu hieu chinh doc gia    Tab:Thoat"};
	gotoxy(15,0);
	cout<<huongdan;
	//Doi mu chu
	TextColor(255);
	SetBGColor(0);
	
	gotoxy(startcol-1,startrow-3);
	cout<<"|MA THE";
	gotoxy(startcol+9,startrow-3);
	cout<<"|HO";
	gotoxy(startcol+39,startrow-3);
	cout<<"|TEN";
	gotoxy(startcol+69,startrow-3);
	cout<<"|PHAI";
	gotoxy(startcol+74,startrow-3);
	cout<<"|TRANG THAI THE";
	
	gotoxy(startcol-1,startrow-2);
	int cnt=90;
	while(cnt--) cout<<'_'; 
	
	for(int i=0;i<soluongdocgia;i++){
		gotoxy(startcol,startrow+i*2);
		cout<<sapxepdocgia[i]->thedocgia.mathe;
		gotoxy(startcol+10,startrow+i*2);
		cout<<sapxepdocgia[i]->thedocgia.ho;
		gotoxy(startcol+40,startrow+i*2);
		cout<<sapxepdocgia[i]->thedocgia.ten;
		gotoxy(startcol+70,startrow+i*2);
		if(sapxepdocgia[i]->thedocgia.gioitinh==Nam) cout<<"Nam";
		else cout<<"Nu";
		gotoxy(startcol+75,startrow+i*2);
		if(sapxepdocgia[i]->thedocgia.trangthai==0) cout<<"DA BI KHOA";
		else cout<<"CO THE SU DUNG";
		
	}
}
void Hieuchinhdocgia(){
	
	char kiemtra[10];
	kiemtra[0]='\0';
	int startrow=30,startcol=40;
	NodeTDG* docgia=NULL;
	
	char field[4][30];//ho, ten
	field[0][0]='\0';
	field[1][0]='\0';
	int chisofield=0;
	int col[4]={40,70,100,105};//noi bat dau ve 
	int width[]={29,29,3,19};
	
	Phai tmpphai;
	int tmptrangthai;
	
	
	char lable[4][20]={"Ho:","Ten:","Phai:","Trang thai the:"};
	//Ve 
	
//	strcpy(field[2],"Nam");
//	strcpy(field[3],"The dang hoat dong");

	SetBGColor(38);
	DrawRectangle(37,startrow-3,30,5);
	Vehopcochu(40,startrow-2,1,strlen("Nhap vao ma doc gia"),0,120,"Nhap vao ma doc gia",false);
	Vehopcochu(40,startrow,1,sizeof(kiemtra),30,120,kiemtra,false);
	
	//
	while(true){
		int type=getch();
		if(type==9) {
			return;
		}
		if(docgia==NULL&&type==13){
//				if(thedocgia==NULL) cout<<"NULL"<<endl;
				docgia=Timkiemdocgia(thedocgia,atoi(kiemtra));
				
				if(docgia!=NULL){
					//Ve lai background
					SetBGColor(0);
					system("cls");
 					Inquanlydocgia(40,5);
					/////////////////
					
					SetBGColor(38);
					DrawRectangle(39,startrow-3,100,5);
					
					for(int i=0;i<4;i++){
						if(i==chisofield){
							Vehopcochu(col[i],startrow,1,width[i],30,120,field[i],false);
						}
						else{
							Vehopcochu(col[i],startrow,1,width[i],0,120,field[i],false);
						}
						Vehopcochu(col[i],startrow-2,1,strlen(lable[i]),0,120,lable[i],false);
					}
	
					strcpy(field[0],docgia->thedocgia.ho);
					strcpy(field[1],docgia->thedocgia.ten);
					if(docgia->thedocgia.gioitinh==Nam) strcpy(field[2],"Nam");
					else strcpy(field[2],"Nu");
					tmpphai=docgia->thedocgia.gioitinh;
					tmptrangthai=docgia->thedocgia.trangthai;
					if(docgia->thedocgia.trangthai==0)  strcpy(field[3],"The bi khoa");
					else strcpy(field[3],"The dang hoat dong");
					
					
					
					for(int i=0;i<4;i++){
							if(i==chisofield){
								Vehopcochu(col[i],startrow,1,width[i],30,120,field[i],false);
							}
							else{
								Vehopcochu(col[i],startrow,1,width[i],0,120,field[i],false);
							}
					}
				}
				else{
					DrawAnnouncement("Khong tim thay doc gia");
				}
			}
		else if(docgia!=NULL&&type==13){
				//Luu du lieu da duoc sua doi
				if(!CheckEmptyAndSpace(field[0])||!CheckEmptyAndSpace(field[1])) {
					DrawAnnouncement("Du lieu nhap vao khong hop le");
					continue;
				}
				
				Chuanhoa(field[0]);
				Chuanhoa(field[1]);
				
				
				strcpy(docgia->thedocgia.ho,field[0]);
				strcpy(docgia->thedocgia.ten,field[1]);
				docgia->thedocgia.gioitinh=tmpphai;
				docgia->thedocgia.trangthai=tmptrangthai;
				
				
				DrawAnnouncement("Sua doi thanh cong");
				if(printbyname) Sapxepdocgiatheohoten(sapxepdocgia,0,soluongdocgia-1);
				else Sapxepdocgiatheomadocgia(sapxepdocgia,0,soluongdocgia-1);
				//Hieu chinh thi can sap xep lai
				return;
		}
		else if(docgia!=NULL&&type==27){
				//Xoa doc gia
					if(docgia->thedocgia.First!=NULL){
						DrawAnnouncement("Khong the xoa doc gia nay vi co the lam mat du lieu");
						continue;
					}
					NodeTDG* nodecanxoa=Xoadocgia(thedocgia,docgia);
					
					
					//Dich qua trai
					int i=soluongdocgia-1;
					NodeTDG* last=NULL;
					while(sapxepdocgia[i]!=nodecanxoa){
						NodeTDG* tmp=sapxepdocgia[i];
						sapxepdocgia[i]=last;
						last=tmp;
						--i;
					}
					sapxepdocgia[i]=last;
					--soluongdocgia;
					delete nodecanxoa;
					DrawAnnouncement("Xoa thanh cong");
					///Xoa thi khong can sap xep lai
					return;
		}
		else if(docgia!=NULL){
				if(type==224){
					type=getch();
					if(type!=75&&type!=77) continue;
					
					Vehopcochu(col[chisofield],startrow,1,width[chisofield],0,120,field[chisofield],false);
					
					if(type==75) chisofield=max(chisofield-1,0);
					if(type==77) chisofield=min(chisofield+1,3);
					
					Vehopcochu(col[chisofield],startrow,1,width[chisofield],30,120,field[chisofield],false);
				}
				else if(chisofield==2||chisofield==3){
					if(chisofield==2){
						if(type==(int)'1') {
							strcpy(field[chisofield],"Nu");
							tmpphai=Nu;
							
						Vehopcochu(col[chisofield],startrow,1,width[chisofield],30,120,field[chisofield],false);
						}
						if(type==(int)'0') {
							strcpy(field[chisofield],"Nam");
							tmpphai=Nam;
						
						Vehopcochu(col[chisofield],startrow,1,width[chisofield],30,120,field[chisofield],false);
						}
					}
					if(chisofield==3){
						if(type==(int)'0'){
							tmptrangthai=0;
							strcpy(field[chisofield],"The bi khoa");
							Vehopcochu(col[chisofield],startrow,1,width[chisofield],30,120,field[chisofield],false);
						}
						if(type==(int)'1'){
							tmptrangthai=1;
							strcpy(field[chisofield],"The dang hoat dong");
							Vehopcochu(col[chisofield],startrow,1,width[chisofield],30,120,field[chisofield],false);
						}
						
					}	
				}
				else {
					if(type==8){
						int nextindex=max((int)strlen(field[chisofield])-1,0);
						field[chisofield][nextindex]='\0';
    					SetBGColor(30);
    					DrawRectangle(col[chisofield]+nextindex,startrow,1,1);
	  					continue;
					}
					int nextindex=strlen(field[chisofield]);
					if(nextindex>=width[chisofield]) continue;
					
					field[chisofield][nextindex]=(char)type;
					field[chisofield][nextindex+1]='\0';
					
					gotoxy(col[chisofield],startrow);
					TextColor(120);
					cout<<field[chisofield];
				}	
			}
		else {//doc gia==NULL nhap vao field de sua doi
				if(type==8){
					int nextindex=max((int)strlen(kiemtra)-1,0);
					kiemtra[nextindex]='\0';
					SetBGColor(30);
					DrawRectangle(col[0]+nextindex,startrow,1,1);
					continue;
				}
				int nextindex=strlen(kiemtra);
				if(nextindex>sizeof(kiemtra)-1) continue;
				
				kiemtra[nextindex]=(char)type;
				kiemtra[nextindex+1]='\0';
				
				gotoxy(col[0],startrow);
				TextColor(120);
				cout<<kiemtra;
		}
	}	

}
void Quanlydocgia(){
	printbyname=true;
	
 	Inquanlydocgia(40,5);
 	while(true){
 		int type=getch();
 		if(type==0){
 			type=getch();
	 		if(type==59){//Them doc gia; Phim F1
	 			Themdocgia();
			}
			else if(type==60){//hieu chinh doc gia; Phim F2
				Hieuchinhdocgia();
			}
			else if(type==61){//dao nguoc in; Phim F3 
				printbyname=!printbyname;
			}
		}
		else if(type==9){//tab
			return;
		}
		
 		Inquanlydocgia(40,5);
	 }
 	
 	
 
}

void Indsdausach(){
	Sapxepdausach(dsdausach,0,dsdausach.soluong-1);//Sap xep lai dau sach
	
	char huongdan[200]={"F1: Them dau sach      Tab: Thoat"};//Huong dan

	
	SetBGColor(0);
	system("cls");
	
	//Ve huong dan
	TextColor(30);
	SetBGColor(0);
	gotoxy(70,0);
	cout<<huongdan;
	////
	
	int startrow=5;
	TextColor(255);
	SetBGColor(0);
	
	//Ve khung
	gotoxy(33,startrow-3);
	cout<<"|ISBN";
	gotoxy(39,startrow-3);
	cout<<"|TEN SACH";
	gotoxy(69,startrow-3);
	cout<<"|TAC GIA";
	gotoxy(99,startrow-3);
	cout<<"|THE LOAI";
	gotoxy(129,startrow-3);
	cout<<"|SO TRANG";
	gotoxy(139,startrow-3);
	cout<<"|NAM XUAT BAN";
	
	int cnt=120;
	gotoxy(35,startrow-2);
	while(cnt--) cout<<'_';
	
	char lasttheloai[200]="";
	
	int printrow=startrow;
	for(int i=0;i<dsdausach.soluong;i++){
		printrow++;
		if(stricmp(lasttheloai,dsdausach.dausach[i]->theloai)!=0){
			strcpy(lasttheloai,dsdausach.dausach[i]->theloai);
			gotoxy(3,printrow);
			cout<<"The loai ";
			TextColor(30);
			SetBGColor(0);
			cout<<lasttheloai;
			printrow+=2;
			cnt=20;
			while(cnt--) cout<<'-';
		}
		TextColor(255);
		SetBGColor(0);
		gotoxy(33,printrow);
		cout<<dsdausach.dausach[i]->isbn;
		gotoxy(39,printrow);
		cout<<dsdausach.dausach[i]->tensach;
		gotoxy(69,printrow);
		cout<<dsdausach.dausach[i]->tacgia;
		gotoxy(99,printrow);
		cout<<dsdausach.dausach[i]->theloai;
		gotoxy(129,printrow);
		cout<<dsdausach.dausach[i]->sotrang;
		gotoxy(139,printrow);
		cout<<dsdausach.dausach[i]->namxuatban;
		
	}
}

void Inthongtindausach(Dausach* dausachcanin){
	SetBGColor(0);
	system("cls");
	TextColor(120);
	int row=10,col=20;
	
	TextColor(255);
	SetBGColor(0);
	//Ve khung
	gotoxy(col,row-3);
	cout<<"|ISBN";
	gotoxy(col+6,row-3);
	cout<<"|TEN SACH";
	gotoxy(col+36,row-3);
	cout<<"|TAC GIA";
	gotoxy(col+66,row-3);
	cout<<"|THE LOAI";
	gotoxy(col+96,row-3);
	cout<<"|NXB";
	gotoxy(col+111,row-3);
	cout<<"|SO TRANG";
	
	int cnt=120;
	gotoxy(col,row-2);
	while(cnt--) cout<<'_';
	
	
	gotoxy(col,row);
	cout<<dausachcanin->isbn;
	gotoxy(col+6,row);
	cout<<dausachcanin->tensach;
	gotoxy(col+36,row);
	cout<<dausachcanin->tacgia;
	gotoxy(col+66,row);
	cout<<dausachcanin->theloai;
	gotoxy(col+96,row);
	cout<<dausachcanin->namxuatban;
	gotoxy(col+111,row);
	cout<<dausachcanin->sotrang;
	
	gotoxy(col+20,row+3);
	cout<<"|MA SACH";
	gotoxy(col+30,row+3);
	cout<<"|VI TRI";
	gotoxy(col+60,row+3);
	cout<<"|TRANG THAI";
	
	cnt=50;
	gotoxy(col+20,row+4);
	while(cnt--) cout<<'_';
	
	
	int i=1;
	NodeDMS* dms=dausachcanin->dms;
	while(dms!=NULL){
		gotoxy(col+20,row+i+5);
		cout<<dms->danhmucsach.masach;
		gotoxy(col+30,row+i+5);
		cout<<dms->danhmucsach.vitri;
		gotoxy(col+60,row+i+5);
		if(dms->danhmucsach.trangthai==0) cout<<"CHO MUON DUOC";
		else if(dms->danhmucsach.trangthai==1) cout<<"DA CO NGUOI MUON";
		else if(dms->danhmucsach.trangthai==2) cout<<"DA THANH LY";
		++i;
		dms=dms->next;
	}
	
	
}
void Themdausach(){
//		char isbn[6];
//	char tensach[30];
//	char tacgia[30];
//	char theloai[30];
//	int sotrang;
//	int namxuatban;
	
	////////////Nho kiem tra ma isbn
	
	int row=20;
	int width[]={5,29,29,29,9,4};
	int col[]={35,41,71,101,131,141,146};
	char field[6][30];
	for(int i=0;i<6;i++) field[i][0]='\0';
	int chisofield=0;
	
	char lable[6][20]={"ISBN:","Ten sach:","Tac gia:","The loai:","So trang:","Nam xuat ban"};
	//Ve 
	SetBGColor(38);
	DrawRectangle(33,row-3,121,5);
	
	for(int i=0;i<6;i++){
		if(i==chisofield){
			Vehopcochu(col[i],row,1,width[i],30,120,field[i],false);
		}
		else{
			Vehopcochu(col[i],row,1,width[i],0,120,field[i],false);
		}
		Vehopcochu(col[i],row-2,1,strlen(lable[i]),0,120,lable[i],false);
	}
	
	while(true){
		int type=getch();
		
		if(type==9) {
			return;
		}
		if(type==13){
			//Kiem tra
			if(!CheckFullOfNumber(field[0])||strlen(field[0])!=width[0]) {
				//isbn
				DrawAnnouncement("ISBN nhap vao khong hop le");
				continue;
			}
			if(!CheckEmptyAndSpace(field[1])||!CheckEmptyAndSpace(field[2])||!CheckEmptyAndSpace(field[3])) {
				DrawAnnouncement("Du lieu nhap vao khong hop le");
				continue;
			}
			if(!CheckFullOfNumber(field[4])||!CheckFullOfNumber(field[5])){
				DrawAnnouncement("Du lieu nhap vao khong hop le");
				continue;
			}
			//Kiem tra ma isbn, isbn khong duoc trung va chi duoc nhap so
			//Kiem tra trung lap cua isbn
			bool f=false;
			for(int i=0;i<dsdausach.soluong;i++){
				if(stricmp(dsdausach.dausach[i]->isbn,field[0])==0)//da ton tai 
				{
					DrawAnnouncement("ISBN nhap vao da ton tai");
					f=true;
					break;
				}
			}
			if(f) continue;
			
			Dausach* dausachmoi=new Dausach;
			
			Chuanhoa(field[1]),Chuanhoa(field[2]),Chuanhoa(field[3]);
			strcpy(dausachmoi->isbn,field[0]);
			strcpy(dausachmoi->tensach,field[1]);
			strcpy(dausachmoi->tacgia,field[2]);
			strcpy(dausachmoi->theloai,field[3]);
			dausachmoi->sotrang=atoi(field[4]);
			dausachmoi->namxuatban=atoi(field[5]);
			
			dsdausach.dausach[dsdausach.soluong++]=dausachmoi;
			
			DrawAnnouncement("Tao dau sach thanh cong");
			return;
		}
		else if(type==224){
					type=getch();
					if(type!=75&&type!=77) continue;
					
					Vehopcochu(col[chisofield],row,1,width[chisofield],0,120,field[chisofield],false);
					
					if(type==75) chisofield=max(chisofield-1,0);
					if(type==77) chisofield=min(chisofield+1,5);
					
					Vehopcochu(col[chisofield],row,1,width[chisofield],30,120,field[chisofield],false);
	
		}
		else{
			if(type==8){
					int nextindex=max((int)strlen(field[chisofield])-1,0);
					field[chisofield][nextindex]='\0';
					SetBGColor(30);
					DrawRectangle(col[chisofield]+nextindex,row,1,1);
					continue;
				}
				int nextindex=strlen(field[chisofield]);
				if(nextindex>=width[chisofield]) continue;
				
				field[chisofield][nextindex]=(char)type;
				field[chisofield][nextindex+1]='\0';
				
				gotoxy(col[chisofield],row);
				TextColor(120);
				cout<<field[chisofield];
		}
	}
}
void Timkiemthongtinsach(){
	char kiemtratensach[30];
	kiemtratensach[0]='\0';
	
	
	SetBGColor(26);
	DrawRectangle(39,7,31,5);
	Vehopcochu(40,8,1,strlen("Nhap vao ten sach"),0,120,"Nhap vao ten sach",false);
	Vehopcochu(40,10,1,29,100,120,kiemtratensach,false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==13){
			bool f=false;
			for(int i=0;i<dsdausach.soluong;i++){
				if(stricmp(dsdausach.dausach[i]->tensach,kiemtratensach)==0){
					Inthongtindausach(dsdausach.dausach[i]);
					f=true;
					break;
				}
			}
			if(f){
				break;
			}
			else{
				DrawAnnouncement("Khong tim thay sach");
				continue;
			}
		}
		else{
				if(type==8){
					int nextindex=max((int)strlen(kiemtratensach)-1,0);
					kiemtratensach[nextindex]='\0';
					SetBGColor(100);
					DrawRectangle(40+nextindex,10,1,1);
					continue;
				}
				int nextindex=strlen(kiemtratensach);
				if(nextindex>=29) continue;
				
				kiemtratensach[nextindex]=(char)type;
				kiemtratensach[nextindex+1]='\0';
				
				gotoxy(40,10);
				TextColor(120);
				cout<<kiemtratensach;
		}
	}
	while(true){
		if(getch()==9) {
			return;
		}
	}
}
void Themdanhmucsach(){
	///Ma sach duoc lay tu dong
	
	char field[10][3][30];//Co 10 hang nhap lieu ma sach, trang thai , vitri
	int width[]={9,29,29};
	int col[]={40,50,80};
	
	int row=10;
	int chisohang=0,chisocot=1;
	int maxchisohang=0;
	int tmptrangthai[10];
	
	//0 cho muon duoc,1 da co doc gia muon,2 sach da thanh ly
	
	
	char kiemtra[6];///nhap ma isbn
	for(int i=0;i<10;i++){
		for(int j=0;j<3;j++) field[i][j][0]='\0';
	}
	kiemtra[0]='\0';
	
	Dausach* dausach=NULL;
	
	char label[3][20]={"Ma sach:","Vi tri:","Trang thai:"};
	
	SetBGColor(38);
	DrawRectangle(col[0]-1,row-3,20,5);
	Vehopcochu(col[0],row-2,1,strlen("Nhap ma dau sach"),0,120,"Nhap ma dau sach",false);
	Vehopcochu(col[0],row,1,5,30,120,kiemtra,false);
	
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(dausach!=NULL&&type==13){
				//Phan kiem tra da hop le chua
				bool f=false;
				for(int i=0;i<=chisohang;i++){
					if(!CheckEmptyAndSpace(field[i][1])){
						f=true;
						break;
					}
				}
				if(f) {
					DrawAnnouncement("Nhap vao khong hop le");
					continue;
				}
				
				//Luu tru va ket thuc
				for(int i=0;i<=maxchisohang;i++){
					NodeDMS* danhmucmoi=new NodeDMS;
					Chuanhoa(field[i][1]);
					
					danhmucmoi->danhmucsach.trangthai=tmptrangthai[i];
					strcpy(danhmucmoi->danhmucsach.masach,field[i][0]);
					strcpy(danhmucmoi->danhmucsach.vitri,field[i][1]);
					
					danhmucmoi->next=dausach->dms;
					dausach->dms=danhmucmoi;
				}
				
				DrawAnnouncement("Luu thanh cong");
				return;
			}
		else if(dausach==NULL&&type==13){
				dausach=Timkiemdausachbangisbn(dsdausach,kiemtra);
				if(dausach!=NULL){
					SetBGColor(0);
					system("cls");
					
					SetBGColor(38);
					DrawRectangle(col[0]-1,row-3,80,14);
					for(int i=0;i<3;i++){
						Vehopcochu(col[i],row-2,1,strlen(label[i]),0,120,label[i],false);
					}
					//Ma sach
					NextString(masach,sizeof(masach));
					strcpy(field[chisohang][0],masach);
					//Trang thai sach
					tmptrangthai[chisohang]=0;
					strcpy(field[chisohang][2],"Cho muon duoc");
					
					for(int i=0;i<10;i++){
							for(int j=0;j<3;j++) {
								if(chisohang==i&&chisocot==j){
									Vehopcochu(col[j],row+i,1,width[j],30,120,field[i][j],false);
								}
								else{
									Vehopcochu(col[j],row+i,1,width[j],0,120,field[i][j],false);
								}
							}
					}
				}
				else{
					DrawAnnouncement("Khong tim thay dau sach");
				}
			}
		else if(dausach!=NULL)
		{
				if(type==224){
						type=getch();
						if(type!=75&&type!=77&&type!=80&&type!=72) continue;
						
						Vehopcochu(col[chisocot],row+chisohang,1,width[chisocot],0,120,field[chisohang][chisocot],false);
						
						if(type==75) chisocot=max(chisocot-1,1);
						if(type==77) chisocot=min(chisocot+1,2);
						
						if(type==72) chisohang=max(chisohang-1,0);
						if(type==80) {
							if(chisohang+1>9) continue;
							//
							chisohang+=1;
							if(chisohang>maxchisohang){
								NextString(masach,sizeof(masach));
								strcpy(field[chisohang][0],masach);
								
								tmptrangthai[chisohang]=0;
								strcpy(field[chisohang][2],"Cho muon duoc");
								
								Vehopcochu(col[0],row+chisohang,1,width[0],0,120,field[chisohang][0],false);
								Vehopcochu(col[2],row+chisohang,1,width[2],0,120,field[chisohang][2],false);
								maxchisohang=chisohang;
							}
							
						}
						
						Vehopcochu(col[chisocot],row+chisohang,1,width[chisocot],30,120,field[chisohang][chisocot],false);
		
				}
				else if(chisocot==2){
					if(type==(int)'0'){
							tmptrangthai[chisohang]=0;
							strcpy(field[chisohang][chisocot],"Cho muon duoc");
							Vehopcochu(col[chisocot],row+chisohang,1,width[chisocot],30,120,field[chisohang][chisocot],false);
					}
					else if(type==(int)'1'){
							tmptrangthai[chisohang]=1;
							strcpy(field[chisohang][chisocot],"Da co doc gia muon");
							Vehopcochu(col[chisocot],row+chisohang,1,width[chisocot],30,120,field[chisohang][chisocot],false);
					}
					else if(type==(int)'2'){
							tmptrangthai[chisohang]=2;
							strcpy(field[chisohang][chisocot],"Da thanh ly");
							Vehopcochu(col[chisocot],row+chisohang,1,width[chisocot],30,120,field[chisohang][chisocot],false);
					}
				}
				else{
					
					if(type==8){
						int nextindex=max((int)strlen(field[chisohang][chisocot])-1,0);
						field[chisohang][chisocot][nextindex]='\0';
						SetBGColor(30);
						DrawRectangle(col[chisocot]+nextindex,row+chisohang,1,1);
						continue;
					}
					int nextindex=strlen(field[chisohang][chisocot]);
					if(nextindex>=width[chisocot]) continue;
					
					field[chisohang][chisocot][nextindex]=(char)type;
					field[chisohang][chisocot][nextindex+1]='\0';
					
					gotoxy(col[chisocot],row+chisohang);
					TextColor(120);
					cout<<field[chisohang][chisocot];
			}
		}
		else {//Dau sach bang NULL
			if(type==8){
					int nextindex=max((int)strlen(kiemtra)-1,0);
					kiemtra[nextindex]='\0';
					SetBGColor(30);
					DrawRectangle(col[0]+nextindex,row,1,1);
					continue;
				}
				int nextindex=strlen(kiemtra);
				if(nextindex>=sizeof(kiemtra)-1) continue;
				
				kiemtra[nextindex]=(char)type;
				kiemtra[nextindex+1]='\0';
				
				gotoxy(col[0],row);
				TextColor(120);
				cout<<kiemtra;
		}
	}
}

void Themdausach_Indanhsachdausach(){
	while(true){
		Indsdausach();
		
		int type=getch();
		
		if(type==9){
			return;
		}
		if(type==0){
			type=getch();
			if(type==59){
				Themdausach();
			}
		}
					
	}
	
}

void Quanlydausach(){
	char label[3][70]={"Them dau sach/In danh sach dau sach","Them danh muc sach","Tim kiem thong tin sach"};
	int index=0;
	
	for(int i=0;i<3;i++){
		if(index==i)
		Vehopcochu(50,10+i*7,3,40,100,120,label[i],true);
		else
		Vehopcochu(50,10+i*7,3,40,10,120,label[i],true);
	}
	while(true){
		int type=getch();
		if(type==13){
			if(index==0){
				Themdausach_Indanhsachdausach();
				return;	
			}
			if(index==1){
				SetBGColor(0);
				system("cls");
		
				Themdanhmucsach();
				return;
			}
			if(index==2){
				SetBGColor(0);
				system("cls");
				
				Timkiemthongtinsach();
				return;
			}
		}
		else if(type==224){
			type=getch();
			if(type==72) {
				Vehopcochu(50,10+index*7,3,40,10,120,label[index],true);
				index=max(index-1,0);
				Vehopcochu(50,10+index*7,3,40,100,120,label[index],true);
			}
			else if(type==80){
				Vehopcochu(50,10+index*7,3,40,10,120,label[index],true);
				index=min(index+1,2);
				Vehopcochu(50,10+index*7,3,40,100,120,label[index],true);
			}
		}
	}
	

}
void Muonsach(){
	char kiemtradocgia[10];
	kiemtradocgia[0]='\0';
	char isbn[6];
	isbn[0]='\0';
	
	Dausach* dausach=NULL;
	NodeTDG* docgia=NULL;
	
	Muontra* dsmt[3];//de in ra
	int chiso=0;
	char field[4][10];//Ma sach, ngay , thang, nam
	for(int i=0;i<4;i++) field[i][0]='\0';
	int col[4]={40,50,53,56};
	int width[4]={9,2,2,4};
	int row=30;
	
	char label[4][20]={"Ma sach","dd","mm","yyyy"};
	int sosachdamuon=0;
	int tmptrangthaitrasach;
	
	SetBGColor(38);
	DrawRectangle(39,7,31,5);
	Vehopcochu(40,8,1,strlen("Nhap vao doc gia"),0,120,"Nhap vao doc gia",false);
	Vehopcochu(40,10,1,9,30,120,kiemtradocgia,false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==13&&docgia==NULL){
			docgia=Timkiemdocgia(thedocgia,atoi(kiemtradocgia));
			
			if(docgia!=NULL){
			
				
				if(docgia->thedocgia.trangthai==0){
					DrawAnnouncement("The doc gia da bi khoa");
					continue;
				}
				
				//Khung nhap vao
			    SetBGColor(0);
			    system("cls");
			    
				SetBGColor(38);
				DrawRectangle(39,27,31,5);
				Vehopcochu(40,28,1,strlen("Nhap vao isbn"),0,120,"Nhap vao isbn",false);
				Vehopcochu(40,30,1,5,30,120,isbn,false);
				
				
				///In thong tin muon sach
				NodeMT* mt=docgia->thedocgia.First;
				Dausach* dausach[3];
				
				while(mt!=NULL){
					if(mt->muontra.trangthai==0){
						dsmt[sosachdamuon]=&mt->muontra;//Tro toi muon tra, tro toi muon tra cua NodeMT hientai; 
						dausach[sosachdamuon]=Timkiemdausachbangmasach(dsdausach,mt->muontra.masach);
						++sosachdamuon;
					}
					mt=mt->right;
					
				}
				
				
				TextColor(255);
				SetBGColor(0);
				
				gotoxy(40,7);
				cout<<"|MA THE";
				gotoxy(50,7);
				cout<<"|TEN DOC GIA";
				gotoxy(80,7);
				cout<<"|SO SACH DA MUON";
				
				int cnt=50;
				gotoxy(40,8);
				while(cnt--) cout<<'_';
				
				gotoxy(40,10);
				cout<<docgia->thedocgia.mathe;
				gotoxy(50,10);
				cout<<docgia->thedocgia.ten;
				gotoxy(80,10);
				cout<<sosachdamuon;
				
				
				gotoxy(40,17);
				cout<<"|MA SACH";
				gotoxy(50,17);
				cout<<"|TEN SACH";
				gotoxy(80,17);
				cout<<"|NGAY MUON";
				
				cnt=50;
				gotoxy(40,18);
				while(cnt--) cout<<'_';
				
			 
				for(int i=0;i<sosachdamuon;i++){
					gotoxy(40,20+i);
					cout<<dsmt[i]->masach;
					gotoxy(50,20+i);
					cout<<dausach[i]->tensach;
					gotoxy(80,20+i);
					cout<<dsmt[i]->ngaymuon.ngay<<'/'<<dsmt[i]->ngaymuon.thang<<'/'<<dsmt[i]->ngaymuon.nam;
				}
			}
			else {
				DrawAnnouncement("Khong tim thay doc gia");
			}
		}
		else if(docgia!=NULL&&dausach==NULL){
		//Nhap isbn
					if(type==13){
						dausach=Timkiemdausachbangisbn(dsdausach,isbn);
						if(dausach!=NULL){
							//In danh sach danh muc sach ve 
							Inthongtindausach(dausach);
							
							SetBGColor(38);
							DrawRectangle(39,row-3,31,5);
						
							for(int i=0;i<4;i++){
								if(i==chiso) Vehopcochu(col[i],row,1,width[i],30,120,field[i],false);
								else Vehopcochu(col[i],row,1,width[i],0,120,field[i],false);
								
								Vehopcochu(col[i],row-2,1,strlen(label[i]),0,120,label[i],false);
							}
						
						}
						else{
							DrawAnnouncement("Khong tim thay dau sach");
						}
					}
					else{
					//Thuc thi isbn
						if(type==8){//Xoa 1 ki tu
							int nextindex=max((int)strlen(isbn)-1,0);
							isbn[nextindex]='\0';
							SetBGColor(30);
							DrawRectangle(40+nextindex,30,1,1);
							continue;
						}
						int nextindex=strlen(isbn);
						if(nextindex>=5) continue;
						
						isbn[nextindex]=(char)type;
						isbn[nextindex+1]='\0';
						
						gotoxy(40,30);
						TextColor(120);
						cout<<isbn;
					}
			}
			else if(docgia!=NULL&&dausach!=NULL){//nhap xong dau sach, nhap vao ma sach va ngay thang nam
				if(type==13){
					//Thuc thi ket thuc
					
					//Xy ly khi muon sach
							//Khong the muon cung luc 2 cuon sach giong nhau, sach nao dang muong thi khong duoc muon tiep nua, khong muong qua 3 cuon sach
						///Kiem tra nhap vao
						if(sosachdamuon>=3) {
							DrawAnnouncement("Khong the muon qua 3 cuon sach");
							continue;
						}
						if(!CheckEmptyAndSpace(field[0])) {
							DrawAnnouncement("Nhap vao khong hop le");
							continue;
						}
						if(!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])) {
							DrawAnnouncement("Nhap vao khong hop le");
							continue;
						}
						if(!ValidDate(atoi(field[1]),atoi(field[2]),atoi(field[3]))) {
							DrawAnnouncement("Ngay khong hop le");
							continue;
						}
 						//Kiem tr xem co sach qua han khong
 						
 						NodeMT* mt=docgia->thedocgia.First;
 						bool f=false;
 						while(mt!=NULL){
 							if(mt->muontra.trangthai==0){
		 						tm tmp={0,0,0,mt->muontra.ngaymuon.ngay,mt->muontra.ngaymuon.thang-1,mt->muontra.ngaymuon.nam-1900};
		 						time_t borrow_time={mktime(&tmp)};
		 						time_t now=time(0);
		 						
		 						int differ=difftime(now,borrow_time) / (60 * 60 * 24);
		 						if(differ>=SONGAYMUONTOIDA) {
		 							f=true;
		 							break;
								}
		 					}
		 					if(f) break;
		 					mt=mt->right;
						}
						if(f) {
							DrawAnnouncement("Doc gia du sach qua thoi han");
							continue;
						}
							
						//Kiem tra ma sach(ma sach phai ton tai va o trang thai co the muong duoc ==0
						NodeDMS* dms=dausach->dms;
						while(dms!=NULL&&stricmp(dms->danhmucsach.masach,field[0])!=0){
							dms=dms->next;
						}
						
						if(dms==NULL||dms->danhmucsach.trangthai!=0) {
							DrawAnnouncement("Khong the muon sach nay");
							continue;
						}
						
						
						NodeMT* newmt=new NodeMT;
						strcpy(newmt->muontra.masach,field[0]);
						newmt->muontra.ngaymuon.ngay=atoi(field[1]);
						newmt->muontra.ngaymuon.thang=atoi(field[2]);
						newmt->muontra.ngaymuon.nam=atoi(field[3]);
						newmt->muontra.trangthai=0;//dang muon
						dms->danhmucsach.trangthai=1;//Da co doc gia muon
						
						InsertFirst(docgia->thedocgia.First,docgia->thedocgia.Last,newmt);
						
						DrawAnnouncement("Muon sach thanh cong");
						return;
				}
				if(type==224){
					type=getch();
					if(type!=75&&type!=77) continue;
					
					Vehopcochu(col[chiso],row,1,width[chiso],0,120,field[chiso],false);
					
						if(type==75) chiso=max(chiso-1,0);
						if(type==77) chiso=min(chiso+1,3);
					
					Vehopcochu(col[chiso],row,1,width[chiso],30,120,field[chiso],false);
	
				}
				else{		
					if(type==8){
						int nextindex=max((int)strlen(field[chiso])-1,0);
						field[chiso][nextindex]='\0';
						SetBGColor(30);
						DrawRectangle(col[chiso]+nextindex,row,1,1);
						continue;
					}
					int nextindex=strlen(field[chiso]);
					if(nextindex>=width[chiso]) continue;
					
					field[chiso][nextindex]=(char)type;
					field[chiso][nextindex+1]='\0';
					
					gotoxy(col[chiso],row);
					TextColor(120);
					cout<<field[chiso];
				}
			}
		else{//docgia==NULL
				if(type==8){
					int nextindex=max((int)strlen(kiemtradocgia)-1,0);
					kiemtradocgia[nextindex]='\0';
					SetBGColor(30);
					DrawRectangle(40+nextindex,10,1,1);
					continue;
				}
				int nextindex=strlen(kiemtradocgia);
				if(nextindex>=9) continue;
				
				kiemtradocgia[nextindex]=(char)type;
				kiemtradocgia[nextindex+1]='\0';
				
				gotoxy(40,10);
				TextColor(120);
				cout<<kiemtradocgia;
		}
	}
}
void Trasach(){
	char kiemtradocgia[10];
	kiemtradocgia[0]='\0';

	
	NodeTDG* docgia=NULL;
	int chiso=0;
	char field[5][10];//Ma sach, ngay , thang, nam,  co sach/mat sach(field[5] dung cho tra sach)
	for(int i=0;i<5;i++) field[i][0]='\0';
	int col[5]={40,50,53,56,61};
	int width[5]={9,2,2,4,9};
	int row=30;
	
	int sosachdamuon=0;
	int tmptrangthaitrasach;
	
	Muontra*  dsmt[3];
	
	char label[5][20]={"Ma sach","dd","mm","yyyy","Trang thai"};
	
	SetBGColor(38);
	DrawRectangle(39,7,31,5);
	Vehopcochu(40,8,1,strlen("Nhap vao doc gia"),0,120,"Nhap vao doc gia",false);
	Vehopcochu(40,10,1,9,30,120,kiemtradocgia,false);
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
		if(type==13&&docgia==NULL){
			docgia=Timkiemdocgia(thedocgia,atoi(kiemtradocgia));
			if(docgia!=NULL){
				if(docgia->thedocgia.trangthai==0){
					DrawAnnouncement("The doc gia da bi khoa");
					continue;
				}
				
				SetBGColor(0);
				system("cls");
				SetBGColor(38);
				DrawRectangle(39,row-3,33,5);
					
				for(int i=0;i<5;i++){
					if(i==chiso) Vehopcochu(col[i],row,1,width[i],30,120,field[i],false);
					else Vehopcochu(col[i],row,1,width[i],0,120,field[i],false);
								
					Vehopcochu(col[i],row-2,1,strlen(label[i]),0,120,label[i],false);
				}
				
				NodeMT* mt=docgia->thedocgia.First;
				Dausach* dausach[3];
				while(mt!=NULL){
					if(mt->muontra.trangthai==0){
						dsmt[sosachdamuon]=&mt->muontra;
						dausach[sosachdamuon]=Timkiemdausachbangmasach(dsdausach,mt->muontra.masach);
						++sosachdamuon;
					}
					mt=mt->right;
					
				}
				
				tmptrangthaitrasach=1;
				strcpy(field[4],"Co sach");
				Vehopcochu(col[4],row,1,width[chiso],30,120,field[4],false);
				
				
			///Thong tinh cua doc gia;
				TextColor(255);
				SetBGColor(0);
				
				gotoxy(40,7);
				cout<<"|MA THE";
				gotoxy(50,7);
				cout<<"|TEN DOC GIA";
				gotoxy(80,7);
				cout<<"|SO SACH DA MUON";
				
				int cnt=50;
				gotoxy(40,8);
				while(cnt--) cout<<'_';
				
				gotoxy(40,10);
				cout<<docgia->thedocgia.mathe;
				gotoxy(50,10);
				cout<<docgia->thedocgia.ten;
				gotoxy(80,10);
				cout<<sosachdamuon;
				
				
				gotoxy(40,17);
				cout<<"|MA SACH";
				gotoxy(50,17);
				cout<<"|TEN SACH";
				gotoxy(80,17);
				cout<<"|NGAY MUON";
				
				cnt=50;
				gotoxy(40,18);
				while(cnt--) cout<<'_';
				
			 
				for(int i=0;i<sosachdamuon;i++){
					gotoxy(40,20+i);
					cout<<dsmt[i]->masach;
					gotoxy(50,20+i);
					cout<<dausach[i]->tensach;
					gotoxy(80,20+i);
					cout<<dsmt[i]->ngaymuon.ngay<<'/'<<dsmt[i]->ngaymuon.thang<<'/'<<dsmt[i]->ngaymuon.nam;
				}
			}
			else{
				DrawAnnouncement("Khong tim thay doc gia");
			}
		}
		else if(docgia!=NULL){//nhap xong dau sach, nhap vao ma sach va ngay thang nam
				if(type==13){
					//Thuc thi ket thuc
					if(!CheckEmptyAndSpace(field[0])){
						DrawAnnouncement("Du lieu nhap vao sai");
						continue;
					}
					if(!CheckFullOfNumber(field[1])||!CheckFullOfNumber(field[2])||!CheckFullOfNumber(field[3])) {
						DrawAnnouncement("Du lieu nhap vao sai");
						continue;
					}
					if(!ValidDate(atoi(field[1]),atoi(field[2]),atoi(field[3]))) {
						DrawAnnouncement("Ngay thang nhap vao bi sai");
						continue;
					}
						
					///Xu ly khi tra sach( can co phan nguoi muon lam mat sach)
						int mt=-1;
						for(int i=0;i<sosachdamuon;i++){
							if(stricmp(dsmt[i]->masach,field[0])==0) {
								mt=i;
								break;
							}
						}
						if(mt!=-1){
							//Ngay nhap vao phai >= hon ngay muon sach
							if(CompareTime(dsmt[mt]->ngaymuon.ngay,dsmt[mt]->ngaymuon.thang,dsmt[mt]->ngaymuon.nam,atoi(field[1]),atoi(field[2]),atoi(field[3]))>0){
								DrawAnnouncement("Ngay thang nhap vao khong hop le");
								continue;
							}
							dsmt[mt]->trangthai=tmptrangthaitrasach;
							dsmt[mt]->ngaytra.ngay=atoi(field[1]);
							dsmt[mt]->ngaytra.thang=atoi(field[2]);
							dsmt[mt]->ngaytra.nam=atoi(field[3]);
							
							//tmptrangthaitrasach==1 thi tra sach thanh cong sach muon tro ve trang thai co the muon
							//tmptrangthaitrasach==2 thi sach do se duoc thanh ly
				
							Danhmucsach* sachduoctra=Timkiemdanhmucsach(dsdausach,field[0]);
							if(tmptrangthaitrasach==1){
								sachduoctra->trangthai=0;
							}
							else if(tmptrangthaitrasach==2){
								sachduoctra->trangthai=2;
							}
							
							
							
							DrawAnnouncement("Tra sach thanh cong");

							return;
						}
						else DrawAnnouncement("Du lieu sai");



					//Ket thuc
				}
				else if(type==224){
					type=getch();
					if(type!=75&&type!=77) continue;
					
					Vehopcochu(col[chiso],row,1,width[chiso],0,120,field[chiso],false);
					
						if(type==75) chiso=max(chiso-1,0);
						if(type==77) chiso=min(chiso+1,4);
					
					Vehopcochu(col[chiso],row,1,width[chiso],30,120,field[chiso],false);
	
				}
				else if(chiso==4){
					if(type==(int)'1'){
						tmptrangthaitrasach=1;
						strcpy(field[chiso],"Co sach");
						Vehopcochu(col[chiso],row,1,width[chiso],30,120,field[chiso],false);
					}
					else if(type==(int)'2'){
						tmptrangthaitrasach=2;
						strcpy(field[chiso],"Mat sach");
						Vehopcochu(col[chiso],row,1,width[chiso],30,120,field[chiso],false);
					}
				}
				else{		
					if(type==8){
						int nextindex=max((int)strlen(field[chiso])-1,0);
						field[chiso][nextindex]='\0';
						SetBGColor(30);
						DrawRectangle(col[chiso]+nextindex,row,1,1);
						continue;
					}
					int nextindex=strlen(field[chiso]);
					if(nextindex>=width[chiso]) continue;
					
					field[chiso][nextindex]=(char)type;
					field[chiso][nextindex+1]='\0';
					
					gotoxy(col[chiso],row);
					TextColor(120);
					cout<<field[chiso];
				}
			}
		else{//docgia==NULL
				if(type==8){
					int nextindex=max((int)strlen(kiemtradocgia)-1,0);
					kiemtradocgia[nextindex]='\0';
					SetBGColor(30);
					DrawRectangle(40+nextindex,10,1,1);
					continue;
				}
				int nextindex=strlen(kiemtradocgia);
				if(nextindex>=9) continue;
				
				kiemtradocgia[nextindex]=(char)type;
				kiemtradocgia[nextindex+1]='\0';
				
				gotoxy(40,10);
				TextColor(120);
				cout<<kiemtradocgia;
		}
	}

}
void Indanhsachquahan(){
	Muontra * dsmt[SOLUONGTOIDA];
	Thedocgia* docgiamuonsachquahan[SOLUONGTOIDA];
	Dausach* dausach[SOLUONGTOIDA];
	int songayquahan[SOLUONGTOIDA];
	int row=7,col=15;
	int sl=0;
	time_t now=time(0);
	for(int i=0;i<soluongdocgia;i++){
		//Kiem tra xem sach do tra chua trangthai==0,va muon qua thoi gian
		NodeMT* mt=sapxepdocgia[i]->thedocgia.First;
		while(mt!=NULL){
			if(mt->muontra.trangthai!=0) {
				mt=mt->right;
				continue;
			}
			
			tm tmp={0,0,0,mt->muontra.ngaymuon.ngay,mt->muontra.ngaymuon.thang-1,mt->muontra.ngaymuon.nam-1900};
		 	time_t borrow_time={mktime(&tmp)};

		 						
		 	int differ=difftime(now,borrow_time) / (60 * 60 * 24);
		 	if(differ>=SONGAYMUONTOIDA) {
		 		dsmt[sl]=&mt->muontra;
		 		docgiamuonsachquahan[sl]=&sapxepdocgia[i]->thedocgia;
		 		songayquahan[sl]=differ;
		 		dausach[sl]=Timkiemdausachbangmasach(dsdausach,mt->muontra.masach);
		 		
		 		++sl;
		 		
		 		break;
			}
			mt=mt->right;
		}
	}
	
	Sapxepdocgiatheoquahan(songayquahan,dsmt,docgiamuonsachquahan,dausach,0,sl-1);
	
	TextColor(255);
	SetBGColor(0);
	
	gotoxy(col,row-3);
	cout<<"|MA THE";
	gotoxy(col+10,row-3);
	cout<<"|HO";
	gotoxy(col+40,row-3);
	cout<<"|TEN";
	gotoxy(col+70,row-3);
	cout<<"|MA SACH";
	gotoxy(col+80,row-3);
	cout<<"|TEN SACH";
	gotoxy(col+110,row-3);
	cout<<"|NGAY MUON";
	gotoxy(col+121,row-3);
	cout<<"|SO NGAY QUA HAN";
	
	int cnt=140;
	gotoxy(col,row-2);
	while(cnt--) cout<<'_';
	
	
	for(int i=0;i<sl;i++){
		gotoxy(col,row+i);
		cout<<docgiamuonsachquahan[i]->mathe;
		gotoxy(col+10,row+i);
		cout<<docgiamuonsachquahan[i]->ho;
		gotoxy(col+40,row+i);
		cout<<docgiamuonsachquahan[i]->ten;
		gotoxy(col+70,row+i);
		cout<<dsmt[i]->masach;
		gotoxy(col+80,row+i);
		cout<<dausach[i]->tensach;
		gotoxy(col+110,row+i);
		cout<<dsmt[i]->ngaymuon.ngay<<'/'<<dsmt[i]->ngaymuon.thang<<'/'<<dsmt[i]->ngaymuon.nam;
		gotoxy(col+121,row+i);
		cout<<songayquahan[i];
		
	}
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
	}
}


struct Top10{
	Dausach* dausach=NULL;
	int soluotmuon=0;
};



void Top10dausach(){
	Top10 sapxeptop10[SOLUONGTOIDA];
	int sl=0;
	
	for(int i=0;i<soluongdocgia;i++){
		NodeMT* mt=sapxepdocgia[i]->thedocgia.First;
		while(mt!=NULL){
//			if(mt->muontra.trangthai!=0){
//				mt=mt->right;
//				continue;
//			}
			//Lay het cac node luoon
			Dausach* dausach=Timkiemdausachbangmasach(dsdausach,mt->muontra.masach);
			bool flag=true;//cai dau sach chua co trong mang sap xep
			for(int j=0;j<sl;j++){
				if(sapxeptop10[j].dausach==dausach){//da co roi
					flag=false;
					sapxeptop10[j].soluotmuon++;
					break;
				}
			}
			if(flag){
				sapxeptop10[sl].dausach=dausach;
				sapxeptop10[sl].soluotmuon++;
				++sl;
			}
			mt=mt->right;
		}
	}
	
	for(int i=0;i<sl-1;i++){
		for(int j=i+1;j<sl;j++){
			if(sapxeptop10[j].soluotmuon>sapxeptop10[i].soluotmuon){
				swap(sapxeptop10[j],sapxeptop10[i]);
			}
		}
	}
	
	TextColor(255);
	SetBGColor(0);
		gotoxy(30,4);
		cout<<"|ISBN";
		gotoxy(36,4);
		cout<<"|TEN SACH";
		gotoxy(66,4);
		cout<<"|TAC GIA";
		gotoxy(96,4);
		cout<<"|THE LOAI";
		gotoxy(126,4);
		cout<<"|SO LUOT MUON";
		int cnt=120;
		gotoxy(30,5);
		while(cnt--) cout<<'_';
	
	for(int i=0;i<10&&i<sl;i++){
		gotoxy(30,7+i);
		cout<<sapxeptop10[i].dausach->isbn;
		gotoxy(36,7+i);
		cout<<sapxeptop10[i].dausach->tensach;
		gotoxy(66,7+i);
		cout<<sapxeptop10[i].dausach->tacgia;
		gotoxy(96,7+i);
		cout<<sapxeptop10[i].dausach->theloai;
		gotoxy(126,7+i);
		cout<<sapxeptop10[i].soluotmuon;
	}
	
	while(true){
		int type=getch();
		if(type==9){
			return;
		}
	}
}
void Chucnang(){
	char label[4][30]={"Muon sach","Tra sach","In danh sach qua han","Top 10 dau sach"};
	int index=0;
	
	
	for(int i=0;i<4;i++){
		if(index==i)
		Vehopcochu(50,10+i*7,3,40,100,120,label[i],true);
		else
		Vehopcochu(50,10+i*7,3,40,10,120,label[i],true);
	}
	while(true){
		int type=getch();
		if(type==13){
			if(index==0){
				SetBGColor(0);
				system("cls");
				Muonsach();

				return;
				
				
			}
			if(index==1){
				SetBGColor(0);
				system("cls");
				Trasach();
			
				return;
		
			}
			if(index==2){
				SetBGColor(0);
				system("cls");
				Indanhsachquahan();
			
				return;
			}
			if(index==3){
				SetBGColor(0);
				system("cls");
				Top10dausach();
				
				return;
			}
		}
		else if(type==224){
			type=getch();
			if(type==72) {
				Vehopcochu(50,10+index*7,3,40,10,120,label[index],true);
				index=max(index-1,0);
				Vehopcochu(50,10+index*7,3,40,100,120,label[index],true);
			}
			else if(type==80){
				Vehopcochu(50,10+index*7,3,40,10,120,label[index],true);
				index=min(index+1,3);
				Vehopcochu(50,10+index*7,3,40,100,120,label[index],true);
			}
		}
	}
	
	
}

void Ghidausachvadanhmucsach(){
	fstream f;
	f.open("dausachvadanhmucsach.txt",ios::out|ios::trunc);
		f<<dsdausach.soluong<<endl;
		for(int i=0;i<dsdausach.soluong;i++){
			f<<dsdausach.dausach[i]->isbn<<endl<<dsdausach.dausach[i]->tensach<<endl<<dsdausach.dausach[i]->tacgia<<endl<<dsdausach.dausach[i]->theloai<<endl<<dsdausach.dausach[i]->sotrang<<endl<<dsdausach.dausach[i]->namxuatban<<endl;
			NodeDMS* dms=dsdausach.dausach[i]->dms;
			while(dms!=NULL){
				f<<dms->danhmucsach.masach<<endl<<dms->danhmucsach.vitri<<endl<<dms->danhmucsach.trangthai;
				f<<endl;
				dms=dms->next;
			}
			f<<"/"<<endl;
		}
	f.close();
}

void Docdausachvadanhmucsach(){
	fstream f;
	f.open("dausachvadanhmucsach.txt",ios::in);
	if(!f) return;
	f>>	dsdausach.soluong;
	char masach[10];
	for(int i=0;i<dsdausach.soluong;i++){
		Dausach* dausach=new Dausach;
		f>>dausach->isbn;
		f.ignore();
		f.getline(dausach->tensach,256);
		f.getline(dausach->tacgia,256);
		f.getline(dausach->theloai,256);
		
		f>>dausach->sotrang>>dausach->namxuatban;
		
		f>>masach;
		while(strcmp(masach,"/")!=0){
			NodeDMS* nodedms=new NodeDMS;
			strcpy(nodedms->danhmucsach.masach,masach);
			f.ignore();
			f.getline(nodedms->danhmucsach.vitri,256);
			f>>nodedms->danhmucsach.trangthai;
			
			//Chen vao danh sach
			nodedms->next=dausach->dms;
			dausach->dms=nodedms;
			////
			
			f>>masach;
		}
		dsdausach.dausach[i]=dausach;
	}
	f.close();
}

void Ghidocgiavamuontra(){
	fstream f;
	f.open("docgiavamuontra.txt",ios::out|ios::trunc);
	f<<soluongdocgia<<endl;
	for(int i=0;i<soluongdocgia;i++){
		f<<sapxepdocgia[i]->thedocgia.mathe<<endl<<sapxepdocgia[i]->thedocgia.ho<<endl<<sapxepdocgia[i]->thedocgia.ten<<endl;
		if(sapxepdocgia[i]->thedocgia.gioitinh==Nam) f<<0;
		else f<<1;
		f<<endl<<sapxepdocgia[i]->thedocgia.trangthai<<endl;
		
		NodeMT* mt=sapxepdocgia[i]->thedocgia.First;
		while(mt!=NULL){
			f<<mt->muontra.masach<<' '<<mt->muontra.ngaymuon.ngay<<' '<<mt->muontra.ngaymuon.thang<<' '<<mt->muontra.ngaymuon.nam<<' '<<mt->muontra.ngaytra.ngay<<' '<<mt->muontra.ngaytra.thang<<' '<<mt->muontra.ngaytra.nam<<' '<<mt->muontra.trangthai;
			f<<endl;
			mt=mt->right;
		}
		f<<"/"<<endl;
	}
	f.close();
}

void Docdocgiavamuontra(){
	fstream f;
	f.open("docgiavamuontra.txt",ios::in);
	if(!f) return;
	f>>soluongdocgia;
	char masach[10];
	for(int i=0;i<soluongdocgia;i++){
		NodeTDG* dg=new NodeTDG;
		f>>dg->thedocgia.mathe;
		
		f.ignore();
		f.getline(dg->thedocgia.ho,256);
		f.getline(dg->thedocgia.ten,256);
		

		int gt;
		f>>gt;
		if(gt==0) dg->thedocgia.gioitinh=Nam;
		else dg->thedocgia.gioitinh=Nu;
		f>>dg->thedocgia.trangthai;
		
		sapxepdocgia[i]=Chendocgia(thedocgia,dg);
		
		f>>masach;
		while(strcmp(masach,"/")!=0){
			NodeMT* nodemt=new NodeMT;
			strcpy(nodemt->muontra.masach,masach);
			f>>nodemt->muontra.ngaymuon.ngay>>nodemt->muontra.ngaymuon.thang>>nodemt->muontra.ngaymuon.nam>>nodemt->muontra.ngaytra.ngay>>nodemt->muontra.ngaytra.thang>>nodemt->muontra.ngaytra.nam>>nodemt->muontra.trangthai;
			
			InsertFirst(dg->thedocgia.First,dg->thedocgia.Last,nodemt);
			
			f>>masach;
		}
//		cout<<nodemt->muontra.masach<<' '<<nodemt->muontra.ngaymuon.ngay<<endl;
	}
	f.close();
}
void Ghimasachhientai(){
	fstream f;
	f.open("masach.txt",ios::out|ios::trunc);
	f<<masach;
	f.close();
}
void Docmasachhientai(){
	fstream f;
	f.open("masach.txt",ios::in);
	if(!f){
		strcpy(masach,"000000000");
	}
	else{
		f>>masach;
	}
	f.close();
}


char label[4][20]={"Quan ly doc gia","Quan ly dau sach","Chuc nang","Thoat"};
int boxwidth=30;
int menuchooser=0;

void DrawMenuBackground(){
	SetBGColor(0);
	system("cls");
	for(int i=0;i<4;i++){
		if(menuchooser==i)
		Vehopcochu(10,10+i*7,3,boxwidth,100,120,label[i],true);
		else
		Vehopcochu(10,10+i*7,3,boxwidth,10,120,label[i],true);
	}
}




int main(){
	Docdausachvadanhmucsach();
	Docdocgiavamuontra();
	Docmasachhientai();
	
	system("mode 1000");
	Docmasachhientai();
	for(int i=0;i<4;i++){
		if(menuchooser==i)
		Vehopcochu(10,10+i*7,3,boxwidth,100,120,label[i],true);
		else
		Vehopcochu(10,10+i*7,3,boxwidth,10,120,label[i],true);
	}
	while(true){
		int type=getch();
		if(type==13){
			if(menuchooser==0){
				Quanlydocgia();
				DrawMenuBackground();
			}
			if(menuchooser==1){
				Quanlydausach();
				DrawMenuBackground();
			}
			if(menuchooser==2){
				Chucnang();
				DrawMenuBackground();
			}
			if(menuchooser==3){
				Ghimasachhientai();
				Ghidocgiavamuontra();
				Ghidausachvadanhmucsach();
				return  0;
			}
		}
		else if(type==224){
			type=getch();
			if(type==72) {
				Vehopcochu(10,10+menuchooser*7,3,boxwidth,10,120,label[menuchooser],true);
				menuchooser=max(menuchooser-1,0);
				Vehopcochu(10,10+menuchooser*7,3,boxwidth,100,120,label[menuchooser],true);
			}
			else if(type==80){
				Vehopcochu(10,10+menuchooser*7,3,boxwidth,10,120,label[menuchooser],true);
				menuchooser=min(menuchooser+1,3);
				Vehopcochu(10,10+menuchooser*7,3,boxwidth,100,120,label[menuchooser],true);
			}
		}
	}

}

