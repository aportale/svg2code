#include <QtGui>
#include <QtSvg>
#include "codepaintdevice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSvgRenderer svgRenderer(QString::fromLatin1(":/somesvg.svg"));
    CodePaintDeviceHTML5Canvas cpdHTML5Canvas("animals");

    const Element cat = { "cat", svgRenderer.boundsOnElement("cat"), QString() };
    cpdHTML5Canvas.addElement(cat);

    QPainter p(&cpdHTML5Canvas);
    svgRenderer.render(&p, cat.id, cat.rect);

    const Element dog = { "dog", svgRenderer.boundsOnElement("dog"), QString() };
    cpdHTML5Canvas.addElement(dog);
    svgRenderer.render(&p, dog.id, dog.rect);

    qDebug() << cpdHTML5Canvas.code();

    return 0;
}
