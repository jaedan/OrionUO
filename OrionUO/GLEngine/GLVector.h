

#ifndef GLVECTOR_H
#define GLVECTOR_H

class CVector
{
public:
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;

    CVector(double x = 0, double y = 0, double z = 0);
    ~CVector(){};

    void Link(const CVector &v);

    void Link(double x, double y, double z);

    void Add(const CVector &v);

    void Add(double x, double y, double z);

    void Merge(const CVector &v);

    void Merge(double x, double y, double z);

    void Normalize();
};

#endif
