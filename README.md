SSHTunnel
=========

A SSH Tunnel app for Android

Build
=====

```bash
NDK_MODULE_PATH=. ndk-build
cp libs/armeabi/sshtunnel assets/
rm -rf libs
adb -Dversion-name=VERSION-NAME
```
