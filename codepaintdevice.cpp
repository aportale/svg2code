#include "codepaintdevice.h"
#include <QtDebug>

class MyPaintEngine : public QObject, public QPaintEngine
{
    Q_OBJECT

public:
    explicit MyPaintEngine(QObject *parent = 0, PaintEngineFeatures features = 0);

    bool begin(QPaintDevice *pdev);
    bool end();
    void updateState(const QPaintEngineState &state);
    void drawPath(const QPainterPath &path);
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    Type type() const;

signals:
    void stateUpdated(const QPaintEngineState &state);
    void pathDrawn(const QPainterPath &path);
};

MyPaintEngine::MyPaintEngine(QObject *parent, PaintEngineFeatures features)
    : QObject(parent)
    , QPaintEngine(features)
{
}

bool MyPaintEngine::begin(QPaintDevice *pdev)
{
    return true;
}

bool MyPaintEngine::end()
{
    return true;
}

void MyPaintEngine::updateState(const QPaintEngineState &state)
{
    emit stateUpdated(state);
}

void MyPaintEngine::drawPath(const QPainterPath &path)
{
    emit pathDrawn(path);
}

void MyPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
}

QPaintEngine::Type MyPaintEngine::type() const
{
    return QPaintEngine::User;
}

CodePaintDevice::CodePaintDevice(QObject *parent, QPaintEngine::PaintEngineFeatures features)
    : QObject(parent)
    , QPaintDevice()
    , m_paintEngine(new MyPaintEngine(this, features))
{
    connect(m_paintEngine, SIGNAL(stateUpdated(QPaintEngineState)), SLOT(updateState(QPaintEngineState)));
    connect(m_paintEngine, SIGNAL(pathDrawn(QPainterPath)), SLOT(drawPath(QPainterPath)));
}

CodePaintDevice::~CodePaintDevice()
{
}

void CodePaintDevice::addElement(const Element &element)
{
    m_elements.append(element);
}

QPaintEngine *CodePaintDevice::paintEngine() const
{
    return m_paintEngine;
}

int CodePaintDevice::metric(PaintDeviceMetric m) const
{
    switch (m) {
    default: return 100;
    }
}

void CodePaintDevice::updateState(const QPaintEngineState &state)
{
    m_currentPen = state.pen();
    m_currentBrush = state.brush();
    onUpdateState(state);
}

void CodePaintDevice::drawPath(const QPainterPath &path)
{
    onDrawPath(path);
}

QString CodePaintDeviceQt::code() const
{
    return QString();
}

void CodePaintDeviceQt::onNewElement(const Element &element)
{
}

void CodePaintDeviceQt::onUpdateState(const QPaintEngineState &state)
{
}

void CodePaintDeviceQt::onDrawPath(const QPainterPath &path)
{
}

CodePaintDeviceHTML5Canvas::CodePaintDeviceHTML5Canvas(QObject *parent)
    : CodePaintDevice(parent, QPaintEngine::PainterPaths)
{
}

QString CodePaintDeviceHTML5Canvas::code() const
{
    QString result = "// This file has been generated by svg2code\n";
    QString drawFunctions;
    QString dictionary = "\nvar elements = {\n    ";
    int count = 0;
    foreach (const Element &element, m_elements) {
        const QString functionName = QString::fromLatin1("svg2code_draw_%1").arg(count, 3, 10, QLatin1Char('0'));
        drawFunctions.append("\n"
                             "function " + functionName + "(c, x, y, width, height) // '" + element.id + "'\n"
                             "{\n"
                             + element.code +
                             "}\n");
        if (count > 0)
            dictionary.append(",\n    ");
        dictionary.append("'" + element.id + "': { bounds: ["
                          + QString::number(element.rect.x(), 'f', 1)
                          + ", " + QString::number(element.rect.y(), 'f', 1)
                          + ", " + QString::number(element.rect.width(), 'f', 1)
                          + ", " + QString::number(element.rect.height(), 'f', 1)
                          + "], drawfunction: " + functionName + " }");
        count++;
    }
    dictionary.append("\n}\n");
    result.append(drawFunctions);
    result.append(dictionary);
    result.append("\n"
                  "function svg2code_draw(context, id, x, y, width, height)\n"
                  "{\n"
                  "    if (elements[id] !== undefined) {\n"
                  "        elements[id].drawfunction(context, x, y, width, height);\n"
                  "    }\n"
                  "}\n");
    return result;
}

void CodePaintDeviceHTML5Canvas::onNewElement(const Element &element)
{
    Q_UNUSED(element)
}

void CodePaintDeviceHTML5Canvas::onUpdateState(const QPaintEngineState &state)
{
}

void CodePaintDeviceHTML5Canvas::onDrawPath(const QPainterPath &path)
{
    QString &code = m_elements.last().code;
    code.append("    c.beginPath();\n");
    for (int i = 0; i < path.elementCount(); i++) {
        const QPainterPath::Element &element = path.elementAt(i);
        switch (element.type) {
            case QPainterPath::LineToElement:
                code.append("    c.lineTo(" + QString::number(element.x, 'f', 1) + ", " + QString::number(element.y, 'f', 1) + ");\n");
            break;
            case QPainterPath::CurveToElement: {
                code.append("    c.bezierCurveTo(" + QString::number(element.x, 'f', 1) + ", " + QString::number(element.y, 'f', 1)  + ", ");
                const QPainterPath::Element &dataElement1 = path.elementAt(++i);
                code.append(QString::number(dataElement1.x, 'f', 1) + ", " + QString::number(dataElement1.y, 'f', 1)  + ", ");
                const QPainterPath::Element &dataElement2 = path.elementAt(++i);
                code.append(QString::number(dataElement2.x, 'f', 1) + ", " + QString::number(dataElement2.y, 'f', 1)  + ");\n");
            }
            break;
            default:
            case QPainterPath::MoveToElement:
                code.append("    c.moveTo(" + QString::number(element.x, 'f', 1) + ", " + QString::number(element.y, 'f', 1) + ");\n");
            break;
        }
    }
    code.append("    c.closePath();\n");
    if (m_currentBrush.style() != Qt::NoBrush) {
        const QColor color = m_currentBrush.color();
        code.append("    c.fillStyle = 'rgba(" + QString::number(color.red()) + ", "
                                                     + QString::number(color.green()) + ", "
                                                     + QString::number(color.blue()) + ", "
                                                     + QString::number(color.alphaF(), 'f', 1) + ")';\n");
        code.append("    c.fill();\n");
    } else if (m_currentPen.style() != Qt::NoPen) {
        const QColor color = m_currentPen.color();
        code.append("    c.strokeStyle = 'rgba(" + QString::number(color.red()) + ", "
                                                       + QString::number(color.green()) + ", "
                                                       + QString::number(color.blue()) + ", "
                                                       + QString::number(color.alphaF(), 'f', 1) + ")';\n");
        code.append("    c.stroke();\n");
    }
}

#include "codepaintdevice.moc"