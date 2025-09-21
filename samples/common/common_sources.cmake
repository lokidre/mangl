# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-11-18

# Source Group: Common
include_directories(${CONFIG_MANGL_SAMPLES_DIR}/common/src)
include_directories(${CONFIG_MANGL_SAMPLES_DIR}/common/generated)
set(src
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/mangl_test.h
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_app.cpp
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_app.h
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_game_screen.h
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_leaderboards.h
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_screen.cpp
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_screen.h
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_screen_fps.cpp
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_screen_layout.cpp
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_static.cpp
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_static.h
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_styles.cpp
  ${CONFIG_MANGL_SAMPLES_DIR}/common/src/test_types.h
)
source_group(TREE ${CONFIG_MANGL_SAMPLES_DIR}/common/src PREFIX "Sources\\Common" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Common
if (WIN32)
  set(src
    ${CONFIG_MANGL_SAMPLES_DIR}/common/mangl-samples.win.manifest
  )
  source_group(TREE ${CONFIG_MANGL_SAMPLES_DIR}/common PREFIX "Sources\\Common" FILES ${src})
  list(APPEND main_SRC ${src})
endif()

# Source Group: Textures
set(src
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/textures/tex_common.png
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/textures/tex_common.yaml
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/textures/tex_tutorial.png
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/textures/tex_tutorial.yaml
)
source_group(TREE ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/textures PREFIX "Bundle\\Textures" FILES ${src})
list(APPEND bundle_SRC ${src})

# Source Group: Sounds
set(src
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/bone_crush_01.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/computerkeyboardtypingsingle_1_51195.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/countdown_1.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/countdown_2.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/countdown_3.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/countdown_4.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/crashcrtmonitor_bw.60466.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/crashcupboard_bw.17164.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/crashglass_bw.17239.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/fireworks_1.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/foley_wood_stick_whip_whoosh_01_26769.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/game_over.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/happyalert.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/movie_things_falling_down_crumble_56719.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/musical_instrument_electric_keyboard_phase_accent_stab_01_31329.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/nicechime.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/outofluck.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/science_fiction_magical_wand_wave_pixie_dust_sparkle_shimmer_delay_01_43998.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/sound_high_score.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/sound_rotate.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/success01.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/success02.flac
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac/wood_crash.flac
)
source_group(TREE ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/sounds/flac PREFIX "Bundle\\Sounds" FILES ${src})
list(APPEND bundle_SRC ${src})

# Source Group: Music
set(src
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/music_bound_and_determined.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/music_distant_horizon.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/music_eyes_ahead.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/music_the_mountains_of_mars.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/music_under_cover.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/music_unraveling_the_mystery.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/night-in-kyoto-avbe-main-version-21302-01-57.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/no-time-to-die-hartzmann-main-version-9251-02-01.mp3
  ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3/nostalgic-moment-monument-music-main-version-21355-03-05.mp3
)
source_group(TREE ${CONFIG_MANGL_SAMPLES_ASSETS_DIR}/music/mp3 PREFIX "Bundle\\Music" FILES ${src})
list(APPEND bundle_SRC ${src})

