#ifndef WIDGET_H
#define WIDGET_H

#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QTimer>

#include "game/game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(int width, int height, QWidget *parent = nullptr);
    ~Widget();

    void handleGameEnd();

    void updatePoints();

    void showMainMenu();
    void removeMainMenu();

    void showRestartMenu();
    void removeRestartMenu();

    void showCarPanel();
    void removeCarPanel();

    void showAiPanel();
    void removeAiPanel();

    void showControlPanel();
    void removeControlPanel();

    void startGame();
    void endGame();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *f_event_p);
    void keyReleaseEvent(QKeyEvent *f_event_p);

private:
    Ui::Widget *m_ui_p;

    QLabel m_pointLabel;
    QLabel m_generationLabel;
    QLabel m_gameLabel;
    QPushButton m_playAloneButton;
    QPushButton m_playWithAiButton;
    QPushButton m_showOnlyAiButton;
    QPushButton m_restartButton;
    QPushButton m_backToMenuButton;

    QCheckBox m_drawRewardGates;
    QCheckBox m_drawTrack;
    QCheckBox m_drawRays;

    Game m_game;
    int m_keys[4];

    bool m_gameIsRunning_b;
    bool m_aiPlays_b;
    bool m_playerPlays_b;
    QTimer* m_timer_p;

private slots:
    void run();
    void handlePlayAlone();
    void handlePlayWithAi();
    void handleShowOnlyAi();
    void handleRestartButton();
    void handleBackToMenu();

};
#endif // WIDGET_H
