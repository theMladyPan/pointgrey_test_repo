#include "ocv_spinnaker.h"

oCV_spinnaker::oCV_spinnaker(ImagePtr image)
{
  const unsigned int XPadding = image->GetXPadding();
  const unsigned int YPadding = image->GetYPadding();
  const unsigned int rowsize = image->GetWidth();
  const unsigned int colsize = image->GetHeight();

  //image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding.

  mat = Mat(colsize + YPadding, rowsize + XPadding, CV_8UC1, image->GetData(), image->GetStride());

}

Mat oCV_spinnaker::getMat()
{
  return mat;
}

void oCV_spinnaker::showMat()
{
  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Display window", mat );
  waitKeyEx(0);
}
