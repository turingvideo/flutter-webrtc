cmake_minimum_required(VERSION 3.10.2)
project(fisheye_mediasdk_jni)

# Vendor MediaSDK: prebuilt .so per ABI + C headers, imported as-is from
# fisheye/android/vendor (see fisheye/README.md for the reference call
# sequence). We only link against libMediaSDK.so directly -- the other
# vendor .so files under vendor/libs/${ANDROID_ABI}/ are libMediaSDK.so's
# own runtime dependencies, resolved by the dynamic linker at load time
# because Gradle packages the whole per-ABI folder into the APK's jniLibs
# (see android/build.gradle), not because we link them here.
set(MEDIASDK_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../vendor)

add_library(mediasdk SHARED IMPORTED)
set_target_properties(mediasdk PROPERTIES
    IMPORTED_LOCATION ${MEDIASDK_VENDOR_DIR}/libs/${ANDROID_ABI}/libMediaSDK.so)

add_library(fisheye_mediasdk_jni SHARED mediasdk_jni.cpp)

target_include_directories(fisheye_mediasdk_jni PRIVATE
    ${MEDIASDK_VENDOR_DIR}/include)

find_library(log-lib log)

target_link_libraries(fisheye_mediasdk_jni
    mediasdk
    ${log-lib})
