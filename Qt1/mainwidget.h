#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtWidgets>

class mainWidget : public QWidget
{
    Q_OBJECT
    QLabel *firstName_LB;
    QLabel *lastName_LB;
    QLabel *age_LB;
    QLabel *gender_LB;

    QLineEdit *firstName_LE;
    QLineEdit *lastName_LE;

    QSpinBox *age_SB;
    QRadioButton *male_RB;
    QRadioButton *female_RB;
    QGroupBox *groupBox_gender;

    QPushButton *refresh_B;
    QTextEdit *text_TE;
    QPushButton *finish_B;

    QVBoxLayout *vLayout;
    QGridLayout *gLayout;
    QVBoxLayout *vLayout_gender;



public:
    explicit mainWidget(QWidget *parent = nullptr);

signals:

public slots:
    void fillSummary();
    void cleanSummary();

};

#endif // MAINWIDGET_H
