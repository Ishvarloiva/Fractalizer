#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include "QGraphicsScene"
#include "QPushButton"
#include "QTimer"
#include "QVector"
#include "QWheelEvent"
#include "QComboBox"
#include "QRadioButton"
#include "QLayout"
#include "QLabel"
#include "QGroupBox"
#include "QPainter"
#include "QPainter"
#include "math.h"
#include "QDebug"

#include "joint.h"

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    void mainSceneInitialization();
    void drawingSceneInitialization();
    void initialize(int i);
    void Koch(int i, QVector<QPointF> *tmp);
    void Levi(int i, QVector<QPointF> *tmp);
    void Minkovski(int i, QVector<QPointF> *tmp);
    void Custom_Drawing(QVector<QPointF> *tmp);
    void wheelEvent (QWheelEvent *e);
    QPainterPath makepath(QVector<QPointF> *points);

    QGraphicsScene *scene;
    QGraphicsScene *drawingScene;
    MainWindow(QWidget *parent = 0);
    QVector<QPointF> points;
    QVector<Joint*> joints;
    QVector<Joint*> joints_backup;
    QVector<QPointF> customRulePoints;
    QVector<QPointF> custom_rule_points_modifier;
    QPushButton *button;
    QPushButton *draw_rule;
    QGraphicsPathItem *fractal;
    QGraphicsPathItem *customRule;
    QComboBox *type;
    QComboBox *rule;
    QRadioButton *enclosed;
    QWidget *mainSceneWidget;
    QVBoxLayout mainSceneLayout;
    QVBoxLayout parametersLayout;
    QGroupBox *parameters;
    QPushButton *new_joint;
    QPushButton *delete_joint_button;
    QPushButton *accept_custom_rule;
    QPushButton *cancel_custom_rule;
    float line;
    float cosine;
    float sine;
    float scaleFactor;
    float transformAngle;
    bool drawModeFlag;
    ~MainWindow();
public slots:
    void step();
    void new_item();
    void draw_mode();
    void change_custom_rule();
    void createJoint();
    void deleteJoint();
    void accept_rule();
    void cancel_rule();
};

#endif // MAINWINDOW_H
