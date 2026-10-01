#include "flappybirdcontroller.h"
#include "../utils/high_scores.h"

static const QString kHighScoreKey = "scores/flappybird";

FlappyBirdController::FlappyBirdController(Model *model, QObject *parent)
    : ButtonController{model, 6, parent} {
  m_bestScore = HighScores::load(kHighScoreKey);
}

int FlappyBirdController::bestScore() const { return m_bestScore; }

bool FlappyBirdController::didJump() const { return m_didJump; }
void FlappyBirdController::setDidJump(bool didJump) {
  if (didJump != this->m_didJump) {
    this->m_didJump = didJump;
    emit didJumpChanged();
  }
}

void FlappyBirdController::enterButtonPressed() {
  this->setDidJump(true);
  this->setDidJump(false);
}

void FlappyBirdController::saveScore(int score) {
  if (score <= m_bestScore)
    return;
  m_bestScore = score;
  HighScores::save(kHighScoreKey, score);
  emit bestScoreChanged();
}
