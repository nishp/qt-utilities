#include <QtGui>
#include "NcGfx2D.h"
#include "VjText.h"
#include "NcConstants.h"
#include "NcPerson.h"
#include "NcEvent.h"
#include "NcLocation.h"



NcGfx2D::NcGfx2D()
{

}

NcGfx2D::~NcGfx2D()
{

}

void NcGfx2D::drawChart( VjChartData *chartData, QPainter* pntr)
{
  //TODO: Optimize code
  int i = 0;

  QRect rect = pntr->window().adjusted(+25,+25,-25,-25);

  //int left = rect.left();
  //int top = rect.top();
  //int w = rect.width();
  //int h = rect.height();

  //int x0 = rect.topLeft().x();
  //int x1 = rect

  /*
  p0----------p1----------p2
  |
  |     p3          p4
  |
  |
  p5           p6          p7

        p8           p9
  |
  |
  |
  p10----------p11---------p12

  */

  QPoint p6(rect.center());
  QPoint p0(rect.topLeft()),p2(rect.topRight());
  QPoint p10(rect.bottomLeft()),p12(rect.bottomRight());
  QPoint p1(p6.x(),p0.y()),p11(p6.x(),p10.y());
  QPoint p5(p0.x(),p6.y()),p7(p2.x(),p6.y());

  QPoint p3(p0.x()+((p6.x()-p0.x())/2),p0.y()+((p6.y()-p0.y())/2));
  QPoint p4(p6.x()+((p2.x()-p6.x())/2),p3.y());
  QPoint p8(p3.x(),p6.y()+((p10.y()-p6.y())/2));
  QPoint p9(p4.x(),p8.y());

  /*
  QPolygon polyHouse1;
  polyHouse1<<p3<<p1<<p4<<p6;
  QPolygon polyHouse2;
  polyHouse2<<p0<<p1<<p3;
  QPolygon polyHouse3;
  polyHouse3<<p0<<p3<<p5;
  QPolygon polyHouse4;
  polyHouse4<<p5<<p3<<p6<<p8;
  QPolygon polyHouse5;
  polyHouse5<<p5<<p8<<p10;
  QPolygon polyHouse6;
  polyHouse6<<p10<<p8<<p11;
  QPolygon polyHouse7;
  polyHouse7<<p8<<p6<<p9<<p11;
  QPolygon polyHouse8;
  polyHouse8<<p11<<p9<<p12;
  QPolygon polyHouse9;
  polyHouse9<<p9<<p7<<p12;
  QPolygon polyHouse10;
  polyHouse10<<p6<<p4<<p7<<p9;
  QPolygon polyHouse11;
  polyHouse11<<p4<<p2<<p7;
  QPolygon polyHouse12;
  polyHouse12<<p1<<p2<<p4;
  */

  //draw frame
  pntr->drawRect(rect);

  //p->drawConvexPolygon(polyHouse1);
  //p->drawConvexPolygon(polyHouse2);
  //p->drawConvexPolygon(polyHouse3);
  //p->drawConvexPolygon(polyHouse4);
  //p->drawConvexPolygon(polyHouse5);
  //p->drawConvexPolygon(polyHouse6);
  //p->drawConvexPolygon(polyHouse7);
  //p->drawConvexPolygon(polyHouse8);
  //p->drawConvexPolygon(polyHouse9);
  //p->drawConvexPolygon(polyHouse10);
  //p->drawConvexPolygon(polyHouse11);
  //p->drawConvexPolygon(polyHouse12);

  //draw diagonals
  pntr->drawLine(p0,p12);
  pntr->drawLine(p2,p10);

  //draw inner square
  pntr->drawLine(p5,p1);
  pntr->drawLine(p1,p7);
  pntr->drawLine(p7,p11);
  pntr->drawLine(p11,p5);

  //testing
  //p->drawLine(p1,p6);
  //p->drawLine(p3,p4);

  //testing
  //QPoint mid1((p3.x()+p1.x())/2,(p3.y()+p1.y())/2);
  //QPoint mid2((p1.x()+p4.x())/2,(p1.y()+p4.y())/2);
  //QPoint mid3((p4.x()+p6.x())/2,(p4.y()+p6.y())/2);
  //QPoint mid4((p6.x()+p3.x())/2,(p6.y()+p3.y())/2);

  //p->drawLine(mid1,mid2);
  //p->drawLine(mid2,mid3);
  //p->drawLine(mid3,mid4);
  //p->drawLine(mid4,mid1);

  QRect rectHouse[Vj::HousesSize];
  const int hMargin = 4;

  rectHouse[Vj::House_01].setCoords( (p3.x()+p1.x())/2,
                          (p3.y()+p1.y())/2,
                          (p6.x()+p4.x())/2,
                          (p6.y()+p4.y())/2);

  rectHouse[Vj::House_02].setCoords( (p0.x()+p3.x())/2,
                          p0.y(),
                          (p3.x()+p1.x())/2,
                          (p3.y()+p1.y())/2);

  rectHouse[Vj::House_03].setCoords( p0.x()+hMargin,
                          (p0.y()+p3.y())/2,
                          (p5.x()+p3.x())/2,
                          (p5.y()+p3.y())/2);

  rectHouse[Vj::House_04].setCoords( (p5.x()+p3.x())/2,
                          (p5.y()+p3.y())/2,
                          (p8.x()+p6.x())/2,
                          (p8.y()+p6.y())/2);

  rectHouse[Vj::House_05].setCoords( p0.x()+hMargin,
                          (p5.y()+p8.y())/2,
                          (p10.x()+p8.x())/2,
                          (p10.y()+p8.y())/2);

  rectHouse[Vj::House_06].setCoords( (p10.x()+p8.x())/2,
                          (p10.y()+p8.y())/2,
                          (p8.x()+p11.x())/2,
                          p10.y());

  rectHouse[Vj::House_07].setCoords( (p8.x()+p6.x())/2,
                          (p8.y()+p6.y())/2,
                          (p11.x()+p9.x())/2,
                          (p11.y()+p9.y())/2);

  rectHouse[Vj::House_08].setCoords( (p11.x()+p9.x())/2,
                          (p11.y()+p9.y())/2,
                          (p9.x()+p12.x())/2,
                          p12.y());

  rectHouse[Vj::House_09].setCoords( (p9.x()+p7.x())/2,
                          (p9.y()+p7.y())/2,
                          p7.x()-hMargin,
                          (p9.y()+p12.y())/2);

  rectHouse[Vj::House_10].setCoords( (p6.x()+p4.x())/2,
                          (p6.y()+p4.y())/2,
                          (p9.x()+p7.x())/2,
                          (p9.y()+p7.y())/2);

  rectHouse[Vj::House_11].setCoords( (p4.x()+p2.x())/2,
                           (p4.y()+p2.y())/2,
                           p2.x(),
                           (p4.y()+p7.y())/2);

  rectHouse[Vj::House_12].setCoords( (p1.x()+p4.x())/2,
                           p1.y(),
                           (p4.x()+p2.x())/2-hMargin,
                           (p4.y()+p2.y())/2);

  /*
  p0----------p1----------p2
  |
  |     p3          p4
  |
  |
  p5           p6          p7

        p8           p9   	|
  |
  |
  p10----------p11---------p12

  */

#if 0
 for ( i = Vj::HousesStart; i < Vj::HousesSize; ++i )
 {
   p->drawRect(rectHouse[i]);
 }

#endif

 for ( i = Vj::HousesStart; i < Vj::HousesSize; ++i )
 {
   if ( chartData->planetsInHouse[i].size() )
   {
     pntr->drawText(rectHouse[i],Qt::AlignCenter|Qt::TextWordWrap, chartData->planetsInHouse[i]);
   }
 }

#if 0
  const int dy=4;
  const int dx=15;
  p->drawText(p6.x(),p6.y()-dy,"6");
  p->drawText(p3.x(),p3.y()-dy,"7");
  p->drawText(p3.x()-dx,p3.y()+dy,"8");
  p->drawText(p6.x()-dx,p6.y()+dy,"9");
  p->drawText(p8.x()-dx,p8.y()+dy,"10");
  p->drawText(p8.x()-4,p8.y()+dy+4,"11");
  p->drawText(p6.x()-4,p6.y()+dy+4,"12");
  p->drawText(p9.x(),p9.y()+dy+4,"1");
  p->drawText(p9.x()+dx,p9.y(),"2");
  p->drawText(p6.x()+dx,p6.y(),"3");
  p->drawText(p4.x()+dx,p4.y(),"4");
  p->drawText(p4.x(),p4.y()-dy,"5");
#endif

  /*
  p0----------p1----------p2
  |
  |     p3          p4
  |
  |
  p5           p6          p7

        p8           p9   	|
  |
  |
  p10----------p11---------p12

  */
  //we want to take the small rect with px's as centers,
  //so that we can draw the rashis in four parts of that square
  int fact = qMin(rect.width(),rect.height())/8;

  QRect tempRect(p6.x()-fact,p6.y()-fact,fact*2,fact*2);
  //-2 so the text not to touch the point
  const int pointMargin = 4;

  //TODO : "-pointMargin-tempRect.height()/2" should be precalculated and kept in a var.
  //house1
  pntr->drawText(tempRect.adjusted(0,0,0,-pointMargin-tempRect.height()/2),Qt::AlignBottom|Qt::AlignHCenter,"6");
  //house4
  pntr->drawText(tempRect.adjusted(0,0,-pointMargin*4-tempRect.width()/2,0),Qt::AlignRight|Qt::AlignVCenter,"9");
  //house7
  pntr->drawText(tempRect.adjusted(0,+pointMargin+tempRect.height()/2,0,0),Qt::AlignTop|Qt::AlignHCenter,"12");
  //house10
  pntr->drawText(tempRect.adjusted(+pointMargin*4+tempRect.width()/2,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,"3");

  //houses 2 & 3
  tempRect.moveLeft(p3.x()-fact);
  tempRect.moveTop(p3.y()-fact);
  //house 2
  pntr->drawText(tempRect.adjusted(0,0,0,-pointMargin-tempRect.height()/2),Qt::AlignBottom|Qt::AlignHCenter,"6");
  //house 3
  pntr->drawText(tempRect.adjusted(0,0,-pointMargin*4-tempRect.width()/2,0),Qt::AlignRight|Qt::AlignVCenter,"9");

  //houses 5 & 6
  tempRect.moveLeft(p8.x()-fact);
  tempRect.moveTop(p8.y()-fact);
  //house5
  pntr->drawText(tempRect.adjusted(0,0,-pointMargin*4-tempRect.width()/2,0),Qt::AlignRight|Qt::AlignVCenter,"9");
  //house6
  pntr->drawText(tempRect.adjusted(0,+pointMargin+tempRect.height()/2,0,0),Qt::AlignTop|Qt::AlignHCenter,"12");

  //houses 8 & 9
  tempRect.moveLeft(p9.x()-fact);
  tempRect.moveTop(p9.y()-fact);
  //house8
  pntr->drawText(tempRect.adjusted(0,+pointMargin+tempRect.height()/2,0,0),Qt::AlignTop|Qt::AlignHCenter,"12");
  //house9
  pntr->drawText(tempRect.adjusted(+pointMargin*4+tempRect.width()/2,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,"3");


  //houses 11 & 12
  tempRect.moveLeft(p4.x()-fact);
  tempRect.moveTop(p4.y()-fact);
  //house11
  pntr->drawText(tempRect.adjusted(+pointMargin*4+tempRect.width()/2,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,"3");
  //house12
  pntr->drawText(tempRect.adjusted(0,0,0,-pointMargin-tempRect.height()/2),Qt::AlignBottom|Qt::AlignHCenter,"6");

  /*
  p0----------p1----------p2
  |
  |     p3          p4
  |
  |
  p5           p6          p7

        p8           p9   	|
  |
  |
  p10----------p11---------p12

  */

}
