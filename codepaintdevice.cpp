#include "codepaintdevice.h"
#include <QtDebug>
#include <QtGui/QPaintEngine>

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
    qDebug() << Q_FUNC_INFO;
    return true;
}

bool MyPaintEngine::end()
{
    qDebug() << Q_FUNC_INFO;
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
    qDebug() << Q_FUNC_INFO;
}

MyPaintEngine::Type MyPaintEngine::type() const
{
    QInternal::CustomRaster;
}

CodePaintDevice::CodePaintDevice(QObject *parent)
    : QObject(parent)
    , QPaintDevice()
    , m_paintEngine(new MyPaintEngine(this))
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
    m_currentState = state;
    onUpdateState(state);
}

void CodePaintDevice::drawPath(const QPainterPath &path)
{
    onDrawPath(path);
}

QString CodePaintDeviceQt::code() const
{

}

void CodePaintDeviceQt::onNewElement(const Element &element)
{
}

void CodePaintDeviceQt::onUpdateState(const QPaintEngineState &state)
{
    qDebug() << state.pen() << state.brush();
}

void CodePaintDeviceQt::onDrawPath(const QPainterPath &path)
{
    qDebug() << path;
}

QString CodePaintDeviceHTML5Canvas::code() const
{

}

void CodePaintDeviceHTML5Canvas::onNewElement(const Element &element)
{
    Q_UNUSED(element)
}

void CodePaintDeviceHTML5Canvas::onUpdateState(const QPaintEngineState &state)
{
    QString &code = m_elements.last().code;
    const QPaintEngine::DirtyFlags flags = state.state();
    if (flags & QPaintEngine::DirtyPen) {
        code.append("context.strokeStyle = ;");
    }
}

void CodePaintDeviceHTML5Canvas::onDrawPath(const QPainterPath &path)
{
    qDebug() << path;
}

#include "codepaintdevice.moc"
