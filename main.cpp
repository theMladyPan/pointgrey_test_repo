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

int main(int argc, char *argv[])
{
  // wait for cameras
    log("Waiting for camera...");
    while(!nOfCamAvail());
    log("Camera found");

    struct ATVars
    {
      int block_size = 15;
      int thresh_C = 15;
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
    namedWindow( "Grabbing", WINDOW_AUTOSIZE );// Create a window for display.

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed;

    while(1){
        start = chrono::high_resolution_clock::now();
        *image = OCV_api.getMat();
        OCV_api.setMatFromCamera(grabber.getResult());
        // OCV_api.showMat();
        adaptiveThreshold(*image, *convertedImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, at_default.block_size, at_default.thresh_C);
        end = chrono::high_resolution_clock::now();
        elapsed = end-start;
        cout << "Got and calculated in " << elapsed.count() << " ms\n";

        if(!convertedImage->empty()){
            imshow( "Grabbing", *convertedImage);
            waitKey(0);
            //this_thread::sleep_for(50ms);
          }else {
            log("Image empty!");
          }
      }

    return 0;
}
