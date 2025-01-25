// MIT License
//
// Copyright (c) 2023 Julia Nelz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "main/main.h"
#include "os_symbian.h"

#include "appui.h"
#include "container.h"

#include <AknDef.h>
#include <eikdoc.h>
#include <aknappui.h>
#include <coecntrl.h>
#include <coemain.h>
#include <w32std.h>
#include <e32event.h>
#ifdef SYMBIAN_FIXED_KEYMAP
#include <algorithm>
#endif
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#if !defined(EKeyLeftUpArrow) && \
    !defined(EKeyRightUpArrow) && \
    !defined(EKeyRightDownArrow) && \
    !defined(EKeyLeftDownArrow)
#define EKeyLeftUpArrow      EKeyDevice10  // Diagonal arrow event
#define EKeyRightUpArrow     EKeyDevice11  // Diagonal arrow event
#define EKeyRightDownArrow   EKeyDevice12  // Diagonal arrow event
#define EKeyLeftDownArrow    EKeyDevice13  // Diagonal arrow event
#endif

#pragma mark - CGodotContainer constants

#ifdef SYMBIAN_FIXED_KEYMAP
extern "C" const char KReMapping[256];
extern "C" const char KReMappingF[256];
extern "C" const char KReMappingS[256];
#endif

// clang-format on

#pragma mark - CGodotContainer impl

CGodotContainer::CGodotContainer() : iSoundBufferPtr{KNullDesC8} {}

void CGodotContainer::ConstructL(const TRect &aRect, CAknAppUi *aAppUi) {
  iAppUi = aAppUi;

  CreateWindowL();
  SetExtentToWholeScreen();
  ActivateL();

  mkdir("/Private/a0060d07", 0777);
  freopen("/Private/a0060d07/stdout.txt", "w+", stdout);
  freopen("/Private/a0060d07/stderr.txt", "w+", stderr);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  mkdir("/data", 0777);
  mkdir("/data/Others", 0777);
  mkdir("/data/Others/Godot", 0777);
  chdir("/data/Others/Godot");

#if 0
  iGodotFrameSize = -1;

  iSoundBufferPtr.Set(reinterpret_cast<TUint8 *>(mem->product.samples.buffer),
                      iGodotFrameSize * TIC80_SAMPLESIZE);

  //iStreamSettings.Query();
  iStreamSettings.iChannels = TMdaAudioDataSettings::EChannelsStereo;
  iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate44100Hz;
  iStreamSettings.iCaps = TMdaAudioDataSettings::ERealTime | TMdaAudioDataSettings::ESampleRateFixed;
	//iStreamSettings.iFlags = TMdaAudioDataSettings::ENoNetworkRouting;

  iOutputStream = CMdaAudioOutputStream::NewL(*this);
  iOutputStream->Open(&iStreamSettings);
#endif

  iEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (iEglDisplay == NULL) {
    _LIT(KGetDisplayFailed, "EGL");
    User::Panic(KGetDisplayFailed, 1);
  }

  if (eglInitialize(iEglDisplay, NULL, NULL) == EGL_FALSE) {
    _LIT(KInitializeFailed, "EGL");
    User::Panic(KInitializeFailed, 2);
  }

  EGLConfig config;
  EGLint numOfConfigs = 0;

  iWinSize = Window().Size();
  TDisplayMode displayMode = Window().DisplayMode();
  TInt bufferSize = TDisplayModeUtils::NumDisplayModeBitsPerPixel(displayMode);

  // clang-format off
#ifdef GLES2_ENABLED
  const EGLint attribList[] = {EGL_BUFFER_SIZE, bufferSize,
                               EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                               EGL_SURFACE_TYPE, EGL_SWAP_BEHAVIOR_PRESERVED_BIT | EGL_WINDOW_BIT,
                               EGL_DEPTH_SIZE, 8,
                               EGL_STENCIL_SIZE, 8,
                               //EGL_BIND_TO_TEXTURE_RGBA, EGL_TRUE,
                               //EGL_BIND_TO_TEXTURE_RGB, EGL_TRUE,
                               EGL_NONE};
#else
  const EGLint attribList[] = {EGL_NONE};
#endif
  // clang-format on

  if (eglChooseConfig(iEglDisplay, attribList, &config, 1, &numOfConfigs) ==
      EGL_FALSE) {
    _LIT(KChooseConfigFailed, "EGL");
    User::Panic(KChooseConfigFailed, 3);
  }

  if (numOfConfigs == 0) {
    _LIT(KNoConfig, "CONT");
    User::Panic(KNoConfig, 18);
  }

  iEglSurface = eglCreateWindowSurface(iEglDisplay, config, &Window(), NULL);

  if (iEglSurface == NULL) {
    _LIT(KCreateWindowSurfaceFailed, "EGL");
    User::Panic(KCreateWindowSurfaceFailed, 7);
  }

#ifdef SYMBIAN_S60V3_ENABLED
  eglSurfaceAttrib(iEglDisplay, iEglSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_DESTROYED);
#endif

  // clang-format off
#ifdef GLES2_ENABLED
  const EGLint contextAttrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
                                 EGL_NONE};
#else
  const EGLint contextAttrs[] = {EGL_NONE};
#endif
  // clang-format on

  iEglContext =
      eglCreateContext(iEglDisplay, config, EGL_NO_CONTEXT, contextAttrs);
  if (iEglContext == NULL) {
    _LIT(KCreateContextFailed, "EGL");
    User::Panic(KCreateContextFailed, 10);
  }

  if (eglMakeCurrent(iEglDisplay, iEglSurface, iEglSurface, iEglContext) ==
      EGL_FALSE) {
    _LIT(KMakeCurrentFailed, "EGL");
    User::Panic(KMakeCurrentFailed, 15);
  }

#if 0
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback([](GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam) {

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
  }, 0);
#endif

  GLsizei sTex;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &sTex);
  GLsizei nTex;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nTex);

  GLsizei sMaxTex = sTex * sTex * 4;
  fprintf(stderr, "VRAM available (guessed from maxSize=%d, maxTexCount=%d): %d KiB\n", sTex, nTex, (nTex * sMaxTex) / 1024);

  GLsizei sMyTex = 512;

  GLuint texture[nTex];
  glGenTextures(nTex, texture);
  int iTex;
  for (iTex = 0; iTex < nTex; ++iTex) {
    glBindTexture(GL_TEXTURE_2D, texture[iTex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sMyTex, sMyTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    GLenum error = glGetError();
    if (error == GL_OUT_OF_MEMORY) {
      break;
    } else if (error != GL_NO_ERROR) {
      fprintf(stderr, "GL Error #%d\n", error);
      _LIT(KGL, "GL");
      User::Panic(KGL, 72);
    }
  }
  glDeleteTextures(nTex, texture);

  fprintf(stderr, "Minimum VRAM available (real): %d KiB\n", (iTex * sMyTex * sMyTex * 4) / 1024);

  iOs = new OS_Symbian(this);

  fprintf(stderr, "OS init'd\n");

	char* args[] = { "-v", "-path", "." };
	Error err = Main::setup("/sys/bin/godot.exe", 3, args, true);

  fprintf(stderr, "inst setup\n");
  auto res = Main::start();

	iOs->main_loop->init();

  fprintf(stderr, "loop init'd\n");

  HandleResourceChange(KEikDynamicLayoutVariantSwitch);

  iMainPeriodic = CPeriodic::NewL(CActive::EPriorityLow);
  iMainPeriodic->Start(100, 100, TCallBack(CGodotContainer::RunEngineThread, this));

  fprintf(stderr, "periodic started\n");

  //iEngineThread.Create(_L("GodotEngine"), RunEngineThread, 0x10000, nullptr, this);
  //fprintf(stderr, "thread created\n");
  //iEngineThread.Resume();

  iPeriodic = CPeriodic::NewL(CActive::EPriorityIdle);
  iPeriodic->Start(10000, 10000, TCallBack(CGodotContainer::ResetInactivityTimeCallBack, this));

#if 0
  Window().PointerFilter(EPointerFilterDrag, 0);
  iWsEventReceiver = CWsEventReceiver::NewL(*this, &CCoeEnv::Static()->WsSession());
#endif
}

TInt CGodotContainer::RunEngineThread(TAny *aInstance) {
#if 0
  User::SetCritical(User::TCritical::EProcessPermanent);
  fprintf(stderr, "thread set up\n");

  CGodotContainer *instance = static_cast<CGodotContainer *>(aInstance);

  fprintf(stderr, "current setup\n");

  fprintf(stderr, "os setup\n");

  OS_Symbian os{instance};

  fprintf(stderr, "main setup\n");

	Error err = Main::setup("/sys/bin/godot.exe", 0, nullptr);

  fprintf(stderr, "inst setup\n");
  if (Main::start()) {
    fprintf(stderr, "main start\n");
    os.run();
    fprintf(stderr, "main stop\n");
  }
  fprintf(stderr, "godot stop\n");
	Main::cleanup();
  fprintf(stderr, "godot cleaned up\n");

  auto ec = os.get_exit_code();
  fprintf(stderr, "ec = %d\n", ec);
  if (ec) {
    _LIT(KGodotEngineError, "GODOT");
    User::Panic(KGodotEngineError, ec);
  }
  return ec;
#endif

  CGodotContainer *instance = static_cast<CGodotContainer *>(aInstance);

  if (Main::iteration() == true) {
    fprintf(stderr, "was last iteration\n");
    instance->iMainPeriodic->Cancel();
  } else {

    User::After(0);
  }

  return 0;
}

void CGodotContainer::MaoscOpenComplete(TInt aError) {
  if (aError == KErrNone) {
    iVolume = iOutputStream->MaxVolume() / 2;
    iVolumeStep = iOutputStream->MaxVolume() / 8;
    if (iVolumeStep == 0)
      iVolumeStep = 1;

    iOutputStream->SetVolume(iVolume);
    iOutputStream->SetPriority(EPriorityNormal, EMdaPriorityPreferenceTime);
    //iOutputStream->SetDataTypeL(KMMFFourCCCodePCM16);

    iOutputStatus = EOpen;

    RequestSound();
  } else {
    iOutputStatus = ENotReady;
  }    
}

void CGodotContainer::MaoscBufferCopied(TInt aError, const TDesC8& aBuffer) {
  if (aError == KErrAbort) {
    iOutputStatus = ENotReady;
    //LPRINTF("Buffer copy aborted!");
  } else if (iOutputStatus != ENotReady) {
    RequestSound();
  }
}

void CGodotContainer::MaoscPlayComplete(TInt aError) {
  iOutputStatus = ENotReady;
  //LPRINTF("Sound output closed");
}

TInt CGodotContainer::ResetInactivityTimeCallBack(TAny *aInstance) {
  CGodotContainer *instance = static_cast<CGodotContainer *>(aInstance);

  User::ResetInactivityTime();

  User::After(10000);

  return 0;
}

void CGodotContainer::SwapBuffers() {
  eglSwapBuffers(iEglDisplay, iEglSurface);
}

void CGodotContainer::MakeCurrent() {
  eglMakeCurrent(iEglDisplay, iEglSurface, iEglSurface, iEglContext);
}

void CGodotContainer::ReleaseCurrent() {
  eglMakeCurrent(iEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool CGodotContainer::IsScanCodeNonModifier(TInt aScanCode) {
#ifdef SYMBIAN_FIXED_KEYMAP
  return !(aScanCode == EStdKeyLeftFunc ||
           aScanCode == EStdKeyRightFunc ||
           aScanCode == EStdKeyLeftCtrl ||
           aScanCode == EStdKeyRightCtrl ||
           aScanCode == EStdKeyLeftShift ||
           aScanCode == EStdKeyRightShift);
#else
  return !(aScanCode == EKeyLeftFunc ||
           aScanCode == EKeyRightFunc ||
           aScanCode == EKeyLeftCtrl ||
           aScanCode == EKeyRightCtrl ||
           aScanCode == EKeyLeftShift ||
           aScanCode == EKeyRightShift);
#endif
}

bool CGodotContainer::HandleWsEvent(TWsEvent &aEvent) {
  TInt scanCode;
  switch (aEvent.Type()) {
  case EEventKeyDown:
    scanCode = aEvent.Key()->iScanCode;
    switch (aEvent.Key()->iCode) {
    case EKeyIncVolume:
      iVolume += iVolumeStep;
      iOutputStatus = ESetVolume;
      return true;
    case EKeyDecVolume:
      iVolume -= iVolumeStep;
      iOutputStatus = ESetVolume;
      return true;
    default:
#ifdef SYMBIAN_FIXED_KEYMAP
      if (!std::count(iPressedKeys.begin(), iPressedKeys.end(), scanCode)) {
        iPressedKeys.push_back(scanCode);
        if (CGodotContainer::IsScanCodeNonModifier(scanCode)) {
          iModifierUsed = true;
        }
      }
      return true;
#else
      return false;
#endif
    }
    break;
#ifdef SYMBIAN_FIXED_KEYMAP
  case EEventKeyUp:
    scanCode = aEvent.Key()->iScanCode;
    if (CGodotContainer::IsScanCodeNonModifier(scanCode)) {
      iPressedKeys.remove(scanCode);
      if (!std::count_if(iPressedKeys.begin(), iPressedKeys.end(), CGodotContainer::IsScanCodeNonModifier)) {
        for (auto modifier : iDepressedModifiers) {
          iPressedKeys.remove(modifier);
        }
        iDepressedModifiers.clear();
      }
    } else if (iModifierUsed) {
      if (!std::count_if(iPressedKeys.begin(), iPressedKeys.end(), CGodotContainer::IsScanCodeNonModifier)) {
        iPressedKeys.remove(scanCode);
        iDepressedModifiers.erase(scanCode);
      } else {
        iPressedKeys.clear();
      }
    } else {
      iDepressedModifiers.insert(scanCode);
    }
    if (!std::count_if(iPressedKeys.begin(), iPressedKeys.end(), [](auto scanCode) {
        return !CGodotContainer::IsScanCodeNonModifier(scanCode);
      })) {
      iModifierUsed = false;
    }
    if (iPressedKeys.empty()) {
      iDepressedModifiers.clear();
    }
    return true;
#endif
  case EEventPointer:
    switch (aEvent.Pointer()->iType) {
    case TPointerEvent::EButton1Down: 
      iPosition = aEvent.Pointer()->iPosition;
      iButton1DownNextFrame = true;
      break;
    case TPointerEvent::EButton1Up:
      iPosition = aEvent.Pointer()->iPosition;
      iButton1DownNextFrame = false;
      break;
    case TPointerEvent::EDrag:
    case TPointerEvent::EMove:
      iPosition = aEvent.Pointer()->iPosition;
      break;
    }
    return true;
  default:
    break;
  }
  return false;
}

void CGodotContainer::Draw(const TRect &aRect) const {}

void CGodotContainer::SizeChanged() {
  eglSwapBuffers(iEglDisplay, iEglSurface);
}

void CGodotContainer::HandleResourceChange(TInt aType) {
  switch (aType) {
  case KEikDynamicLayoutVariantSwitch:
    SetExtentToWholeScreen();
    TSize size = GetWindowSize();
    iOs->set_video_mode(OS::VideoMode(size.iWidth, size.iHeight, false));
    eglSwapBuffers(iEglDisplay, iEglSurface);
    break;
  }
}

void CGodotContainer::RequestSound() {
#if 0
  studio_sound(iStudio);
  auto mem = studio_mem(iStudio);
#else
#warning "Unimplemented!"
  return;
#endif

  if (iOutputStatus == ESetVolume) {
    iOutputStatus = EOpen;
    if (iVolume < 0)
      iVolume = 0;
    if (iVolume > iOutputStream->MaxVolume())
      iVolume = iOutputStream->MaxVolume();
    //LPRINTF("Set volume to %d", iVolume);
    iOutputStream->SetVolume(iVolume);
  }

  iOutputStream->WriteL(iSoundBufferPtr);
}

TInt CGodotContainer::CountComponentControls() const { return 0; }

CCoeControl *CGodotContainer::ComponentControl(TInt aIndex) const {
  return nullptr;
}

CGodotContainer::~CGodotContainer() {
  iMainPeriodic->Cancel();
  delete iMainPeriodic;

	iOs->main_loop->finish();

	Main::cleanup();

  delete iOs;

  delete iPeriodic;


  auto ec = iOs->get_exit_code();

  if (iOutputStatus != ENotReady)
    iOutputStream->Stop();
  delete iOutputStream;

  eglMakeCurrent(iEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(iEglDisplay, iEglSurface);
  eglDestroyContext(iEglDisplay, iEglContext);
  eglTerminate(iEglDisplay);
#ifdef GLES2_ENABLED
  eglReleaseThread();
#endif
}

#pragma mark - CWsEventReceiver impl

CWsEventReceiver::CWsEventReceiver()
    : CActive(CActive::EPriorityHigh), iParent(NULL) {}

CWsEventReceiver::~CWsEventReceiver() { Cancel(); }

CWsEventReceiver *CWsEventReceiver::NewL(CGodotContainer &aParent,
                                         RWsSession *aWsSession) {
  CWsEventReceiver *self = new (ELeave) CWsEventReceiver;

  CleanupStack::PushL(self);

  self->ConstructL(aParent, aWsSession);

  CleanupStack::Pop(self);

  return self;
}

void CWsEventReceiver::ConstructL(CGodotContainer &aParent, RWsSession *aWsSession) {
  iParent = &aParent;
  iWsSession = aWsSession;
  iWsSession->EventReady(&iStatus);

  CActiveScheduler::Add(this);

  SetActive();
}

void CWsEventReceiver::RunL() {
  TWsEvent wsEvent;
  iWsSession->GetEvent(wsEvent);

  if (!iParent->HandleWsEvent(wsEvent)) {
    static_cast<CGodotAppUi *>(iParent->iAppUi)->HandleEventL(wsEvent);
  }

  iWsSession->EventReady(&iStatus);

  User::After(0);

  SetActive();
}

void CWsEventReceiver::DoCancel() {
  iWsSession->EventReadyCancel();
}
