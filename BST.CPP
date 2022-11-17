#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
struct tree
{
	int info;
	struct tree *left;
	struct tree *right;
};
struct tree *root,*node,*check;
char *s,*sd;
int num,pri,t,nd;
/////////////////////////////////////////////////////////////////////////////
unsigned initmouse()
{
	_AX=0;
	geninterrupt(0x33);
	return _AX;
}
void showmouse()
{
	_AX=1;
	geninterrupt(0x33);
}
void hidemouse()
{
	_AX=2;
	geninterrupt(0x33);
}
void restrictmouse(int x1,int y1,int x2,int y2)
{
	_AX=7;
	_CX=x1;
	_DX=x2;
	geninterrupt(0x33);
	_AX=8;
	_CX=y1;
	_DX=y2;
	geninterrupt(0x33);
}
void getmouse(int *button,int *x,int *y)
{
	_AX=3;
	geninterrupt(0x33);
	*button=_BX;
	*x=_CX;
	*y=_DX;
}
void setmouse(int x,int y)
{
	_AX=4;
	_CX=x;
	_DX=y;
	geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////////////////////
void insert(tree **root,int ins)
{
	tree *node,*move;
	move=*root;
	node=(tree *)malloc(sizeof(struct tree));
	node->left=node->right=0;
	node->info=ins;
	if(*root==0)
		*root=node;
	else
	{
		while(move!=0)
		{
			if(node->info > move->info)
			{
				if(move->right!=0)
					move=move->right;
				else
				{
					move->right=node;
					break;
				}
			}
			else
			{
				if(move->left!=0)
					move=move->left;
				else
				{
					move->left=node;
					break;
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
int noden(tree *node)
{
	tree *tnode=node;
	if(tnode!=0)
	{
		if((tnode->left==0)&&(tnode->right==0))
			return 1;
		else
			return(noden(tnode->left)+noden(tnode->right)+1);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
int leaf(tree *node)
{
	tree *tnode=node;
	if(tnode!=0)
	{
		if((tnode->left==0)&&(tnode->right==0))
			return 1;
		else
			return(leaf(tnode->left)+leaf(tnode->right));
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
int depth(tree *node)
{
	int l,r;
	tree *tnode=node;
	if(tnode!=0)
	{
		l=depth(tnode->left);
		r=depth(tnode->right);
		if(l>r)
			return l+1;
		else
			return r+1;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
void maxmin(int flag)
{
	setviewport(165,40,615,405,0);
	char *sm;
	int pmax;
	tree *parent;
	parent=root;
	if(flag)
		while(parent->right)
			parent=parent->right;
	else
		while(parent->left)
			parent=parent->left;
	if(flag)
	{
		pmax=parent->info;
		itoa(pmax,sm,10);
		outtextxy(280,30,sm);
	}
	else
	{
		pmax=parent->info;
		itoa(pmax,sm,10);
		outtextxy(280,45,sm);
	}
}
/////////////////////////////////////////////////////////////////////////////
void empty(tree **root)
{
	setcolor(10);
	if(*root==0)
		outtextxy(280,105,"Empty");
	else
		outtextxy(280,105,"Not Empty");
	setcolor(14);
}
/////////////////////////////////////////////////////////////////////////////
void sremove(tree **root,int rem,int *found,tree **sekptr,tree **parent)
{
	*sekptr=*root;
	*parent=0;
	*found=0;
	for(;;)
	{
		if((*found) || *sekptr==0)
			return;
		if(rem<(*sekptr)->info)
		{
			*parent=*sekptr;
			*sekptr=(*sekptr)->left;
		}
		else if(rem > (*sekptr)->info)
		{
			*parent=*sekptr;
			*sekptr=(*sekptr)->right;
		}
		else
			(*found)=1;
	}
}
/////////////////////////////////////////////////////////////////////////////
void remove(tree **root,int rem)
{
	int found;
	tree *x,*parent,*succ,*sutree;
	sremove(root,rem,&found,&x,&parent);
	if(!found)
	{
		outtextxy(260,80,"(     ) Node Not Founded!!!");
		sound(500);
		delay(500);
		nosound();
		return;
	}
	if(x->left!=0 && x->right!=0)
	{
		succ=x->right;
		parent=x;
		while(succ->left!=0)
		{
			parent=succ;
			succ=succ->left;
		}
		x->info=succ->info;
		x=succ;
	}
	sutree=x->left;
	if(sutree==0)
		sutree=x->right;
	if(parent==0)
		(*root)=sutree;
	else if(parent->left==x)
		parent->left=sutree;
	else
		parent->right=sutree;
	free(x);
	outtextxy(260,80,"(     ) Remove From BST Tree.");
}
/////////////////////////////////////////////////////////////////////////////
void search(tree *node,int find)
{
	tree *seek=node;
	while(seek!=0)
	{
		if(find < seek->info)
			seek=seek->left;
		else if(find > seek->info)
			seek=seek->right;
		else if(find==seek->info)
		{
			outtextxy(260,80,"(     ) Node Founded!!!");
			break;
		}
	}
	if(find!=seek->info && seek==0)
		outtextxy(260,80,"(     ) Node Not Founded!!!");
		sound(500);
		delay(500);
		nosound();

}
/////////////////////////////////////////////////////////////////////////////
void showtreeL(tree *root1,int x,int y,int level)
{
	check=root;
	if(root1 != NULL)
	{
		setcolor(14);
		nd=root1->info;
		itoa(nd,sd,10);
		outtextxy(x,y,sd);
		setcolor(12);
		circle(x+6,y+6,8);
		setcolor(15);
		if(root1->left != NULL)
			line(x-2,y+10,x-47,y+32);
		if(root1->right != NULL)
			line(x+14,y+10,x+49,y+32);
		showtreeL(root1->left,x-60,y+30,level+1);
		if(check==root1)
			goto l1;
		showtreeL(root1->right,x+50,y+30,level+1);
	}l1:
}
/////////////////////////////////////////////////////////////////////////////
void showtreeR(tree *root1,int x,int y,int level)
{
	check=root;
	if(root1 != NULL)
	{
		setcolor(14);
		nd=root1->info;
		itoa(nd,sd,10);
		outtextxy(x,y,sd);
		setcolor(12);
		circle(x+6,y+6,8);
		setcolor(15);
		if(root1->left != NULL)
			line(x-2,y+10,x-37,y+32);
		if(root1->right != NULL)
			line(x+14,y+10,x+59,y+32);
		showtreeR(root1->right,x+60,y+30,level+1);
		if(check==root1)
			goto l2;
		showtreeR(root1->left,x-50,y+30,level+1);
	}l2:
}
/////////////////////////////////////////////////////////////////////////////
void preorder(tree *node)
{
	if(node==0)
		return;
	pri=node->info;
	itoa(pri,s,10);
	t+=25;
	outtextxy(t,420,s);
	preorder(node->left);
	preorder(node->right);
}
/////////////////////////////////////////////////////////////////////////////
void inorder(tree *node)
{
	if(node==0)
		return;
	inorder(node->left);
	pri=node->info;
	itoa(pri,s,10);
	t+=25;
	outtextxy(t,435,s);
	inorder(node->right);
}
/////////////////////////////////////////////////////////////////////////////
void postorder(tree *node)
{
	if(node==0)
		return;
	postorder(node->left);
	postorder(node->right);
	pri=node->info;
	itoa(pri,s,10);
	t+=25;
	outtextxy(t,453,s);
}
/////////////////////////////////////////////////////////////////////////////
void travels()
{
	setviewport(0,0,639,479,0);
	setcolor(12);
	t=80;
	preorder(root);
	t=80;
	inorder(root);
	t=80;
	postorder(root);
	setcolor(14);
}
/////////////////////////////////////////////////////////////////////////////
void design()
{
	settextstyle(0,0,0);
	setfillstyle(7,8);
	setcolor(14);
	bar(20,15,150,410);
	setfillstyle(9,8);
	bar3d(160,35,620,410,0,0);
	rectangle(0,0,639,480);
	rectangle(1,1,638,479);
	rectangle(2,1,637,478);
	rectangle(3,1,636,477);
	setcolor(14);
	rectangle(20,15,150,410);
	outtextxy(172,20,"Data Structue Project           Binary Search Tree(BST)");
	rectangle(160,15,620,30);
	rectangle(160,35,620,410);
	outtextxy(25,420,"Preorder :");
	outtextxy(25,436,"Inorder  :");
	outtextxy(25,453,"Postorder:");
	setfillstyle(7,8);
	setcolor(12);
	bar(23,18,145,28);
	outtextxy(25,20 ,"Create-Insert");
	bar(23,38,145,48);
	outtextxy(25,40 ,"Tree Info");
	bar(23,58,145,68);
	outtextxy(25,60 ,"Remove Node");
	bar(23,78,145,88);
	outtextxy(25,80 ,"Search Node");
	bar(23,98,145,108);
	outtextxy(25,100,"Show Tree");
	bar(23,118,145,128);
	outtextxy(25,120,"Empty BST");
	bar(23,138,145,148);
	outtextxy(25,140,"About");
	bar(23,158,145,168);
	outtextxy(25,160,"Exit");
	setcolor(14);
}
/////////////////////////////////////////////////////////////////////////////
void tinfo()
{
	int tn,le,de;
	char *st,*sl,*sd;
	design();
	setviewport(165,40,615,405,0);
	setcolor(12);
	outtextxy(150,0 ,"BST Information");
	setcolor(14);
	outtextxy(0,30 ,"Maximum Node In BST Tree Is:");
	outtextxy(0,45 ,"Minimum Node In BST Tree Is:");
	outtextxy(0,60 ,"Tree Nodes Number In BST Is:");
	outtextxy(0,75 ,"Tree Leafs Number In BST Is:");
	outtextxy(0,90 ,"Your Tree Depth Is         :");
	outtextxy(0,105,"Tree Empty Testing Is      :");
	setcolor(10);
	maxmin(1);
	maxmin(0);
	tn=noden(root);			//Calculate Nodes Number
	itoa(tn,st,10);
	outtextxy(280,60,st);
	le=leaf(root);			//Claculate Leaf Number
	itoa(le,sl,10);
	outtextxy(280,75,sl);
	de=depth(root);			//Claculate Depth Tree
	itoa(de,sd,10);
	outtextxy(280,90,sd);
	empty(&root);
	setcolor(14);
}
/////////////////////////////////////////////////////////////////////////////
void strtoint()
{
	setviewport(160,35,620,410,0);
	hidemouse();
	setfillstyle(9,6);
	bar3d(0,0,460,375,0,0);
	outtextxy(70,50,"Enter Number To Convert String(4-Digit)");
	long number=0;
	int che;
	char a[5],ch,*s,*sn,*endptr;
	for(int i=0;i<10;i++)
	{
		che=ch=getch();
		if(che==13)
			break;
		if(ch>='0' && ch<='9')
		{
			a[i]=ch;
			number=strtol(a,&endptr,10);
			itoa(number,s,10);
			outtextxy(210,100,s);
		}
		else if(che==8)
		{
			i--;
			a[i]='\0';
			i--;
			number=strtol(a,&endptr,10);
			setfillstyle(9,6);
			bar3d(0,0,460,375,0,0);
			setcolor(14);
			outtextxy(70,50,"Enter Number To Convert String(4-Digit)");
			itoa(number,s,10);
			outtextxy(210,100,s);
		}
		else if(che>=0 && che<48 || che>57 && che<=255)
			goto l1;
	}
l1:	setviewport(0,0,639,479,0);
	design();
	setcolor(3);
	itoa(number,sn,10);
	if(*sn!='9' && *sn!='0')
		outtextxy(270,80,sn);
	outtextxy(270,80,sn);
	setcolor(14);
	num=number;
	showmouse();
}
/////////////////////////////////////////////////////////////////////////////
void about()
{
	design();
	setviewport(165,40,615,405,0);
	setcolor(15);
	settextstyle(1,0,1);
	outtextxy(140,20 ,"In The Name Of God");
	outtextxy(40,40 ,"Islamic Azad University-Sabzevar Branch");
	outtextxy(65,100 ,"Master : Mr Saeed Norouzian");
	outtextxy(65,120,"Design : Mehdi Baratzadeh Jovaini");
	outtextxy(65,140,"Email  : M9647113369@Yahoo.com");
	outtextxy(50,240,"Copyright(C) 2007-All Rights Reserved.");
	settextstyle(0,0,0);
}
/////////////////////////////////////////////////////////////////////////////
void delbst()
{
	int i,x=0,y=20;
	cleardevice();
	setcolor(12);
	outtextxy(250,200,"Please Wait ...");
	rectangle(230,220,380,230);
	for(i=0;i<=148;i++)
	{
		delay(10);
		setcolor(15);
		line(231+x,221,231+x,229);
		y=y+20;
		x=x+1;
	}
	cleardevice();
}
/////////////////////////////////////////////////////////////////////////////
void startup()
{
	int i,x=0,y=20;
	cleardevice();
	setcolor(12);
	outtextxy(250,200,"Please Wait ...");
	rectangle(100,220,520,230);
	for(i=0;i<=418;i++)
	{
		delay(10);
		setcolor(15);
		line(101+x,221,101+x,229);
		y=y+20;
		x=x+1;
	}
	cleardevice();
}
/////////////////////////////////////////////////////////////////////////////
void mouse()
{
	int *mx,*my,*click;
	while(1)
	{
		getmouse(click,mx,my);
		setfillstyle(1,5);
		if(*mx>23 && *mx<145 && *my>18  && *my<28  && *click)
		{
			strtoint();
			outtextxy(260,80,"(     ) Insert Into BST Tree.");
			insert(&root,num);
			setviewport(105,420,630,460,1);
			clearviewport();
			travels();
		}
		if(*mx>23 && *mx<145 && *my>38  && *my<48  && *click)
		{
			tinfo();
			travels();
		}
		if(*mx>23 && *mx<145 && *my>58  && *my<68  && *click)
		{
			strtoint();
			remove(&root,num);
			setviewport(105,420,630,460,1);
			clearviewport();
			travels();
		}
		if(*mx>23 && *mx<145 && *my>78  && *my<88  && *click)
		{
			strtoint();
			search(root,num);
		}
		if(*mx>23 && *mx<145 && *my>98  && *my<108 && *click)
		{
			setviewport(0,0,639,479,1);
			hidemouse();
			clearviewport();
			settextstyle(2,0,4);
			outtextxy(5,465,"Press Enter To Exit.");
			showtreeL(root,310,10,1);
			showtreeR(root,310,10,1);
			getch();
			setviewport(0,0,639,479,1);
			settextstyle(0,0,0);
			clearviewport();
			design();
			showmouse();
			travels();
		}
		if(*mx>23 && *mx<145 && *my>118 && *my<128 && *click)
		{
			root=NULL;
			hidemouse();
			delbst();
			design();
			showmouse();
			travels();
		}
		if(*mx>23 && *mx<145 && *my>138 && *my<148 && *click)
		{
			about();
			setviewport(0,0,639,479,1);
			setcolor(14);
		}
		if(*mx>23 && *mx<145 && *my>158 && *my<168 && *click)
			abort();
	}
}
/////////////////////////////////////////////////////////////////////////////
void start()
{
		int flow=1,y=500,x=220,xx=75,n=45,i=1,a,b,c,mx,my;
		mx=getmaxx();
		my=getmaxy();
		time_t tt,st;
		setbkcolor(0);
		setcolor(11);
		tt=time(NULL);
		while(time(NULL)<=tt+30)
			{
				if(flow==1)
					y-=3;
				else
					y+=3;
		cleardevice();
		settextstyle(10,0,7);
		outtextxy(x-126,y,"BST TREE");
		delay(50);
		if(y<=0)
			flow=0;
		if(y>=100&&flow==0)
			{
				st=time(NULL);
				while(time(NULL)<=st+1)
					{
						a=random(mx);
						b=random(my);
						c=random(15);
						putpixel(a,b,c);
						delay(1);
					}
				while(time(NULL)<=st+5)
					{
						setcolor(i++);
						outtextxy(x-126,y,"BST TREE");
						delay(100);
						setcolor(3);
						outtextxy(x-126,y,"BST TREE");
					}
				break;
			}
		}
	setcolor(14);
	settextstyle(7,0,2);
	st=time(NULL);
}
/////////////////////////////////////////////////////////////////////////////
void main()
{
	int driver=VGA,mode=2;
	initgraph(&driver,&mode,"");
	setbkcolor(1);
//	startup();
	start();
	design();
	initmouse();
	showmouse();
	mouse();
	closegraph();
}