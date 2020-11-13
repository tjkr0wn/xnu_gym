
![Logo](logo.png) # A TK Cracks Project

# xnu_gym

  `xnu_gym` is a project meant to purposely inject bugs into an iOS kernel that are
  trigerrable from Userspace. This is perfect for any beginner to iOS kernel
  exploitation so that they can practice their skills from exploitation to launching
  a full jailbreak on a device.

  **_PLEASE NOTE_**: This is not a live build. This project is still not runnable.
  Progress IS being made, however I still have to do some reverse engineering of
  the XNU kernel to verify some functions as well as do some reading on previous vulnerabilities
  in order to properly reintroduce them. Thanks for your patience!

# Restrictions

  **Devices: ** Devices compatible with the checkra1n jailbreak (https://checkra.in/)

  **iOS Version: ** I'm only supporting iOS versions 13.0 to 13.7.

  **Tested iOS Versions:* * iOS 13.6.1

# Loading/usage

  Loading and using `xnu_gym` is super easy. In the loader directory you'll find a `load_xnu_gym.py`
  Python3 script. This is a quick script I whipped up to load the `xnu_gym` binary pongoOS module
  and subsequently provide an easy console to select your vulnerabilities and reboot into XNU!

  Simply run `python3 load_xnu_gym.py`

  A nice interactive console will pop up for you that makes it easy and automates your selection
  of vulnerabilities.

  **_PLEASE NOTE_**: Any errors with the module itself will appear on your device's screen, so be
  aware. All loader-related issues will appear normally in the Terminal/console on your computer.


# How it works

  `xnu_gym` works as a pongoOS module. Once your device boots from pongoOS into the kernel,
  `xnu_gym` will work by looking for and patching your selected bugs.

# Building

  I provide a binary in the builds directory. If you don't feel comfortable using one of
  these binaries, feel free to build the module from source!

  *SHA256 Hashes of builds:*
  
    Live build: <none>
