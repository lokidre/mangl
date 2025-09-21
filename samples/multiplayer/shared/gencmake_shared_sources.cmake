# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-07-12

# Source Group: MultiplayerShared
set(src
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_backend.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_frontend.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_instance.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_npc.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_packet.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_packet_gen.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_packet_gen_json.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_packet_gen_yaml.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_screen.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_game_state.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_mp_protocol.hpp
  ${CONFIG_MANGL_DIR}/samples/multiplayer/shared/sample_mp_types.hpp
)
source_group(TREE ${CONFIG_MANGL_DIR}/samples/multiplayer/shared PREFIX "Sources\\MultiplayerShared" FILES ${src})
list(APPEND main_SRC ${src})

