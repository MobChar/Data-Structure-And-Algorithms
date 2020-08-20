#include<string>
#include"Bill.h"

#ifndef NHANVIEN_H
#define NHANVIEN_H


using namespace std;

class nhanvien
{
	public:
	string sex,middle,first,last,manv;
	int billcount;//chi so hoa don trong runtime
	nhanvien(){
		billcount=-1;
	}
	nhanvien(string Ho,string Ten,string Dem,string Phai,string Manv)
	{
		first=Ten;
		last=Ho;
		middle=Dem;
		sex=Phai;
		manv=Manv;
		billcount=-1;
	}
	const Bill* billPointer[300];//Bill da co san trong billTab
	void add(string Ho,string Ten,string Dem,string Phai,string Manv)
	{
		Ho.resize(30),Ten.resize(30),Dem.resize(30);
		first=Ten;
		last=Ho;
		middle=Dem;
		sex=Phai;
		manv=Manv;
	}
	void AddBill(Bill* bill){
		billPointer[++billcount]=bill;
	}
	
};
string chuanhoa(string s){
	char k=s[s.size()-1];
	if(s.size()==1) return s;
	else
	{
		while(s[0]==' ') s.erase(s.begin()+0);
		for(int i=1;i<s.size()-2;i++)
			if(s[i]==' '&&s[i+1]==' ') 
			{
				s.erase(s.begin()+i);
				i--;	
			}
		while(s[s.size()-1]==' '||s[s.size()-1]==k) s.erase(s.begin()+s.size()-1);
	}
	return s;
}
string ChuanHoa(string s){
	if(s.size()==1) return s;
	else
	{
		while(s[0]==' ') s.erase(s.begin()+0);
		for(int i=1;i<s.size()-2;i++)
			if(s[i]==' '&&s[i+1]==' ') 
			{
				s.erase(s.begin()+i);
				i--;	
			}
		while(s[s.size()-1]==' ') s.erase(s.begin()+s.size()-1);
	}
	//
	cout<<"S la: "<<s<<endl;
	return s;
}

void quickSort(nhanvien Data[], int l , int r)
{	
	if (l <= r)
	{
		
		string key = Data[(l+r)/2].first+Data[(l+r)/2].last+Data[(l+r)/2].middle+"";
 		
 
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp((Data[i].first+Data[i].last+Data[i].middle).c_str(),key.c_str())<0)
				i++;
			while (stricmp((Data[j].first+Data[j].last+Data[j].middle).c_str(),key.c_str())>0)
				j--;
 
			if (i <= j)
			{
				swap(Data[i], Data[j]);
				i++;
				j--;
			}
		}
 
	
		if (l < j)
			quickSort(Data, l, j);
		if (r > i)
			quickSort(Data, i, r);
	}
}
struct node
{
	string data;
	long long sl;
	struct node *pLeft;
	struct node *pRight;
};
typedef struct node NODE;
typedef NODE* TREE;
void khoitao(TREE &t)
{
	t=NULL;
}
void themNODE(TREE &t,string x,int sl)
{
	if(t==NULL)
	{
		NODE *p=new NODE;
		p->data=x;
		p->sl=sl;
		p->pLeft=NULL;
		p->pRight=NULL;
		t=p;
	}
	else
	{
		if(t->data>x)
			themNODE(t->pLeft,x,sl);
		else if(t->data<x)
			themNODE(t->pRight,x,sl);
		else
		{
			t->sl+=sl;
		}
	}
}

struct thongke
{
	int tien;
	string ten;
	string ma;

};
typedef struct thongke TK;



void heap(int r,int n,TK a[])
{
	int k=2*r+1;
	TK x=a[r];
	bool check=true;
	while(k<=n-1&&check)
	{
		if(k<n-1) // khong co nhanh phai
			if(a[k].tien<a[k+1].tien)
				k++;
		if(a[k].tien<=x.tien) check=false;
		else
		{
			a[r]=a[k];
			r=k;
			k=2*r+1;
		}
		a[r]=x;
	}
}
void heap_sort(TK a[],int n,int x)
{
	TK temp;
	for(int i=n/2-1;i>=0;i--)	
		heap(i,n,a);
	for(int i=n-2,j=0;i>=0&&j<x;i--,j++)
	{
		temp=a[0];
		a[0]=a[i+1];
		a[i+1]=temp;
		heap(0,i+1,a);
	}
}


#endif
