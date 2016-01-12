#include "mainwindow.h"

#define PI 3.14159265359
MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView(parent)
{
    drawModeFlag = false;

    //Initialize parameters of main window widget
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mainSceneInitialization();
    drawingSceneInitialization();

    new_item();
}

void MainWindow::mainSceneInitialization()
{
    //Create scene
    scene = new QGraphicsScene;
    setScene(scene);
    resize(1000,1000);

    //Place path on scene
    fractal = new QGraphicsPathItem;
    scene->addItem(fractal);

    //Configure button that calculate next step of fractal by chosen rule
    button = new QPushButton("Step");
    connect(button,SIGNAL(clicked()),
            this,SLOT(step()));

    //Configure combobox that define rule for calculation
    rule = new QComboBox();
    rule->addItem("Koch");
    rule->addItem("Levi");
    rule->addItem("Minkovski");
    rule->addItem("Custom");

    //Configure button to enter drawing of custom rule
    draw_rule = new QPushButton("Draw custom rule");
    connect(draw_rule,SIGNAL(clicked()),
            this,SLOT(draw_mode()));

    //Configure combobox of starting figures
    type = new QComboBox();
    type->addItem("Line");
    type->addItem("Triangle");
    type->addItem("Square");
    type->addItem("Pentagon");
    type->addItem("Hexagon");
    type->addItem("Septagon");
    type->addItem("Octagon");

    //Configure enclosed figure radiobutton
    enclosed = new QRadioButton("Enclosed");

    //Redraw figure if figure of enclosed were changed
    connect(type,SIGNAL(activated(int)),
            this,SLOT(new_item()));
    connect(enclosed,SIGNAL(toggled(bool)),
            this,SLOT(new_item()));

    //Place type and eclosed in one parameters groupbox
    parameters = new QGroupBox("Parameters");
    parameters->setLayout(&parametersLayout);
    parametersLayout.addWidget(type);
    parametersLayout.addWidget(enclosed);

    //Create widget for all elements of main scene and place it there
    mainSceneWidget = new QWidget(this);
    mainSceneWidget->resize(200,200);
    mainSceneWidget->setLayout(&mainSceneLayout);
    mainSceneLayout.addWidget(button);
    mainSceneLayout.addWidget(rule);
    mainSceneLayout.addWidget(draw_rule);
    mainSceneLayout.addWidget(parameters);
}

void MainWindow::drawingSceneInitialization()
{
    //Create drawing scene
    drawingScene = new QGraphicsScene;
    drawingScene->setSceneRect(0,0,1000,1000);

    //Configure button that create new joint of custom rule
    new_joint = new QPushButton("New joint",this);
    connect(new_joint,SIGNAL(clicked()),
            this,SLOT(createJoint()));
    new_joint->setGeometry(0,0,100,50);
    new_joint->hide();

    //Configure button that delete last created joint of custom rule
    delete_joint_button = new QPushButton("Delete joint",this);
    connect(delete_joint_button,SIGNAL(clicked()),
            this,SLOT(deleteJoint()));
    delete_joint_button->setGeometry(0,50,100,50);
    delete_joint_button->hide();

    //Configure button that accepts and save custom rule
    accept_custom_rule = new QPushButton("Accept", this);
    connect(accept_custom_rule,SIGNAL(clicked()),
            this,SLOT(accept_rule()));
    accept_custom_rule->setGeometry(0,100,100,50);
    accept_custom_rule->hide();

    //Configure button that cancel and restore previous custom rule
    cancel_custom_rule = new QPushButton("Cancel", this);
    connect(cancel_custom_rule,SIGNAL(clicked()),
            this,SLOT(cancel_rule()));
    cancel_custom_rule->setGeometry(0,150,100,50);
    cancel_custom_rule->hide();

    //Make scene dynamically redraw
    connect(drawingScene,SIGNAL(changed(QList<QRectF>)),
        this,SLOT(change_custom_rule()));

    //Create and place stub for custom rule
    customRulePoints << QPointF(150,500) << QPointF(850,500);
    customRule = new QGraphicsPathItem();
    customRule->setPath(makepath(&customRulePoints));
    drawingScene->addItem(customRule);
}

MainWindow::~MainWindow()
{

}

//Calculates next step of fractal
void MainWindow::step()
{
    QVector<QPointF> *tmp = new QVector<QPointF>;
    for(int i = 0; i < points.size() - 1; i++)
    {
        *tmp << points.at(i);
        initialize(i);
        switch(rule->currentIndex())
        {
        case 0:
            Koch(i, tmp);
            break;
        case 1:
            Levi(i, tmp);
            break;
        case 2:
            Minkovski(i, tmp);
            break;
        case 3:
            Custom_Drawing(tmp);
            break;
        }
    }
    *tmp << points.last();
    points = *tmp;
    fractal->setPath(makepath(&points));
}

void MainWindow::Koch(int i, QVector<QPointF> *tmp)
{
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 - (line/6 * cosine),(points.at(i+1).y()+points.at(i).y())/2 - (line/6 * sine));
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 + (line/3 * sine)*cos(PI/6),(points.at(i+1).y()+points.at(i).y())/2 - (line/3 * cosine)*cos(PI/6));
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 + (line/6 * cosine),(points.at(i+1).y()+points.at(i).y())/2 + (line/6 * sine));
}

void MainWindow::Levi(int i, QVector<QPointF> *tmp)
{

    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 + (line * sine * 0.5),(points.at(i+1).y()+points.at(i).y())/2 - (line * cosine * 0.5));
}

void MainWindow::Minkovski(int i, QVector<QPointF> *tmp)
{
    *tmp << QPointF(points.at(i).x() + line * cosine / 4,points.at(i).y() + line * sine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine / 4 + line * sine / 4,points.at(i).y() + line * sine / 4 - line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine / 2 + line * sine / 4,points.at(i).y() + line * sine / 2 - line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine / 2,points.at(i).y() + line * sine / 2);
    *tmp << QPointF(points.at(i).x() + line * cosine / 2 - line * sine / 4,points.at(i).y() + line * sine / 2 + line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine*3/4 - line * sine / 4,points.at(i).y() + line * sine*3/4 + line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine*3/4,points.at(i).y() + line * sine*3/4);
}

void MainWindow::Custom_Drawing(QVector<QPointF> *tmp)
{
    QPointF anchor(tmp->last().x(),tmp->last().y());
    for(int i = 0; i < custom_rule_points_modifier.size(); i++)
    {
        float x = custom_rule_points_modifier.at(i).x();
        float y = custom_rule_points_modifier.at(i).y();
        *tmp << QPointF(anchor.x() + x * line * cosine - y * line * sine,
                        anchor.y() + x * line * sine + y * line * cosine);
    }
}

void MainWindow::initialize(int i)
{
    line = sqrt(pow(points.at(i).x()-points.at(i+1).x(),2) + pow(points.at(i).y()-points.at(i+1).y(),2));
    cosine = (points.at(i+1).x()-points.at(i).x()) / line;
    sine   = (points.at(i+1).y()-points.at(i).y()) / line;
}

QPainterPath MainWindow::makepath(QVector<QPointF> *points)
{
    QPainterPath tmp;
    tmp.moveTo(points->first());
    for(int i = 1; i < points->size(); i++)
        tmp.lineTo(points->at(i));
    return tmp;
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    if(!drawModeFlag)
    {
        if(e->modifiers() == Qt::ShiftModifier)
        {
            if(e->delta() > 0) transformAngle += PI / 36;
            else transformAngle -= PI / 36;
        }
        else
        {
            if(e->delta() > 0) scaleFactor *= 1.1;
            else scaleFactor /= 1.1;
        }
        setTransform(QTransform(scaleFactor*cos(transformAngle),scaleFactor*sin(transformAngle),scaleFactor*-sin(transformAngle),scaleFactor*cos(transformAngle),0,0));
    }
}

void MainWindow::new_item()
{
    points.clear();
    for(int i = 0; i < type->currentIndex() + 2; i++)
    {
        float angle = (2 * PI * i + PI) / (type->currentIndex() + 2);
        points << QPointF(500 - 300 * sin(angle),
                          500 + 300 * cos(angle));
    }
    if(enclosed->isChecked()) points << points.first();
    fractal->setPath(makepath(&points));
    scaleFactor = 1;
    transformAngle = 0;
    setTransform(QTransform(1,0,0,1,0,0));
    //fractal->update(0,0,1000,1000);
}

void MainWindow::draw_mode()
{
    drawModeFlag = true;
    mainSceneWidget->hide();
    setScene(drawingScene);
    new_joint->show();
    delete_joint_button->show();
    accept_custom_rule->show();
    cancel_custom_rule->show();
    setDragMode(NoDrag);
    setTransform(QTransform(1,0,0,1,0,0));
}

void MainWindow::createJoint()
{
    Joint *joint = new Joint;
    joint->setPos(500,500);
    joints << joint;
    drawingScene->addItem(joint);
}

void MainWindow::deleteJoint()
{
    if(joints.size())
    {
        joints.last()->~Joint();
        joints.removeLast();
    }
}

void MainWindow::change_custom_rule()
{
    customRulePoints.clear();
    customRulePoints << QPointF(150,500);
    for(int i = 0; i < joints.size(); i ++)
    {
        customRulePoints << joints.at(i)->pos();
    }
    customRulePoints << QPointF(850,500);
    customRule->setPath(makepath(&customRulePoints));
}

void MainWindow::accept_rule()
{
    drawModeFlag = false;
    custom_rule_points_modifier.clear();
    for(int i = 1; i < customRulePoints.size() - 1; i++)
    {
        custom_rule_points_modifier << QPointF((customRulePoints.at(i).x() - 150)/700,(customRulePoints.at(i).y() - 500)/700);
    }
    mainSceneWidget->show();
    setScene(scene);
    new_joint->hide();
    delete_joint_button->hide();
    accept_custom_rule->hide();
    cancel_custom_rule->hide();
    setDragMode(ScrollHandDrag);
\
    joints_backup.clear();
    joints_backup = joints;
    setTransform(QTransform(1,0,0,1,0,0));
}

void MainWindow::cancel_rule()
{
    drawModeFlag = false;
    mainSceneWidget->show();
    setScene(scene);
    new_joint->hide();
    delete_joint_button->hide();
    accept_custom_rule->hide();
    cancel_custom_rule->hide();
    setDragMode(ScrollHandDrag);

    joints.clear();
    joints = joints_backup;
    change_custom_rule();
    setTransform(QTransform(1,0,0,1,0,0));
}
