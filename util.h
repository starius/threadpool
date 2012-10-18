#ifndef advq_util_h
#define advq_util_h


#include <QtCore>


inline
QStringList sorted(const QStringList & orig)
{
    QStringList _r = orig;
    _r.sort();
    return _r;
}


class QTextStreamIterator
{

    QTextStream & ts;

public:

    QTextStreamIterator(QTextStream & ts):
        ts(ts)
    {
    }

    bool hasNext() const
    {
        return not ts.atEnd();
    }

    QString next()
    {
        return ts.readLine();
    }

}; // class QTextStreamIterator


#endif // advq_util_h
