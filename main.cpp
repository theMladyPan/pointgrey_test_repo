#include <iostream>
#include <cstdlib>
#include "grabber.h"
#include "ocv_spinnaker.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;
using namespace cv;

#define DEBUG

void log(string text){
  #ifdef DEBUG
  cout << text << endl;
  #endif // DEBUG
};

void is(string name, Mat &mat){
    namedWindow(name, WINDOW_NORMAL);
    imshow(name, mat);
    //waitKey(1);
}

void hold(){
    int key;
    cout << "Paused\n";
    do{
        key = waitKey(0);
    }while(key!=112); // until small "p" was not typed
    cout << "Unpaused";
}

int main(int argc, char *argv[])
{
  // wait for cameras
    log("Waiting for camera...");
    while(!nOfCamAvail());
    log("Camera found");

    struct ATVars
    {
      int block_size = 45;
      int thresh_C = 25;
    } at_default;

    if(argc>2){
        at_default.block_size = atoi(argv[1]);
        at_default.thresh_C = atoi(argv[2]);
      }

    // get first camera
    Grabber grabber = Grabber();
    log(grabber.getSerialNr());
    grabber.init(AcquisitionMode_Continuous);
    grabber.start();
    log("Acq. started.");

    // fill opencv mat
    auto OCV_api = oCV_spinnaker(grabber.getResult());         // Show our image inside it.

    Mat* image = new Mat();
    Mat* convertedImage = new Mat();
    namedWindow( "Grabbing", WINDOW_NORMAL );// Create a window for display.

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed;

    cv::Rect roi;
    roi.x = 0;
    roi.y = 0;
    roi.width = 500;
    roi.height = 500;
    int key(0);
    int i(0);
    stringstream nazov;
    Mat original;
    Mat laplacian;
    Mat adaptive;
    Mat inverted;
    Mat substract;
    Mat weighed;
    do{
        start = chrono::high_resolution_clock::now();
        *image = OCV_api.getMat();
        image->copyTo(original);
        OCV_api.setMatFromCamera(grabber.getResult());
        adaptiveThreshold(*image, adaptive, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, at_default.block_size, at_default.thresh_C);
        is("adaptive", adaptive);
        Laplacian(*image, laplacian, CV_8UC1);
        normalize(laplacian, laplacian, 0,1,NORM_L2);
        //is("laplacian", laplacian);
        bitwise_not(adaptive, inverted);
        //is("laplacian inverted", lapl_inv);
        addWeighted(*image, 0.5, adaptive, .5, 0, weighed);
        is("Blend", weighed);
        subtract(*image, inverted, substract);
        is("substract", substract);
        end = chrono::high_resolution_clock::now();
        elapsed = end-start;
        cout << "Got and calculated in " << elapsed.count() << " ms\n";

        key = waitKey(50);
        if (key == 32){
            nazov.str(string());
            nazov<<"img/eroded"<<i<<".png";
            cout << "Saved as:" << nazov.str() << "\n";
            imwrite(nazov.str(), substract);

            nazov.str(string());
            nazov<<"img/original"<<i<<".png";
            cout << "Saved as:" << nazov.str() << "\n";
            imwrite(nazov.str(), original);

            nazov.str(string());
            nazov<<"img/threshold"<<i++<<".png";
            cout << "Saved as:" << nazov.str() << "\n";
            imwrite(nazov.str(), adaptive);
            this_thread::sleep_for(2s);
        }else if (key==112) {
            hold();
        }

        if(!image->empty()){
            imshow( "Grabbing", *image);
            /*
            nazov.str(string());
            nazov<<"img/test"<<i++<<".png";
            cout << "Saved as:" << nazov.str() << "\n";
            imwrite(nazov.str(), *image);
            */
          }else {
            log("Image empty!");
          }
      }while(key!=27);
    delete image;
    delete convertedImage;

    return 0;
}
