// Autogenerated from Pigeon (v10.1.6), do not edit directly.
// See also: https://pub.dev/packages/pigeon
// ignore_for_file: public_member_api_docs, non_constant_identifier_names, avoid_as, unused_import, unnecessary_parenthesis, prefer_null_aware_operators, omit_local_variable_types, unused_shown_name, unnecessary_import

import 'dart:async';
import 'dart:typed_data' show Float64List, Int32List, Int64List, Uint8List;

import 'package:flutter/foundation.dart' show ReadBuffer, WriteBuffer;
import 'package:flutter/services.dart';

class PlayerMessage {
  PlayerMessage({required this.playerId});

  int playerId;

  Object encode() {
    return <Object?>[playerId];
  }

  static PlayerMessage decode(Object result) {
    result as List<Object?>;
    return PlayerMessage(playerId: result[0]! as int);
  }
}

class LoopingMessage {
  LoopingMessage({required this.playerId, required this.isLooping});

  int playerId;

  bool isLooping;

  Object encode() {
    return <Object?>[playerId, isLooping];
  }

  static LoopingMessage decode(Object result) {
    result as List<Object?>;
    return LoopingMessage(
      playerId: result[0]! as int,
      isLooping: result[1]! as bool,
    );
  }
}

class VolumeMessage {
  VolumeMessage({required this.playerId, required this.volume});

  int playerId;

  double volume;

  Object encode() {
    return <Object?>[playerId, volume];
  }

  static VolumeMessage decode(Object result) {
    result as List<Object?>;
    return VolumeMessage(
      playerId: result[0]! as int,
      volume: result[1]! as double,
    );
  }
}

class PlaybackSpeedMessage {
  PlaybackSpeedMessage({required this.playerId, required this.speed});

  int playerId;

  double speed;

  Object encode() {
    return <Object?>[playerId, speed];
  }

  static PlaybackSpeedMessage decode(Object result) {
    result as List<Object?>;
    return PlaybackSpeedMessage(
      playerId: result[0]! as int,
      speed: result[1]! as double,
    );
  }
}

class TrackMessage {
  TrackMessage({required this.playerId, required this.tracks});

  int playerId;

  List<Map<Object?, Object?>?> tracks;

  Object encode() {
    return <Object?>[playerId, tracks];
  }

  static TrackMessage decode(Object result) {
    result as List<Object?>;
    return TrackMessage(
      playerId: result[0]! as int,
      tracks: (result[1] as List<Object?>?)!.cast<Map<Object?, Object?>?>(),
    );
  }
}

class TrackTypeMessage {
  TrackTypeMessage({required this.playerId, required this.trackType});

  int playerId;

  String trackType;

  Object encode() {
    return <Object?>[playerId, trackType];
  }

  static TrackTypeMessage decode(Object result) {
    result as List<Object?>;
    return TrackTypeMessage(
      playerId: result[0]! as int,
      trackType: result[1]! as String,
    );
  }
}

class SelectedTracksMessage {
  SelectedTracksMessage({
    required this.playerId,
    required this.trackId,
    required this.trackType,
  });

  int playerId;

  int trackId;

  String trackType;

  Object encode() {
    return <Object?>[playerId, trackId, trackType];
  }

  static SelectedTracksMessage decode(Object result) {
    result as List<Object?>;
    return SelectedTracksMessage(
      playerId: result[0]! as int,
      trackId: result[1]! as int,
      trackType: result[2]! as String,
    );
  }
}

class PositionMessage {
  PositionMessage({required this.playerId, required this.position});

  int playerId;

  int position;

  Object encode() {
    return <Object?>[playerId, position];
  }

  static PositionMessage decode(Object result) {
    result as List<Object?>;
    return PositionMessage(
      playerId: result[0]! as int,
      position: result[1]! as int,
    );
  }
}

class CreateMessage {
  CreateMessage({
    this.asset,
    this.uri,
    this.packageName,
    this.formatHint,
    this.httpHeaders,
    this.drmConfigs,
    this.playerOptions,
    this.streamingProperty,
  });

  String? asset;

  String? uri;

  String? packageName;

  String? formatHint;

  Map<Object?, Object?>? httpHeaders;

  Map<Object?, Object?>? drmConfigs;

  Map<Object?, Object?>? playerOptions;

  Map<Object?, Object?>? streamingProperty;

  Object encode() {
    return <Object?>[
      asset,
      uri,
      packageName,
      formatHint,
      httpHeaders,
      drmConfigs,
      playerOptions,
      streamingProperty,
    ];
  }

  static CreateMessage decode(Object result) {
    result as List<Object?>;
    return CreateMessage(
      asset: result[0] as String?,
      uri: result[1] as String?,
      packageName: result[2] as String?,
      formatHint: result[3] as String?,
      httpHeaders:
          (result[4] as Map<Object?, Object?>?)?.cast<Object?, Object?>(),
      drmConfigs:
          (result[5] as Map<Object?, Object?>?)?.cast<Object?, Object?>(),
      playerOptions:
          (result[6] as Map<Object?, Object?>?)?.cast<Object?, Object?>(),
      streamingProperty:
          (result[7] as Map<Object?, Object?>?)?.cast<Object?, Object?>(),
    );
  }
}

class MixWithOthersMessage {
  MixWithOthersMessage({required this.mixWithOthers});

  bool mixWithOthers;

  Object encode() {
    return <Object?>[mixWithOthers];
  }

  static MixWithOthersMessage decode(Object result) {
    result as List<Object?>;
    return MixWithOthersMessage(mixWithOthers: result[0]! as bool);
  }
}

class GeometryMessage {
  GeometryMessage({
    required this.playerId,
    required this.x,
    required this.y,
    required this.width,
    required this.height,
  });

  int playerId;

  int x;

  int y;

  int width;

  int height;

  Object encode() {
    return <Object?>[playerId, x, y, width, height];
  }

  static GeometryMessage decode(Object result) {
    result as List<Object?>;
    return GeometryMessage(
      playerId: result[0]! as int,
      x: result[1]! as int,
      y: result[2]! as int,
      width: result[3]! as int,
      height: result[4]! as int,
    );
  }
}

class DurationMessage {
  DurationMessage({required this.playerId, this.durationRange});

  int playerId;

  List<int?>? durationRange;

  Object encode() {
    return <Object?>[playerId, durationRange];
  }

  static DurationMessage decode(Object result) {
    result as List<Object?>;
    return DurationMessage(
      playerId: result[0]! as int,
      durationRange: (result[1] as List<Object?>?)?.cast<int?>(),
    );
  }
}

class StreamingPropertyTypeMessage {
  StreamingPropertyTypeMessage({
    required this.playerId,
    required this.streamingPropertyType,
  });

  int playerId;

  String streamingPropertyType;

  Object encode() {
    return <Object?>[playerId, streamingPropertyType];
  }

  static StreamingPropertyTypeMessage decode(Object result) {
    result as List<Object?>;
    return StreamingPropertyTypeMessage(
      playerId: result[0]! as int,
      streamingPropertyType: result[1]! as String,
    );
  }
}

class StreamingPropertyMessage {
  StreamingPropertyMessage({
    required this.playerId,
    required this.streamingPropertyType,
    required this.streamingPropertyValue,
  });

  int playerId;

  String streamingPropertyType;

  String streamingPropertyValue;

  Object encode() {
    return <Object?>[playerId, streamingPropertyType, streamingPropertyValue];
  }

  static StreamingPropertyMessage decode(Object result) {
    result as List<Object?>;
    return StreamingPropertyMessage(
      playerId: result[0]! as int,
      streamingPropertyType: result[1]! as String,
      streamingPropertyValue: result[2]! as String,
    );
  }
}

class BufferConfigMessage {
  BufferConfigMessage({
    required this.playerId,
    required this.bufferConfigType,
    required this.bufferConfigValue,
  });

  int playerId;

  String bufferConfigType;

  int bufferConfigValue;

  Object encode() {
    return <Object?>[playerId, bufferConfigType, bufferConfigValue];
  }

  static BufferConfigMessage decode(Object result) {
    result as List<Object?>;
    return BufferConfigMessage(
      playerId: result[0]! as int,
      bufferConfigType: result[1]! as String,
      bufferConfigValue: result[2]! as int,
    );
  }
}

class RotationMessage {
  RotationMessage({required this.playerId, required this.rotation});

  int playerId;

  int rotation;

  Object encode() {
    return <Object?>[playerId, rotation];
  }

  static RotationMessage decode(Object result) {
    result as List<Object?>;
    return RotationMessage(
      playerId: result[0]! as int,
      rotation: result[1]! as int,
    );
  }
}

class DisplayModeMessage {
  DisplayModeMessage({required this.playerId, required this.displayMode});

  int playerId;

  int displayMode;

  Object encode() {
    return <Object?>[playerId, displayMode];
  }

  static DisplayModeMessage decode(Object result) {
    result as List<Object?>;
    return DisplayModeMessage(
      playerId: result[0]! as int,
      displayMode: result[1]! as int,
    );
  }
}

class DashPropertyTypeListMessage {
  DashPropertyTypeListMessage({required this.playerId, required this.typeList});

  int playerId;

  List<String?> typeList;

  Object encode() {
    return <Object?>[playerId, typeList];
  }

  static DashPropertyTypeListMessage decode(Object result) {
    result as List<Object?>;
    return DashPropertyTypeListMessage(
      playerId: result[0]! as int,
      typeList: (result[1] as List<Object?>?)!.cast<String?>(),
    );
  }
}

class DashPropertyMapMessage {
  DashPropertyMapMessage({required this.playerId, required this.mapData});

  int playerId;

  Map<Object?, Object?> mapData;

  Object encode() {
    return <Object?>[playerId, mapData];
  }

  static DashPropertyMapMessage decode(Object result) {
    result as List<Object?>;
    return DashPropertyMapMessage(
      playerId: result[0]! as int,
      mapData: (result[1] as Map<Object?, Object?>?)!.cast<Object?, Object?>(),
    );
  }
}

class _VideoPlayerAvplayApiCodec extends StandardMessageCodec {
  const _VideoPlayerAvplayApiCodec();
  @override
  void writeValue(WriteBuffer buffer, Object? value) {
    if (value is BufferConfigMessage) {
      buffer.putUint8(128);
      writeValue(buffer, value.encode());
    } else if (value is CreateMessage) {
      buffer.putUint8(129);
      writeValue(buffer, value.encode());
    } else if (value is CreateMessage) {
      buffer.putUint8(130);
      writeValue(buffer, value.encode());
    } else if (value is DashPropertyMapMessage) {
      buffer.putUint8(131);
      writeValue(buffer, value.encode());
    } else if (value is DashPropertyTypeListMessage) {
      buffer.putUint8(132);
      writeValue(buffer, value.encode());
    } else if (value is DisplayModeMessage) {
      buffer.putUint8(133);
      writeValue(buffer, value.encode());
    } else if (value is DurationMessage) {
      buffer.putUint8(134);
      writeValue(buffer, value.encode());
    } else if (value is GeometryMessage) {
      buffer.putUint8(135);
      writeValue(buffer, value.encode());
    } else if (value is LoopingMessage) {
      buffer.putUint8(136);
      writeValue(buffer, value.encode());
    } else if (value is MixWithOthersMessage) {
      buffer.putUint8(137);
      writeValue(buffer, value.encode());
    } else if (value is PlaybackSpeedMessage) {
      buffer.putUint8(138);
      writeValue(buffer, value.encode());
    } else if (value is PlayerMessage) {
      buffer.putUint8(139);
      writeValue(buffer, value.encode());
    } else if (value is PositionMessage) {
      buffer.putUint8(140);
      writeValue(buffer, value.encode());
    } else if (value is RotationMessage) {
      buffer.putUint8(141);
      writeValue(buffer, value.encode());
    } else if (value is SelectedTracksMessage) {
      buffer.putUint8(142);
      writeValue(buffer, value.encode());
    } else if (value is StreamingPropertyMessage) {
      buffer.putUint8(143);
      writeValue(buffer, value.encode());
    } else if (value is StreamingPropertyTypeMessage) {
      buffer.putUint8(144);
      writeValue(buffer, value.encode());
    } else if (value is TrackMessage) {
      buffer.putUint8(145);
      writeValue(buffer, value.encode());
    } else if (value is TrackTypeMessage) {
      buffer.putUint8(146);
      writeValue(buffer, value.encode());
    } else if (value is VolumeMessage) {
      buffer.putUint8(147);
      writeValue(buffer, value.encode());
    } else {
      super.writeValue(buffer, value);
    }
  }

  @override
  Object? readValueOfType(int type, ReadBuffer buffer) {
    switch (type) {
      case 128:
        return BufferConfigMessage.decode(readValue(buffer)!);
      case 129:
        return CreateMessage.decode(readValue(buffer)!);
      case 130:
        return CreateMessage.decode(readValue(buffer)!);
      case 131:
        return DashPropertyMapMessage.decode(readValue(buffer)!);
      case 132:
        return DashPropertyTypeListMessage.decode(readValue(buffer)!);
      case 133:
        return DisplayModeMessage.decode(readValue(buffer)!);
      case 134:
        return DurationMessage.decode(readValue(buffer)!);
      case 135:
        return GeometryMessage.decode(readValue(buffer)!);
      case 136:
        return LoopingMessage.decode(readValue(buffer)!);
      case 137:
        return MixWithOthersMessage.decode(readValue(buffer)!);
      case 138:
        return PlaybackSpeedMessage.decode(readValue(buffer)!);
      case 139:
        return PlayerMessage.decode(readValue(buffer)!);
      case 140:
        return PositionMessage.decode(readValue(buffer)!);
      case 141:
        return RotationMessage.decode(readValue(buffer)!);
      case 142:
        return SelectedTracksMessage.decode(readValue(buffer)!);
      case 143:
        return StreamingPropertyMessage.decode(readValue(buffer)!);
      case 144:
        return StreamingPropertyTypeMessage.decode(readValue(buffer)!);
      case 145:
        return TrackMessage.decode(readValue(buffer)!);
      case 146:
        return TrackTypeMessage.decode(readValue(buffer)!);
      case 147:
        return VolumeMessage.decode(readValue(buffer)!);
      default:
        return super.readValueOfType(type, buffer);
    }
  }
}

class VideoPlayerAvplayApi {
  /// Constructor for [VideoPlayerAvplayApi].  The [binaryMessenger] named argument is
  /// available for dependency injection.  If it is left null, the default
  /// BinaryMessenger will be used which routes to the host platform.
  VideoPlayerAvplayApi({BinaryMessenger? binaryMessenger})
    : _binaryMessenger = binaryMessenger;
  final BinaryMessenger? _binaryMessenger;

  static const MessageCodec<Object?> codec = _VideoPlayerAvplayApiCodec();

  Future<void> initialize() async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.initialize',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList = await channel.send(null) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<PlayerMessage> create(CreateMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.create',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as PlayerMessage?)!;
    }
  }

  Future<void> dispose(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.dispose',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> setLooping(LoopingMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setLooping',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> setVolume(VolumeMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setVolume',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> setPlaybackSpeed(PlaybackSpeedMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setPlaybackSpeed',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> play(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.play',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<bool> setDeactivate(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setDeactivate',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<bool> setActivate(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setActivate',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<TrackMessage> track(TrackTypeMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.track',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as TrackMessage?)!;
    }
  }

  Future<bool> setTrackSelection(SelectedTracksMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setTrackSelection',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<PositionMessage> position(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.position',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as PositionMessage?)!;
    }
  }

  Future<DurationMessage> duration(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.duration',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as DurationMessage?)!;
    }
  }

  Future<void> seekTo(PositionMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.seekTo',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> pause(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.pause',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> setMixWithOthers(MixWithOthersMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setMixWithOthers',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> setDisplayGeometry(GeometryMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setDisplayGeometry',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<String> getStreamingProperty(
    StreamingPropertyTypeMessage arg_msg,
  ) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.getStreamingProperty',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as String?)!;
    }
  }

  Future<bool> setBufferConfig(BufferConfigMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setBufferConfig',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<void> setStreamingProperty(StreamingPropertyMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setStreamingProperty',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<bool> setDisplayRotate(RotationMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setDisplayRotate',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<bool> setDisplayMode(DisplayModeMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setDisplayMode',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<void> suspend(int arg_playerId) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.suspend',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_playerId]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<void> restore(
    int arg_playerId,
    CreateMessage? arg_msg,
    int arg_resumeTime,
  ) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.restore',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_playerId, arg_msg, arg_resumeTime])
            as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else {
      return;
    }
  }

  Future<bool> setData(DashPropertyMapMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.setData',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as bool?)!;
    }
  }

  Future<DashPropertyMapMessage> getData(
    DashPropertyTypeListMessage arg_msg,
  ) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.getData',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as DashPropertyMapMessage?)!;
    }
  }

  Future<TrackMessage> getActiveTrackInfo(PlayerMessage arg_msg) async {
    final BasicMessageChannel<Object?> channel = BasicMessageChannel<Object?>(
      'dev.flutter.pigeon.video_player_avplay.VideoPlayerAvplayApi.getActiveTrackInfo',
      codec,
      binaryMessenger: _binaryMessenger,
    );
    final List<Object?>? replyList =
        await channel.send(<Object?>[arg_msg]) as List<Object?>?;
    if (replyList == null) {
      throw PlatformException(
        code: 'channel-error',
        message: 'Unable to establish connection on channel.',
      );
    } else if (replyList.length > 1) {
      throw PlatformException(
        code: replyList[0]! as String,
        message: replyList[1] as String?,
        details: replyList[2],
      );
    } else if (replyList[0] == null) {
      throw PlatformException(
        code: 'null-error',
        message: 'Host platform returned null value for non-null return value.',
      );
    } else {
      return (replyList[0] as TrackMessage?)!;
    }
  }
}
