#### SDK Android version: 1.3.5
- The Java wrappers are built on the C++ sdk version 1.3.5 this includes share actions but not group actions.


The Android native library is compiled with **android-ndk-r21** with **api 29** for ABI x86_64 and tested on the following android emulator:
```
Name: Pixel_4a_API_30
CPU/ABI: Google APIs Intel Atom (x86_64)
Path: /home/cloudbackend1/.android/avd/Pixel_4a_API_30.avd
Target: google_apis [Google APIs] (API level 30)
Skin: pixel_4a
SD Card: 512M
fastboot.chosenSnapshotFile: 
runtime.network.speed: full
hw.accelerometer: yes
hw.device.name: pixel_4a
hw.lcd.width: 1080
hw.initialOrientation: Portrait
image.androidVersion.api: 30
tag.id: google_apis
hw.mainKeys: no
hw.camera.front: emulated
avd.ini.displayname: Pixel 4a API 30
hw.gpu.mode: software
hw.ramSize: 1536
PlayStore.enabled: false
fastboot.forceColdBoot: no
hw.cpu.ncore: 4
hw.keyboard: yes
hw.sensors.proximity: yes
hw.dPad: no
hw.lcd.height: 2340
vm.heapSize: 256
skin.dynamic: yes
hw.device.manufacturer: Google
hw.gps: yes
hw.audioInput: yes
image.sysdir.1: system-images/android-30/google_apis/x86_64/
showDeviceFrame: yes
hw.camera.back: virtualscene
AvdId: Pixel_4a_API_30
hw.lcd.density: 440
hw.arc: false
hw.device.hash2: MD5:a71dbdf58af14a724eb82b86d3f87030
fastboot.forceChosenSnapshotBoot: no
fastboot.forceFastBoot: yes
hw.trackBall: no
hw.battery: yes
hw.sdCard: yes
tag.display: Google APIs
runtime.network.latency: none
disk.dataPartition.size: 6442450944
hw.sensors.orientation: yes
avd.ini.encoding: UTF-8
hw.gpu.enabled: yes
```
The libAndroidCBE.jar containes the native library that you add to jniLibs or libs depending on how you choose your file structure. 

Make sure to add in the preload of your app the following: 

```
static {
    System.loadLibrary("cb_sdk_shared_android");
}
```


The java wrappers are located in the com.cbe.jar and can be added in a folder called jni.
The following was added to the gradle.build file:

```
dependencies {
    implementation files('src/main/jni/com.cbe.jar')
}
```

and inside the android brackets:
```
sourceSets {
    main {
        jniLibs.srcDirs = ['libs']
    }
}
```

As a last setting before using the library add the following to the AndroidManifest.xml file:
```
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
``` 
Only LogIn have been tested this far on Android but the Java wrappers have been tested for more extensive tests. Still no warrenties of any kind are issued for this pre-release of the SDK.

------------------------------------------------------------------------------

#### Copyright © CloudBackend AB 2021.