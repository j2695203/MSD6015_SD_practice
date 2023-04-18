#ifndef ENTRY_H
#define ENTRY_H

#include <QWidget>
#include <QtWidgets>

class entry : public QWidget{
    Q_OBJECT

    QLabel *intro_LB;
    QPushButton *simple_B;
    QPushButton *msd_B;

    QVBoxLayout *vLayout;

public:
    explicit entry(QWidget *parent = nullptr);

signals:

public slots:
    void goSimpleForm();
    void goMsdScript();
};

#endif // ENTRY_H
