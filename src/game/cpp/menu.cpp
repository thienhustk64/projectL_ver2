#include <iostream>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <menu.h>
 
using namespace std;
 
//Hàm vẽ khung menu và sự kiện hover menu
void menu(int &x, int &y)
{
    settextstyle(4,0,5);     setcolor(14); //Đặt kiểu chữ và màu vẽ
    outtextxy(40,20,"MENU GAME");
    settextstyle(1,0,1);    setcolor(12);   settextstyle(4,0,5);    setcolor(14); //Đặt kiểu chữ và màu vẽ
    outtextxy(390,670,"-----BY QUOCTRIEU----");
 
    if(ismouseclick(WM_MOUSEMOVE)) //Nếu di chuyển con chuột
    {   
        getmouseclick(WM_MOUSEMOVE, x, y); //Lấy tọa độ vị trí con trỏ
    //Lệnh if này với các giá trị x,y tương ứng với đang di chuột tới phần tử JOIN GAME(Tức là ta đang hover JOIN GAME)
    if((y>=230 && y<=260) && (x>=255 && x<=450))
    {
    settextstyle(1,0,3);    setcolor(14);//Đặt kiểu chữ kích cỡ chữ để viết chữ JOIN GAME
    outtextxy(255,230,"JOIN GAME");
    settextstyle(1,0,2);    setcolor(15);//Đặt kiểu chữ kích cỡ chữ để viết các phần tử còn lại
    outtextxy(290,260," HOST GAME         ");
    outtextxy(305,290," EXIT         ");
    }
//Đang hover phần tử JOIN GAME -> tương tự với các phần tử phía dưới
    else if((y>=260 && y<=290) && (x>=255 && x<=450))
    {
    settextstyle(1,0,2);    setcolor(15);
    outtextxy(255,230," JOIN GAME                ");
    settextstyle(1,0,3);    setcolor(14);
    outtextxy(290,260,"HOST GAME");
    settextstyle(1,0,2);    setcolor(15);
    outtextxy(305,290," EXIT                ");
    }
        else if((y>=290 && y<=320) && (x>=255 && x<=450))
    {
    settextstyle(1,0,2);    setcolor(15);
    outtextxy(255,230," JOIN GAME               ");
    outtextxy(290,260," HOST GAME             ");
    settextstyle(1,0,3);    setcolor(14);
    outtextxy(305,290,"EXIT");
    }
    else
    {
     settextstyle(1,0,2);    setcolor(15);
    outtextxy(255,230," JOIN GAME            ");
    outtextxy(290,260," HOST GAME               ");
    outtextxy(305,290," EXIT                 ");
    } 
    }
}
 
 
Hàm xử lý các sự kiện click vào menu thành phần
void clickmouse(int &thoat, int x, int y) 
{
//Nếu click vào chuột trái(click chứ kg phải click đúp)
    if(ismouseclick(WM_LBUTTONDBLCLK) && (y>230 && y<380) && (x>255 && x<450))
    {
        int x1, y1;
        cleardevice();//Lệnh xóa màn hình hiện tại
        settextstyle(1,0,1);    setcolor(12);
        outtextxy(390,670,"-----BY QUOCTRIEU----");
        getmouseclick((WM_LBUTTONDBLCLK), x1, y1);  //Lấy tọa độ x, y vị trí click chuột
 
//Click vào JOIN GAME
        if((y1>=230 && y1<=260) && (x1>=255 && x1<=450))
        {
            setcolor(15); outtext("Ban vua click "); setcolor(12);outtext("JOIN GAME"); setcolor(15); outtext("  - BY QUOCTRIEU");
        }
 
        if((y1>=260 && y1<=290) && (x1>=255 && x1<=450))//Click HOST GAME
        {
        setcolor(15);   outtext("Ban vua click ");setcolor(12);outtext("JOIN GAME"); setcolor(15); outtext("  - BY QUOCTRIEU");
        }
        if((y1>=290 && y1<=320) && (x1>=255 && x1<=450))//click EXIT
        {
            thoat = 1;
        }
 
//Xử lý sự kiện để có thể quay lại menu khi đang hiển thị màn hình mới
     if(thoat !=1)
     {
        int tamdung = 0;
      do{
        if(ismouseclick(WM_MOUSEMOVE))
        {
         getmouseclick(WM_MOUSEMOVE, x, y); //Lay toa do vi tri con tro
        }
          bar(300,500,395,530);
          if((y>=500 && y<=530) && (x>=300 && x<=395)) setbkcolor(10);
          else setbkcolor(15);
          settextstyle(1,0,1); setcolor(12);  
          outtextxy(303,505, " EXIT ");
          setbkcolor(0);
       if(ismouseclick(WM_LBUTTONDBLCLK))
       {
         getmouseclick(WM_LBUTTONDBLCLK, x, y);
         if((y>=500 && y<=530) && (x>=300 && x<=395)) tamdung = 1;
       }
          delay(10);
       }while(tamdung == 0);
     }
     cleardevice();
     menu(x, y);
   }
}
int main()
{
    int x, y, thoat;
    initwindow(700,700);
    do{
        menu(x, y);
       clickmouse(thoat, x, y);
        delay(10);
       }while(thoat !=1);
closegraph();
return 0;
}