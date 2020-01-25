#ifndef GAME_H
#define GAME_H

#include <vector>
#include <QPainter>

#include "objects/car.h"
#include "objects/track.h"
#include "algo/genetic_algo.h"

class Game
{
public:
    Game(int f_width, int f_height)
        :m_track(f_width, f_height)
        ,m_playerCar(m_track.m_startX, m_track.m_startY, 20, Qt::blue)
        ,m_pointToWin_i(100)
        ,m_end_b(false)
        ,m_playerWon_b(false)
        ,m_aiWon_b(false)
        ,m_geneticAlgo(m_track.m_startX, m_track.m_startY, 20, m_track, m_pointToWin_i)
        ,m_playerInGame_b(false)
        ,m_aiInGame_b(false)
    {}

    void init(bool f_playerInGame_b, bool f_aiInGame_b)
    {
        reset();

        m_playerInGame_b = f_playerInGame_b;
        m_aiInGame_b = f_aiInGame_b;
    }

    void reset()
    {
        m_playerCar.reset();
        m_geneticAlgo.reset();
        m_end_b = false;
        m_playerWon_b = false;
        m_aiWon_b = false;
    }

    void run(int (&f_keys_arr_r)[4])
    {
        if(m_playerInGame_b)
        {
            if(m_end_b)
            {
                m_playerCar.reset();
                m_end_b = false;
            }
            // Move car
            m_playerCar.move(f_keys_arr_r);
            m_playerCar.calc_state(m_track);

            // Check collision
            if(m_playerCar.collided(m_track))
            {
                m_end_b = true;
                m_playerWon_b = false;
            }else
            {
                m_playerCar.checkGates(m_track);

                if(m_playerCar.getPoint() >= m_pointToWin_i)
                {
                    m_end_b = true;
                    m_playerWon_b = true;
                }
                else
                {
                    // Intentionally left blank
                }
            }
        }

        if(m_aiInGame_b)
        {
            // Move population
            m_geneticAlgo.move();

            if(m_geneticAlgo.is_population_died())
            {
                m_geneticAlgo.natural_selection();
            }
        }
    }

    void drawGameElements(QPainter& f_qpainter_r, bool f_drawRays_b, bool f_drawTrack_b, bool f_drawPointGates_b)
    {
        m_track.draw(f_qpainter_r, f_drawTrack_b, f_drawPointGates_b);

        if(m_aiInGame_b)
            m_geneticAlgo.draw(f_qpainter_r);

        if(m_playerInGame_b)
            m_playerCar.draw(f_qpainter_r, f_drawRays_b);
    }

    bool isUserWon()
    {
        return m_playerWon_b;
    }

    bool isGameEnded()
    {
        if(m_playerInGame_b && !m_aiInGame_b)
        {
            return m_end_b;
        }
        else if(!m_playerInGame_b && m_aiInGame_b)
        {
            return m_aiWon_b;
        }
        else if (m_playerInGame_b && m_aiInGame_b)
        {
            return m_playerWon_b || m_aiWon_b;
        }
        else
        {
            qDebug() << "Nothing is playing! It should not happen!";
        }
        return m_end_b;
    }

    Track m_track;
    Car m_playerCar;

    int m_pointToWin_i;
    bool m_end_b;
    bool m_playerWon_b;
    bool m_aiWon_b;
    GeneticAlgo m_geneticAlgo;

    bool m_playerInGame_b;
    bool m_aiInGame_b;
};

#endif // GAME_H
