#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(){

     cv::VideoCapture cap(0);
     if(!cap.isOpened())
       {
         cout << "Error opening video stream" << endl;
         return -1;
       }

     while(cap.isOpened())
     {
         Mat hsv,skin_mask,mask,blur;
         Mat thresh,area_contour;
         cv::Mat frame,result;
         vector<vector<Point> > contour;
         vector<cv::Vec4i> hierarchy;
         Rect bounding_rect;

        cap >> frame; // getting new frame from camera


        cv:: cvtColor(frame,hsv,CV_BGR2HSV);
        cv:: inRange(hsv,Scalar(0,58.65,50),Scalar(50,173.4,255),skin_mask); //it detects skin color & returns binary of it

        int an = 5,blurSize=7;
        cv::medianBlur(skin_mask, skin_mask, blurSize);                      //blurring the image for more smoothening
        Mat labelImage(frame.size(), CV_32S);
//       int label = connectedComponents(skin_mask,labelImage,8);            //returns number of blob in frame
 //cout<< label<<" ";

        int a = frame.rows;
        for(int i = 0;i<15;i++){
           cv::line( skin_mask,{0, a} ,{frame.cols, a} , Scalar( 0, 0, 0) ,10,8 );
           a = a - 10;
//         if(label ==2)
//             break;
}

        Mat kernel = cv::getStructuringElement(MORPH_ELLIPSE, Size(an*2+1, an*2+1), Point(an, an));
        cv::erode(skin_mask,skin_mask, kernel);
        cv::dilate(skin_mask,skin_mask, kernel);
 //cout<<frame.cols<<" ";
 //cout<<frame.rows<<" ";
        cv::namedWindow("skin_segment1",1);
        cv::imshow("skin_segment1", skin_mask);

        cv:: findContours(skin_mask,contour,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);     //finding contours

        Point2f rect_points[4],vertices[4];

        for(size_t i = 0; i < contour.size(); i++){
           double area = contourArea(contour[i]);
           if (area>100 ){
            RotatedRect box = minAreaRect(contour[i]);
            box.points( rect_points );
//  double angle = box.angle;
            Point2f center = box.center;
// cout<<center<<endl;
            double width = box.size.width;
            double height =  box.size.height;
 // cout<<angle;
            if(height>width){
                box.size.height =(float)(0.33)*box.size.height;
                box.center = (rect_points[1]+rect_points[2])/2 + (rect_points[0]-rect_points[1])/6;
            } else {
                box.size.width =(float)(0.33)*box.size.width;
                box.center = (rect_points[2]+rect_points[3])/2 + (rect_points[0]-rect_points[3])/6;
            }
               box.points( rect_points );
               Point2f center1 = box.center;

//cout<<center1<<endl;
           for(int j =0; j <4; j++){
              line( frame, rect_points[j], rect_points[(j+1)%4], Scalar( 0, 255, 0) ,2,8 );
           }

           //drawContours( frame, contour, i, Scalar( 0, 255, 0 ), 1 ); // Draw the largest contour
       }
     }

    cv::namedWindow("ROI",1);
    cv::imshow("ROI", frame);
    if(cv::waitKey(30) >= 0) break;
    }

     cap.release();
     waitKey(1);


}
