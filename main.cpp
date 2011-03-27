#include <QtGui>
#include "codepaintdevice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPainterPath pp;
    pp.moveTo(10, 10);
    pp.lineTo(40, 60);
    pp.cubicTo(30, 30, 70, 90, 90, 70);

    CodePaintDeviceHTML5Canvas cpdHTML5Canvas;
    Element cat = { "cat", QRectF(10, 20, 130, 140) };
    cpdHTML5Canvas.addElement(cat);
    QPainter p(&cpdHTML5Canvas);
    p.fillPath(pp, Qt::red);

    Element dog = { "dog", QRectF(30, 40, 150, 160) };
    cpdHTML5Canvas.addElement(dog);
    p.setPen(Qt::red);
    p.drawPath(pp);

    qDebug() << cpdHTML5Canvas.code();

    return 0;
}
