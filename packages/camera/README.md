# camera_tizen

[![pub package](https://img.shields.io/pub/v/camera_tizen.svg)](https://pub.dev/packages/camera_tizen)

The Tizen implementation of [`camera`](https://pub.dev/packages/camera).

## Supported devices

This plugin is currently experimental and does not support any devices.

## Required privileges

To use this plugin, add below lines under the `<manifest>` section in your `tizen-manifest.xml` file.

```xml
<privileges>
    <privilege>http://tizen.org/privilege/camera</privilege>
    <privilege>http://tizen.org/privilege/recorder</privilege>
</privileges>
```

## Usage

This package is not an _endorsed_ implementation of `camera`. Therefore, you have to include `camera_tizen` alongside `camera` as dependencies in your `pubspec.yaml` file.

```yaml
dependencies:
  camera: ^0.9.4
  camera_tizen: ^0.3.5
```

Then you can import `camera` in your Dart code:

```dart
import 'package:camera/camera.dart';
```

For detailed usage, see https://pub.dev/packages/camera#example.

## Notes

For the camera preview to rotate correctly, you have to modify the `CameraPreview` class (`camera_preview.dart`) as follows.

```dart
Widget _wrapInRotatedBox({required Widget child}) {
  // if (kIsWeb || defaultTargetPlatform != TargetPlatform.android) {
  //   return child;
  // }

  return RotatedBox(
    quarterTurns: _getQuarterTurns(),
    child: child,
  );
}
```
