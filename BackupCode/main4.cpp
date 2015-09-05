#include <QCoreApplication>
#include <QDirIterator>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace cv;
using namespace std;
struct ColorMoments{
	double ch1Av;
	double ch2Av;
	double ch3Av;

	double ch1stdDev;
	double ch2stdDev;
	double ch3stdDev;

	double ch1Sk;
	double ch2Sk;
	double ch3Sk;
};

void removeSmallFiles(int maxSize, QString index, QString actual);
void removeSmallFiles(int maxSize, QString index);
void addNewPictoDB(string path, ColorMoments colorMoments);
//Convex,Box,
void removeSmallFiles(int maxSize, QString index){
	removeSmallFiles(maxSize, index, index);
}
void removeSmallFiles(int maxSize, QString index, QString actual){

	QDirIterator index_it(index, QDir::Files);

	while (index_it.hasNext())
	{
		qDebug() << "Processing: " << index_it.next();

		bool result;
		QFileInfo fileInfo = index_it.fileInfo();
		int size = fileInfo.size();
		if (size < 5000)
		{
			QString absoluteFilePath = fileInfo.absoluteFilePath();
			QString box_file_Name = fileInfo.fileName();

			QString cmask_file_Name = fileInfo.fileName();
			cmask_file_Name.replace("_box_", "_cmask_");
			QString convex_file_Name = fileInfo.fileName();
			convex_file_Name.replace("_box_", "_convex_");
			QString frame_file_Name = fileInfo.fileName();
			frame_file_Name.replace("_box_", "_frame_");

			QDir directory(index);
			directory.remove(box_file_Name);
			directory.remove(cmask_file_Name);
			directory.remove(convex_file_Name);
			directory.remove(frame_file_Name);

			QDir directoryAct(actual);
			directoryAct.remove(box_file_Name);
			directoryAct.remove(cmask_file_Name);
			directoryAct.remove(convex_file_Name);
			directoryAct.remove(frame_file_Name);

			//qDebug() << file;
			//result = QFile::remove(it.fileInfo().absoluteFilePath());


		}
	}
}
void addNewPictoDB(string fileName, ColorMoments colorMoments){

}


void main(int argc, char *argv[])
{
	//QCoreApplication app(argc, argv);

	QStringList illegalFileTypes;
	illegalFileTypes << ".exe" << ".dll" << ".py" << ".jar" << ".au3" << ".lua" << ".msi";

	QString index = "C:\\Projects\\Output\\file1\\correct\\box\\";
	QString actual = "C:\\Projects\\Output\\file1\\correct\\";
	removeSmallFiles(5000,index,actual);

	QDirIterator imgIt(index, QDir::Files);
	while (true)
	{
		if (!imgIt.hasNext())
			break;

		qDebug() << "Processing: " << imgIt.next();

		QFileInfo fileInfo = imgIt.fileInfo();
		QString box_file_Name = fileInfo.fileName();
		QString cmask_file_Name = fileInfo.fileName();
		cmask_file_Name.replace("_box_", "_cmask_");
		QString convex_file_Name = fileInfo.fileName();
		convex_file_Name.replace("_box_", "_convex_");

		QString hit_id = fileInfo.fileName();
		hit_id.replace("img_box_", " ");
		hit_id.replace(".jpg", " ");
		hit_id.trimmed();
		int hit_id_int = hit_id.toInt();


		QString pathBox = actual + box_file_Name;
		string pathBoxStr = pathBox.toStdString();
		Mat boxImage = imread((actual + box_file_Name).toStdString());
		Mat cmaskImage = imread((actual + cmask_file_Name).toStdString());
		Mat conImage = imread((actual + convex_file_Name).toStdString());

		//cvNamedWindow("boxImage",CV_WINDOW_NORMAL);
		//cvNamedWindow("cmaskImage", CV_WINDOW_NORMAL);
		//cvNamedWindow("conImage", CV_WINDOW_NORMAL);
		//imshow("boxImage", boxImage);
		//imshow("cmaskImage", cmaskImage);
		//imshow("conImage", conImage);

		cvtColor(cmaskImage, cmaskImage, CV_BGR2GRAY);
		double pixelCount = 0;
		double tot0 = 0;
		double tot1 = 0;
		double tot2 = 0;
		//Average
		for (int i = 0; i < boxImage.rows; i++)
		{
			for (int j = 0; j < boxImage.cols; j++)
			{
				int mask = cmaskImage.at<uchar>(i, j);
				if (mask == 0){
					continue;
				}
				else{
					pixelCount++;
					int original0 = boxImage.at<cv::Vec3b>(i, j)[0];
					int original1 = boxImage.at<cv::Vec3b>(i, j)[1];
					int original2 = boxImage.at<cv::Vec3b>(i, j)[2];

					tot0 += original0;
					tot1 += original1;
					tot2 += original2;
				}
			}
		}
		double av0 = tot0 / pixelCount;
		double av1 = tot1 / pixelCount;
		double av2 = tot2 / pixelCount;

		qDebug() << "Tot 0 " << QString::number(tot0);
		qDebug() << "Tot 1 " << QString::number(tot1);
		qDebug() << "Tot 2 " << QString::number(tot2);
		qDebug() << "AV 0 " << QString::number(tot0/pixelCount);
		qDebug() << "AV 1 " << QString::number(tot1/pixelCount);
		qDebug() << "AV 2 " << QString::number(tot2/pixelCount);
		//End of Average


		//Calculate skewness
		double varSquareTot0 = 0;
		double varSquareTot1 = 0;
		double varSquareTot2 = 0;
		double varCubeTot0 = 0;
		double varCubeTot1 = 0;
		double varCubeTot2 = 0;

		for (int i = 0; i < boxImage.rows; i++)
		{
			for (int j = 0; j < boxImage.cols; j++)
			{
				int mask = cmaskImage.at<uchar>(i, j);
				if (mask == 0){
					continue;
				}
				else{
					
					int original0 = boxImage.at<cv::Vec3b>(i, j)[0];
					int original1 = boxImage.at<cv::Vec3b>(i, j)[1];
					int original2 = boxImage.at<cv::Vec3b>(i, j)[2];

					varSquareTot0 += pow(av0 - original0,2);
					varSquareTot1 += pow(av1 - original1,2);
					varSquareTot2 += pow(av2 - original2,2);

					varCubeTot0 += pow(av0 - original0, 3);
					varCubeTot1 += pow(av1 - original1, 3);
					varCubeTot2 += pow(av2 - original2, 3);
				}
			}
		}
		double stdDev0 = varSquareTot0 / pixelCount;
		double stdDev1 = varSquareTot1 / pixelCount;
		double stdDev2 = varSquareTot2 / pixelCount;
		stdDev0 = pow(stdDev0,0.5);
		stdDev1 = pow(stdDev0, 0.5);
		stdDev2 = pow(stdDev0, 0.5);

		double skew0   = varCubeTot0 / pixelCount;
		double skew1   = varCubeTot1 / pixelCount;
		double skew2   = varCubeTot2 / pixelCount;
		skew0 = cbrt(skew0);
		skew1 = cbrt(skew1);
		skew2 = cbrt(skew2);

		qDebug() << "stdDev0 0 " << QString::number(stdDev0);
		qDebug() << "stdDev1 1 " << QString::number(stdDev1);
		qDebug() << "stdDev2 2 " << QString::number(stdDev2);
		qDebug() << "skew0 0 " << QString::number(skew0);
		qDebug() << "skew1 1 " << QString::number(skew1);
		qDebug() << "skew2 2 " << QString::number(skew2);

		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;

		

		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		stmt = con->createStatement();
		stmt->execute("USE camera");


		QString insertQuery_p1 = "INSERT INTO human_hits(img_id,path,av0,av1,av2,stdDev0,stdDev1,stdDev2,skew0,skew1,skew2,hit_id) VALUES(";
		QString insertQuery_p2 = QString("'")+ QString(convex_file_Name) + QString("','")
								+ QString(fileInfo.absoluteFilePath())		+ QString("',")
								+ QString::number(av0)						+ QString("," )
								+ QString::number(av1)						+ QString("," )
								+ QString::number(av2)						+ QString("," )
								+ QString::number(stdDev0)					+ QString("," )
								+ QString::number(stdDev1)					+ QString("," )
								+ QString::number(stdDev2)					+ QString("," )
								+ QString::number(av0)						+ QString("," )
								+ QString::number(av1)						+ QString("," )
								+ QString::number(av2)						+ QString("," )
								+ QString::number(hit_id_int)				
								+ QString(")");
							
		QString finalQuery = insertQuery_p1 + insertQuery_p2; 
		string stdString = finalQuery.toStdString();
		stmt->execute(stdString);
		delete con;
		delete stmt;

		


		//cvWaitKey(2000);
	}
	
	
	//return app.exec();
}