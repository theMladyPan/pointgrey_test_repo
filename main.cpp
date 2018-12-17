#include <iostream>
#include <cstdlib>
#include "grabber.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;
using namespace cv;

int main()
{
  // initialize spinnaker system
    SystemPtr system = System::GetInstance();
    CameraList camList = system->GetCameras();
    while (camList.GetSize() == 0) {
      cout << "No camera connected" << endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    };

    Grabber grabber = Grabber();
    grabber.init(AcquisitionMode_SingleFrame);
    grabber.acquire();

    // Create a unique filename
    ostringstream filename;

    filename << "Acquisition-" << grabber.getSerialNr() << "-" << "test.jpg";

    grabber.getLastResult()->Save(filename.str().c_str());

    while(1){
        auto convertedImage = grabber.getResult();
        unsigned int XPadding = convertedImage->GetXPadding();
        unsigned int YPadding = convertedImage->GetYPadding();
        unsigned int rowsize = convertedImage->GetWidth();
        unsigned int colsize = convertedImage->GetHeight();

        //image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding.

        Mat image = Mat(colsize + YPadding, rowsize + XPadding, CV_8UC1, convertedImage->GetData(), convertedImage->GetStride());
        cout << image;
        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
        imshow( "Display window", image );
        waitKeyEx(0);
      }
                   // Show our image inside it.


    cout << "Image saved at " << filename.str() << endl;

    camList.Clear();
    if(!system->IsInUse())
      {
      system->ReleaseInstance();
      }

    return 0;
}
