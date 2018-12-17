#include "grabber.h"



Grabber::Grabber(uint8_t index)
{
  pSystem = System::GetInstance();
  CameraList camList = pSystem->GetCameras();
  if (camList.GetSize() == 0){
      throw runtime_error("No camera available");
    }

  pCam = camList.GetByIndex(index);
  camList.Clear();
}

Grabber::Grabber(CameraPtr pCamera)
{
  pCam = pCamera;
}

Grabber::Grabber(string serialNr)
{
  pSystem = System::GetInstance();
  CameraList camList = pSystem->GetCameras();
  pCam = camList.GetBySerial(serialNr);
}

Grabber::~Grabber()
{
  this->free();
}

void Grabber::init()
{
  this->init(AcquisitionMode_Continuous);
}

void Grabber::init(AcquisitionModeEnums modeEnum)
{

  pCam->Init();
  INodeMap &nodeMap = pCam->GetNodeMap();
  CEnumerationPtr ptrAcquisitionMode = nodeMap.GetNode("AcquisitionMode");
  int64_t acquisitionMode ;
  switch (modeEnum) {
    case AcquisitionMode_Continuous:{
       acquisitionMode = ptrAcquisitionMode->GetEntryByName("Continuous")->GetValue(); }
    case AcquisitionMode_SingleFrame:{
      acquisitionMode = ptrAcquisitionMode->GetEntryByName("SingleFrame")->GetValue(); }
    case AcquisitionMode_MultiFrame:{
      acquisitionMode = ptrAcquisitionMode->GetEntryByName("MultiFrame")->GetValue(); }
    default:{
      acquisitionMode = ptrAcquisitionMode->GetEntryByName("Continuous")->GetValue();
      }

    }

  ptrAcquisitionMode->SetIntValue(acquisitionMode);
}

void Grabber::acquire(){
  pCam->BeginAcquisition();
}

string Grabber::getSerialNr(){
  gcstring deviceSerialNumber("");
  INodeMap &nodeMapTLDevice = pCam->GetTLDeviceNodeMap();
  CStringPtr ptrStringSerial = nodeMapTLDevice.GetNode("DeviceSerialNumber");
  if (IsAvailable(ptrStringSerial) && IsReadable(ptrStringSerial)) {
    deviceSerialNumber = ptrStringSerial->GetValue();

    return deviceSerialNumber.c_str();
  }
  else{
      return "";
    }
}

ImagePtr Grabber::getResult()
{
  // size_t width = pResultImage->GetWidth();
  // size_t height = pResultImage->GetHeight();
  lastResult = pCam->GetNextImage()->Convert(PixelFormat_Mono8, HQ_LINEAR);
  return lastResult;
}

ImagePtr Grabber::getLastResult()
{
  if(lastResult != NULL){
      return lastResult;
    }
  else{
      return getResult();
    }
}

void Grabber::free()
{
  pCam->EndAcquisition();
  pCam = NULL;
  if(pSystem != NULL){
    if(pSystem->IsInUse()){
      pSystem->ReleaseInstance();
      }
  }
}

uint8_t nOfCamAvail()
{
  SystemPtr system = System::GetInstance();
  CameraList camList = system->GetCameras();
  uint8_t listSize = uint8_t(camList.GetSize());
  camList.Clear();
  if(!system->IsInUse()){
      system->ReleaseInstance();
    }
  return uint8_t(listSize);
}

vector<string> camAvail()
{
  vector<string> cameras;
  for(uint8_t i=0; i<nOfCamAvail();i++){
      Grabber grb = Grabber(i);
      cameras.push_back(grb.getSerialNr());
      //grb.free();
    }
  return cameras;
}
