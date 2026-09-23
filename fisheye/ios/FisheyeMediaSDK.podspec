#
# Carries the vendored MediaSDK xcframeworks (see ../README.md) as their
# own pod, separate from flutter_webrtc.podspec, purely because CocoaPods
# does not correctly link/embed a pod's own vendored_frameworks when that
# same pod has static_framework = true (which flutter_webrtc.podspec sets,
# and has for a long time, unrelated to this spike -- not something to
# flip off just for this). Confirmed by direct comparison in this exact
# project: WebRTC-SDK's own xcframework (a dependency of flutter_webrtc,
# declared in WebRTC-SDK's own non-static podspec) gets embedded into the
# app correctly, while these same frameworks silently failed to link or
# embed at all when declared directly on flutter_webrtc.podspec, whether
# as plain .framework or .xcframework.
#
# Consuming app's ios/Podfile needs one extra line for this to resolve
# (a plain `s.dependency 'FisheyeMediaSDK'` on flutter_webrtc.podspec is
# not enough -- CocoaPods pod dependencies need the *app's* Podfile to
# know where to find a pod with no published spec):
#   pod 'FisheyeMediaSDK', :path => '<path to this file's directory>'
#
Pod::Spec.new do |s|
  s.name             = 'FisheyeMediaSDK'
  s.version          = '0.0.1'
  s.summary          = 'Vendored MediaSDK fisheye libs for the flutter_webrtc fisheye spike.'
  s.homepage         = 'https://github.com/turingvideo/flutter-webrtc'
  s.license          = { :type => 'Proprietary', :text => 'Vendor-supplied binary, not open source.' }
  s.author           = { 'Turing Video' => 'noreply@turingvideo.com' }
  s.source           = { :path => '.' }
  s.ios.deployment_target = '13.0'
  s.vendored_frameworks = [
    'vendor/XCFrameworks/libMediaSDK.xcframework',
    'vendor/XCFrameworks/libParser.xcframework',
    'vendor/XCFrameworks/libRtsp.xcframework',
    'vendor/XCFrameworks/libUHls.xcframework',
    'vendor/XCFrameworks/libURtmp.xcframework',
    'vendor/XCFrameworks/libEncrypt.xcframework',
    'vendor/XCFrameworks/libUmpeg.xcframework',
  ]
end
