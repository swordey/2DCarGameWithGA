#ifndef SIMPLE_BRAIN_H
#define SIMPLE_BRAIN_H

#include <vector>
#include <QDebug>
#include <QRandomGenerator>

class CSimpleBrain
{
public:
    CSimpleBrain(int f_moveNumber)
        :m_moveIdx(0)
    {
        m_moves_vec.resize(f_moveNumber);
        generateRandom();
    }

    ~CSimpleBrain(){}

    CSimpleBrain(const CSimpleBrain& f_brain_r)
        :m_moveIdx(f_brain_r.m_moveIdx)
    {
        m_moves_vec.resize(f_brain_r.m_moves_vec.size());
        const int size = f_brain_r.m_moves_vec.size();
        for(int i=0; i < size; ++i)
        {
            m_moves_vec[i] = f_brain_r.m_moves_vec[i];
        }
    }

    CSimpleBrain& operator=(const CSimpleBrain& f_brain_r)
    {
        m_moveIdx = f_brain_r.m_moveIdx;
        int size = static_cast<int>(f_brain_r.m_moves_vec.size());
        for(int i = 0; i < size; ++i)
        {
            m_moves_vec[i] = f_brain_r.m_moves_vec[i];
        }
        return *this;
    }

    void generateRandom()
    {
        const int size = m_moves_vec.size();
        for(int i=0; i < size; ++i)
        {
            int r = getRandomN();
            m_moves_vec[i] = r;
        }
    }

    void reset()
    {
        m_moveIdx = 0;
    }

    CSimpleBrain breed(const CSimpleBrain& f_otherBrain_r)
    {
        int crossOver1 = QRandomGenerator::global()->bounded(static_cast<int>(m_moves_vec.size()));
        int crossOver2 = QRandomGenerator::global()->bounded(static_cast<int>(m_moves_vec.size()));
        CSimpleBrain brain(*this);
        for(int i = crossOver1; i < crossOver2; ++i)
        {
            brain.m_moves_vec[i] = f_otherBrain_r.m_moves_vec[i];
        }
        return brain;
    }

    void mutate()
    {
        const int size = static_cast<int>(m_moves_vec.size());
        for(int i = 0; i < size; ++i)
        {
             double rand = QRandomGenerator::global()->generateDouble();
             if(rand < 0.3)
             {
                m_moves_vec[i] = getRandomN();
             }
        }
    }

    void predict(int (&f_res_arr_r)[4])
    {
        const int e = m_moves_vec[m_moveIdx];
        for(int i=0;i<4;++i)
        {
            f_res_arr_r[i] = m_map[e][i];
        }
        ++m_moveIdx;
    }

    static int getRandomN()
    {
        return QRandomGenerator::global()->bounded(3);
    }

    const int m_map[3][4] = {{1,1,0,0},{0,1,0,0},{0,1,1,0}};
    int m_moveIdx;
    std::vector<int> m_moves_vec;
};

#endif // SIMPLE_BRAIN_H
