/* ==================================================
   Threading project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   copied to FGx project
   ================================================== */
/* ********************************************************
   *  workThread.cpp[.h]
   *
   *  Created by Geoff R. Mclane, Paris
   *  (c) Aug 2011 GPL2 (or later)
   *
   *  Create a simple 'worker' thread, using QThread
   *  Instantiation: Either through declaration
   *  workThread thread, or allocated
   *  workThread * worker = new workThread, and remember
   *  to do 'delete worker' at end of the application.
   *
   *  API: Call job_number = worker->work( function ), with the function
   *       to be run. Function must be void foo() type.
   *       2011-08-29: Or the function can be void foo(void *) type
   *  This can be called multiple times, and the 'jobs' will be stacked,
   *  and processed one after the other.
   *
   *  SIGNAL: workThread emits a work_done(int,int) when completed, passing the
   *               job number, and ms of running, so use
   *           connect(worker, SIGNAL(work_done(int,int)),
   *                   this, SLOT(done_it(int,int)));
   *          to connect this to a SLOT job_done(int job_number, int ms)
   *
   *  Presently, if processing a LONG job at application exit, the full exit of
   *     the application will only happen when the job is done.
   *  A future enhancement would be to provide a 'cancel' mechanism, to enable
   *     killing the thread even if 'working'...
   *
   ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ** "AS IS" WITH NO EXPRESS OR IMPLIED WARRANTIES OF ANY TYPE.
   *
   ******************************************************** */
#ifndef WORKTHREAD_H
#define WORKTHREAD_H
/* =======================================================
   HISTORY: Growing upwards
   2011/12/14 - In qt_osm_map project, remove macro - always use (void *), and
                removed 'WorkType' enum. change to int, defined by user.
                Add loginfo(QString), for easy change of outLog(msg)
   2011/08/31 - Add passing of a void * to the function, under macro ADD_VOID_PTR
   2011/08/24 - First implementation in qt_test_gui/src/workThread.cpp
   ======================================================= */
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <QList>
#include "utilities.h"  // for outLog(msg) and getElapTimeStg(ms)

typedef void (*ACTION)(void *);
typedef struct tagWORKITEM {
    ACTION act;
    void * vp;
    int user_type;  // defined by user
    bool abort;
}WORKITEM, * PWORKITEM;
typedef QList<WORKITEM> THREAD_LIST;

class workThread : public QThread
{
    Q_OBJECT
public:
    workThread(QObject *parent = 0);
    ~workThread();
    bool in_function;
    bool was_terminated;
    int work(ACTION func, void *vp);
    void loginfo(QString msg) {
#ifdef GEN_UTILITIES_H
        outLog(msg);
#else
        Q_UNUSED(msg);
#endif // #ifdef GEN_UTILITIES_H

    }

signals:
    void work_done(int jn, int ms);

protected:
    void run();

private:
    QMutex mutex;
    QWaitCondition condition;
    int restart;
    bool abort;
    THREAD_LIST jobs;
    int jobs_in;
    int jobs_out;
    int cyc_count;
    int cum_running;
    QTime wt;
};

// a convenient SLEEP (ms) function
class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

// structure for the application
// to keep track of 'work' passed to thread
typedef struct tagWORK {
    int work_type;  // will determine actions when DONE
    int work_num;
    QTime work_tt;
    QString work_desc;
    bool work_done;
    bool abort_work;
}WORK, *PWORK;

typedef QList<PWORK> WORK_LIST;

#endif // WORKTHREAD_H
// eof - workthread.h
