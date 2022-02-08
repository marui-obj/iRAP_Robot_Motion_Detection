#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


const char* params
                = "{ help h |           | Print usge}"
                  "{ input  | C:\\Users\\nawin\\Desktop\\Folder\\Coding\\C++\\motion_detection\\video.mp4 | Path to a video or sequenc of image }"
                  "{algo    | MOG2      | Background subtraction method (KNN, MOG2) }";



int main (int argc, char* argv[])
{
    cv::CommandLineParser parser(argc, argv, params);
    parser.about("This program show how to use background subtraction methods provided by"
                 "OpneCv. You can process both video and images. \n");
    
    if (parser.has("help"))
    {
        parser.printMessage();
    }

    cv::Ptr<cv::BackgroundSubtractor> pBackSub1, pBackSub2;

    if (parser.get<cv::String>("algo") == "MOG2"){
        pBackSub1 = cv::createBackgroundSubtractorMOG2();
        pBackSub2 = cv::createBackgroundSubtractorMOG2();
    }
    else
        pBackSub1 = cv::createBackgroundSubtractorKNN();

    cv::VideoCapture capture (cv::samples::findFile(parser.get<cv::String>("input")));
    if(!capture.isOpened())
    {
        std::cerr << "Unable to open: " << parser.get<cv::String>("input") << std::endl;
        return 0;
    }

    cv::Mat frame, fgMaskProcess, fgMask, ori_frame, blur_frame;

    while (true) 
    {
        capture >> ori_frame;
        if (ori_frame.empty()) 
            break;
        frame = ori_frame;
        //preprocessing
        // cv::GaussianBlur(frame, blur_frame, cv::Size(7, 7), 0);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        // cv::threshold(frame, frame, 100, 255, cv::THRESH_BINARY);

        //end preprocessing
        
        pBackSub1->apply(frame, fgMaskProcess);
        pBackSub2->apply(ori_frame, fgMask);
        

        cv::morphologyEx(fgMaskProcess, fgMaskProcess, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(6, 6)));


        cv::rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
                      cv::Scalar(255,255,255), -1);
        std::stringstream ss;
        ss << capture.get(cv::CAP_PROP_POS_FRAMES);
        std::string frameNumberString = ss.str().c_str();
        cv::putText(frame, frameNumberString, cv::Point(15, 15),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

        //find contour

        cv::imshow("Original Frame", ori_frame);
        cv::imshow("Preprocess Frame", frame);
        cv::imshow("FG Mask Preprocess", fgMaskProcess);
        cv::imshow("FG Mask Original", fgMask);

        int keyboard = cv::waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
}
