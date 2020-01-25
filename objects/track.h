#ifndef TRACK_H
#define TRACK_H

#include <QLineF>
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include <QDebug>

#define TRACKSIZE 60

class Track
{
public:
    Track(int f_width,int f_height)
        :m_innerWalls()
        ,m_outerWalls()
        ,m_width(f_width)
        ,m_height(f_height)
        ,m_startX(f_width/2)
        ,m_startY(f_height/2)
    {
        init();
    }

    void init()
    {
        const int trackWallCount = TRACKSIZE;

        qreal centerX = m_width/2;
        qreal centerY = m_height/2;
        qreal r = 120;
        qreal R = 250;

        // Get start point for cars
        m_startX -= ((r + 30 * qSin((-45)*M_PI/180) ) * 1.5+(R + 60 * qSin((-45)*M_PI/180) ) * 1.5)/2;

        QPointF innerPoints[trackWallCount];
        QPointF outerPoints[trackWallCount];
        int idx=0;
        for(int angle = 180; angle<540; angle+=360/trackWallCount, ++idx)
        {
            innerPoints[idx].setX(centerX + (r + 30 * qSin((-45+(angle-180)*6)*M_PI/180) ) * 1.5 * qCos(angle*M_PI/180));
            innerPoints[idx].setY(centerY + (r + 30 * qSin((-45+(angle-180)*6)*M_PI/180) ) * qSin(angle*M_PI/180));

            outerPoints[idx].setX(centerX + (R + 60 * qSin((-45+(angle-180)*6)*M_PI/180) ) * 1.5 * qCos(angle*M_PI/180));
            outerPoints[idx].setY(centerY + (R + 60 * qSin((-45+(angle-180)*6)*M_PI/180) ) * qSin(angle*M_PI/180));
        }

        for(int i = 0; i < trackWallCount; ++i)
        {
            m_innerWalls[i].setPoints(innerPoints[i%trackWallCount], innerPoints[(i+1)%trackWallCount]);
            m_outerWalls[i].setPoints(outerPoints[i%trackWallCount], outerPoints[(i+1)%trackWallCount]);

            m_pointGates[i].setPoints(innerPoints[i%trackWallCount], outerPoints[i%trackWallCount]);
        }
    }

    void draw(QPainter& f_qpainter_r, bool f_drawTrack_b, bool f_drawRewardGate_b)
    {
        for(int i = 0; i< TRACKSIZE; ++i)
        {
            // Draw walls
            if(f_drawTrack_b)
            {
                QPen pen;
                f_qpainter_r.setPen(pen);
                pen.setBrush(Qt::blue);
                f_qpainter_r.drawLine(m_innerWalls[i]);
                f_qpainter_r.drawLine(m_outerWalls[i]);
            }

            // Draw Point Gates
            if(f_drawRewardGate_b)
            {
                QPen pen;
                f_qpainter_r.setPen(pen);
                pen.setBrush(Qt::red);
                f_qpainter_r.setPen(pen);
                f_qpainter_r.drawLine(m_pointGates[i]);
            }
        }        
    }

    QLineF m_innerWalls[TRACKSIZE];
    QLineF m_outerWalls[TRACKSIZE];

    QLineF m_pointGates[TRACKSIZE];

    int m_width;
    int m_height;
    qreal m_startX;
    qreal m_startY;
};

#endif // TRACK_H
