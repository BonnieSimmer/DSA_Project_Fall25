#include "../include/GraphicalUserInterface.h"

#include <QApplication>
#include <QPushButton>

int run_gui(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
