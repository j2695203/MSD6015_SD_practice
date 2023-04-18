#include<QApplication>
#include "entry.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);

    entry *entWindow = new entry();
    entWindow->setWindowTitle("Entry Window");
    entWindow->setMinimumWidth(300);
    entWindow->show();

    return app.exec();
}
