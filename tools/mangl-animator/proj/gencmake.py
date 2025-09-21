#! /usr/bin/env python3

from genproj import GenProj

proj = GenProj()

proj.buildConfig = "../../../build_config.yaml"
proj.sources = "Sources.yaml"
proj.targetCmakeSources = "Sources.cmake"
proj.targetCmakeConfig = ".build_config.cmake"

proj.generate()
proj.printResult()
