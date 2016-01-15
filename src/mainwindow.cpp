#include "mainwindow.h"

#define PI 3.14159265359
MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView(parent)
{
    //Initialize parameters of main window widget
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mainSceneInitialization();

    drawingWindow = new DrawingWindow(&customRulePoints);

    resetFractal();
}

//Initialize main scene where fractal is drawn
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
            this,SLOT(openDrawingWindow()));

    //Configure combobox of starting figures
    figure = new QComboBox();
    figure->addItem("Line");
    figure->addItem("Triangle");
    figure->addItem("Square");
    figure->addItem("Pentagon");
    figure->addItem("Hexagon");
    figure->addItem("Septagon");
    figure->addItem("Octagon");

    //Configure enclosed figure radiobutton
    enclosed = new QRadioButton("Enclosed");

    //Redraw figure if figure of enclosed were changed
    connect(figure,SIGNAL(activated(int)),
            this,SLOT(resetFractal()));
    connect(enclosed,SIGNAL(toggled(bool)),
            this,SLOT(resetFractal()));

    //Place type and eclosed in one parameters groupbox
    parameters = new QGroupBox("Parameters");
    parameters->setLayout(&parametersLayout);
    parametersLayout.addWidget(figure);
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

MainWindow::~MainWindow()
{

}

//Calculates next step of fractal
void MainWindow::step()
{
    //First create empty vector of points
    QVector<QPointF> *tmp = new QVector<QPointF>;

    //Next go through all points of current fractal
    for(int i = 0; i < points.size() - 1; i++)
    {
        //Put current point in new path
        *tmp << points.at(i);

        //Calculate length of line from current to next point and at what degree to horizontal line it situated
        float line = sqrt(pow(points.at(i).x()-points.at(i+1).x(),2) + pow(points.at(i).y()-points.at(i+1).y(),2));
        float cosine = (points.at(i+1).x()-points.at(i).x()) / line;
        float sine   = (points.at(i+1).y()-points.at(i).y()) / line;

        //Next by index of combobox of four possible rules are applied where fourth one is custom
        //Each rule works by defining how each line will become compound path
        //There are defined where to place points regarding first one
        switch(rule->currentIndex())
        {
        case 0:
            Koch(i, tmp, line, cosine, sine);
            break;
        case 1:
            Levi(i, tmp, line, cosine, sine);
            break;
        case 2:
            Minkovski(i, tmp, line, cosine, sine);
            break;
        case 3:
            customRuleStep(tmp, line, cosine, sine);
            break;
        }
    }
    //There are no calculation from last point but it should not be forgotten
    *tmp << points.last();

    //Apply changes
    points = *tmp;

    //Don't forget to release memory used
    tmp->clear();

    //Create path by just calculated points
    fractal->setPath(createPath(points));
}

//Describe Koch rule for fractal step
void MainWindow::Koch(int i, QVector<QPointF> *tmp, float line, float cosine, float sine)
{
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 - (line/6 * cosine),(points.at(i+1).y()+points.at(i).y())/2 - (line/6 * sine));
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 + (line/3 * sine)*cos(PI/6),(points.at(i+1).y()+points.at(i).y())/2 - (line/3 * cosine)*cos(PI/6));
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 + (line/6 * cosine),(points.at(i+1).y()+points.at(i).y())/2 + (line/6 * sine));
}

//Describe Levi rule for fractal step
void MainWindow::Levi(int i, QVector<QPointF> *tmp, float line, float cosine, float sine)
{
    *tmp << QPointF((points.at(i+1).x()+points.at(i).x())/2 + (line * sine * 0.5),(points.at(i+1).y()+points.at(i).y())/2 - (line * cosine * 0.5));
}

//Describe Minkovski rule for fractal step
void MainWindow::Minkovski(int i, QVector<QPointF> *tmp, float line, float cosine, float sine)
{
    *tmp << QPointF(points.at(i).x() + line * cosine / 4,points.at(i).y() + line * sine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine / 4 + line * sine / 4,points.at(i).y() + line * sine / 4 - line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine / 2 + line * sine / 4,points.at(i).y() + line * sine / 2 - line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine / 2,points.at(i).y() + line * sine / 2);
    *tmp << QPointF(points.at(i).x() + line * cosine / 2 - line * sine / 4,points.at(i).y() + line * sine / 2 + line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine*3/4 - line * sine / 4,points.at(i).y() + line * sine*3/4 + line * cosine / 4);
    *tmp << QPointF(points.at(i).x() + line * cosine*3/4,points.at(i).y() + line * sine*3/4);
}

//Run algorithm by defined custom rule
void MainWindow::customRuleStep(QVector<QPointF> *tmp, float line, float cosine, float sine)
{
    QPointF anchor(tmp->last().x(),tmp->last().y());
    for(int i = 0; i < customRulePoints.size(); i++)
    {
        float x = (customRulePoints.at(i).x() - 150) / 700;
        float y = (customRulePoints.at(i).y() - 500) / 700;
        *tmp << QPointF(anchor.x() + x * line * cosine - y * line * sine,
                        anchor.y() + x * line * sine + y * line * cosine);
    }
}

//Zoom in and out by mouse wheel, with shift rotates
void MainWindow::wheelEvent(QWheelEvent *e)
{
    if(e->modifiers() == Qt::ShiftModifier)
    {
        if(e->delta() > 0) transformAngle += PI / 36;
        else transformAngle -= PI / 36;
    }
    else
    {
        if(e->delta() > 0) scaleFactor *= 1.2;
        else scaleFactor /= 1.2;
    }
    setTransform(QTransform(scaleFactor*cos(transformAngle),
                            scaleFactor*sin(transformAngle),
                            scaleFactor*-sin(transformAngle),
                            scaleFactor*cos(transformAngle),0,0));
}

//Reset path to simple figure which is chosen by figure combobox
void MainWindow::resetFractal()
{
    points.clear();

    //Cycle build ideal figure by points
    for(int i = 0; i < figure->currentIndex() + 2; i++)
    {
        float angle = (2 * PI * i + PI) / (figure->currentIndex() + 2);
        points << QPointF(500 - 300 * sin(angle),
                          500 + 300 * cos(angle));
    }

    //If figure should enclosed then add first point again
    if(enclosed->isChecked()) points << points.first();
    fractal->setPath(createPath(points));

    //Reset zooming and rotation
    scaleFactor = 1;
    transformAngle = 0;
    setTransform(QTransform(1,0,0,1,0,0));
}

//Slot that shows drawing window for custom rule
void MainWindow::openDrawingWindow()
{
    drawingWindow->show();
}
