// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// ignore_for_file: public_member_api_docs
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'page.dart';

class MapIdPage extends GoogleMapExampleAppPage {
  const MapIdPage({Key? key})
      : super(const Icon(Icons.map), 'Cloud-based maps styling', key: key);

  @override
  Widget build(BuildContext context) {
    return const MapIdBody();
  }
}

class MapIdBody extends StatefulWidget {
  const MapIdBody({super.key});

  @override
  State<StatefulWidget> createState() => MapIdBodyState();
}

const LatLng _kMapCenter = LatLng(52.4478, -3.5402);

class MapIdBodyState extends State<MapIdBody> {
  GoogleMapController? controller;

  Key _key = const Key('mapId#');
  String? _mapId;
  final TextEditingController _mapIdController = TextEditingController();

  @override
  void initState() {
    super.initState();
  }

  void _setMapId() {
    setState(() {
      _mapId = _mapIdController.text;

      // Change key to initialize new map instance for new mapId.
      _key = Key(_mapId ?? 'mapId#');
    });
  }

  @override
  Widget build(BuildContext context) {
    final GoogleMap googleMap = GoogleMap(
      onMapCreated: _onMapCreated,
      initialCameraPosition: const CameraPosition(
        target: _kMapCenter,
        zoom: 7.0,
      ),
      key: _key,
      cloudMapId: _mapId,
    );

    final List<Widget> columnChildren = <Widget>[
      Padding(
        padding: const EdgeInsets.all(10.0),
        child: Center(
          child: SizedBox(width: 300.0, height: 200.0, child: googleMap),
        ),
      ),
      Padding(
        padding: const EdgeInsets.all(10.0),
        child: TextField(
          controller: _mapIdController,
          decoration: const InputDecoration(hintText: 'Map Id'),
        ),
      ),
      Padding(
        padding: const EdgeInsets.all(10.0),
        child: ElevatedButton(
          onPressed: () => _setMapId(),
          child: const Text('Press to use specified map Id'),
        ),
      ),
    ];

    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      children: columnChildren,
    );
  }

  @override
  void dispose() {
    _mapIdController.dispose();
    super.dispose();
  }

  void _onMapCreated(GoogleMapController controllerParam) {
    setState(() {
      controller = controllerParam;
    });
  }
}
