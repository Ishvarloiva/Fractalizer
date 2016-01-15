#include "drawingwindow.h"

DrawingWindow::DrawingWindow(QVector<QPointF> *customRulePoints, QWidget *parent) : QGraphicsView(parent)
{
    //Connect main window with this one by custome rule points vector
    this->customRulePoints = customRulePoints;

    //Create drawing scene
    drawingScene = new QGraphicsScene;
    drawingScene->setSceneRect(0,0,1000,1000);
    setScene(drawingScene);

    //Configure button that create new joint of custom rule
    createJointButton = new QPushButton("New joint", this);
    connect(createJointButton,SIGNAL(clicked()),
            this,SLOT(createJoint()));

    //Configure button that delete last created joint of custom rule
    deleteJointButton = new QPushButton("Delete joint", this);
    connect(deleteJointButton,SIGNAL(clicked()),
            this,SLOT(deleteJoint()));

    //Configure button that accepts and save custom rule
    acceptCustomRule = new QPushButton("Accept", this);
    connect(acceptCustomRule,SIGNAL(clicked()),
            this,SLOT(acceptRule()));

    //Configure button that cancel and restore previous custom rule
    cancelCustomRule = new QPushButton("Cancel", this);
    connect(cancelCustomRule,SIGNAL(clicked()),
            this,SLOT(cancelRule()));

    //Place all buttons in one widget with vertical layout
    drawingSceneWidget = new QWidget;
    drawingSceneWidget->resize(200, 200);
    drawingSceneWidget->setLayout(&drawingSceneLayout);
    drawingSceneLayout.addWidget(createJointButton);
    drawingSceneLayout.addWidget(deleteJointButton);
    drawingSceneLayout.addWidget(acceptCustomRule);
    drawingSceneLayout.addWidget(cancelCustomRule);
    drawingScene->addWidget(drawingSceneWidget);

    //Make scene dynamically redraw
    connect(drawingScene,SIGNAL(changed(QList<QRectF>)),
        this,SLOT(redraw()));

    //Draw line showing custom rule
    customRule = new QGraphicsPathItem();
    customRule->setPath(createPath(joints));
    drawingScene->addItem(customRule);

    //Make it draws smooth and resize
    setRenderHint(QPainter::Antialiasing);
    resize(1000,1000);
}

DrawingWindow::~DrawingWindow()
{

}

//Redraw scene, called by any changes in scene
void DrawingWindow::redraw()
{
    customRule->setPath(createPath(joints));
}

//Create new joint and place it in the middle of screen
void DrawingWindow::createJoint()
{
    Joint *joint = new Joint;
    joint->setPos(500,500);
    joints << joint;
    drawingScene->addItem(joint);
}

//Delete last joint
void DrawingWindow::deleteJoint()
{
    if(joints.size())
    {
        joints.last()->~Joint();
        joints.removeLast();
    }
}

//Convert vector of Joints to QPointF that can be used for drawing in main window
void DrawingWindow::acceptRule()
{
    customRulePoints->clear();
    for(int i = 0; i < joints.size(); i++)
    {
        *customRulePoints << QPointF(joints.at(i)->x(), joints.at(i)->y());
    }
    hide();
}

//Cancel last changes
void DrawingWindow::cancelRule()
{
    hide();

    while(!joints.isEmpty())
    {
        joints.last()->~Joint();
        joints.removeLast();
    }
    for(int i = 0; i < customRulePoints->size(); i++)
    {
        Joint *joint = new Joint;
        joint->setPos(customRulePoints->at(i).x(), customRulePoints->at(i).y());
        joints << joint;
        drawingScene->addItem(joint);
    }
}
