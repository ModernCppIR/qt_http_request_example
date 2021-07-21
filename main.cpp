/***************************************************
*  File %{Cpp:License:FileName}
*  Copyright (C) 2021-7-21 ModernCpp.ir . All Rights Reserved.  *
***************************************************/

#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QTimer>

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	QTimer timer;
	QNetworkAccessManager* manager = new QNetworkAccessManager();

	QNetworkRequest req;
	timer.start(30000);
	timer.setSingleShot(true);

	req.setUrl(QUrl("https://moderncpp.ir"));
	QNetworkReply* rep = manager->post(req, "");
	QObject::connect(&timer, &QTimer::timeout, &a, [&rep]() { rep->abort(); });

	QObject::connect(manager, &QNetworkAccessManager::finished, manager,
					 &QNetworkAccessManager::deleteLater);
	QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
	QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply) {
		if (reply->error())
		{
			qDebug() << "ERROR!";
			qDebug() << reply->errorString();
		}
		else
		{
			qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
			qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
			qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
			qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
			qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
			QFile f("moderncpp.html");

			auto data = reply->readAll();
			qDebug() << data;
			if (f.open(QIODevice::WriteOnly))
			{
				f.write(data);
			}
			else
			{
				qDebug() << "error can not open file";
				qDebug() << reply->readAll();
			}
		}
	});

	return a.exec();
}
