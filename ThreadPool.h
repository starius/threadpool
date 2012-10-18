#ifndef advq_ThreadPool_h
#define advq_ThreadPool_h


#include <QtCore>

#include "Future.h"


class ThreadPool:
    public QThreadPool
{

    template <typename SourceIterator, typename ArgumentType, typename ResultType>
    struct MapFuture:
        public IterableFuture<ResultType>
    {

        typedef ResultType (*MapFunction)(ArgumentType);

        struct Worker:
            public QRunnable
        {

            typedef ResultType (*MapFunction)(ArgumentType);

            MapFunction f;
            ArgumentType arg;
            ResultType result;
            QMutex resultReadyMutex;
            bool resultReady;

            Worker(MapFunction f, ArgumentType arg):
                f(f),
                arg(arg),
                resultReady(false)
            {
                setAutoDelete(false);
            }

            void run()
            {
                QMutexLocker locker(&resultReadyMutex);
                result = f(arg);
                resultReady = true;
            }

            void waitForReadyResult()
            {
                forever
                {
                    if ( resultReady )
                        return;
                    QMutexLocker locker(&resultReadyMutex); // wait for result
                }
            }
        };

        QThreadPool & pool;

        SourceIterator sourceIterator;
        MapFunction f;
        QQueue<Worker*> workers;

        MapFuture(QThreadPool & pool, SourceIterator sourceIterator, MapFunction f):
            pool(pool),
            sourceIterator(sourceIterator),
            f(f)
        {
            for ( int i = 0; i < pool.maxThreadCount(); ++i )
            {
                if ( sourceIterator.hasNext() )
                {
                    Worker * worker = new Worker(f, sourceIterator.next());
                    pool.start(worker);
                    workers.enqueue(worker);
                }
                else
                {
                    break;
                }
            }
        }

        /// blocking!
        /// \returns if there is some results
        virtual
        bool waitForReadyResult()
        {
            if ( workers.isEmpty() )
                return false;
            Worker & firstWorker = *workers.first();
            firstWorker.waitForReadyResult();
            return true;
        }

        /// blocking!
        virtual
        ResultType takeReadyResult()
        {
            Q_ASSERT ( not workers.isEmpty() );

            Worker * firstWorker = workers.dequeue();

            // wait for result ready
            firstWorker->waitForReadyResult();

            // take the result
            ResultType result = firstWorker->result;

            // run another worker
            delete firstWorker;
            if ( sourceIterator.hasNext() )
            {
                Worker * worker = new Worker(f, sourceIterator.next());
                pool.start(worker);
                workers.enqueue(worker);
            }

            return result;
        }

    };

public:

    template <typename SourceIterator, typename ArgumentType, typename ResultType>
    FutureIterator<ResultType> map(SourceIterator sourceIterator, ResultType (f)(ArgumentType))
    {
        QSharedPointer<IterableFuture<ResultType>> future(
            new MapFuture<SourceIterator, ArgumentType, ResultType>(
                *this, sourceIterator, f
            )
        );
        return FutureIterator<ResultType>(future);
    }

}; // class ThreadPool


#endif // advq_ThreadPool_h
