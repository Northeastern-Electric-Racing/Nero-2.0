#include "game2048controller.h"
#include "../utils/high_scores.h"

Game2048Controller::Game2048Controller(Model *model, QObject *parent)
    : ButtonController{model, 8, parent} {
  m_bestScore = loadHighScore("scores/2048");
}

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

int Game2048Controller::bestScore() const { return m_bestScore; }

bool Game2048Controller::hasSavedGame() const { return m_hasSavedGame; }

QVariantList Game2048Controller::savedBoard() const { return m_savedBoard; }

bool Game2048Controller::savedHasWon() const { return m_savedHasWon; }

void Game2048Controller::saveState(const QVariantList &board, bool hasWon) {
  m_savedBoard = board;
  m_savedHasWon = hasWon;
  m_hasSavedGame = true;
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
  if (score <= m_bestScore)
    return;
  m_bestScore = score;
  saveHighScore("scores/2048", score);
  emit bestScoreChanged();
}
