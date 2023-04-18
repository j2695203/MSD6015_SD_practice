#include "mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget{parent}
{
    firstName_LB = new QLabel("First Name");
    lastName_LB = new QLabel("Last Name");
    age_LB = new QLabel("Age");
    gender_LB = new QLabel("Gender");

    firstName_LE = new QLineEdit();
    lastName_LE = new QLineEdit();

    age_SB = new QSpinBox();
    male_RB = new QRadioButton("Male");
    female_RB = new QRadioButton("Female");
    groupBox_gender = new QGroupBox();

    refresh_B = new QPushButton("Refresh");
    text_TE = new QTextEdit();
    finish_B = new QPushButton("Finish");

    vLayout = new QVBoxLayout();
    gLayout = new QGridLayout();
    vLayout_gender = new QVBoxLayout();

    // add widgets to grid layout
    gLayout->addWidget(firstName_LB,0,0);
    gLayout->addWidget(lastName_LB,1,0);
    gLayout->addWidget(firstName_LE,0,1);
    gLayout->addWidget(lastName_LE,1,1);

    gLayout->addWidget(age_LB,0,2);
    gLayout->addWidget(age_SB,0,3);

    gLayout->addWidget(gender_LB,2,0);
    vLayout_gender->addWidget(male_RB);
    vLayout_gender->addWidget(female_RB);
    groupBox_gender->setLayout(vLayout_gender);
    gLayout->addWidget(groupBox_gender,2,1);

    gLayout->addWidget(refresh_B,3,0);

    // add grid layout & widgets to vertical layout
    vLayout->addItem(gLayout);
    vLayout->addWidget(text_TE);
    vLayout->addWidget(finish_B);

    // spaces for layout
    gLayout->setSpacing(10);

    // set layout to window/mainLayout
    setLayout(vLayout);

    // connect to slots
    connect(refresh_B, &QPushButton::clicked, this, &mainWidget::fillSummary);
    connect(finish_B, &QPushButton::clicked, this, &mainWidget::cleanSummary);
}


void mainWidget::fillSummary(){
    // for text in gender radio button
    QString gender_str = "";
    if(male_RB->isChecked()){
        gender_str = "Male";
    }
    if(female_RB->isChecked()){
        gender_str = "Female";
    }
    // fill all the text
    QString result_str = "First Name: " + firstName_LE->text() + " Last Name: " + lastName_LE->text() +
                         " Age: " + age_SB->text() + " Gender: " + gender_str;
    text_TE->setText(result_str);
}

void mainWidget::cleanSummary(){
    firstName_LE->clear();
    lastName_LE->clear();
    age_SB->setValue(0);

    // reset gender radiobox
    QList<QRadioButton *> listRadioButtons = groupBox_gender->findChildren<QRadioButton *>();
    for (auto radio:listRadioButtons){
        radio->setAutoExclusive(false);
        radio->setChecked(false);
        radio->setAutoExclusive(true);
    }

    text_TE->setText("");
}
