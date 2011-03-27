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
    QPainter p(&cpdHTML5Canvas);
    p.setBrush(Qt::green);
    p.drawPath(pp);
    p.fillPath(pp, Qt::red);

    return 0;
}
