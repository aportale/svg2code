#include <QtGui>

class MyPaintDevice;

class MyPaintEngine : public QObject, public QPaintEngine
{
    Q_OBJECT

public:
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

class MyPaintDevice : public QObject, public QPicture
{
    Q_OBJECT

public:
    MyPaintDevice();

signals:
    void stateUpdated(const QPaintEngineState &state);
    void pathDrawn(const QPainterPath &path);

protected:
    QPaintEngine *paintEngine() const;
    int metric(PaintDeviceMetric m) const;

private:
    mutable QScopedPointer<QPaintEngine> m_paintEngine;
};

class QtCode : public QObject
{
    Q_OBJECT

public slots:
    void updateState(const QPaintEngineState &state);
    void drawPath(const QPainterPath &path);
};

bool MyPaintEngine::begin(QPaintDevice *pdev)
{
    qDebug() << Q_FUNC_INFO;
    return true;
}

MyPaintDevice::MyPaintDevice()
    : QPicture()
{
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

void QtCode::updateState(const QPaintEngineState &state)
{
    qDebug() << state.pen() << state.brush();
}

void QtCode::drawPath(const QPainterPath &path)
{
    qDebug() << path;
}

QPaintEngine *MyPaintDevice::paintEngine() const
{
    if (!m_paintEngine) {
        m_paintEngine.reset(new MyPaintEngine);
        connect(static_cast<MyPaintEngine*>(m_paintEngine.data()), SIGNAL(stateUpdated(QPaintEngineState)),
                this, SIGNAL(stateUpdated(QPaintEngineState)));
        connect(static_cast<MyPaintEngine*>(m_paintEngine.data()), SIGNAL(pathDrawn(QPainterPath)),
                this, SIGNAL(pathDrawn(QPainterPath)));
    }
    return m_paintEngine.data();
}

int MyPaintDevice::metric(PaintDeviceMetric m) const
{
    switch (m) {
    default: return 100;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyPaintDevice d;
    QtCode qc;
    qc.connect(&d, SIGNAL(stateUpdated(QPaintEngineState)), SLOT(updateState(QPaintEngineState)));
    qc.connect(&d, SIGNAL(pathDrawn(QPainterPath)), SLOT(drawPath(QPainterPath)));

    QPainterPath pp;
    pp.moveTo(10, 10);
    pp.lineTo(40, 60);
    pp.lineTo(60, 40);

    QPainter p(&d);
    p.setBrush(Qt::green);
    p.drawPath(pp);
    p.fillPath(pp, Qt::red);

    return 0;
}

#include "main.moc"
