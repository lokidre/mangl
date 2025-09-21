# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-12-01

# Source Group: Mangl/include
include_directories(${CONFIG_MANGL_DIR}/include)
set(src
  ${CONFIG_MANGL_DIR}/include/mangl/ads.h
  ${CONFIG_MANGL_DIR}/include/mangl/analytics.h
  ${CONFIG_MANGL_DIR}/include/mangl/android.h
  ${CONFIG_MANGL_DIR}/include/mangl/animation.h
  ${CONFIG_MANGL_DIR}/include/mangl/app.h
  ${CONFIG_MANGL_DIR}/include/mangl/audio.h
  ${CONFIG_MANGL_DIR}/include/mangl/background.h
  ${CONFIG_MANGL_DIR}/include/mangl/blueprint.h
  ${CONFIG_MANGL_DIR}/include/mangl/capture.h
  ${CONFIG_MANGL_DIR}/include/mangl/debug.h
  ${CONFIG_MANGL_DIR}/include/mangl/design.h
  ${CONFIG_MANGL_DIR}/include/mangl/dispatch.h
  ${CONFIG_MANGL_DIR}/include/mangl/effects.h
  ${CONFIG_MANGL_DIR}/include/mangl/environment.h
  ${CONFIG_MANGL_DIR}/include/mangl/error.h
  ${CONFIG_MANGL_DIR}/include/mangl/file.h
  ${CONFIG_MANGL_DIR}/include/mangl/font.h
  ${CONFIG_MANGL_DIR}/include/mangl/framework.h
  ${CONFIG_MANGL_DIR}/include/mangl/game.h
  ${CONFIG_MANGL_DIR}/include/mangl/iap.h
  ${CONFIG_MANGL_DIR}/include/mangl/ios.h
  ${CONFIG_MANGL_DIR}/include/mangl/layout.h
  ${CONFIG_MANGL_DIR}/include/mangl/main.h
  ${CONFIG_MANGL_DIR}/include/mangl/main_window.h
  ${CONFIG_MANGL_DIR}/include/mangl/mangl.h
  ${CONFIG_MANGL_DIR}/include/mangl/memory_info.h
  ${CONFIG_MANGL_DIR}/include/mangl/multiplayer.h
  ${CONFIG_MANGL_DIR}/include/mangl/multiplayer_packet.h
  ${CONFIG_MANGL_DIR}/include/mangl/native.h
  ${CONFIG_MANGL_DIR}/include/mangl/network.h
  ${CONFIG_MANGL_DIR}/include/mangl/packet.h
  ${CONFIG_MANGL_DIR}/include/mangl/parse.h
  ${CONFIG_MANGL_DIR}/include/mangl/particles.h
  ${CONFIG_MANGL_DIR}/include/mangl/pchheaders.h
  ${CONFIG_MANGL_DIR}/include/mangl/resources.h
  ${CONFIG_MANGL_DIR}/include/mangl/scene.h
  ${CONFIG_MANGL_DIR}/include/mangl/screen.h
  ${CONFIG_MANGL_DIR}/include/mangl/social.h
  ${CONFIG_MANGL_DIR}/include/mangl/storage.h
  ${CONFIG_MANGL_DIR}/include/mangl/style.h
  ${CONFIG_MANGL_DIR}/include/mangl/texture.h
  ${CONFIG_MANGL_DIR}/include/mangl/theme.h
  ${CONFIG_MANGL_DIR}/include/mangl/timer.h
  ${CONFIG_MANGL_DIR}/include/mangl/types.h
  ${CONFIG_MANGL_DIR}/include/mangl/util.h
  ${CONFIG_MANGL_DIR}/include/mangl/widgets.h
)
source_group(TREE ${CONFIG_MANGL_DIR}/include PREFIX "Sources\\Mangl/include" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core
set(src
  ${CONFIG_MANGL_DIR}/src/core/mangl_ads.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_ads.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_analytics.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_color.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_color_names.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_common.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_config.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_config.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_core.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_debug.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_debug.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_dispatch.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_dispatch.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_environment.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_error.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_file.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_font.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_font.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_format.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_framework.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_keyboard.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_keyboard.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_local_log.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_matrix.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_multiplayer.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_network.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_packet.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_parse.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_parse.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_prim.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_prim_params.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_random.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_random.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_render.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_resources.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_resources.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_scene.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_scene_decl.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_schedule_trigger.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_schedule_trigger.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_setup.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_storage.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_styles.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_texture.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_texture_atlas.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_time.cpp
  ${CONFIG_MANGL_DIR}/src/core/mangl_time.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_touches.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_transition.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_types.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_util.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_widgets.h
  ${CONFIG_MANGL_DIR}/src/core/mangl_widgets_decl.h
)
source_group(TREE ${CONFIG_MANGL_DIR}/src/core PREFIX "Sources\\Mangl/core" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core/app
set(src
  ${CONFIG_MANGL_DIR}/src/core/app/mangl_app.h
  ${CONFIG_MANGL_DIR}/src/core/app/mangl_app_params.h
)
source_group(TREE ${CONFIG_MANGL_DIR}/src/core/app PREFIX "Sources\\Mangl/core/app" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core/main
set(src
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_environment.cpp
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_error.cpp
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_main.cpp
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_main.h
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_resources.cpp
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_thread.cpp
  ${CONFIG_MANGL_DIR}/src/core/main/mangl_thread.h
)
source_group(TREE ${CONFIG_MANGL_DIR}/src/core/main PREFIX "Sources\\Mangl/core/main" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core/framework
set(src
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_framework.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_framework_screens.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_framework_static.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_framework_stats.h
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_framework_timer.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_framework_touches.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_runloop_timer.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_runloop_timer.h
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_screen_base.h
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_screen_transition.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_screen_transition.h
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_timer_handler.cpp
  ${CONFIG_MANGL_DIR}/src/core/framework/mangl_timer_handler.h
)
source_group(TREE ${CONFIG_MANGL_DIR}/src/core/framework PREFIX "Sources\\Mangl/core/framework" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core/storage
set(src
  ${CONFIG_MANGL_DIR}/src/core/storage/mangl_sqlite.cpp
  ${CONFIG_MANGL_DIR}/src/core/storage/mangl_sqlite.h
)
source_group(TREE ${CONFIG_MANGL_DIR}/src/core/storage PREFIX "Sources\\Mangl/core/storage" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core/packet
set(src
  ${CONFIG_MANGL_DIR}/src/core/packet/mangl_packet_load.cpp
  ${CONFIG_MANGL_DIR}/src/core/packet/mangl_packet_pack.cpp
)
source_group(TREE ${CONFIG_MANGL_DIR}/src/core/packet PREFIX "Sources\\Mangl/core/packet" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/core/multiplayer
if (CONFIG_PROJECT_MULTIPLAYER)
  set(src
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_base.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_base.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_base_debug.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_base.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_connection.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_packet.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_packet_recv.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_packet_send.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_status.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_client_timer.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_common.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_game_instance.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_game_instance_base.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_match.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_match_container.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_match_stage.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet_character.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet_element.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet_field.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet_player.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet_proto.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_packet_queue.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_params_game.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_params_player.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_peer.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_player.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_player_container.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_base.h
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_connection.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_debug.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_error.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_game.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_match.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_npc.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_packet.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_packet_recv.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_packet_recv_game.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_packet_recv_match.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_packet_recv_system.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_packet_send.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_timer.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_timer_game.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_server_timer_match.cpp
    ${CONFIG_MANGL_DIR}/src/core/multiplayer/mangl_mp_types.h
  )
  source_group(TREE ${CONFIG_MANGL_DIR}/src/core/multiplayer PREFIX "Sources\\Mangl/core/multiplayer" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/core/network
if (CONFIG_PROJECT_MULTIPLAYER)
  set(src
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_ixwebsocket_client.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_ixwebsocket_client.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_ixwebsocket_server.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_ixwebsocket_server.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_base.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_base.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery_client.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery_client.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery_info.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery_server.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_discovery_server.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_event.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_event_queue.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_matchmaker_base.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_packet.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_params.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_peer.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_peer_container.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_network_queue.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_uwebsockets_client.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_uwebsockets_client.h
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_uwebsockets_server.cpp
    ${CONFIG_MANGL_DIR}/src/core/network/mangl_uwebsockets_server.h
  )
  source_group(TREE ${CONFIG_MANGL_DIR}/src/core/network PREFIX "Sources\\Mangl/core/network" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/3rdparty/sqlite
include_directories(${CONFIG_MANGL_3RDPARTY_DIR})
set(src
  ${CONFIG_MANGL_3RDPARTY_DIR}/sqlite/sqlite3.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/sqlite/sqlite3.h
)
source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/sqlite PREFIX "Sources\\Mangl/3rdparty/sqlite" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/macos
if (APPLE AND NOT IOS)
  set(src
    ${CONFIG_MANGL_DIR}/src/macos/mangl_environment_mac.mm
  )
  source_group(TREE ${CONFIG_MANGL_DIR}/src/macos PREFIX "Sources\\Mangl/macos" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/linux
if (LINUX)
  set(src
    ${CONFIG_MANGL_DIR}/src/linux/mangl_linux_bt_client.cpp
    ${CONFIG_MANGL_DIR}/src/linux/mangl_linux_bt_client.h
    ${CONFIG_MANGL_DIR}/src/linux/mangl_linux_bt_context.h
    ${CONFIG_MANGL_DIR}/src/linux/mangl_linux_bt_server.cpp
    ${CONFIG_MANGL_DIR}/src/linux/mangl_linux_bt_server.h
    ${CONFIG_MANGL_DIR}/src/linux/mangl_linux_env.cpp
  )
  source_group(TREE ${CONFIG_MANGL_DIR}/src/linux PREFIX "Sources\\Mangl/linux" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/windows
if (WIN32)
  set(src
    ${CONFIG_MANGL_DIR}/src/windows/mangl_win_env.cpp
  )
  source_group(TREE ${CONFIG_MANGL_DIR}/src/windows PREFIX "Sources\\Mangl/windows" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/3rdparty/ixwebsocket
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_IXWEBSOCKET)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXBase64.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXBench.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXBench.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXCancellationRequest.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXCancellationRequest.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXConnectionState.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXConnectionState.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXDNSLookup.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXDNSLookup.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXExponentialBackoff.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXExponentialBackoff.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXGetFreePort.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXGetFreePort.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXGzipCodec.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXGzipCodec.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXHttp.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXHttp.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXHttpClient.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXHttpClient.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXHttpServer.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXHttpServer.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXNetSystem.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXNetSystem.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXProgressCallback.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterrupt.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterrupt.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterruptEvent.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterruptEvent.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterruptFactory.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterruptFactory.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterruptPipe.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSelectInterruptPipe.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSetThreadName.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSetThreadName.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocket.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocket.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketAppleSSL.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketAppleSSL.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketConnect.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketConnect.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketFactory.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketFactory.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketMbedTLS.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketMbedTLS.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketOpenSSL.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketOpenSSL.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketServer.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketServer.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketTLSOptions.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXSocketTLSOptions.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXStrCaseCompare.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXStrCaseCompare.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUdpSocket.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUdpSocket.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUniquePtr.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUrlParser.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUrlParser.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUserAgent.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUserAgent.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUtf8Validator.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUuid.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXUuid.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocket.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocket.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketCloseConstants.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketCloseConstants.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketCloseInfo.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketErrorInfo.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketHandshake.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketHandshake.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketHandshakeKeyGen.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketHttpHeaders.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketHttpHeaders.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketInitResult.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketMessage.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketMessageType.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketOpenInfo.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketPerMessageDeflate.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketPerMessageDeflate.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketPerMessageDeflateCodec.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketPerMessageDeflateCodec.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketPerMessageDeflateOptions.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketPerMessageDeflateOptions.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketProxyServer.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketProxyServer.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketSendData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketSendInfo.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketServer.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketServer.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketTransport.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketTransport.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket/IXWebSocketVersion.h
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/ixwebsocket PREFIX "Sources\\Mangl/3rdparty/ixwebsocket" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/mbedtls
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_IXWEBSOCKET)
  include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/aes.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/aesce.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/aesce.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/aesni.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/aesni.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/alignment.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/aria.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/asn1parse.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/asn1write.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/base64.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_core.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_core.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_mod.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_mod.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_mod_raw.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_mod_raw.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bignum_mod_raw_invasive.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/bn_mul.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/camellia.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ccm.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/chacha20.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/chachapoly.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/check_crypto_config.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/cipher.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/cipher_wrap.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/cipher_wrap.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/cmac.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/common.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/constant_time.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/constant_time_internal.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/constant_time_invasive.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ctr_drbg.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/debug.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/des.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/dhm.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecdh.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecdsa.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecjpake.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecp.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecp_curves.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecp_internal_alt.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ecp_invasive.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/entropy.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/entropy_poll.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/entropy_poll.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/error.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/gcm.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/hash_info.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/hash_info.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/hkdf.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/hmac_drbg.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/lmots.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/lmots.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/lms.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/md.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/md5.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/md_wrap.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/memory_buffer_alloc.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/mps_common.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/mps_error.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/mps_reader.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/mps_reader.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/mps_trace.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/mps_trace.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/net_sockets.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/nist_kw.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/oid.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/padlock.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/padlock.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pem.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pk.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pk_wrap.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pk_wrap.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pkcs12.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pkcs5.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pkcs7.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pkparse.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pkwrite.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/pkwrite.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/platform.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/platform_util.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/poly1305.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_aead.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_aead.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_cipher.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_cipher.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_client.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_core.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_driver_wrappers.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_driver_wrappers.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_ecp.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_ecp.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_hash.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_hash.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_invasive.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_its.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_mac.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_mac.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_pake.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_random_impl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_rsa.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_rsa.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_se.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_se.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_slot_management.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_slot_management.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_storage.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_crypto_storage.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/psa_its_file.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ripemd160.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/rsa.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/rsa_alt_helpers.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/rsa_alt_helpers.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/sha1.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/sha256.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/sha512.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_cache.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_ciphersuites.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_client.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_client.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_cookie.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_debug_helpers.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_debug_helpers_generated.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_misc.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_msg.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_ticket.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls12_client.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls12_server.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls13_client.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls13_generic.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls13_invasive.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls13_keys.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls13_keys.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/ssl_tls13_server.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/threading.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/timing.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/version.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/version_features.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509_create.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509_crl.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509_crt.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509_csr.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509write_crt.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/library/x509write_csr.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/aes.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/aria.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/asn1.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/asn1write.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/base64.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/bignum.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/build_info.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/camellia.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ccm.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/chacha20.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/chachapoly.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/check_config.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/cipher.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/cmac.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/compat-2.x.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/config_psa.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/constant_time.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ctr_drbg.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/debug.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/des.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/dhm.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ecdh.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ecdsa.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ecjpake.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ecp.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/entropy.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/error.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/gcm.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/hkdf.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/hmac_drbg.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/legacy_or_psa.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/lms.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/mbedtls_config.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/md.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/md5.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/memory_buffer_alloc.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/net_sockets.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/nist_kw.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/oid.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/pem.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/pk.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/pkcs12.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/pkcs5.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/pkcs7.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/platform.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/platform_time.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/platform_util.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/poly1305.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/private_access.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/psa_util.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ripemd160.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/rsa.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/sha1.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/sha256.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/sha512.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ssl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ssl_cache.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ssl_ciphersuites.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ssl_cookie.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/ssl_ticket.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/threading.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/timing.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/version.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/x509.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/x509_crl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/x509_crt.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/mbedtls/x509_csr.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_builtin_composites.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_builtin_primitives.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_compat.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_config.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_driver_common.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_driver_contexts_composites.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_driver_contexts_primitives.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_extra.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_platform.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_se_driver.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_sizes.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_struct.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_types.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls/psa/crypto_values.h
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/mbedtls PREFIX "Sources\\Mangl/3rdparty/mbedtls" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/uwebsockets
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_UWEBSOCKETS)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/App.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/AsyncSocket.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/AsyncSocketData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/BloomFilter.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/ChunkedEncoding.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/ClientApp.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Http3App.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Http3Context.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Http3ContextData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Http3Request.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Http3Response.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Http3ResponseData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpContext.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpContextData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpErrors.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpParser.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpResponse.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpResponseData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/HttpRouter.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/LocalCluster.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Loop.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/LoopData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/MessageParser.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/MoveOnlyFunction.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Multipart.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/PerMessageDeflate.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/ProxyParser.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/QueryParser.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/TopicTree.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/Utilities.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocket.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocketContext.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocketContextData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocketData.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocketExtensions.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocketHandshake.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets/WebSocketProtocol.h
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/uwebsockets PREFIX "Sources\\Mangl/3rdparty/uwebsockets" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/usockets
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_UWEBSOCKETS)
  add_compile_definitions(LIBUS_NO_SSL)
  include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/usockets)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/bsd.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/context.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/crypto/openssl.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/crypto/sni_tree.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/eventing/asio.cpp
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/eventing/epoll_kqueue.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/eventing/gcd.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/eventing/libuv.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/eventing/asio.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/eventing/epoll_kqueue.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/eventing/gcd.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/eventing/libuv.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/internal.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/loop_data.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/internal/networking/bsd.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/io_uring/internal.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/io_uring/io_context.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/io_uring/io_loop.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/io_uring/io_socket.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/libusockets.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/loop.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/quic.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/quic.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/socket.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/usockets/udp.c
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/usockets PREFIX "Sources\\Mangl/3rdparty/usockets" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/libuv/include
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_UWEBSOCKETS)
  include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/aix.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/bsd.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/darwin.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/errno.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/linux.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/os390.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/posix.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/sunos.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/threadpool.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/tree.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/unix.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/version.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv/win.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include/uv.h
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/include PREFIX "Sources\\Mangl/3rdparty/libuv/include" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/libuv/src
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_UWEBSOCKETS)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/fs-poll.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/heap-inl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/idna.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/idna.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/inet.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/queue.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/random.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/strscpy.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/strscpy.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/strtok.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/strtok.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/thread-common.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/threadpool.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/timer.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/uv-common.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/uv-common.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/uv-data-getter-setters.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src/version.c
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/src PREFIX "Sources\\Mangl/3rdparty/libuv/src" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/libuv/win
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_UWEBSOCKETS AND WIN32)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/async.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/atomicops-inl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/core.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/detect-wakeup.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/dl.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/error.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/fs-event.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/fs-fd-hash-inl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/fs.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/getaddrinfo.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/getnameinfo.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/handle-inl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/handle.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/internal.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/loop-watcher.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/pipe.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/poll.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/process-stdio.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/process.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/req-inl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/signal.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/snprintf.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/stream-inl.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/stream.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/tcp.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/thread.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/tty.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/udp.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/util.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/winapi.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/winapi.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/winsock.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win/winsock.h
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/win PREFIX "Sources\\Mangl/3rdparty/libuv/win" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

# Source Group: Mangl/3rdparty/libuv/unix
if (CONFIG_PROJECT_MULTIPLAYER AND CONFIG_MANGL_UWEBSOCKETS AND UNIX)
  add_compile_definitions(_GNU_SOURCE)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/async.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/core.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/fs.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/internal.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/linux.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/loop-watcher.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/loop.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/pipe.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/poll.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/process.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/procfs-exepath.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/proctitle.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/random-devurandom.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/random-getrandom.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/random-sysctl-linux.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/signal.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/stream.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/tcp.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/thread.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/tty.c
    ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix/udp.c
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/libuv/unix PREFIX "Sources\\Mangl/3rdparty/libuv/unix" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

