#include "mainmsd.h"
#include "parse.hpp"
#include "env.hpp"
#include "val.hpp"

mainMSD::mainMSD(QWidget *parent)
    : QWidget{parent}
{
    // create widgets
    expr_LB = new QLabel("Expression:");
    result_LB = new QLabel("Result:");

    expr_TE = new QTextEdit();
    result_TE = new QTextEdit();

    interp_RB = new QRadioButton("Interp");
    prettyPrt_RB = new QRadioButton("Pretty Print");
    func_groupBox = new QGroupBox();

    readFile_B = new QPushButton("Read Expression from File");
    submit_B = new QPushButton("Submit");
    reset_B = new QPushButton("Reset");

    // create layouts
    func_vLayout = new QVBoxLayout();
    gLayout = new QGridLayout();

    // add widgets/layouts to layouts
    func_vLayout->addWidget(interp_RB);
    func_vLayout->addWidget(prettyPrt_RB);
    func_groupBox->setLayout(func_vLayout);

    gLayout->addWidget(expr_LB,0,0);
    gLayout->addWidget(expr_TE,0,1);
    gLayout->addWidget(readFile_B,1,1);
    gLayout->addWidget(func_groupBox,2,1);
    gLayout->addWidget(submit_B,3,1);
    gLayout->addWidget(result_LB,4,0);
    gLayout->addWidget(result_TE,4,1);
    gLayout->addWidget(reset_B,5,1);

    // set layouts to windows
    setLayout(gLayout);

    // connect to slots
    connect(submit_B, &QPushButton::clicked, this, &mainMSD::fillResult);
    connect(reset_B, &QPushButton::clicked, this, &mainMSD::cleanResult);
    connect(readFile_B, &QPushButton::clicked, this, &mainMSD::readFile);
}

void mainMSD::fillResult(){
    QString input_str = expr_TE->toPlainText();
    std::string str_to_be_parsed = input_str.toUtf8().constData();

    if(interp_RB->isChecked()){
        QString result_to_display = QString::fromStdString(parse_str(str_to_be_parsed)->interp(Env::empty)->to_string());
        result_TE->setText(result_to_display);
    }

    if(prettyPrt_RB->isChecked()){
        QString result_to_display = QString::fromStdString(parse_str(str_to_be_parsed)->to_string());
        result_TE->setText(result_to_display);
    }

}

void mainMSD::cleanResult(){
    expr_TE->clear();
    result_TE->clear();

    // reset func radiobox
    QList<QRadioButton *> listRadioButtons = func_groupBox->findChildren<QRadioButton *>();
    for (auto radio:listRadioButtons){
        radio->setAutoExclusive(false);
        radio->setChecked(false);
        radio->setAutoExclusive(true);
    }
}

void mainMSD::readFile(){

    // get filename by choosing file from directory
    QString filename = QFileDialog::getOpenFileName(this, tr("Open text file"), "/Users/jinny/MSD/my6015Repo/Qt1", tr("Text Files(*.txt)"));

    // read file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        expr_TE->setText("fail to open file");
    QTextStream in(&file);
    QString str_expr = in.readAll();

    // show file content in expression
    expr_TE->setText(str_expr);
}
