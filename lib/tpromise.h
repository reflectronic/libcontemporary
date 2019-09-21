#ifndef TPROMISE_H
#define TPROMISE_H

#include <QObject>
#include <functional>
#include <QtConcurrent/QtConcurrent>
#include <the-libs_global.h>
#include <QFuture>

#define TPROMISE_CALL_NO_ARGS(fn) QMetaObject::invokeMethod(this, fn, Qt::QueuedConnection)
#define TPROMISE_CALL(fn, ...) QMetaObject::invokeMethod(this, fn, Qt::QueuedConnection, __VA_ARGS__)

template <typename T> class tPromise;

template<typename T> struct tPromiseResults {
    T result;
    QString error = "";
};

template<> struct tPromiseResults<void> {
    QString error = "";
};


template <typename T> struct tPromisePrivate {
    using SuccessFunction = std::function<void(T)>;
    using FailureFunction = std::function<void(QString)>;

    using RunFunction = std::function<T(QString&)>;
    using RunAsyncFunction = std::function<void(SuccessFunction, FailureFunction)>;

    bool resolved = false;
    bool errored = false;
    bool functionSetToRunAfterSuccess = false;
    bool functionSetToRunAfterFailure = false;
    bool deleteAfter = false;
    tPromiseResults<T> resolvedValue;
    QFuture<void> runFuture;

    SuccessFunction fnAfterSuccess;
    FailureFunction fnAfterFailure;
};

template <> struct tPromisePrivate<void> {
    using SuccessFunction = std::function<void()>;
    using FailureFunction = std::function<void(QString)>;

    using RunFunction = std::function<void(QString&)>;
    using RunAsyncFunction = std::function<void(SuccessFunction, FailureFunction)>;

    bool resolved = false;
    bool errored = false;
    bool functionSetToRunAfterSuccess = false;
    bool functionSetToRunAfterFailure = false;
    bool deleteAfter = false;
    tPromiseResults<void> resolvedValue;
    QFuture<void> runFuture;

    SuccessFunction fnAfterSuccess;
    FailureFunction fnAfterFailure;
};

template <typename T> class tPromise
{
    public:
        explicit tPromise(typename tPromisePrivate<T>::RunFunction functionToRun);
        explicit tPromise(typename tPromisePrivate<T>::RunAsyncFunction functionToRun);
        ~tPromise();

        tPromise<T>* then(typename tPromisePrivate<T>::SuccessFunction functionToRunAfterSuccess);
        tPromise<T>* error(typename tPromisePrivate<T>::FailureFunction functionToRunOnFailure);
        tPromiseResults<T> await();

        bool isResolved() {
            return d->resolved;
        }

        bool isErrored() {
            return d->errored;
        }

        bool isPending() {
            return !d->resolved && !d->errored;
        }

        void setDeleteAfter(bool deleteAfter) {
            d->deleteAfter = deleteAfter;
        }

        void deleteLater() {
            QTimer::singleShot(0, [=] {
                delete this;
            });
        }

    private:
        tPromisePrivate<T>* d;

        void watch();
};

template<typename T> void tPromise<T>::watch()
{
    QFutureWatcher<void>* runFutureWatcher = new QFutureWatcher<void>();
    runFutureWatcher->setFuture(d->runFuture);
    QObject::connect(runFutureWatcher, &QFutureWatcher<T>::finished, [=] {
        runFutureWatcher->deleteLater();

        if (d->resolvedValue.error != "") {
            d->errored = true;
            if (d->functionSetToRunAfterFailure) {
                d->fnAfterFailure(d->resolvedValue.error);
            }
        } else {
            d->resolved = true;
            if (d->functionSetToRunAfterSuccess) {
                d->fnAfterSuccess(d->resolvedValue.result);
            }
        }

        if (d->deleteAfter) this->deleteLater();
    });
}

template<> void tPromise<void>::watch()
{
    QFutureWatcher<void>* runFutureWatcher = new QFutureWatcher<void>();
    runFutureWatcher->setFuture(d->runFuture);
    QObject::connect(runFutureWatcher, &QFutureWatcher<void>::finished, [=] {
        runFutureWatcher->deleteLater();

        if (d->resolvedValue.error != "") {
            d->errored = true;
            if (d->functionSetToRunAfterFailure) {
                d->fnAfterFailure(d->resolvedValue.error);
            }
        } else {
            d->resolved = true;
            if (d->functionSetToRunAfterSuccess) {
                d->fnAfterSuccess();
            }
        }

        if (d->deleteAfter) this->deleteLater();
    });
}

template<typename T> tPromise<T>::tPromise(typename tPromisePrivate<T>::RunFunction functionToRun) {
    d = new tPromisePrivate<T>;
    d->runFuture = QtConcurrent::run([=]() -> void {
        QString err;
        d->resolvedValue.result = functionToRun(err);
        d->resolvedValue.error.swap(err);
    });

    watch();
}

template<> tPromise<void>::tPromise(typename tPromisePrivate<void>::RunFunction functionToRun) {
    d = new tPromisePrivate<void>;
    d->runFuture = QtConcurrent::run([=]() -> void {
        QString err;
        functionToRun(err);
        d->resolvedValue.error.swap(err);
    });

    watch();
}

template<typename T> tPromise<T>::tPromise(typename tPromisePrivate<T>::RunAsyncFunction functionToRun)
{

    d = new tPromisePrivate<T>;
    d->runFuture = QtConcurrent::run([=]() -> void {
        QEventLoop* loop = new QEventLoop;

        typename tPromisePrivate<T>::SuccessFunction successFunction = [=](T retVal) {
            d->resolvedValue.result = retVal;
            d->resolvedValue.error = "";
            QTimer::singleShot(0, loop, &QEventLoop::quit);
        };
        typename tPromisePrivate<T>::FailureFunction failureFunction = [=](QString error) {
            d->resolvedValue.error.swap(error);
            QTimer::singleShot(0, loop, &QEventLoop::quit);
        };

        functionToRun(successFunction, failureFunction);
        loop->exec();

        loop->deleteLater();
    });

    watch();
}

template<> tPromise<void>::tPromise(typename tPromisePrivate<void>::RunAsyncFunction functionToRun)
{

    d = new tPromisePrivate<void>;
    d->runFuture = QtConcurrent::run([=]() -> void {
        QEventLoop* loop = new QEventLoop;

        typename tPromisePrivate<void>::SuccessFunction successFunction = [=]() {
            d->resolvedValue.error = "";
            QTimer::singleShot(0, loop, &QEventLoop::quit);
        };
        typename tPromisePrivate<void>::FailureFunction failureFunction = [=](QString error) {
            d->resolvedValue.error.swap(error);
            QTimer::singleShot(0, loop, &QEventLoop::quit);
        };

        functionToRun(successFunction, failureFunction);
        loop->exec();

        loop->deleteLater();
    });

    watch();
}

template<typename T> tPromise<T>::~tPromise()
{
    delete d;
}

template<typename T> tPromise<T>* tPromise<T>::then(typename tPromisePrivate<T>::SuccessFunction functionToRunAfterSuccess) {
    Q_ASSERT(!d->functionSetToRunAfterSuccess);
    if (!d->resolved && !d->functionSetToRunAfterSuccess) {
        d->fnAfterSuccess = functionToRunAfterSuccess;
        d->functionSetToRunAfterSuccess = true;
        d->deleteAfter = true;
    }
    return this;
}

template<typename T> tPromise<T>* tPromise<T>::error(typename tPromisePrivate<T>::FailureFunction functionToRunOnFailure) {
    Q_ASSERT(!d->functionSetToRunAfterFailure);
    if (!d->resolved && !d->functionSetToRunAfterFailure) {
        d->fnAfterFailure = functionToRunOnFailure;
        d->functionSetToRunAfterFailure = true;
        d->deleteAfter = true;
    }
    return this;
}

template<typename T> tPromiseResults<T> tPromise<T>::await() {
    if (this->isPending()) {
        QEventLoop* loop = new QEventLoop();
        this->then(std::bind(&QEventLoop::quit, loop));
        this->error(std::bind(&QEventLoop::quit, loop));
        this->setDeleteAfter(false);
        loop->exec();
        loop->deleteLater();
    }

    tPromiseResults<T> retval = d->resolvedValue;
    this->deleteLater();
    return retval;
}


#endif // TPROMISE_H
