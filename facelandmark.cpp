
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include <opencv2/face/facemark.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/face.hpp>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>




cv::Mat return_gray(cv::InputArray _src)
{
cv::Mat frame = _src.getMat();
cv::Mat gray;

// resize(frame,frame,cv::Size(460,460),0,0,cv::INTER_LINEAR_EXACT);

if(frame.channels()>1)
    {
    //frameをグレスケに変換後grayに格納
    cvtColor(frame,gray,cv::COLOR_BGR2GRAY);
    }
    else
    {
    //grayにframeのクローンを格納
    gray = frame.clone();
    }
//ヒストグラムの均一化
equalizeHist( gray, gray);
return gray;
}

cv::Mat drow_facelandmark(cv::Mat frame,cv::Mat gray)
{
std::vector<std::vector<cv::Point2f>> shapes;
std::vector<cv::Rect> faces;
cv::CascadeClassifier face_cascade;
cv::Ptr<cv::face::Facemark> facemark= cv::face::createFacemarkKazemi();
face_cascade.load("/Users/thankyou/opencv/data/haarcascades/haarcascade_frontalface_default.xml");
// face_cascade.load("/Users/thankyou/opencv/data/lbpcascades/lbpcascade_frontalface.xml");
facemark->loadModel("/Users/thankyou/opencv/opencv_3rdparty/face_landmark_model.dat");

//grayから検出した顔をfacesに格納、1.1=検出ウィンドウのスケールファクター、3は近傍の値（小さいほど精度が上がる）０はオプション、sizeは３０以上３０以下の大きさのやつを探知する
face_cascade.detectMultiScale( gray, faces, 1.1, 3, 0, cv::Size(30, 30));
//facemarkオブジェクトのfit
//frame=画像、認識した顔が格納
//faces =出力パラメータとして、顔の形状モデルのランドマークが格納される配列
if(faces.size() > 1)
{
    for (size_t i = 0; i<faces.size(); i++)
    {
        cv::rectangle(frame,faces[i],cv::Scalar(255,0,0));
    }
    if(facemark->fit(frame,faces,shapes))
    {
        for(unsigned long  i= 0; i<faces.size(); i++)
        {   for(unsigned long k=0;k<shapes[i].size();k++)
            {
                cv::circle(frame,shapes[i][k],5,cv::Scalar(0,0,255),cv::FILLED);
            }
        }
        return frame;
    }
    else
    {
        return frame;
    }
}
else
{
    return frame;
}
}


cv::Mat video(){
    cv::Mat frame;
    cv::VideoCapture cap;
    std::vector<cv::Rect> faces;


    int deviceID = 0;
    int apiID = cv::CAP_ANY;
    cap.open(deviceID, apiID);

    if (!cap.isOpened()) {
        std::cerr << "エラー！ カメラを開くのに失敗しました" << std::endl;
        return frame;
    }

    for (;;) {
        // カメラから新しいフレームをキャプチャする
        cap.read(frame);
        // フレームが空かどうかをチェックする
        if (frame.empty()) {
            std::cerr << "エラー！ 空のフレームがキャプチャされました" << std::endl;
            break;
        }
        // キャプチャしたフレームを表示する
        resize(frame,frame,cv::Size(640,460
        」),0,0,cv::INTER_LINEAR_EXACT);
        cv::Mat gray = return_gray(frame);
        cv::Mat f_landmarkframe = drow_facelandmark(frame,gray);
        cv::namedWindow("live");
        imshow("live",f_landmarkframe);

        //キャプチャしたフレームに適応する

        // ESCキーが押された場合に終了する
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // カメラを解放する
    cap.release();

    return frame;
}







int main(){
video();
}