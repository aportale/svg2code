#include <QtGui>
#include <QtSvg>
#include "codepaintdevice.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList args = QApplication::arguments();
    const QString exeName = QFileInfo(args.takeFirst()).fileName();

    if (args.count() < 3) {
         std::cerr << qPrintable(exeName) << " svgfile elementid_1 elementid_2 ... elementid_n outfilename" << std::endl;
         return 1;
    }

    const QString svgName = args.takeFirst();
    QSvgRenderer svgRenderer(svgName);
    if (!svgRenderer.isValid()) {
        std::cerr << "Unable to load SVG file " << qPrintable(svgName) << std::endl;
        return 2;
    }

    const QFileInfo outFileInfo(args.takeLast());

    CodePaintDeviceHTML5Canvas cpdHTML5Canvas;
    QPainter p(&cpdHTML5Canvas);

    foreach (const QString& elementId, args) {
        if (!svgRenderer.elementExists(elementId)) {
            std::cerr << "SVG element " << qPrintable(elementId) << " not found." << std::endl;
            return 3;
        }
        const Element element = { elementId, svgRenderer.boundsOnElement(elementId), QString() };
        cpdHTML5Canvas.addElement(element);
        svgRenderer.render(&p, element.id, element.rect);
    }

    qDebug() << cpdHTML5Canvas.code();

    return 0;
}
