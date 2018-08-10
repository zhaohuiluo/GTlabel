// groundTruth.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <stdarg.h> 
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
using namespace cv;


// Global variables
bool is_drawing = false;
//vector<Rect> biaozhu_boxs;
//Rect drawing_box;
vector<Point> ps;
vector<vector<Point>> box_p;
Point p;
Mat img_original, img_drawing;
//! assigns callback for mouse events




static void help()
{
	cout << "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tn - next frame of the video\n"
		"\tz - undo the last label\n"
		"\tc - clear all the labels\n"
		<< endl;
}


static void onMouse(int event, int x, int y, int, void*)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN: //the left up point of the rect
		is_drawing = true;
		p.x = x;
		p.y = y;
		break;

	case CV_EVENT_LBUTTONUP: //finish drawing the rect (use color green for finish)
		circle(img_drawing, p,3 , Scalar(0, 0, 255), -1);
		ps.push_back(p);
		int l = ps.size();
		if (l == 4) {
			vector<Point> draw_p;
			for (int i = 0; i<4; i++) {
				draw_p.push_back(ps[i]);
			}
			RotatedRect box = minAreaRect(Mat(draw_p));
			Point2f vertex[4];
			box.points(vertex);
			vector<Point> temp;
			for (int i = 0; i < 4; i++) {
				line(img_drawing, vertex[i], vertex[(i + 1) % 4], Scalar(0, 255, 0), 1);
				temp.push_back(Point(vertex[i]));
			}
			box_p.push_back(temp);
			ps.clear();
		}
		imshow("Video", img_drawing);
		waitKey(10);
		break;
	}
	return;
}




int main()
{
	/*Mat pic = imread("E:\\613\\613\\infra\\JPEGImages\\000001.jpg");
	imshow("",pic);
	waitKey(0);*/
	ifstream file("C:\\Users\\luo\\Desktop\\VOCALL\\VOCship_v1\\c2\\names.txt");
	string filepath("C:\\Users\\luo\\Desktop\\VOCALL\\VOCship_v1\\c2\\");
	//string writepath;
	char txt_cont[20], name[20];
	char img_file[200];
	//namedWindow("Video");
	string outpath("C:\\Users\\luo\\Desktop\\VOCALL\\VOCship_v1\\gt\\");
	ofstream outfile;
	string outfilepath;
	help();
	/*VideoCapture capture("test.avi");
	capture >> img_original;*/


	file.getline(txt_cont, 20);
	string imgpath = filepath + txt_cont;
	img_original = imread(imgpath);
	img_original.copyTo(img_drawing);
	//img_drawing = img_original;
	strcpy(name, txt_cont);
	//for (vector<Rect>::iterator it = biaozhu_boxs.begin();
	//	it != biaozhu_boxs.end(); ++it)
	//{
	//	rectangle(img_drawing, (*it), Scalar(0, 255, 0));
	//}
	imshow("Video", img_original);
	waitKey(30);
	cvSetMouseCallback("Video", onMouse, 0);


	//int frame_counter = 0;
	int counter = 0;
	char path[50];

	while (1)
	{
		int c = waitKey(0);
		if ((c & 255) == 27)
		{
			cout << "Exiting ...\n";
			break;
		}
		switch ((char)c)
		{
		case 'n': //read the next frame
			 //save all of the labeling rects
			outfilepath = outpath + txt_cont + ".txt";
			outfile.open(outfilepath);
			for (int i=0;i<box_p.size();i++)
			{
					outfile <<box_p[i][2].x<< ","<< box_p[i][2].y << "," << box_p[i][3].x << ","<< box_p[i][3].y<<","
						<< box_p[i][0].x << "," << box_p[i][0].y<<"," <<box_p[i][1].x << "," << box_p[i][1].y<<","<<"ship"<<endl;

				/*Mat roi_img = img_original(Range((*it).x, (*it).x + (*it).width), Range((*it).y, (*it).y + (*it).height));
				sprintf(path, "E:\\car\\pos\\%d.jpg", counter);
				imwrite(path, roi_img);*/
			}
			outfile.close();
			box_p.clear();
			cout << counter++ << endl;
			file.getline(txt_cont, 200);
			imgpath = filepath + txt_cont;
			img_original = imread(imgpath);
			if (img_original.empty())
			{
				cout << "\n Finished!" << endl;
				return 0;
			}
			img_original.copyTo(img_drawing);
			//img_original.copyTo(img_drawing);
			strcpy(name, txt_cont);
			break;
		case 'z': //undo the latest labeling
			if (!box_p.empty())
			{
				vector<vector<Point>>::iterator it_end = box_p.end();
				--it_end;
				box_p.erase(it_end);
			}
			img_original.copyTo(img_drawing);
			for (vector<vector<Point>>::iterator it = box_p.begin();
				it != box_p.end(); ++it)
			{
				for (int i = 0; i < 4; i++)
				{
					line(img_drawing, (*it)[i], (*it)[(i + 1) % 4], Scalar(0, 0, 255), 2);
				}
			}
			break;
		case 'c': //clear all the rects on the image
			box_p.clear();
			img_original.copyTo(img_drawing);
		}
		imshow("Video", img_drawing);



		//waitKey(10);
	}
	return 0;
}


