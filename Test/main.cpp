#include <QApplication>
#include <QPushButton>
#include "window.h"
#include "pch.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    Window window;


    window.show();

    return app.exec();
}
