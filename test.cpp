#include <QtCore>

#include "Future.h"
#include "ThreadPool.h"
#include "util.h"


const uint N = 10000;
const QString Salt = "5417";


QByteArray md5_N_times(QString line)
{
    QByteArray data = line.toUtf8();
    for ( uint i = 0; i < N; ++i )
        data = QCryptographicHash::hash(Salt.toUtf8() + data, QCryptographicHash::Md5);
    return data;
}


int main()
{
    QTextStream input(stdin);
    QTextStream output(stdout);

    /*

        // Sequential

        while ( not input.atEnd() )
            output << md5_N_times(input.readLine()).toHex() << endl;

    /*/

        // Parallel

        ThreadPool pool;

        FutureIterator<QByteArray> results = pool.map(
            QTextStreamIterator(input),
            md5_N_times
        );

        while ( results.hasNext() )
        {
            output << results.next().toHex() << endl;
        }

    //*/

    return 0;
}
