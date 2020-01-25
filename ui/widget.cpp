#include "ui/widget.h"
#include "./ui_widget.h"
#include <QInputEvent>
#include <QDebug>

Widget::Widget(int f_width, int f_height, QWidget *parent_p)
    : QWidget(parent_p)
    , m_ui_p(new Ui::Widget)
    , m_pointLabel("Points: 0", this)
    , m_generationLabel("Generation: 0", this)
    , m_gameLabel("Start the game", this)
    , m_playAloneButton("Play alone!", this)
    , m_playWithAiButton("Beat an AI!", this)
    , m_showOnlyAiButton("Show only AI", this)
    , m_restartButton("Play Again",this)
    , m_backToMenuButton("Back to menu", this)
    , m_drawRewardGates(tr("Show reward gates"), this)
    , m_drawTrack(tr("Show track"), this)
    , m_drawRays(tr("Show lidar"), this)
    , m_game(f_width, f_height)
    , m_gameIsRunning_b(false)
    , m_aiPlays_b(false)
    , m_playerPlays_b(false)
    , m_timer_p(nullptr)
{
    m_ui_p->setupUi(this);

    setWindowTitle("2DCarGameWithGA");

    this->resize(f_width, f_height);

    QFont font = m_pointLabel.font();
    font.setPointSize(10);
    font.setBold(true);

    // Control Panel
    m_drawRewardGates.move(0,0);
    m_drawRewardGates.setFont(font);

    m_drawTrack.move(0,20);
    m_drawTrack.setFont(font);
    m_drawTrack.setChecked(true);

    // Car Panel
    m_drawRays.move(0,50);
    m_drawRays.setFont(font);

    m_pointLabel.move(0,70);
    m_pointLabel.setFont(font);

    // Ai Panel
    m_generationLabel.move(0,100);
    m_generationLabel.setFont(font);

    font.setPointSize(40);
    m_gameLabel.setFont(font);
    m_gameLabel.move(f_width/2 - 150, f_height/2 - 180);

    m_playAloneButton.move(f_width/2 - 150, f_height/2 - 100);
    m_playAloneButton.setFont(font);
    connect(&m_playAloneButton, SIGNAL (released()),this, SLOT (handlePlayAlone()));

    m_playWithAiButton.move(f_width/2 - 150, f_height/2 - 20);
    m_playWithAiButton.setFont(font);
    connect(&m_playWithAiButton, SIGNAL (released()),this, SLOT (handlePlayWithAi()));

    m_showOnlyAiButton.move(f_width/2 - 180, f_height/2 + 60);
    m_showOnlyAiButton.setFont(font);
    connect(&m_showOnlyAiButton, SIGNAL (released()),this, SLOT (handleShowOnlyAi()));

    m_restartButton.move(f_width/2 - 150, f_height/2 - 100);
    m_restartButton.setFont(font);
    connect(&m_restartButton, SIGNAL (released()),this, SLOT (handleRestartButton()));

    m_backToMenuButton.move(f_width/2 - 150, f_height/2 - 20);
    m_backToMenuButton.setFont(font);
    connect(&m_backToMenuButton, SIGNAL (released()),this, SLOT (handleBackToMenu()));

    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_KeyCompression);

    m_timer_p = new QTimer(this);
    connect(m_timer_p, SIGNAL(timeout()), this, SLOT(run()));

    for(int& key : m_keys)
        key = 0;

    removeControlPanel();
    removeCarPanel();
    removeAiPanel();
    removeRestartMenu();
    showMainMenu();
}

void Widget::showMainMenu()
{
    m_gameLabel.setText("Start the game");
    m_gameLabel.setVisible(true);
    m_playAloneButton.setVisible(true);
    m_playWithAiButton.setVisible(true);
    m_showOnlyAiButton.setVisible(true);
}

void Widget::removeMainMenu()
{
    m_gameLabel.setVisible(false);
    m_playAloneButton.setVisible(false);
    m_playWithAiButton.setVisible(false);
    m_showOnlyAiButton.setVisible(false);
}

void Widget::showRestartMenu()
{
    m_gameLabel.setVisible(true);
    m_backToMenuButton.setVisible(true);
    m_restartButton.setVisible(true);
}

void Widget::removeRestartMenu()
{
    m_gameLabel.setVisible(false);
    m_backToMenuButton.setVisible(false);
    m_restartButton.setVisible(false);
}

void Widget::showCarPanel()
{
    m_pointLabel.setVisible(true);
    m_drawRays.setVisible(true);
}

void Widget::removeCarPanel()
{
    m_pointLabel.setVisible(false);
    m_drawRays.setVisible(false);
}

void Widget::showAiPanel()
{
    m_generationLabel.setVisible(true);
}

void Widget::removeAiPanel()
{
    m_generationLabel.setVisible(false);
}

void Widget::showControlPanel()
{
    m_drawRewardGates.setVisible(true);
    m_drawTrack.setVisible(true);
}

void Widget::removeControlPanel()
{
    m_drawRewardGates.setVisible(false);
    m_drawTrack.setVisible(false);
}

void Widget::startGame()
{
    m_game.init(m_playerPlays_b, m_aiPlays_b);
    m_gameIsRunning_b = true;
    m_timer_p->start(20);
}

void Widget::endGame()
{
    m_gameIsRunning_b = false;
    m_timer_p->stop();
}

Widget::~Widget()
{
    delete m_ui_p;
}

void Widget::run()
{
    m_game.run(m_keys);

    handleGameEnd();

    updatePoints();

    update();
}

void Widget::handleGameEnd()
{
    if(m_game.isGameEnded())
    {
        endGame();
        if(m_playerPlays_b && m_game.isUserWon())
        {
            m_gameLabel.setText("You Won!");
            m_restartButton.setText("Play Again!");
        }
        else if(m_playerPlays_b)
        {
            m_gameLabel.setText("You Loose!");
            m_restartButton.setText("Try Again!");
        }
        else
        {
            m_gameLabel.setText("Game Ended!");
            m_restartButton.setText("Try Again!");
        }
        showRestartMenu();
    }
}

void Widget::updatePoints()
{
    m_pointLabel.setText("Points: " + QVariant(m_game.m_playerCar.getPoint()).toString());
    m_pointLabel.resize(m_pointLabel.sizeHint());

    m_generationLabel.setText("Generation: " + QVariant(m_game.m_geneticAlgo.m_generationCntr).toString());
    m_generationLabel.resize(m_generationLabel.sizeHint());
}

/////////////////////////////// SCREEN UPDATES //////////////////////////////////

void Widget::paintEvent(QPaintEvent *)
{
    QPainter qpainter(this);

    if(m_gameIsRunning_b)
    {
        // Draw game
        m_game.drawGameElements(qpainter, m_drawRays.isChecked(), m_drawTrack.isChecked() , m_drawRewardGates.isChecked());
    }
}

///////////////////////////// KEYHANDLER FUNCTIONS /////////////////////////////

void Widget::keyPressEvent(QKeyEvent *f_event_p)
{
    m_keys[f_event_p->key() - Qt::Key_Left] = 1;
}

void Widget::keyReleaseEvent(QKeyEvent *f_event_p)
{
    m_keys[f_event_p->key() - Qt::Key_Left] = 0;
}

/////////////////////////////// SLOT FUNCTIONS ////////////////////////////////

void Widget::handlePlayAlone()
{
    m_playerPlays_b = true;
    m_aiPlays_b = false;
    removeMainMenu();
    showControlPanel();
    showCarPanel();
    startGame();
}

void Widget::handlePlayWithAi()
{
    m_playerPlays_b = true;
    m_aiPlays_b = true;
    removeMainMenu();
    showControlPanel();
    showCarPanel();
    showAiPanel();
    startGame();
}

void Widget::handleShowOnlyAi()
{
    m_playerPlays_b = false;
    m_aiPlays_b = true;
    removeMainMenu();
    showControlPanel();
    showAiPanel();
    startGame();
}

void Widget::handleRestartButton()
{
    m_game.reset();
    removeRestartMenu();
    startGame();
}

void Widget::handleBackToMenu()
{
    removeRestartMenu();
    removeControlPanel();
    removeCarPanel();
    removeAiPanel();
    showMainMenu();
}

