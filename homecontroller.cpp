#include "homecontroller.h"
#include "qdebug.h"
HomeController::HomeController(QObject *parent) : QObject{parent} {}

void HomeController::callMe() { qDebug() << "Clicked"; }
