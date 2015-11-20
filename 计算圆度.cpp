double* Getsquare(int **Array1,int Width,int Height//Array1图像灰度矩阵

{

  int x,y;
  double pSum,dx,dy;
  int xmax,xmin,ymax,ymin,xSum,ySum,PointSum;
  double dd,n1,n2,xAve,yAve;
  pSum=Height*Width;
  xSum=0;ySum=0;PointSum=0;
  xmin=10000;ymin=10000;xmax=-1;ymax=-1;
  for (y=0 ;y<Height; y++)
  {
    for (x=0; x<Width; x++)
    {
      if (Array1[x][y]==255)
      {
       continue;
      }
      xSum=xSum+x;ySum=ySum+y;++PointSum;
      if (x<xmin)
       {
          xmin=x;
       }
      if (x>xmax)
      {
         xmax=x;
      }
    }
    if (y<ymin)
    {
       ymin=y;
    }
    if (y>ymax)
    {
      ymax=y;
    }
  }
if (pSum==0)
{
 goto Loop;
}
xAve=xSum/pSum;
yAve=ySum/pSum;
///////////////////////////////////////////上面为计算x,y平均值
for (x=1; x<16; x++)
{
 SqureNumber[x]=0;
}
// 11 20 02 21+ 21- 12+ 12- 30+ 30- 03+ 02-
for (y=0; y<Height; y++)
{

    for (x=0; x<Width; x++)

    {

      if (Array1[x][y]==255)

      {

       continue;

      }

      dx=x-xAve;dy=y-yAve;

      SqureNumber[1]=SqureNumber[1]+dx*dx;      // 计算 u(11) 11

      SqureNumber[2]=SqureNumber[2]+dx*dx;      // 计算 u(20) 20

      SqureNumber[3]=SqureNumber[3]+dy*dy;      // 计算 u(02) 02

      if (dy>0)

      {

        SqureNumber[4]=SqureNumber[4]+dx*dx*dy; // 计算 u(21)+ 21+

      }

      else

      {

        SqureNumber[5]=SqureNumber[5]+dx*dx*dy; // 计算 u(21)- 21-

      }

      if( dx>0 )

      {

        SqureNumber[6]=SqureNumber[6]+dx*dy*dy; // 计算 u(12)+ 12+

      }

      else

      {

        SqureNumber[7]=SqureNumber[7]+dx*dy*dy; // 计算 u(12}- 12-

      }

      if (dx>0 )

      {

        SqureNumber[8]=SqureNumber[8]+dx*dx*dx ;// 计算 u(30)+ 30+

      }

      else

      {

        SqureNumber[9]=SqureNumber[9]+dx*dx*dx; // 计算 u(30)- 30-

      }

      if (dy>0)

      {

        SqureNumber[10]=SqureNumber[10]+dy*dy*dy ;// 计算 u(03)+ 03+

      }

      else

      {

        SqureNumber[11]=SqureNumber[11]+dy*dy*dy;// 计算 u(03)- 03-

      }

    }//end for x

}// end for y;

for (x=1; x<12; x++)

{

 SqureNumber[x]=SqureNumber[x]/pSum;

}

///////////////////////////////////////////////////////计算图像的各阶矩

for (x=12; x<21; x++)

{

 SqureNumber[x]=0;

}

SqureNumber[12]=((SqureNumber[2]-SqureNumber[3])/(SqureNumber[2]+SqureNumber[3]))/2; //长宽比特征

 

dd=sqrt((SqureNumber[2]-SqureNumber[3])*(SqureNumber[2]-SqureNumber[3])+4*SqureNumber[1]*SqureNumber[1]);

dd=dd+(SqureNumber[2]-SqureNumber[3]);

SqureNumber[16]=2*atan(dd/(2*SqureNumber[1]))/M_PI; ///字型倾斜度

 

dd=sqrt((SqureNumber[2]-SqureNumber[3])*(SqureNumber[2]-SqureNumber[3])+4*SqureNumber[1]*SqureNumber[1]);

n1=((SqureNumber[2]+SqureNumber[3])+dd)/2;

n2=((SqureNumber[2]+SqureNumber[3])-dd)/2;

SqureNumber[14]=(n1-n2)/(n1+n2);                   //拉长度

 

n1=sqrt((ymax-ymin)*(xmax-xmin));

dd=sqrt((SqureNumber[2]+SqureNumber[3])/PointSum);

SqureNumber[15]=dd/n1;                             //伸展度

 

dd=(SqureNumber[8]-SqureNumber[9])/(SqureNumber[8]+SqureNumber[9]);

SqureNumber[16]=(dd+1)/2;                          //水平偏移度

 

dd=(SqureNumber[10]-SqureNumber[11])/(SqureNumber[10]+SqureNumber[11]);

SqureNumber[17]=(dd+1)/2;                          //垂直偏移度

 

dd=(SqureNumber[4]-SqureNumber[5])/(SqureNumber[4]+SqureNumber[5]);

SqureNumber[18]=(dd+1)/2;                          //水平伸展度度

 

dd=(SqureNumber[6]-SqureNumber[7])/(SqureNumber[6]+SqureNumber[7]);

SqureNumber[19]=(dd+1)/2;                          //垂直伸展度

Loop:;

}