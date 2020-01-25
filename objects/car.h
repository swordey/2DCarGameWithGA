#ifndef CAR_H
#define CAR_H

#include <QtMath>
#include <QPainter>
#include <QPointF>
#include <QDebug>

#include "objects/track.h"
#include "algo/simple_brain.h"

class Ray
{
public:
    Ray(int f_numOfRays)
        :m_numOfRays(f_numOfRays)
        ,m_rayLength(150)
        ,m_center()
        ,m_rayEndpoints_p(nullptr)
        ,m_dists_p(nullptr)
    {
        m_rayEndpoints_p = new QPointF[m_numOfRays];
        m_dists_p = new float[m_numOfRays];
    }
    ~Ray()
    {
    }

    Ray(const Ray& f_ray_r)
        :m_numOfRays(f_ray_r.m_numOfRays)
        ,m_rayLength(150)
        ,m_center()
        ,m_rayEndpoints_p(nullptr)
        ,m_dists_p(nullptr)
    {
        m_rayEndpoints_p = new QPointF[m_numOfRays];
        m_dists_p = new float[m_numOfRays];
        for(int i=0;i < m_numOfRays; ++i)
        {
            m_rayEndpoints_p[i] = f_ray_r.m_rayEndpoints_p[i];
            m_dists_p[i] = f_ray_r.m_dists_p[i];
        }
    }

    Ray& operator= (const Ray& f_ray_r)
    {
        m_numOfRays = f_ray_r.m_numOfRays;
        m_rayLength = f_ray_r.m_rayLength;
        m_center = f_ray_r.m_center;
        for(int i=0;i < m_numOfRays; ++i)
        {
            m_rayEndpoints_p[i] = f_ray_r.m_rayEndpoints_p[i];
            m_dists_p[i] = f_ray_r.m_dists_p[i];
        }
        return *this;
    }

    void update(int f_carAngle, QPointF f_center, Track& f_track_r)
    {
        m_center = f_center;
        int idx=0;
        for(int angle = 0;angle < 360; angle += 360/m_numOfRays, ++idx)
        {
            m_rayEndpoints_p[idx].setX(m_center.x() + m_rayLength * qSin((f_carAngle + angle)*M_PI/180));
            m_rayEndpoints_p[idx].setY(m_center.y() - m_rayLength * qCos((f_carAngle + angle)*M_PI/180));
            m_dists_p[idx] = 1;

            QLineF line(m_center, m_rayEndpoints_p[idx]);
            QPointF inters;
            for(int i=0;i<TRACKSIZE;++i)
            {
                if(f_track_r.m_outerWalls[i].intersects(line, &inters) == 1 || f_track_r.m_innerWalls[i].intersects(line, &inters) == 1)
                {
                    QPointF newVec = (inters - m_center);
                    float newDist = qSqrt(qPow(newVec.x(),2)+qPow(newVec.y(),2))/m_rayLength;
                    if(m_dists_p[idx] > newDist)
                    {
                        m_rayEndpoints_p[idx] = inters;
                        m_dists_p[idx] = newDist;
                    }
                }
            }
        }
    }

    float* getDists()
    {
        return m_dists_p;
    }

    void draw(QPainter& f_qpainter_r)
    {
        QPointF* currRay = m_rayEndpoints_p;
        for(int i = 0; i < m_numOfRays; ++i,++currRay)
        {
            f_qpainter_r.drawLine(m_center, *currRay);
        }
    }

    int m_numOfRays;
    int m_rayLength;
    QPointF m_center;
    QPointF* m_rayEndpoints_p;
    float* m_dists_p;
};

class Car
{
public:
    enum
    {
        ROTATIONAL_SPEED = 180/20,
        SPEED = 10,
        WIDTH = 40,
        HEIGHT = 60,
    };

    Car(int f_x, int f_y, int f_numOfRays, Qt::GlobalColor f_color = Qt::green)
        :m_startX(f_x)
        ,m_startY(f_y)
        ,m_centerX(f_x)
        ,m_centerY(f_y)
        ,m_angle(0.0F)
        ,m_velocity_x(0)
        ,m_velocity_y(0)
        ,m_carColor(f_color)
        ,m_color(f_color)
        ,m_died(false)
        ,m_point(0)
        ,m_fitness(0)
        ,m_currGateId(1)
        ,m_carLines_vec(4, QLineF(0,0,0,0))
        ,m_ray(f_numOfRays)
        ,m_brain(500)
    {
    }

    Car(const Car& f_car_r)
        :m_startX(f_car_r.m_startX)
        ,m_startY(f_car_r.m_startY)
        ,m_centerX(f_car_r.m_centerX)
        ,m_centerY(f_car_r.m_centerY)
        ,m_angle(f_car_r.m_angle)
        ,m_velocity_x(f_car_r.m_velocity_x)
        ,m_velocity_y(f_car_r.m_velocity_y)
        ,m_carColor(f_car_r.m_carColor)
        ,m_color(f_car_r.m_color)
        ,m_died(f_car_r.m_died)
        ,m_point(f_car_r.m_point)
        ,m_fitness(f_car_r.m_fitness)
        ,m_currGateId(f_car_r.m_currGateId)
        ,m_carLines_vec(f_car_r.m_carLines_vec)
        ,m_ray(f_car_r.m_ray)
        ,m_brain(f_car_r.m_brain)
    {}

    Car& operator=(const Car& f_car_r)
    {
        m_startX = f_car_r.m_startX;
        m_startY = f_car_r.m_startY;
        m_centerX = f_car_r.m_centerX;
        m_centerY = f_car_r.m_centerY;
        m_angle = f_car_r.m_angle;
        m_velocity_x = f_car_r.m_velocity_x;
        m_velocity_y = f_car_r.m_velocity_y;
        m_carColor = f_car_r.m_carColor;
        m_color = f_car_r.m_color;
        m_died = f_car_r.m_died;
        m_point = f_car_r.m_point;
        m_fitness = f_car_r.m_fitness;
        m_currGateId = f_car_r.m_currGateId;
        m_carLines_vec = f_car_r.m_carLines_vec;
        m_ray = f_car_r.m_ray;
        m_brain = f_car_r.m_brain;
        return *this;
    }

    ~Car()
    {
    }

    void reset()
    {
        m_currGateId = 1;
        m_point = 0;
        m_centerX = m_startX;
        m_centerY = m_startY;
        m_angle = 0.F;
        m_died = false;
        m_fitness = 0;
        m_color = m_carColor;
        m_brain.reset();
    }

    void calcCarLines()
    {
        qreal angle_rad = m_angle*M_PI/180;

        QPointF topLeftCorner(
                    m_centerX - WIDTH * qCos(angle_rad)/2 + HEIGHT * qSin(angle_rad)/2,
                    m_centerY - HEIGHT * qCos(angle_rad)/2 - WIDTH * qSin(angle_rad)/2);

        QPointF corners[]={{topLeftCorner.x(), topLeftCorner.y()},
                          {topLeftCorner.x() + WIDTH * qCos(angle_rad),topLeftCorner.y() + WIDTH * qSin(angle_rad)},
                          {topLeftCorner.x() + WIDTH * qCos(angle_rad) - HEIGHT * qSin(angle_rad),topLeftCorner.y() + WIDTH * qSin(angle_rad) + HEIGHT * qCos(angle_rad)},
                          {topLeftCorner.x() - HEIGHT * qSin(angle_rad),topLeftCorner.y()+HEIGHT*qCos(angle_rad)}};

        for(int i = 0; i < 4; ++i)
        {
            m_carLines_vec[i].setPoints(corners[i%4], corners[(i+1)%4]);
        }
    }

    void move(int (&f_dirs_r)[4]) // Left, Up, Right, Down
    {
        --m_fitness;

        if(f_dirs_r[0] == 1) // Left
        {
            m_angle -= ROTATIONAL_SPEED;
        }
        if(f_dirs_r[2] == 1) // Right
        {
            m_angle += ROTATIONAL_SPEED;
        }
        if(f_dirs_r[1] == 1) // Up
        {
            m_velocity_y = -qCos(-m_angle*M_PI/180.) * SPEED;
            m_velocity_x = -qSin(-m_angle*M_PI/180.) * SPEED;
        }
        else
        {
            m_velocity_x = 0;
            m_velocity_y = 0;
        }

        m_centerX += m_velocity_x;
        m_centerY += m_velocity_y;

        calcCarLines();
    }

    void calc_state(Track& f_track_r)
    {
        m_ray.update(m_angle, QPointF(m_centerX,m_centerY), f_track_r);
    }

    void draw(QPainter& f_qpainter_r, bool f_drawRays_b)
    {
        QPen pen;
        f_qpainter_r.setPen(pen);
        pen.setBrush(Qt::black);

        f_qpainter_r.translate(m_centerX, m_centerY);       //(3)
        f_qpainter_r.rotate(m_angle);                     //(2)
        f_qpainter_r.translate(-m_centerX, -m_centerY);     //(1)

        // Draw rect
        f_qpainter_r.drawRect(m_centerX - (WIDTH/2), m_centerY - (HEIGHT/2), WIDTH, HEIGHT);
        f_qpainter_r.fillRect(m_centerX - (WIDTH/2), m_centerY - (HEIGHT/2), WIDTH, HEIGHT, m_color);

        f_qpainter_r.resetTransform();

        if(f_drawRays_b)
            m_ray.draw(f_qpainter_r);
    }

    int getFitness() const
    {
        return m_fitness > 0 ? m_fitness : 0;
    }

    int getPoint() const
    {
        return m_point;
    }

    void checkGates(Track& f_track_r)
    {
        QPointF inters;
        for(QLineF line : m_carLines_vec)
        {
            if(line.intersects(f_track_r.m_pointGates[m_currGateId],&inters) == 1)
            {
                m_point += 1;
                m_fitness += 10;
                m_currGateId = (m_currGateId + 1) % TRACKSIZE;
                return;
            }
        }
    }

    bool collided(Track& f_track_r)
    {
        QPointF intersection;
        for(QLineF line : m_carLines_vec)
        {
            for(int i=0;i < TRACKSIZE; ++i)
            {
                if(f_track_r.m_innerWalls[i].intersects(line, &intersection) == 1 || f_track_r.m_outerWalls[i].intersects(line, &intersection) == 1)
                {
                    m_died = true;
                    m_color = Qt::red;
                    return true;
                }
            }
        }
        m_color = m_carColor;
        return false;
    }

    // Position
    qreal m_startX;
    qreal m_startY;
    qreal m_centerX;
    qreal m_centerY;
    qreal m_angle;
    qreal m_velocity_x;
    qreal m_velocity_y;

    Qt::GlobalColor m_carColor;
    Qt::GlobalColor m_color;
    bool m_died;
    int m_point;
    int m_fitness;

    // Params
    int m_currGateId;
    std::vector<QLineF> m_carLines_vec;
    Ray m_ray;
    CSimpleBrain m_brain;
};

struct more_point
{
    inline bool operator() (const Car& car1, const Car& car2)
    {
        return (car1.getFitness() > car2.getFitness());
    }
};

#endif // CAR_H
