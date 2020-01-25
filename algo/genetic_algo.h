#ifndef GENETIC_ALGO_H
#define GENETIC_ALGO_H

#include "objects/car.h"
#include "objects/track.h"

#include <algorithm>

class GeneticAlgo
{
    enum
    {
        PARENT_TO_SURVIVE = 50,
        CAR_NUMBER = 100,
    };

public:
    GeneticAlgo(int f_x, int f_y, int f_rayNumber, Track& f_track_r, int f_pointToWin)
        :m_bestPoint(0)
        ,m_bestCar_p(nullptr)
        ,m_cars_vec()
        ,m_track_r(f_track_r)
        ,m_diedCarsCntr(0)
        ,m_fitnessSum(0)
        ,m_pointToWin(f_pointToWin)
        ,m_generationCntr(0)
    {
        for(int i=0;i<CAR_NUMBER;++i)
        {
            m_cars_vec.push_back(Car(f_x, f_y, f_rayNumber));
        }
    }

    void reset()
    {
        for(auto& car : m_cars_vec)
        {
            car.reset();
            car.m_brain.generateRandom();
        }
    }

    void move()
    {
        m_bestPoint = 0;
        m_diedCarsCntr = 0;
        for(Car& car : m_cars_vec)
        {
            if(!car.m_died)
            {
                int dirs[4];
                car.m_brain.predict(dirs);
                car.move(dirs);
                car.calc_state(m_track_r);

                if(car.collided(m_track_r))
                {

                }
                car.checkGates(m_track_r);

                if(car.m_point > m_bestPoint)
                {
                    m_bestPoint = car.m_point;
                    m_bestCar_p = &car;
                }
            }
            else
            {
                ++m_diedCarsCntr;
            }
        }
    }

    bool didWin()
    {
        return m_bestPoint >= m_pointToWin;
    }

    void calc_point_sum()
    {
        m_bestPoint = 0;
        m_fitnessSum = 0;
        for(auto& car: m_cars_vec)
        {
            int curPoint = car.getPoint();
            m_bestPoint = (curPoint > m_bestPoint) ? curPoint : m_bestPoint;
            m_fitnessSum += car.getFitness();
        }
    }

    Car* select_parent()
    {
        int random = QRandomGenerator::global()->bounded(m_fitnessSum+1);
        int runningSum = 0;
        int size = static_cast<int>(m_cars_vec.size());
        for(int i = 0; i < size; ++i)
        {
            runningSum += m_cars_vec[i].getFitness();
            if(runningSum >= random)
            {
                return &(m_cars_vec[i]);
            }
        }
        qDebug() << "Couldn't find parent!";
        return &(m_cars_vec[0]);
    }

    void mutate_n_reset()
    {
        for(int i = 0; i < m_cars_vec.size(); ++i)
        {
            if(PARENT_TO_SURVIVE <= i)
            {
                m_cars_vec[i].m_brain.mutate();
            }
            m_cars_vec[i].reset();
        }
    }

    void breed()
    {
        const int numOfChildsToBreed = (CAR_NUMBER - PARENT_TO_SURVIVE);
        Car* parents[numOfChildsToBreed*2];
        for(int i = 0; i < numOfChildsToBreed*2; ++i)
        {
            parents[i] = select_parent();
        }

        std::vector<Car> newCars(m_cars_vec);
        for(int i = PARENT_TO_SURVIVE; i < m_cars_vec.size(); ++i)
        {
            newCars[i].m_brain = parents[i - PARENT_TO_SURVIVE]->m_brain.breed(parents[i - PARENT_TO_SURVIVE + numOfChildsToBreed]->m_brain);
        }
        m_cars_vec = newCars;
    }

    void natural_selection()
    {
        // Sort cars
        std::sort(m_cars_vec.begin(),m_cars_vec.end(),more_point());

        // Calculate point sum
        calc_point_sum();

        // Breed
        breed();

        // Mutate brains and reset cars
        mutate_n_reset();

        ++m_generationCntr;
    }

    bool is_population_died()
    {
        return CAR_NUMBER == m_diedCarsCntr;
    }

    void draw(QPainter& qpainter)
    {
        for(Car& car : m_cars_vec)
        {
            car.draw(qpainter, false);
        }
    }

    int m_bestPoint;
    Car* m_bestCar_p;
    std::vector<Car> m_cars_vec;
    Track& m_track_r;
    int m_diedCarsCntr;
    int m_fitnessSum;
    int m_pointToWin;
    int m_generationCntr;
};


#endif // GENETIC_ALGO_H
