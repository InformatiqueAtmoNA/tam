#ifndef MESUREINDIVIDUELLE_H
#define MESUREINDIVIDUELLE_H
#include "float.h"


// \class MesureIndividuelle Informations de mesure
class MesureIndividuelle : public QObject {
    QVector<float> m_mesures;

public:
    MesureIndividuelle() : QObject() {}
    MesureIndividuelle (const MesureIndividuelle& mesureIndividuelle) : QObject()
    {
        for(int i=0;i<mesureIndividuelle.count();i++)
            m_mesures.append(mesureIndividuelle.at(i));
    }

    MesureIndividuelle& operator=(const MesureIndividuelle& mesureIndividuelle)
    {
        for(int i=0;i<mesureIndividuelle.count();i++)
            m_mesures.append(mesureIndividuelle.at(i));
        return *this;
    }

    inline float operator[](int i) {return m_mesures.at(i);}
    inline void append(float polluant) {m_mesures.append(polluant);}
    inline float at(int i) const {return m_mesures.at(i);}
    inline bool isEmpty() const {return m_mesures.isEmpty();}
    inline int count() const {return m_mesures.count();}
    inline void clear() {m_mesures.clear();}

    inline float getMin()
    {
        float min = FLT_MAX;
        for(int i=0;i<m_mesures.count();i++) {
            if(m_mesures.at(i)<min)
                min = m_mesures.at(i);
        }
        return min;
    }

    inline float getMax()
    {
        float max = FLT_MIN;
        for(int i=0;i<m_mesures.count();i++) {
            if(m_mesures.at(i)<max)
                max = m_mesures.at(i);
        }
        return max;
    }
};

#endif // MESUREINDIVIDUELLE_H
