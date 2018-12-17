#ifndef OCV_SPINNAKER_H
#define OCV_SPINNAKER_H

#include "Spinnaker.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;
using namespace cv;

class oCV_spinnaker
{
private:
    Mat mat;
public:
  oCV_spinnaker(ImagePtr image);
  Mat getMat();
  void showMat();
};

#endif // OCV_SPINNAKER_H
