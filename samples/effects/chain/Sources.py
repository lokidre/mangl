#! /usr/bin/python3

import sys
sys.path.append('../../../scripts/python' )

import GenerateSources

GenerateSources.generate ( "Sources.yaml", "Sources.cmake" )

