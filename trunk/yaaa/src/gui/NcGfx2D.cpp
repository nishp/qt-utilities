#include <QtGui>
#include "NcGfx2D.h"

#define VJ_MAX_HOUSES 12

NcGfx2D::NcGfx2D()
{

}

NcGfx2D::~NcGfx2D()
{

}

void NcGfx2D::drawChart(QPainter* p)
{
  //TODO: Optimize code

  QRect rect = p->window().adjusted(+25,+25,-25,-25);

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
  p->drawRect(rect);

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
  p->drawLine(p0,p12);
  p->drawLine(p2,p10);

  //draw inner square
  p->drawLine(p5,p1);
  p->drawLine(p1,p7);
  p->drawLine(p7,p11);
  p->drawLine(p11,p5);

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

  QRect rectHouse[VJ_MAX_HOUSES];
  const int hMargin = 4;
  rectHouse[0].setCoords( (p3.x()+p1.x())/2,
                          (p3.y()+p1.y())/2,
                          (p6.x()+p4.x())/2,
                          (p6.y()+p4.y())/2);

  rectHouse[1].setCoords( (p0.x()+p3.x())/2,
                          p0.y(),
                          (p3.x()+p1.x())/2,
                          (p3.y()+p1.y())/2);

  rectHouse[2].setCoords( p0.x()+hMargin,
                          (p0.y()+p3.y())/2,
                          (p5.x()+p3.x())/2,
                          (p5.y()+p3.y())/2);

  rectHouse[3].setCoords( (p5.x()+p3.x())/2,
                          (p5.y()+p3.y())/2,
                          (p8.x()+p6.x())/2,
                          (p8.y()+p6.y())/2);

  rectHouse[4].setCoords( p0.x()+hMargin,
                          (p5.y()+p8.y())/2,
                          (p10.x()+p8.x())/2,
                          (p10.y()+p8.y())/2);

  rectHouse[5].setCoords( (p10.x()+p8.x())/2,
                          (p10.y()+p8.y())/2,
                          (p8.x()+p11.x())/2,
                          p10.y());

  rectHouse[6].setCoords( (p8.x()+p6.x())/2,
                          (p8.y()+p6.y())/2,
                          (p11.x()+p9.x())/2,
                          (p11.y()+p9.y())/2);

  rectHouse[7].setCoords( (p11.x()+p9.x())/2,
                          (p11.y()+p9.y())/2,
                          (p9.x()+p12.x())/2,
                          p12.y());

  rectHouse[8].setCoords( (p9.x()+p7.x())/2,
                          (p9.y()+p7.y())/2,
                          p7.x()-hMargin,
                          (p9.y()+p12.y())/2);

  rectHouse[9].setCoords( (p6.x()+p4.x())/2,
                          (p6.y()+p4.y())/2,
                          (p9.x()+p7.x())/2,
                          (p9.y()+p7.y())/2);

  rectHouse[10].setCoords( (p4.x()+p2.x())/2,
                           (p4.y()+p2.y())/2,
                           p2.x(),
                           (p4.y()+p7.y())/2);

  rectHouse[11].setCoords( (p1.x()+p4.x())/2,
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
  p->drawRect(rectHouse[0]);
  p->drawRect(rectHouse[1]);
  p->drawRect(rectHouse[2]);
  p->drawRect(rectHouse[3]);
  p->drawRect(rectHouse[4]);
  p->drawRect(rectHouse[5]);
  p->drawRect(rectHouse[6]);
  p->drawRect(rectHouse[7]);
  p->drawRect(rectHouse[8]);
  p->drawRect(rectHouse[9]);
  p->drawRect(rectHouse[10]);
  p->drawRect(rectHouse[11]);
#endif

  p->drawText(rectHouse[0],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[1],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[2],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[3],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[4],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[5],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[6],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[7],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[8],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[9],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[10],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  p->drawText(rectHouse[11],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");
  //p->drawText(rectHouse[0],Qt::AlignCenter|Qt::TextWordWrap,"Ma Su Ju Ku Su As");

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
  p->drawText(tempRect.adjusted(0,0,0,-pointMargin-tempRect.height()/2),Qt::AlignBottom|Qt::AlignHCenter,"6");
  //house4
  p->drawText(tempRect.adjusted(0,0,-pointMargin*4-tempRect.width()/2,0),Qt::AlignRight|Qt::AlignVCenter,"9");
  //house7
  p->drawText(tempRect.adjusted(0,+pointMargin+tempRect.height()/2,0,0),Qt::AlignTop|Qt::AlignHCenter,"12");
  //house10
  p->drawText(tempRect.adjusted(+pointMargin*4+tempRect.width()/2,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,"3");

  //houses 2 & 3
  tempRect.moveLeft(p3.x()-fact);
  tempRect.moveTop(p3.y()-fact);
  //house 2
  p->drawText(tempRect.adjusted(0,0,0,-pointMargin-tempRect.height()/2),Qt::AlignBottom|Qt::AlignHCenter,"6");
  //house 3
  p->drawText(tempRect.adjusted(0,0,-pointMargin*4-tempRect.width()/2,0),Qt::AlignRight|Qt::AlignVCenter,"9");

  //houses 5 & 6
  tempRect.moveLeft(p8.x()-fact);
  tempRect.moveTop(p8.y()-fact);
  //house5
  p->drawText(tempRect.adjusted(0,0,-pointMargin*4-tempRect.width()/2,0),Qt::AlignRight|Qt::AlignVCenter,"9");
  //house6
  p->drawText(tempRect.adjusted(0,+pointMargin+tempRect.height()/2,0,0),Qt::AlignTop|Qt::AlignHCenter,"12");

  //houses 8 & 9
  tempRect.moveLeft(p9.x()-fact);
  tempRect.moveTop(p9.y()-fact);
  //house8
  p->drawText(tempRect.adjusted(0,+pointMargin+tempRect.height()/2,0,0),Qt::AlignTop|Qt::AlignHCenter,"12");
  //house9
  p->drawText(tempRect.adjusted(+pointMargin*4+tempRect.width()/2,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,"3");


  //houses 11 & 12
  tempRect.moveLeft(p4.x()-fact);
  tempRect.moveTop(p4.y()-fact);
  //house11
  p->drawText(tempRect.adjusted(+pointMargin*4+tempRect.width()/2,0,0,0),Qt::AlignLeft|Qt::AlignVCenter,"3");
  //house12
  p->drawText(tempRect.adjusted(0,0,0,-pointMargin-tempRect.height()/2),Qt::AlignBottom|Qt::AlignHCenter,"6");

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
