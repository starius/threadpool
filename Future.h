#ifndef advq_Future_h
#define advq_Future_h


#include <QtCore>


/// \interface
template <class T>
class IterableFuture
{

public:

    virtual
    bool waitForReadyResult() = 0;

    virtual
    T takeReadyResult() = 0;

}; // class IterableFuture


template <class T>
class FutureIterator
{

    QSharedPointer<IterableFuture<T>> future;

public:

    FutureIterator(QSharedPointer<IterableFuture<T>> future):
        future(future)
    {
    }

    /// blocking!
    bool hasNext()
    {
        return future->waitForReadyResult();
    }

    /// blocking!
    T next()
    {
        return future->takeReadyResult();
    }

}; // class FutureIterator


#endif // advq_Future_h
