#include "game2048controller.h"

Game2048Controller::Game2048Controller(Model *model, QObject *parent)
    : ButtonController{model, 8, parent} {}

bool Game2048Controller::gameOver() const { return m_gameOver; }

void Game2048Controller::setGameOver(bool gameOver) {
  if (gameOver != m_gameOver) {
    m_gameOver = gameOver;
    emit gameOverChanged();
  }
}

int Game2048Controller::score() const { return m_score; }

void Game2048Controller::setScore(int score) {
  if (score != m_score) {
    m_score = score;
    emit scoreChanged();
  }
}

void Game2048Controller::enterButtonPressed() {
  if (m_gameOver) {
    emit restartRequested();
  }
}

void Game2048Controller::upButtonPressed() {
  if (!m_gameOver)
    emit moveMade(0);
}

void Game2048Controller::downButtonPressed() {
  if (!m_gameOver)
    emit moveMade(2);
}

void Game2048Controller::leftButtonPressed() {
  if (!m_gameOver)
    emit moveMade(3);
}

void Game2048Controller::rightButtonPressed() {
  if (!m_gameOver)
    emit moveMade(1);
}

void Game2048Controller::saveScore(int score) {
  QString topic = "2048/SCORE";
  m_model->sendMessage(topic, score);
}
