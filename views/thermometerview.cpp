#include "thermometerview.h"

thermometerview::thermometerview(QObject *parent)
    : QObject{parent}
    , m_maxValue(0.0)
    , m_minValue(0.0)
{

}
