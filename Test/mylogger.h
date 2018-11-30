#pragma once
#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <QObject>

class MyLogger : public QObject
{
	Q_OBJECT
public:
	static MyLogger & instance() {
		static MyLogger * _instance = 0;
		if (_instance == 0) {
			_instance = new MyLogger();
		}
		return *_instance;
	}

private:
	MyLogger() {}
	~MyLogger() = default;
	MyLogger& operator=(const MyLogger&) = delete;
	MyLogger(const MyLogger&) = delete;
signals:
	void writeLog(QString log);
};


#endif // MYLOGGER_H