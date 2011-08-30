#ifndef MESUREINDIVIDUELLE_H
#define MESUREINDIVIDUELLE_H

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
};

#endif // MESUREINDIVIDUELLE_H
