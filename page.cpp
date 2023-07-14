#include "page.h"

Page::Page(QFrame* parent, Model* model, const QString& name) : QFrame(parent),
    model(model),
    name(name),
    height(model->pageHeight),
    width(model->pageWidth)
{

}

/**
 * @brief Page::update Default Implementation for updating
 */
void Page::update() {}

/**
 * @brief Page::enterButtonPressed Default Implementation for the enter button being pressed
 */
void Page::enterButtonPressed() {}

/**
 * @brief Page::upButtonPressed Default Implementation for the up button being pressed
 */
void Page::upButtonPressed() {}

/**
 * @brief Page::downButtonPressed Default Implementation for the down button being pressed
 */
void Page::downButtonPressed() {}
