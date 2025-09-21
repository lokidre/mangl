#!/usr/bin/env python3

from genproj import GenProj

proj = GenProj()

proj.projectConfig  = "mangl_animator.yaml"
proj.sources = "Sources.yaml"
proj.buildConfig = '../../../build_config.yaml'
proj.targetDir  = "_build"
proj.targetXcodeGen = "project.yml"

proj.generate()
proj.printResult()

