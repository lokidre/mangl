# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-12-01

# Source Group: Mangl/core
set(src
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation.h
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_banners.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_callbacks.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_data.h
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_interstitials.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_requests.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_setup.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ad_mediation_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_client.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_client.h
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_database.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_http.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_scene.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_server.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_thread.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/ads/mangl_ads_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/animation/mangl_animation.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/animation/mangl_animation.h
  ${CONFIG_MANGL_SRC_DIR}/core/animation/mangl_animation_eval.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/animation/mangl_animation_exec.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/animation/mangl_animation_parse.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/animation/mangl_animation_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/app/mangl_app.h
  ${CONFIG_MANGL_SRC_DIR}/core/app/mangl_app_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_audio_file.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_audio_file.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_audio_system.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_audio_system.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_audio_system_queue.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_game_music.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_game_music.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_game_music_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_music.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_music.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_music_dummy.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_sound.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_sound.h
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_sound_openal.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/audio/mangl_sound_openal.h
  ${CONFIG_MANGL_SRC_DIR}/core/background/mangl_background.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/background/mangl_background.h
  ${CONFIG_MANGL_SRC_DIR}/core/background/mangl_background_appear.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/background/mangl_background_parse.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/background/mangl_background_start.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/background/mangl_background_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/animation/mangl_bp_animation.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/animation/mangl_bp_keyframe.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/engine/mangl_bp_engine.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/loader/mangl_bp_load.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/loader/mangl_bp_loader.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_create.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_debug.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_eval.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_eval_value.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_load.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_loader.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_loader.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parse_color.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parse_coord.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parse_imports.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parse_node.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parse_texture.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parse_vars.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_parser.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_runtime.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_structure.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/mangl_blueprint_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/objects_parse.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/objects_parse.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/parser/mangl_bp_parser.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_component.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_container.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_coord.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_layer.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_layout.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_project.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_props.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_viewport.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/project/mangl_bp_world.h
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/screen_parse.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/blueprint/screen_parse.h
  ${CONFIG_MANGL_SRC_DIR}/core/console/mangl_console_main.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/debug/mang_debug_layout.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/debug/mangl_debug.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/design/mangl_design_grid.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/design/mangl_design_grid.h
  ${CONFIG_MANGL_SRC_DIR}/core/design/mangl_design_grid_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_blink_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_blink_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_chain_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_chain_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_crossfade_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_crossfade_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_crossfade_effect_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_custom_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_custom_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_disappear_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_disappear_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_disappear_effect_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_easing.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_fade_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_fade_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_fade_effect_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_frames_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_frames_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_game_score_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_game_score_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_move_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_move_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_move_effect_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_rotate_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_rotate_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_rotate_effect_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_scale_effect.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_scale_effect.h
  ${CONFIG_MANGL_SRC_DIR}/core/effects/mangl_scale_effect_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_framework.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_framework_screens.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_framework_static.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_framework_stats.h
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_framework_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_framework_touches.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_runloop_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_runloop_timer.h
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_screen_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_screen_transition.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_screen_transition.h
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_timer_handler.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/framework/mangl_timer_handler.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_game_screen.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_game_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_game_screen_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_game_screen_popup.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_game_screen_style.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_game_screen_types.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_leaderboard.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_leaderboard.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_background.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_background.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_background_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_background_style.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_buttons.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_buttons_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_close_button.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_close_button.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_grid.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_grid.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_title.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_title.h
  ${CONFIG_MANGL_SRC_DIR}/core/game/mangl_screen_title_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/gui/mangl_game_button.h
  ${CONFIG_MANGL_SRC_DIR}/core/gui/mangl_screen_button.h
  ${CONFIG_MANGL_SRC_DIR}/core/gui/mangl_screen_title.h
  ${CONFIG_MANGL_SRC_DIR}/core/gui/mangl_transitions.h
  ${CONFIG_MANGL_SRC_DIR}/core/layout/mangl_layout.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/layout/mangl_layout.h
  ${CONFIG_MANGL_SRC_DIR}/core/layout/mangl_layout_eval.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/layout/mangl_layout_load.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/layout/mangl_layout_parser.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/layout/mangl_layout_parser.h
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_environment.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_error.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_main.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_main.h
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_resources.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_thread.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/main/mangl_thread.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_ads.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_ads.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_analytics.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_color.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_color_names.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_common.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_config.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_config.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_core.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_debug.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_debug.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_dispatch.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_dispatch.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_environment.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_error.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_file.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_font.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_font.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_format.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_framework.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_keyboard.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_keyboard.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_local_log.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_matrix.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_multiplayer.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_network.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_packet.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_parse.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_parse.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_prim.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_prim_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_random.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_random.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_render.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_resources.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_resources.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_scene.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_scene_decl.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_schedule_trigger.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_schedule_trigger.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_setup.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_storage.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_styles.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_texture.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_texture_atlas.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_time.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_time.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_touches.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_transition.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_types.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_util.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_widgets.h
  ${CONFIG_MANGL_SRC_DIR}/core/mangl_widgets_decl.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_base.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_base_debug.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_connection.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_packet.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_packet_recv.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_packet_send.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_status.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_client_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_common.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_game_instance.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_game_instance_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_match.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_match_container.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_match_stage.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet_character.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet_element.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet_field.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet_player.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet_proto.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_packet_queue.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_params_game.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_params_player.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_peer.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_player.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_player_container.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_connection.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_debug.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_error.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_game.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_match.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_npc.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_packet.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_packet_recv.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_packet_recv_game.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_packet_recv_match.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_packet_recv_system.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_packet_send.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_timer_game.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_server_timer_match.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/multiplayer/mangl_mp_types.h
  ${CONFIG_MANGL_SRC_DIR}/core/native/mangl_appstore.h
  ${CONFIG_MANGL_SRC_DIR}/core/native/mangl_iap.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/native/mangl_iap.h
  ${CONFIG_MANGL_SRC_DIR}/core/native/mangl_native_gui.h
  ${CONFIG_MANGL_SRC_DIR}/core/native/mangl_native_settings.h
  ${CONFIG_MANGL_SRC_DIR}/core/native/mangl_web.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_ixwebsocket_client.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_ixwebsocket_client.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_ixwebsocket_server.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_ixwebsocket_server.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_base.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery_client.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery_client.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery_info.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery_server.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_discovery_server.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_event.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_event_queue.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_matchmaker_base.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_packet.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_peer.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_peer_container.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_network_queue.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_uwebsockets_client.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_uwebsockets_client.h
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_uwebsockets_server.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/network/mangl_uwebsockets_server.h
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl.h
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_scene.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_screenshot.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_shader.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_shader.h
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_shader_decl.h
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_shader_sources.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_shader_sources.h
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_static.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_textures.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/opengl/mangl_opengl_viewport.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/packet/mangl_packet_load.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/packet/mangl_packet_pack.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/particles/mangl_particles.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/particles/mangl_particles.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_bezier.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_box.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_circle.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_line.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_model.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_particles.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_point.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_rect.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_tex.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pipeline_tri.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_buffers.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_circle.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_color.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_depth.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_draw.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_fog.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_global.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_global.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_header.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_light.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_line.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_pack.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_projection.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_rect.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_scissor.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_pl_util_tex.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_render_pipeline.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_render_vertex_pack.h
  ${CONFIG_MANGL_SRC_DIR}/core/render/mangl_renderer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/scene/mangl_model.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/scene/mangl_model.h
  ${CONFIG_MANGL_SRC_DIR}/core/scene/mangl_model_obj.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/scene/mangl_scene.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/scene/mangl_scene_timer.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/scene/mangl_scene_touches.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_app_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_endgame_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_game_play_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_high_scores_screen.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_high_scores_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_intro_screen.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_intro_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_invite_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_settings_screen.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_settings_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_title_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_tutorial_screen.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/screens/mangl_tutorial_screen.h
  ${CONFIG_MANGL_SRC_DIR}/core/social/mangl_game_center.h
  ${CONFIG_MANGL_SRC_DIR}/core/social/mangl_invites.h
  ${CONFIG_MANGL_SRC_DIR}/core/storage/mangl_sqlite.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/storage/mangl_sqlite.h
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_border.h
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_button.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_button.h
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_colors.h
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_load.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_palette.h
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_save.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_separator.h
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_static.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/styles/mangl_styles_text.h
  ${CONFIG_MANGL_SRC_DIR}/core/textures/mangl_render_texture_png.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/textures/mangl_render_textures.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/textures/mangl_texture_atlas.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/textures/mangl_texture_load.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/textures/mangl_texture_pool.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/textures/mangl_texture_pool.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_background_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_background_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_background_widget_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_button.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_button.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_button_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_button_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_button_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_button_widget_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_checkbox.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_checkbox.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_checkbox_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_dialog.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_dialog.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_dialog_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_gradient_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_gradient_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_grid_layout.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_grid_layout.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_grid_layout_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_image.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_image.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_label.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_label.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_label_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_label_styles.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_mesh_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_mesh_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_scroll.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_scroll.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_scroll_handler.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_scroll_handler.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_segment_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_segment_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_segment_widget_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_segmented.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_segmented.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_segmented_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_slider.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_slider.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_slider_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_stack_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_stack_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_stack_widget_params.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_table_styles.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_table_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_table_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_widget.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_widget.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_widget_border.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_widget_border.h
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_widget_layout.cpp
  ${CONFIG_MANGL_SRC_DIR}/core/widgets/mangl_widget_layout.h
)
source_group(TREE ${CONFIG_MANGL_SRC_DIR}/core PREFIX "Sources\\Mangl/core" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/include
include_directories(${CONFIG_MANGL_INCLUDE_DIR})
set(src
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/ads.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/analytics.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/android.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/animation.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/app.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/audio.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/background.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/blueprint.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/capture.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/debug.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/design.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/dispatch.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/effects.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/environment.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/error.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/file.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/font.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/framework.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/game.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/iap.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/ios.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/layout.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/main.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/main_window.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/mangl.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/memory_info.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/multiplayer.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/multiplayer_packet.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/native.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/network.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/packet.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/parse.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/particles.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/pchheaders.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/resources.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/scene.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/screen.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/social.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/storage.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/style.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/texture.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/theme.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/timer.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/types.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/util.h
  ${CONFIG_MANGL_INCLUDE_DIR}/mangl/widgets.h
)
source_group(TREE ${CONFIG_MANGL_INCLUDE_DIR} PREFIX "Sources\\Mangl/include" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Mangl/3rdparty/sqlite
include_directories(${CONFIG_MANGL_3RDPARTY_DIR})
set(src
  ${CONFIG_MANGL_3RDPARTY_DIR}/sqlite/sqlite3.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/sqlite/sqlite3.h
)
source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/sqlite PREFIX "Sources\\Mangl/3rdparty/sqlite" FILES ${src})
list(APPEND main_SRC ${src})

if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# Source Group: Mangl/3rdparty/flac
add_compile_definitions(FLAC__NO_DLL)
add_compile_definitions(FLAC__HAS_OGG)
include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/flac)
include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include)
include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/include)
set(src
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_all.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_assert.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_callback.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_export.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_format.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_metadata.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_ordinals.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_stream_decoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC/flac_stream_encoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC++/flac_all.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC++/flac_decoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC++/flac_encoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC++/flac_export.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/FLAC++/flac_metadata.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_bitmath.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_bitreader.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_bitwriter.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_cpu.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_crc.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_fixed.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_fixed_intrin_avx2.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_fixed_intrin_sse2.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_fixed_intrin_sse42.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_fixed_intrin_ssse3.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_float.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_format.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_lpc.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_lpc_intrin_avx2.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_lpc_intrin_fma.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_lpc_intrin_neon.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_lpc_intrin_sse2.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_lpc_intrin_sse41.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_md5.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_memory.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_metadata_iterators.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_metadata_object.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_ogg_decoder_aspect.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_ogg_encoder_aspect.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_ogg_helper.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_ogg_mapping.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_stream_decoder.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_stream_encoder.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_stream_encoder_framing.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_stream_encoder_intrin_avx2.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_stream_encoder_intrin_sse2.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_stream_encoder_intrin_ssse3.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/flac_window.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_all.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_bitmath.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_bitreader.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_bitwriter.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_cpu.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_crc.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_fixed.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_float.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_format.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_lpc.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_macros.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_md5.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_memory.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_metadata.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_ogg_decoder_aspect.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_ogg_encoder_aspect.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_ogg_helper.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_ogg_mapping.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_stream_encoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_stream_encoder_framing.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/private/flac_window.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/protected/flac_all.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/protected/flac_stream_decoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC/include/protected/flac_stream_encoder.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC++/flac_metadata.cpp
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC++/flac_stream_decoder.cpp
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/libFLAC++/flac_stream_encoder.cpp
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/include/ogg/ogg_config_types.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/include/ogg/ogg_ogg.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/include/ogg/ogg_os_types.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/src/ogg_bitwise.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/src/ogg_crctable.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/ogg/src/ogg_framing.c
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_alloc.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_compat.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_endswap.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_getopt.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_grabbag.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_macros.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_private.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_replaygain_analysis.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_replaygain_synthesis.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_safe_str.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_utf8.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/flac_win_utf8_io.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/grabbag/flac_cuesheet.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/grabbag/flac_file.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/grabbag/flac_picture.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/grabbag/flac_replaygain.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/grabbag/flac_seektable.h
  ${CONFIG_MANGL_3RDPARTY_DIR}/flac/share/win_utf8_io/flac_win_utf8_io.c
)
source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/flac PREFIX "Sources\\Mangl/3rdparty/flac" FILES ${src})
list(APPEND main_SRC ${src})

if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# Source Group: Mangl/ios
if (IOS)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/admob/mangl_ad_adaptor_admob.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/admob/mangl_ad_adaptor_admob.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/admob/mangl_ad_network_admob.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/admob/mangl_ad_network_admob.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/applovin/mangl_ad_adaptor_applovin.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/applovin/mangl_ad_adaptor_applovin.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/applovin/mangl_ad_network_applovin.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/applovin/mangl_ad_network_applovin.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/facebook/mangl_ad_adaptor_facebook.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/facebook/mangl_ad_adaptor_facebook.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/facebook/mangl_ad_network_facebook.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/facebook/mangl_ad_network_facebook.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mangl_ad_events.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mangl_ad_events.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mangl_ad_network.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mangl_ad_network.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mangl_ad_network_base.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mangl_ad_network_base.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mediation/mangl_ad_mediation_adaptor_base.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mediation/mangl_ad_mediation_adaptor_base.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mediation/mangl_ad_network_mediation.h
    ${CONFIG_MANGL_SRC_DIR}/ios/ads/mediation/mangl_ad_network_mediation.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_analytics.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_audio_session_ios.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_environment_ios.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_google_signin.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_google_signin.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_gui.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_gui.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_hooks_ios.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_hooks_ios.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_invite.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_invite.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_ios_static.cpp
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_ios_static.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_app.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_app.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+ads.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+ads.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+except.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+except.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+timer.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+timer.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+touches.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc+touches.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_vc_decl.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_window.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_main_window.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_native_util.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_scene_view.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_scene_view_metal.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_scene_view_metal.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_scene_view_opengl.h
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_scene_view_opengl.mm
    ${CONFIG_MANGL_SRC_DIR}/ios/mangl_textures.mm
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/ios PREFIX "Sources\\Mangl/ios" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/apple
if (APPLE AND NOT CONFIG_TARGET_CONSOLE)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/apple/dummy.swift
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_apple_bluetooth.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_apple_bluetooth.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_apple_gamekit.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_apple_nearby.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_audio_system_platform.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_dispatch_gcd.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_game_center.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_game_center.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_gamekit.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_gamekit.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_imacos_util.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_imacos_util.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_memory_info.cpp
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_metal_render_view.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_metal_render_view.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_metal_shader.metal
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_metal_types.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_music_audioqueue.cpp
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_music_audioqueue.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_music_audiounit.cpp
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_music_audiounit.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_nearby_mc_base.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_nearby_mc_base.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_nearby_mc_client.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_nearby_mc_client.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_nearby_mc_server.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_nearby_mc_server.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_rate_annoyer.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_rate_annoyer.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_reachability.h
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_reachability.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_settings.mm
    ${CONFIG_MANGL_SRC_DIR}/apple/mangl_sk_iap.mm
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/apple PREFIX "Sources\\Mangl/apple" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/macos
if (MACOS AND NOT CONFIG_TARGET_CONSOLE)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/macos/mangl_app_delegate.h
    ${CONFIG_MANGL_SRC_DIR}/macos/mangl_gui.h
    ${CONFIG_MANGL_SRC_DIR}/macos/mangl_main_vc.h
    ${CONFIG_MANGL_SRC_DIR}/macos/mangl_main_view.h
    ${CONFIG_MANGL_SRC_DIR}/macos/mangl_main_window.h
    ${CONFIG_MANGL_SRC_DIR}/macos/mangl_rate_annoyer.h
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/macos PREFIX "Sources\\Mangl/macos" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/android
if (ANDROID)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/CPUTracer.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/CPUTracer.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/ChoreographerFilter.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/ChoreographerFilter.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/ChoreographerThread.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/ChoreographerThread.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/CpuInfo.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/CpuInfo.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/FrameStatistics.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/FrameStatistics.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/Settings.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/Settings.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/SwappyCommon.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/SwappyCommon.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/SwappyDisplayManager.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/SwappyDisplayManager.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/SwappyLog.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/Thread.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/Thread.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/common/swappy_c.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/include/common/gamesdk_common.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/include/swappy/swappyGL.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/include/swappy/swappyGL_extra.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/include/swappy/swappyVk.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/include/swappy/swappy_common.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/EGL.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/EGL.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/FrameStatisticsGL.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/FrameStatisticsGL.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/SwappyGL.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/SwappyGL.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/games-frame-pacing/opengl/swappyGL_c.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/ChoreographerShim.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/JNIUtil.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/Log.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/StringShim.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/Trace.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/apk_utils.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/apk_utils.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/jni/jni_helper.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/jni/jni_helper.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/jni/jni_wrap.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/jni/jni_wrap.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/jni/jnictx.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/jni/jnictx.h
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/system_utils.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/3rdparty/gamesdk/common/system_utils.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_assets.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_assets.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_common.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_debug.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_env.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_ads.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_bluetooth.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_bluetooth.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_callbacks.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_device.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_gui.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_invites.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_music.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_music.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_utils.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_jni_utils.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_keyboard.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_keyboard.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_log.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main_engine.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main_engine.h
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main_events.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main_inputs.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main_loop.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_main_opengl.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_memory_info.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_opensl.cpp
    ${CONFIG_MANGL_SRC_DIR}/android/mangl_and_opensl.h
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/android PREFIX "Sources\\Mangl/android" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/linux
if (LINUX)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/linux/mangl_linux_bt_client.cpp
    ${CONFIG_MANGL_SRC_DIR}/linux/mangl_linux_bt_client.h
    ${CONFIG_MANGL_SRC_DIR}/linux/mangl_linux_bt_context.h
    ${CONFIG_MANGL_SRC_DIR}/linux/mangl_linux_bt_server.cpp
    ${CONFIG_MANGL_SRC_DIR}/linux/mangl_linux_bt_server.h
    ${CONFIG_MANGL_SRC_DIR}/linux/mangl_linux_env.cpp
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/linux PREFIX "Sources\\Mangl/linux" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/gtk
if (CONFIG_MANGL_GTK)
  add_compile_definitions(MANGL_GTK=1)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK REQUIRED gtk4)
  include_directories(${GTK_INCLUDE_DIRS})
  link_directories(${GTK_LIBRARY_DIRS})
  link_libraries(${GTK_LIBRARIES})
  set(src
    ${CONFIG_MANGL_SRC_DIR}/gtk/mangl_gtk_main.cpp
    ${CONFIG_MANGL_SRC_DIR}/gtk/mangl_gtk_main_window.cpp
    ${CONFIG_MANGL_SRC_DIR}/gtk/mangl_gtk_main_window.h
    ${CONFIG_MANGL_SRC_DIR}/gtk/mangl_gtk_native_gui.cpp
    ${CONFIG_MANGL_SRC_DIR}/gtk/mangl_gtk_opengl.cpp
    ${CONFIG_MANGL_SRC_DIR}/gtk/mangl_gtk_timer.cpp
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/gtk PREFIX "Sources\\Mangl/gtk" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/sdl
if (CONFIG_MANGL_SDL)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl.h
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_keyboard.cpp
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_keyboard.h
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_loop.cpp
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_loop.h
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_main.cpp
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_main_window.cpp
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_main_window.h
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_native_gui.cpp
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_opengl.cpp
    ${CONFIG_MANGL_SRC_DIR}/sdl/mangl_sdl_timer.cpp
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/sdl PREFIX "Sources\\Mangl/sdl" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/3rdparty/glew
if (NOT APPLE AND NOT ANDROID)
  add_compile_definitions(GLEW_STATIC)
  include_directories(${CONFIG_MANGL_3RDPARTY_DIR}/glew)
  set(src
    ${CONFIG_MANGL_3RDPARTY_DIR}/glew/GL/mangl_eglew.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/glew/GL/mangl_glew.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/glew/GL/mangl_glxew.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/glew/GL/mangl_wglew.h
    ${CONFIG_MANGL_3RDPARTY_DIR}/glew/mangl_glew.c
  )
  source_group(TREE ${CONFIG_MANGL_3RDPARTY_DIR}/glew PREFIX "Sources\\Mangl/3rdparty/glew" FILES ${src})
  list(APPEND main_SRC ${src})
  
if (MSVC)
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "/W0")
else()
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-w")
endif()

  set_source_files_properties(${src} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
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

# Source Group: Mangl/windows
if (WIN32)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_action.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_action_gui.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_audio_system.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_directsound.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_directsound.h
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_dispatch.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_dispatch.h
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_env.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_keyboard.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_keyboard.h
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_launch.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_layout.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_loop.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_opengl.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_window.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_window.h
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_main_wndproc.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_music_mci.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_music_mci.h
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_stubs.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_util.cpp
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_win_util.h
    ${CONFIG_MANGL_SRC_DIR}/windows/mangl_winmain.cpp
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/windows PREFIX "Sources\\Mangl/windows" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Mangl/wasm
if (EMSCRIPTEN)
  set(src
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_app.cpp
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_app.h
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_env.cpp
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_keyboard.cpp
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_keyboard.h
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_loop.cpp
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_loop.h
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_main_window.cpp
    ${CONFIG_MANGL_SRC_DIR}/wasm/mangl_wasm_main_window.h
  )
  source_group(TREE ${CONFIG_MANGL_SRC_DIR}/wasm PREFIX "Sources\\Mangl/wasm" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

