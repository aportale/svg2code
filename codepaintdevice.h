#ifndef CODEPAINTDEVICE_H
#define CODEPAINTDEVICE_H

#include <QtCore/QObject>
#include <QtGui/QPaintDevice>
#include <QtGui/QPaintEngine>

struct Element {
    QString id;
    QRectF rect;
    QString code; // Convenience for CodePaintDevice subclasses
};

class CodePaintDevice : public QObject, public QPaintDevice
{
    Q_OBJECT

protected:
    explicit CodePaintDevice(QObject *parent, QPaintEngine::PaintEngineFeatures features);

public:
    virtual ~CodePaintDevice();

    void addElement(const Element &id);
    virtual QString code() const = 0;

protected:
    QPaintEngine *paintEngine() const;
    int metric(PaintDeviceMetric m) const;

    virtual void onNewElement(const Element &id) = 0;
    virtual void onDrawPath(const QPainterPath &path) = 0;

private slots:
    void updateState(const QPaintEngineState &state);
    void drawPath(const QPainterPath &path);

protected:
    QList<Element> m_elements;
    QPen m_pen;
    QPen m_activePen;
    QBrush m_brush;
    QBrush m_activeBrush;

private:
    mutable class MyPaintEngine *m_paintEngine;
};

class CodePaintDeviceQt : public CodePaintDevice
{
public:
    QString code() const;

protected:
    void onNewElement(const Element &id);
    void onDrawPath(const QPainterPath &path);
};

class CodePaintDeviceHTML5Canvas : public CodePaintDevice
{
public:
    CodePaintDeviceHTML5Canvas(QObject *parent = 0);

    QString code() const;

protected:
    void onNewElement(const Element &id);
    void onDrawPath(const QPainterPath &path);
};

#endif // CODEPAINTDEVICE_H
