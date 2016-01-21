#include <node.h>
#include <v8.h>
#include <nan.h>
#include <node_buffer.h>

// C standard library
#include <cstdlib>
#include <ctime>
#include <string.h>

#include "camera.h"

using namespace v8;
using namespace node;

namespace {

  struct CallbackData {
    Nan::Persistent<v8::Object> thisObj;
  };
  
  class Camera : public Nan::ObjectWrap {
  public:
    static  NAN_MODULE_INIT(RegisterModule);
  private:
    static NAN_METHOD(CaptureFrame);
    static NAN_METHOD(StartCapture);
    static NAN_METHOD(StopCapture);
    static NAN_METHOD(CameraOn);
    static NAN_METHOD(CameraOff);
    static NAN_METHOD(ControlSet);
      
    Camera();
    ~Camera();
  };

  //Nan Method
  NAN_METHOD(Camera::CameraOn) {
    auto thisObj = info.Holder();

    if (info.Length() != 4) {
      Nan::ThrowTypeError("cameraOn requires 4 arguments");
      return;
    }

    const auto deviceString = info[0]->ToString();
    const auto width = info[1]->Uint32Value();
    const auto height = info[2]->Uint32Value();
    const auto fps = info[3]->Uint32Value();
    
    auto success = bool{camera_on(deviceString, width, height, fps)};
    info.GetReturnValue().Set(thisObj);
  }


  //Nan Method
  NAN_METHOD(Camera::CameraOff) {
    auto thisObj = info.Holder();

    if (info.Length() != 1) {
      Nan::ThrowTypeError("cameraOff requires 1 arguments");
      return;
    }

    const auto fd = info[0]->Uint32Value();
    auto success = bool{camera_off(fd)};
    info.GetReturnValue().Set(thisObj);
  }


  //Nan Method
  NAN_METHOD(Camera::StartCapture) {
    auto thisObj = info.Holder();
    if (info.Length() != 1) {
      Nan::ThrowTypeError("startCapture requires 1 argument");
      return;
    }

    const auto fd = info[0]->Uint32Value();
    auto success = false;
    do {
      success = bool{(0 == start_capturing(fd))};
      if (!success) stop_capturing(fd);
    } while (!success);

    info.GetReturnValue().Set(thisObj);
  }

  

    //Nan Method
  NAN_METHOD(Camera::StopCapture) {
    auto thisObj = info.Holder();
    if (info.Length() != 1) {
      Nan::ThrowTypeError("stopCapture requires 1 argument");
      return;
    }

    const auto fd = info[0]->Uint32Value();
    stop_capturing(fd);
    info.GetReturnValue().Set(thisObj);

  }

  NAN_METHOD(Camera::CaptureFrame) {
    auto thisObj = info.Holder();
    if (info.Length() != 2) {
      Nan::ThrowTypeError("captureFrame requires 2 arguments");
      return;
    }

    const auto fd = info[0]->Uint32Value();
    v8::Local<v8::Object> buffer = info[1]->ToObject();
    char* bufferData   = node::Buffer::Data(buffer);
    size_t bufferLength = node::Buffer::Length(buffer);
    capture_frame(fd, bufferData, bufferLength);  
    info.GetReturnValue().Set(bufferData);
  }


//Nan Method
  NAN_METHOD(Camera::ControlSet) {
    if (info.Length() != 2) {
      Nan::ThrowTypeError("controlSet requires 3 arguments");
      return;
    }

    const auto fd = info[0]->Uint32Value();
    const auto id = info[1]->Uint32Value();
    const auto value = info[2]->Int32Value();
    auto thisObj = info.Holder();
    control_set(fd, id, value);
    info.GetReturnValue().Set(thisObj);
  }


//Nan Module initializer.
  NAN_MODULE_INIT(Camera::RegisterModule) {
    const auto name = Nan::New("Camera").ToLocalChecked();
    auto ctor = Nan::New<v8::FunctionTemplate>(New);
    auto ctorInst = ctor->InstanceTemplate();
    ctor->SetClassName(name);
    ctorInst->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(ctor, "cameraOn", CameraOn);
    Nan::SetPrototypeMethod(ctor, "cameraOff", CameraOff);
    Nan::SetPrototypeMethod(ctor, "startCapture", StartCapture);
    Nan::SetPrototypeMethod(ctor, "stopCapture", StopCapture);
    Nan::SetPrototypeMethod(ctor, "captureFrame", CaptureFrame);
    Nan::SetPrototypeMethod(ctor, "controlSet", ControlSet);
    Nan::Set(target, name, Nan::GetFunction(ctor).ToLocalChecked());
  }

/*Handle<Value> CaptureFrame(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 2) {
      return ThrowException(
        Exception::TypeError(String::New("captureFrame requires 2 arguments"))
        );
    }

    int fd = args[0]->IntegerValue();

    v8::Local<v8::Object> buffer = args[1]->ToObject();
    char* bufferData   = node::Buffer::Data(buffer);
    size_t bufferLength = node::Buffer::Length(buffer);
    int result = capture_frame(fd, bufferData, bufferLength);

    return scope.Close(Integer::New(result));
  }*/

//Deprecated - Original Node-Seret Build
  /*Handle<Value> CameraOn(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 4) {
      return ThrowException(
        Exception::TypeError(String::New("cameraOn requires 4 arguments"))
        );
    }

    String::AsciiValue deviceString(args[0]->ToString());
    uint32_t width = args[1]->IntegerValue();
    uint32_t height = args[2]->IntegerValue();
    uint32_t fps = args[3]->IntegerValue();

    int fd = camera_on(*deviceString, width, height, fps);

    return scope.Close(Integer::New(fd));
  }*/

//Deprecated - Original Node-Seret Build
/*Handle<Value> ControlSet(const Arguments& args) {
  HandleScope scope;

  if (args.Length() != 3) {
    return ThrowException(
      Exception::TypeError(String::New("captureFrame requires 3 arguments"))
    );
  }

  int fd = args[0]->IntegerValue();
  uint32_t id = args[1]->Uint32Value();
  int32_t value = args[2]->Int32Value();

  control_set(fd, id, value);

  return scope.Close(Null());
}*/

  //Deprecated - Original Node-Seret Build
    /*Handle<Value> StopCapture(const Arguments& args) {
      HandleScope scope;

      if (args.Length() != 1) {
        return ThrowException(
          Exception::TypeError(String::New("stopCapture requires 1 argument"))
          );
      }

      int fd = args[0]->IntegerValue();

      stop_capturing(fd);

      return scope.Close(Null());
    }*/

   //Deprecated - Original Node-Seret Build
    /*Handle<Value> StartCapture(const Arguments& args) {
      HandleScope scope;

      if (args.Length() != 1) {
        return ThrowException(
          Exception::TypeError(String::New("startCapture requires 1 argument"))
          );
      }

      int fd = args[0]->IntegerValue();

      int success = 0;
      do {
        success = (0 == start_capturing(fd));
        if (!success) stop_capturing(fd);
      } while (!success);

      return scope.Close(Null());
    }*/

    //Deprecated - Original Node-Seret Build
  /*Handle<Value> CameraOff(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 1) {
      return ThrowException(
        Exception::TypeError(String::New("cameraOff requires 1 argument"))
        );
    }

    int fd = args[0]->IntegerValue();

    camera_off(fd);

    return scope.Close(Null());
  }*/
//Deprecated - Original Node-Seret Build.
/*void RegisterModule(Handle<Object> target) {

  // target is the module object you see when require()ing the .node file.
  target->Set(String::NewSymbol("cameraOn"),
    FunctionTemplate::New(CameraOn)->GetFunction());
  target->Set(String::NewSymbol("cameraOff"),
    FunctionTemplate::New(CameraOff)->GetFunction());
  target->Set(String::NewSymbol("startCapture"),
    FunctionTemplate::New(StartCapture)->GetFunction());
  target->Set(String::NewSymbol("stopCapture"),
    FunctionTemplate::New(StopCapture)->GetFunction());
  target->Set(String::NewSymbol("captureFrame"),
    FunctionTemplate::New(CaptureFrame)->GetFunction());
  target->Set(String::NewSymbol("controlSet"),
    FunctionTemplate::New(ControlSet)->GetFunction());
}*/

}
NODE_MODULE(seret, Camera::RegisterModule)
