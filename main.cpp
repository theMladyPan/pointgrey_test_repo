#include <iostream>
#include <cstdlib>
#include "grabber.h"
#include "ocv_spinnaker.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

int main()
{
  // wait for cameras
    while(!nOfCamAvail());

    // get first camera
    Grabber grabber = Grabber();
    grabber.init(AcquisitionMode_SingleFrame);
    grabber.acquire();

    // fill opencv mat
    auto OCV_api = oCV_spinnaker(grabber.getLastResult());         // Show our image inside it.
    OCV_api.showMat();

    // print out raw data
    cout << OCV_api.getMat();

    // Create a unique filename and save
    ostringstream filename;
    filename << "Acquisition-" << grabber.getSerialNr() << "-" << "test.jpg";
    grabber.saveLast(filename.str());
    cout << "Image saved at " << filename.str() << endl;

    return 0;
}
