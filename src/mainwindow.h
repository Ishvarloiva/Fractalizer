#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVector>
#include <QWheelEvent>
#include <QComboBox>
#include <QRadioButton>
#include <QLayout>
#include <QGroupBox>

#include "math.h"

#include "joint.h"
#include "createpath.h"
#include "drawingwindow.h"

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void mainSceneInitialization();
    void Koch(int i, QVector<QPointF> *tmp, float line, float cosine, float sine);
    void Levi(int i, QVector<QPointF> *tmp, float line, float cosine, float sine);
    void Minkovski(int i, QVector<QPointF> *tmp, float line, float cosine, float sine);
    void customRuleStep(QVector<QPointF> *tmp, float line, float cosine, float sine);
    void wheelEvent (QWheelEvent *e);

    QVector<QPointF> points;
    QVector<QPointF> customRulePoints;
    QGraphicsPathItem *fractal;
    QVBoxLayout parametersLayout;
    QVector<Joint*> joints;

    QGraphicsScene *scene;

    QWidget *mainSceneWidget;
    QVBoxLayout mainSceneLayout;
    QPushButton *button;
    QPushButton *draw_rule;
    QComboBox *figure;
    QComboBox *rule;
    QGroupBox *parameters;
    QRadioButton *enclosed;

    DrawingWindow *drawingWindow;

    float scaleFactor;
    float transformAngle;
    ~MainWindow();
public slots:
    void step();
    void resetFractal();
    void openDrawingWindow();
};

#endif // MAINWINDOW_H
