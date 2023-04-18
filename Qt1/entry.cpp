#include "entry.h"
#include "mainwidget.h"
#include "mainmsd.h"

entry::entry(QWidget *parent)
    : QWidget{parent}
{
    // create widgets
    simple_B = new QPushButton("Simple Form");
    msd_B = new QPushButton("MSDscript");
    intro_LB = new QLabel("Choose one:");

    // create layout
    vLayout = new QVBoxLayout();

    // add widgets to layout
    vLayout->addWidget(intro_LB);
    vLayout->addWidget(simple_B);
    vLayout->addWidget(msd_B);

    // set layout to window
    setLayout(vLayout);

    // connect to slots
    connect(simple_B, &QPushButton::clicked, this, &entry::goSimpleForm);
    connect(msd_B, &QPushButton::clicked, this, &entry::goMsdScript);

}

void entry::goSimpleForm(){
    mainWidget *m1 = new mainWidget();
    m1->setWindowTitle("6015 Qt1 Part1");
    m1->show();
}

void entry::goMsdScript(){
    mainMSD *msdWindow = new mainMSD();
    msdWindow->setWindowTitle("6015 Qt1 Part2");
    msdWindow->show();
}
