#ifndef MAINMSD_H
#define MAINMSD_H

#include <QWidget>
#include <QtWidgets>

class mainMSD : public QWidget
{
    Q_OBJECT

    // widgets
    QLabel *expr_LB;
    QLabel *result_LB;

    QTextEdit *expr_TE;
    QTextEdit *result_TE;

    QRadioButton *interp_RB;
    QRadioButton *prettyPrt_RB;
    QGroupBox *func_groupBox;

    QPushButton *readFile_B;
    QPushButton *submit_B;
    QPushButton *reset_B;

    // layouts
    QVBoxLayout *func_vLayout;
    QGridLayout *gLayout;


public:
    explicit mainMSD(QWidget *parent = nullptr);

signals:

public slots:
    void fillResult();
    void cleanResult();
    void readFile();

};

#endif // MAINMSD_H
