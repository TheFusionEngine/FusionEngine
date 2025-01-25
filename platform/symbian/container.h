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

#pragma once

#ifdef GLES2_ENABLED
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#else
#include <GLES/egl.h>
#include <GLES/gl.h>
#endif
#include <aknappui.h>
#include <coecntrl.h>
#include <mdaaudiosampleeditor.h>
#include <mdaaudiooutputstream.h>
#include <mda/common/audio.h>
#include <mmf/common/mmfutilities.h>
#include <e32base.h>
#include <e32cmn.h>
#include <e32std.h>
#include <w32std.h>

#ifdef SYMBIAN_FIXED_KEYMAP
#include <list>
#include <set>
#endif

struct CGodotAppUi;
struct CWsEventReceiver;
struct CActiveMain;
struct OS_Symbian;

struct CGodotContainer : CCoeControl, MMdaAudioOutputStreamCallback {
  friend struct OS_Symbian;
  friend struct CGodotAppUi;
  friend struct CWsEventReceiver;

  void ConstructL(const TRect &aRect, CAknAppUi *aAppUi);

  CGodotContainer();

  ~CGodotContainer() override;

  static TInt ResetInactivityTimeCallBack(TAny *aInstance);

  bool HandleWsEvent(TWsEvent &aEvent);

private:
  static TInt RunEngineThread(TAny *aInstance);

public:
  void SwapBuffers();

  void MakeCurrent();

  void ReleaseCurrent();

  TSize GetWindowSize() { return iWinSize; }

public:
  void MaoscOpenComplete(TInt aError) override;

  void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer) override;
  
  void MaoscPlayComplete(TInt aError) override;

private:
  void SizeChanged() override;

  void HandleResourceChange(TInt aType) override;

  TInt CountComponentControls() const override;

  CCoeControl *ComponentControl(TInt aIndex) const override;

  void Draw(const TRect &aRect) const override;

  void ProcessInputs();

  void RequestSound();

  static bool IsScanCodeNonModifier(TInt iScanCode);

private:
  enum TStatus {
    ENotReady,
    EOpen,
    ESetVolume
  };

  enum { KBufferMaxFrames = 10 };

  CWsEventReceiver *iWsEventReceiver;

  OS_Symbian *iOs;

  EGLDisplay iEglDisplay;
  EGLSurface iEglSurface;
  EGLContext iEglContext;
  TSize iWinSize;
  CPeriodic *iPeriodic;
  CPeriodic *iMainPeriodic;
  CAknAppUi *iAppUi;

  bool iButton1Down;
  bool iButton1DownNextFrame;
  TPoint iPosition;
#if SYMBIAN_FIXED_KEYMAP
  std::list<TInt> iPressedKeys;
  std::set<TInt> iDepressedModifiers;
  bool iModifierUsed;
#else
  TKeyEvent iPressedKey;
#endif

  TPtrC8 iSoundBufferPtr;
  TUint iGodotFrameSize;

  TInt iVolume;
  TUint iVolumeStep;

  TMdaAudioDataSettings iStreamSettings;
  CMdaAudioOutputStream* iOutputStream;
  TStatus iOutputStatus;

  //RThread iEngineThread;
};


struct CWsEventReceiver : CActive {
  void RunL() override;

  void DoCancel() override;

  static CWsEventReceiver *NewL(CGodotContainer &aParent, RWsSession *aWsSession);

  ~CWsEventReceiver();

private:
  CWsEventReceiver();

  void ConstructL(CGodotContainer &aParent, RWsSession *aWsSession);

private:
  RWsSession *iWsSession;

  CGodotContainer *iParent;
};
