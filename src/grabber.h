#ifndef GRABBER_H
#define GRABBER_H

#include "Spinnaker.h"
#include <string>
#include <stdexcept>
#include <vector>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

class Grabber
{
private:
  CameraPtr pCam = NULL;
  ImagePtr lastResult = NULL;
  SystemPtr pSystem = NULL;
public:
  /// Construct Grabber with 'index'-th camera available
  Grabber(uint8_t index);

  /// Construct Grabber with first camera available
  Grabber() : Grabber(0){};

  /// Construct Grabber with pCamera
  Grabber(CameraPtr pCamera);

  /// Construct grabber with specific serial Nr camera
  Grabber(string serialNr);

  ~Grabber();

  /// Initialize grbber
  void init();

  /** Initialize grabber with specific acquistion mode.
   * \param modeEnum Acquisiton mode */
  void init(AcquisitionModeEnums modeEnum);

  /// Acquire image
  void start();

  /// Stop acquisition
  void stop();

  /** Get current camera Serial Nr.
   * \return string with serial number */
  string getSerialNr();

  /** Get new image from camera
   * \return ImagePtr pointer to image */
  ImagePtr getResult();

  /** Get last image from camera
   * \return ImagePtr pointer to last image acquired */
  ImagePtr getLastResult();

  /** Save last image to disk
   * \param string filename name of file on disk */
  void saveLast(string filename);

  /// Frees the grabber
  void free();
};

/** Get number of available cameras
 * \return uint8_t number of cameras */
uint8_t nOfCamAvail();
/** Return all available cameras
 * \return vector<string> with all cameras available */
vector<string> camAvail();

#endif // GRABBER_H
